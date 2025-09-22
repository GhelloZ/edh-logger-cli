#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
#include <curl/curl.h>
#include <stddef.h>

#include "cJSON.h"
#include "utils.h"

#define DB_REL_PATH "/.config/edh/database.db"

char* get_db_path(){
	const char* home = getenv("HOME");
	if (!home) {
		fprintf(stderr, "Could not determine HOME directory\n");
		return NULL;
	}

	// Allocate full path: $HOME + DB_REL_PATH
	size_t len = strlen(home) + strlen(DB_REL_PATH) + 1;
	char* full_path = malloc(len);
	if (!full_path) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	snprintf(full_path, len, "%s%s", home, DB_REL_PATH);

	// Ensure directory ~/.config/edh exists
	char dir_path[1024];
	snprintf(dir_path, sizeof(dir_path), "%s/.config/edh", home);
	mkdir(dir_path, 0755); // ignore error if it already exists

	return full_path; // caller must free()
}

sqlite3 *open_db(){
	sqlite3 *db;

	char* db_path = NULL;
	db_path = get_db_path();
	if (!db_path) return NULL;

	// Open (or create) the database file
	int rc = sqlite3_open(db_path, &db);
	free(db_path);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	return db;
}

int init_db(){
	sqlite3 *db = open_db();
	if(db == NULL) return DB_NOT_FOUND;

	char *err_msg = 0;
	int rc;

	// SQL schema
	const char* sql_tables =
    "PRAGMA foreign_keys = ON;"
    /* Games */
    "CREATE TABLE IF NOT EXISTS Games ("
    "    game_id      INTEGER PRIMARY KEY AUTOINCREMENT,"
    "    date_played  INTEGER NOT NULL,"
    "    duration_min INTEGER,"
    "    comment      TEXT"
    ");"
    /* Players */
    "CREATE TABLE IF NOT EXISTS Players ("
    "    player_id        INTEGER PRIMARY KEY AUTOINCREMENT,"
    "    name             TEXT NOT NULL UNIQUE,"
    "    total_games      INTEGER DEFAULT 0,"
    "    wins             INTEGER DEFAULT 0,"
    "    second_places    INTEGER DEFAULT 0,"
    "    third_places     INTEGER DEFAULT 0,"
    "    other_finishes   INTEGER DEFAULT 0,"
    "    decks_owned_cnt  INTEGER DEFAULT 0,"
    "    decks_used_cnt   INTEGER DEFAULT 0"
    ");"
    /* Decks */
    "CREATE TABLE IF NOT EXISTS Decks ("
    "    deck_id         INTEGER PRIMARY KEY AUTOINCREMENT,"
    "    title           TEXT NOT NULL,"
    "    commander_1     TEXT NOT NULL,"
    "    commander_2     TEXT,"
    "    companion       TEXT,"
    "    card_list       TEXT,"
    "    total_games     INTEGER DEFAULT 0,"
    "    wins            INTEGER DEFAULT 0,"
    "    second_places   INTEGER DEFAULT 0,"
    "    third_places    INTEGER DEFAULT 0,"
    "    other_finishes  INTEGER DEFAULT 0,"
    "    owner_id        INTEGER,"
    "    FOREIGN KEY(owner_id) REFERENCES Players(player_id) ON DELETE SET NULL"
    ");"
    /* GamePlayers: which player used which deck in a game */
    "CREATE TABLE IF NOT EXISTS GamePlayers ("
    "    game_id     INTEGER NOT NULL,"
    "    player_id   INTEGER NOT NULL,"
    "    deck_id     INTEGER NOT NULL,"
    "    rank        INTEGER,"
    "    FOREIGN KEY (game_id) REFERENCES Games(game_id) ON DELETE CASCADE,"
    "    FOREIGN KEY (player_id) REFERENCES Players(player_id) ON DELETE CASCADE,"
    "    FOREIGN KEY (deck_id) REFERENCES Decks(deck_id) ON DELETE CASCADE,"
    "    PRIMARY KEY (game_id, player_id)"
    ");"
    /* PlayerDeckStats: per-player-per-deck aggregated counters (fast lookups & UPSERT-able) */
    "CREATE TABLE IF NOT EXISTS PlayerDeckStats ("
    "    player_id      INTEGER NOT NULL,"
    "    deck_id        INTEGER NOT NULL,"
    "    total_games    INTEGER DEFAULT 0,"
    "    wins           INTEGER DEFAULT 0,"
    "    second_places  INTEGER DEFAULT 0,"
    "    third_places   INTEGER DEFAULT 0,"
    "    other_finishes INTEGER DEFAULT 0,"
    "    PRIMARY KEY (player_id, deck_id),"
    "    FOREIGN KEY (player_id) REFERENCES Players(player_id) ON DELETE CASCADE,"
    "    FOREIGN KEY (deck_id)   REFERENCES Decks(deck_id)   ON DELETE CASCADE"
    ");"
    /* Useful indexes */
    "CREATE INDEX IF NOT EXISTS idx_decks_owner_id ON Decks(owner_id);"
    "CREATE INDEX IF NOT EXISTS idx_gp_deck_id ON GamePlayers(deck_id);"
    "CREATE INDEX IF NOT EXISTS idx_gp_player_id ON GamePlayers(player_id);"
    "CREATE INDEX IF NOT EXISTS idx_pdst_player_id ON PlayerDeckStats(player_id);"
    "CREATE INDEX IF NOT EXISTS idx_pdst_deck_id ON PlayerDeckStats(deck_id);"
    ;

	// Run sql prompts
	rc = sqlite3_exec(db, sql_tables, 0, 0, &err_msg);
	sqlite3_close(db);

	if(rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s", err_msg ? err_msg : "(no message)");
		sqlite3_free(err_msg);
		return rc;
	}

	printf("Database initiated\n");
	return SQLITE_OK;
}

int check_if_db_exists(){
	// Checks if the database exists and if it doesnt it initializes it
	char* path = get_db_path();
	if (!path) return DB_NO_PATH;

	FILE *database = fopen(path, "r");
	if (database){
		fclose(database);
		free(path);
		return DB_EXISTS;
	}else{
		free(path);
		return DB_NOT_FOUND;
	}
}

int validate_regex(const char* string, const char* pattern){
	/************************
	 * Returns 1 if the string matches the pattern,
	 * 0 otherwise
	 ***********************/
	if (!string || !pattern) return REGEX_NULL_PARAM;
	regex_t re;
	int flags = REG_EXTENDED | REG_NOSUB;
	if (regcomp(&re, pattern, flags) != 0) return REGEX_COMPILE_ERR;
	int ret = regexec(&re, string, 0, NULL, 0);
	regfree(&re);

	if(ret == 0) 			return REGEX_OK;
	if(ret == REG_NOMATCH)	return REGEX_NO_MATCH;
	return REGEX_UNDEFINED_ERR;
}

struct curl_response {
	char *output;
	size_t size;
};

// Callback function for libcurl to write the response into output
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct curl_response *mem = (struct curl_response *)userp;

    char *ptr = realloc(mem->output, mem->size + real_size + 1);
    if (ptr == NULL) {
        // out of output
        printf("Not enough output (realloc returned NULL)\n");
        return 0;
    }

    mem->output = ptr;
    memcpy(&(mem->output[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->output[mem->size] = 0; // null-terminate

    return real_size;
}

ApiResponse fetch_api(const char *uri){
	CURLcode res;
	CURL *handle;
	ApiResponse response = {0};

	struct curl_response chunk = {0};
	chunk.output = malloc(1);
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, "https://naas.isalman.dev/no"); // Just a default

	if(!handle){
		free(chunk.output);
		fprintf(stderr, "For some reason no curl handle\n");
		return response;
	}

	curl_easy_setopt(handle, CURLOPT_URL, uri); 
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk); 
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L); 

	res = curl_easy_perform(handle);
	if(res!=CURLE_OK){
		fprintf(stderr, "Failed to execute GET request from %s: %s", uri, curl_easy_strerror(res));
		free(chunk.output);
	} else {
		response.output = chunk.output;
		response.size = chunk.size;
	}

	curl_easy_cleanup(handle);
	curl_global_cleanup();
	return response; // Must free in the caller
}

void parse_deck_info(char *json_blob){
	cJSON *json = cJSON_Parse(json_blob);

//	printf("func test: %s\n", cJSON_Print(json));
}

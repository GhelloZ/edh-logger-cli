#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>

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

int init_db(){
	sqlite3 *db;
	char *err_msg = 0;
	int rc;

	// If no custom path provided, use default
	char* default_path = NULL;
	default_path = get_db_path();
	if (!default_path) return -1;
	const char* path = default_path;

	// Open (or create) the database file
	rc = sqlite3_open(path, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		free(default_path);
		return rc;
	}

	// SQL schema
	const char* sql_tables = 
		"PRAGMA foreign_keys = ON;"
		"CREATE TABLE IF NOT EXISTS Games ("
		"    game_id      INTEGER PRIMARY KEY AUTOINCREMENT,"
		"    date_played  DATETIME NOT NULL,"
		"    duration_min INTEGER,"
		"    comment      TEXT"
		");"
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
		"    other_finishes  INTEGER DEFAULT 0"
		");"
		"CREATE TABLE IF NOT EXISTS GamePlayers ("
		"    game_id     INTEGER NOT NULL,"
		"    player_id   INTEGER NOT NULL,"
		"    deck_id     INTEGER NOT NULL,"
		"    rank        INTEGER,"
		"    FOREIGN KEY (game_id) REFERENCES Games(game_id),"
		"    FOREIGN KEY (player_id) REFERENCES Players(player_id),"
		"    FOREIGN KEY (deck_id) REFERENCES Decks(deck_id),"
		"    PRIMARY KEY (game_id, player_id)"
		");"
		"CREATE TABLE IF NOT EXISTS PlayerDeckOwnership ("
		"    player_id INTEGER NOT NULL,"
		"    deck_id   INTEGER NOT NULL,"
		"    FOREIGN KEY (player_id) REFERENCES Players(player_id),"
		"    FOREIGN KEY (deck_id) REFERENCES Decks(deck_id),"
		"    PRIMARY KEY (player_id, deck_id)"
		");";

	// Run sql prompts
	rc = sqlite3_exec(db, sql_tables, 0, 0, &err_msg);
	if(rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		free(default_path);
		return rc;
	}

	printf("Database initiated\n");
	sqlite3_close(db);
	free(default_path);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "log_utils.h"
#include "stats_calc.h"

int init_db(const char* db_name){
	sqlite3 *db;
	char *err_msg = 0;
	int rc;

	// Open (or create) the database file
	rc = sqlite3_open(db_name, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
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
		return rc;
	}

	printf("Database initiated\n");
	sqlite3_close(db);
	return SQLITE_OK;
}

int main(int argc, char* argv[]){
	char* command = argv[1];

	FILE *database = fopen("database.db", "r");
	if (database){
		fclose(database);
	}else{
		printf("Database not present\n");
		init_db("database.db");
	}

	if(strcmp(command, "log") == 0){
		printf("This command exists\n");
	}else if(strcmp(command, "stats") == 0){
		printf("This command also exists\n");
	}else{
		printf("Not a valid command\n");
		return 1;
	}

	return 0;
}

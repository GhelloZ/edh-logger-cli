#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "player.h"
#include "utils.h"

int add_player(const char* name){

	// Name validation
	if (!name) {
		return ADDPLAYER_NO_NAME;
	}

	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(name, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return ADDPLAYER_INVALID_NAME;    // name doesn't match allowed pattern
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return ADDPLAYER_VALIDATION_ERROR;
		}
	}

	// Open DB
	char *db_path = get_db_path(); // returns malloc'd string or NULL on error
	if (!db_path) {
		return ADDPLAYER_DB_ERROR; // couldn't find DB path
	}

	sqlite3 *db = NULL;
	int rc = sqlite3_open(db_path, &db);
	// We can free the db_path immediately — sqlite3_open copies the filename internally.
	free(db_path);
	if (rc != SQLITE_OK) {
		if (db) sqlite3_close(db);
		return ADDPLAYER_DB_ERROR;
	}

	// Prepare INSERT query
	const char *sql_query =
		"INSERT INTO Players (name, total_games, wins, second_places, third_places, other_finishes, decks_owned_cnt, decks_used_cnt) "
		"VALUES (?, 0, 0, 0, 0, 0, 0, 0);";

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "add_player: prepare failed: %s\n", sqlite3_errmsg(db));
		if (stmt) sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ADDPLAYER_DB_ERROR;
	}

	// Bind the name. Use SQLITE_TRANSIENT so SQLite makes a copy immediately (safe lifetime).
	rc = sqlite3_bind_text(stmt, 1, name, -1, (sqlite3_destructor_type)SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "add_player: bind failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ADDPLAYER_DB_ERROR;
	}

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		if (rc == SQLITE_CONSTRAINT) {
			// Unique constraint violation (player name exists)
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return ADDPLAYER_EXISTS;
		}

		// Some other DB error
		fprintf(stderr, "add_player: step failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return ADDPLAYER_DB_ERROR;
	}

	/* success */
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return ADDPLAYER_OK;
}

int delete_player(const char* name){
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

int rename_player(
		const char* current_name,
		const char* new_name
		){
	// We assume that the current name is already valid since the only way to
	// add one is through the add_player() function that already validates it

	if (!current_name) {
		return RENAMEPLAYER_NO_NAME;
	}

	// New Name validation
	if (!new_name) {
		return RENAMEPLAYER_NO_NAME;
	}

	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(new_name, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return RENAMEPLAYER_INVALID_NAME;    // name doesn't match allowed pattern
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return RENAMEPLAYER_VALIDATION_ERROR;
		}
	}

	// Open DB
	char *db_path = get_db_path(); // returns malloc'd string or NULL on error
	if (!db_path) {
		return ADDPLAYER_DB_ERROR; // couldn't find DB path
	}

	sqlite3 *db = NULL;
	int rc = sqlite3_open(db_path, &db);
	// We can free the db_path immediately — sqlite3_open copies the filename internally.
	free(db_path);
	if (rc != SQLITE_OK) {
		if (db) sqlite3_close(db);
		return ADDPLAYER_DB_ERROR;
	}

	const char* sql_rename_player = 
		"UPDATE Players SET name = ? WHERE name = ?;";

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare_v2(db, sql_rename_player, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "add_player: prepare failed: %s\n", sqlite3_errmsg(db));
		if (stmt) sqlite3_finalize(stmt);
		sqlite3_close(db);
		return RENAMEPLAYER_DB_ERROR;
	}

	// Bind the name. Use SQLITE_TRANSIENT so SQLite makes a copy immediately (safe lifetime).
	rc = sqlite3_bind_text(stmt, 1, new_name, -1, (sqlite3_destructor_type)SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "rename_player: new_name bind failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return RENAMEPLAYER_DB_ERROR;
	}
	rc = sqlite3_bind_text(stmt, 2, current_name, -1, (sqlite3_destructor_type)SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "rename_player: current_name bind failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return RENAMEPLAYER_DB_ERROR;
	}

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		if (rc == SQLITE_CONSTRAINT) {
			// Unique constraint violation (player name exists)
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return RENAMEPLAYER_EXISTS;
		}

		// Some other DB error
		fprintf(stderr, "add_player: step failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return RENAMEPLAYER_DB_ERROR;
	}

	/* success */
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return RENAMEPLAYER_OK;
}

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#include "deck.h"
#include "utils.h"

int add_deck(const char *title,
		const char *commander,
		const char *owner,
		const char *partner,
		const char *companion,
		const char *link,
		FILE *card_list){
	fprintf(stderr, "\033[33mNot fully implemented\033[0m\n");
	/**************
	 * VALIDATION *
	 **************/
	// Title
	//fprintf(stderr, "\033[90mTitle: %s\n\033[0m", title);
	if(!strcmp(title, "")) {
		return ADDDECK_NO_TITLE;
	}

	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(title, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return ADDDECK_INVALID_TITLE;    // name doesn't match allowed pattern
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return ADDDECK_VALIDATION_ERROR;
		}
	}

	// Commander
	//fprintf(stderr, "\033[90mCommander: %s\n\033[0m", commander);
	if(!strcmp(commander, "")) {
		return ADDDECK_NO_COMMANDER;
	}

	pattern = "^[A-Za-z0-9 ._',-]{4,64}$";
	validate_rc = validate_regex(commander, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return ADDDECK_INVALID_COMMANDER;
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return ADDDECK_VALIDATION_ERROR;
		}
	}

	// Owner
	//fprintf(stderr, "\033[90mOwner: %s\n\033[0m", owner);
	if(!strcmp(owner, "")) {
		return ADDDECK_NO_OWNER;
	}
	
	pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	validate_rc = validate_regex(owner, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return ADDDECK_INVALID_OWNER;
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return ADDDECK_VALIDATION_ERROR;
		}
	}

	// Partner
	//fprintf(stderr, "\033[90mPartner: %s\n\033[0m", partner);
	if(strcmp(partner, "")){
		pattern = "^[A-Za-z0-9 ._',-]{4,64}$";
		validate_rc = validate_regex(partner, pattern);
		if (validate_rc != REGEX_OK) {
			switch (validate_rc) {
				case REGEX_NO_MATCH:
					return ADDDECK_INVALID_PARTNER;    // name doesn't match allowed pattern
				case REGEX_COMPILE_ERR:
				default:
					// Something wrong with the validator or pattern — treat as validation failure.
					return ADDDECK_VALIDATION_ERROR;
			}
		}
	}

	// Companion
	//fprintf(stderr, "\033[90mCompanion: %s\n\033[0m", companion);
	if(strcmp(companion, "")){
		pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
		validate_rc = validate_regex(companion, pattern);
		if (validate_rc != REGEX_OK) {
			switch (validate_rc) {
				case REGEX_NO_MATCH:
					return ADDDECK_INVALID_COMPANION;    // name doesn't match allowed pattern
				case REGEX_COMPILE_ERR:
				default:
					// Something wrong with the validator or pattern — treat as validation failure.
					return ADDDECK_VALIDATION_ERROR;
			}
		}
	}

	// Deck link
	// https://archidekt.com/decks/5918632/halana
	//fprintf(stderr, "\033[90mDeck link: %s\n\033[0m", link);
	pattern = "^https:\\/\\/archidekt\\.com\\/decks\\/[0-9]{7}\\/[a-zA-Z_0-9]+$";
	if(strcmp(link, "")){
		validate_rc = validate_regex(link, pattern);
		if (validate_rc != REGEX_OK) {
			switch (validate_rc) {
				case REGEX_NO_MATCH:
					return ADDDECK_INVALID_LINK;    // name doesn't match allowed pattern
				case REGEX_COMPILE_ERR:
				default:
					// Something wrong with the validator or pattern — treat as validation failure.
					return ADDDECK_VALIDATION_ERROR;
			}
		}
	}

	// Card list
	//fprintf(stderr, "\033[90mCard list\n\033[0m");
	pattern = "^[0-9]{1,2}x? [a-zA-Z ,'-]+$";

	char buffer[MAX_CARD_NAME_LEN];

	if(card_list){
		int line_count = 0;
		while(fgets(buffer, MAX_CARD_NAME_LEN, card_list)){
			// If the deck list is too long the program will interrupt
			line_count++;
			if( line_count>=MAX_CARD_ENTRIES) return ADDDECK_LIST_TOO_LONG;

			buffer[strcspn(buffer, "\n")] = '\0';			// Removes the newline char so that the
															// regex doesn't fail

			// Skips the line if it's empty
			// When exporting from archidekt there's a blank line at the end
			char *line = buffer;
			while (*line == ' ' || *line == '\t') line++;
			if (*line == '\0') continue;

			//printf("\033[36m%s\033[0m\n", buffer);
			validate_rc = validate_regex(buffer, pattern);
			if (validate_rc != REGEX_OK){
				switch (validate_rc){
					case REGEX_NO_MATCH:
						return ADDDECK_INVALID_CARD_LIST;	// One or more lines don't match
															// the regex rule
					case REGEX_COMPILE_ERR:
					default:
						// Something wrong with the validator or pattern — treat as validation failure.
						return ADDDECK_VALIDATION_ERROR;
				}
			}
		}
	}
	//fprintf(stderr, "\033[32mValidation completed!\033[0m\n");
	
	/*************
	* SQL PROMPT *
	*************/
	// Get owner player_id
	int rc;
	sqlite3_stmt *stmt;
	sqlite3 *db = open_db();

	int owner_id = -1;
	const char *sql_get_owner = "SELECT player_id FROM Players WHERE name = ?;";
	rc = sqlite3_prepare_v2(db, sql_get_owner, -1, &stmt, NULL);
	if (rc != SQLITE_OK){
		fprintf(stderr, "Failed to prepare owner query: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	sqlite3_bind_text(stmt, 1, owner, -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) == SQLITE_ROW){
		owner_id = sqlite3_column_int(stmt, 0);
	} else {
		fprintf(stderr, "Owner not found in players table\n");
		return ADDDECK_INVALID_OWNER;
	}
	sqlite3_finalize(stmt);

	// Read the card list to a string
	char *card_list_text = NULL;
	if (card_list){
		fseek(card_list, 0, SEEK_END);
		const int file_size = (int) ftell(card_list);
		fseek(card_list, 0, SEEK_SET);

		card_list_text = malloc(file_size + 1);
		if (!card_list_text) {
			fprintf(stderr, "Memory allocation for card list failed\n");
			return ADDDECK_FAILED_CARD_LIST_ALLOCATION;
		}

		fread(card_list_text, 1, file_size, card_list);
		card_list_text[file_size] = '\0';
	}

	// Insert into deck table
	const char *sql_insert_deck =
		"INSERT INTO Decks (title, commander_1, commander_2, companion, card_list, owner_id) "
		"VALUES (?, ?, ?, ?, ?, ?);";

	rc = sqlite3_prepare_v2(db, sql_insert_deck, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare INSERT statement: %s\n", sqlite3_errmsg(db));
		free(card_list_text);
		return rc;
	}

	sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, commander, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, partner && strlen(partner)>0 ? partner : NULL, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, companion && strlen(companion)>0 ? companion : NULL, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, card_list_text ? card_list_text : NULL, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 6, owner_id);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	free(card_list_text);
	if (rc != SQLITE_DONE){
		fprintf(stderr, "Insert failed: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	// Increment owner deck count
	const char *sql_update_count = "UPDATE Players SET decks_owned_cnt = decks_owned_cnt+1 WHERE player_id = ?;";
	rc = sqlite3_prepare_v2(db, sql_update_count, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare update player query: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	sqlite3_bind_int(stmt, 1, owner_id);
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE){
		fprintf(stderr, "Failed to update %s decks_owned_cnt: %s\n", owner, sqlite3_errmsg(db));
		return rc;
	}

	sqlite3_close(db);
	return ADDDECK_OK;
}

// Wrapper that takes a file path as input instead of a file
int add_deck_list_file_path(
		const char *title, 
		const char *commander, 
		const char *owner,
		const char *partner, 
		const char *companion, 
		const char *link, 
		const char *card_list_path)
{
	FILE *card_list_file = fopen(card_list_path, "r");
	if(!card_list_file) return ADDDECK_INVALID_CARD_LIST_PATH;

	int rc = add_deck(
			title,
			commander,
			owner,
			partner,
			companion,
			link,
			card_list_file
			);

	if(card_list_file){
		fclose(card_list_file);
	}

	return rc;
}


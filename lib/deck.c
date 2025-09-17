#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

#include "deck.h"
#include "utils.h"

int add_deck(char *title, char *commander, char *partner, char *companion, char *link, FILE *card_list){
	/**************
	 * VALIDATION *
	 **************/
	// Title
	fprintf(stderr, "\033[90mTitle: %s\n\033[0m", title);
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
	fprintf(stderr, "\033[90mCommander: %s\n\033[0m", commander);
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

	// Partner
	fprintf(stderr, "\033[90mPartner: %s\n\033[0m", partner);
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
	fprintf(stderr, "\033[90mCompanion: %s\n\033[0m", companion);
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
	fprintf(stderr, "\033[90mDeck link: %s\n\033[0m", link);
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
	fprintf(stderr, "\033[90mCard list\n\033[0m");
	pattern = "^[0-9]{1,2}x? [a-zA-Z ,'-]+$";

	char buffer[MAX_CARD_NAME_LEN];

	if(card_list){
		while(fgets(buffer, MAX_CARD_NAME_LEN, card_list)){
			buffer[strcspn(buffer, "\n")] = '\0';

			// Skips the line if it's empty
			// When exporting from archidekt there's a blank line at the end
			char *line = buffer;
			while (*line == ' ' || *line == '\t') line++;
			if (*line == '\0') continue;

			printf("\033[36m%s\033[0m\n", buffer);
			validate_rc = validate_regex(buffer, pattern);
			if (validate_rc != REGEX_OK){
				switch (validate_rc){
					case REGEX_NO_MATCH:
						return ADDDECK_INVALID_CARD_LIST; // One or more lines don't match
														  // the regex rule
					case REGEX_COMPILE_ERR:
					default:
						// Something wrong with the validator or pattern — treat as validation failure.
						return ADDDECK_VALIDATION_ERROR;
				}
			}
		}
	}
	fprintf(stderr, "\033[32mValidation completed!\033[0m\n");

	return ADDDECK_OK;
}

// Wrapper that takes a file path as input instead of a file
int add_deck_list_file_path(
		char *title, 
		char *commander, 
		char *partner, 
		char *companion, 
		char *link, 
		char *card_list_path)
{
	FILE *card_list_file = fopen(card_list_path, "r");

	int rc = add_deck(
			title,
			commander,
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


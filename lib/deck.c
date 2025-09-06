#include <stdio.h>
#include <sqlite3.h>

#include "deck.h"
#include "utils.h"


int add_deck(char *title, char *commander, char *partner, char *companion, char *link, char *card_list){
	/**************
	 * VALIDATION *
	 **************/
	// Title
	if (!title) {
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
	if (!commander) {
		return ADDDECK_NO_COMMANDER;
	}

	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(commander, pattern);
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
	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(partner, pattern);
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

	// Companion
	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(companion, pattern);
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

	// Deck link
	// https://archidekt.com/decks/5918632/halana
	const char *pattern = "^https:\/\/archidekt\.com\/decks\/\d{7}\/[a-zA-Z_\d]+$";
	int validate_rc = validate_regex(link, pattern);
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

	// Card list
	const char *pattern = "^[A-Za-z0-9 ._'-]{4,64}$";
	int validate_rc = validate_regex(card_list, pattern);
	if (validate_rc != REGEX_OK) {
		switch (validate_rc) {
			case REGEX_NO_MATCH:
				return ADDDECK_INVALID_CARD_LIST;    // name doesn't match allowed pattern
			case REGEX_COMPILE_ERR:
			default:
				// Something wrong with the validator or pattern — treat as validation failure.
				return ADDDECK_VALIDATION_ERROR;
		}
	}
}

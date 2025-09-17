#ifndef DECK_H
#define DECK_H

#include <stdio.h>

typedef enum {
	ADDDECK_OK = 0,				// Success

	ADDDECK_NO_TITLE,			// Missing title
	ADDDECK_INVALID_TITLE,		// Title didn't match regex validation
	ADDDECK_NO_COMMANDER,		// Missing commander
	ADDDECK_INVALID_COMMANDER,	
	ADDDECK_INVALID_PARTNER,
	ADDDECK_INVALID_COMPANION,
	ADDDECK_INVALID_LINK,		// Link spelled incorrectly or website
								// not supported at the moment (only archidekt
								// for now)
	ADDDECK_INVALID_CARD_LIST,
	ADDDECK_INVALID_CARD_LIST_PATH,	// The file for the decklist wasn't found in
									// the specified path
	ADDDECK_VALIDATION_ERROR	// Generic error from the validator
} AddDeckReturn;
#define MAX_CARD_NAME_LEN 146	// Our Market Research Shows That Players Like Really Long Card Names So We Made this Card to Have the Absolute Longest Card Name Ever Elemental
								// it's a 141 character long card, plus 4 characters 
								// for quantity (12x ) and a trailing \n or \0

int add_deck(
		char *title, 
		char *commander, 
		char *partner, 
		char *companion, 
		char *link, 
		FILE *card_list
		);
int add_deck_list_file_path(
		char *title, 
		char *commander, 
		char *partner, 
		char *companion, 
		char *link, 
		char *card_list
		);
int delete_deck();
int update_deck();

#endif

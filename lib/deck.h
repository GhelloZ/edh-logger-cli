#ifndef DECK_H
#define DECK_H

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
	ADDDECK_ ,
	ADDDECK_ ,
} AddDeckReturn;

int add_deck(
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

#ifndef PLAYER_H
#define PLAYER_H

// Return codes for add_player()
typedef enum {
	ADDPLAYER_OK = 0,			// Success
	ADDPLAYER_EXISTS,			// Player with that name already exists
	ADDPLAYER_INVALID_NAME,		// Invalid name provided
	ADDPLAYER_NO_NAME,			// No name provided
	ADDPLAYER_VALIDATION_ERROR,	// Regex validation error
	ADDPLAYER_DB_ERROR			// Database error
} AddPlayerReturn;

int add_player(const char* name);
int delete_player();
int rename_player();

#endif

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

typedef enum {
	RENAMEPLAYER_OK = 0,			// Success
	RENAMEPLAYER_EXISTS,			// Player with that name already exists
	RENAMEPLAYER_INVALID_NAME,		// Invalid name provided
	RENAMEPLAYER_NO_NAME,			// No name provided
	RENAMEPLAYER_VALIDATION_ERROR,	// Regex error
	RENAMEPLAYER_DB_ERROR			// Database error
} RenamePlayerReturn;

int add_player(const char* name);
int delete_player();
int rename_player(
		const char* current_name,
		const char* new_name
		);

#endif

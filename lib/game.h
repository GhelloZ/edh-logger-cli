#ifndef GAME_H
#define GAME_H

typedef enum {
	ADDGAME_OK = 0,
	ADDGAME_FAILED_ALLOCATION,
	ADDGAME_FAILED_SQL_PREPARE,
	ADDGAME_FAILED_SQL_EXECUTION
} AddGameReturn;

int add_game(
		char** players,
		char** decks,
		int* ranks,
		size_t count,
		unsigned short int time,
		long long int timestamp
		);
int delete_game();
int list_games();

#endif

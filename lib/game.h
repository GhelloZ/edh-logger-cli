#ifndef GAME_H
#define GAME_H

int add_game(
		char** players,
		char** decks,
		char** ranks,
		unsigned short int count,
		unsigned short int time,
		long long int timestamp
		);
int delete_game();
int list_games();

#endif

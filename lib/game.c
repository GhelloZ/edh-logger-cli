#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "utils.h"

int add_game(
		char** players,
		char** decks,
		char** ranks,
		unsigned short int count,
		unsigned short int duration,
		long long int timestamp
		){
	/**********************
	 * edh game add [-t MINUTES] -p Mario,Luigi[,Peach[,Browser\ Jr]] [-r 1,2,3,3] -d Marwyn\ Ramp,Marchesa[,Funny\ Artifact\ Boy[,Dinasours]]
	 * 		-t|--time MINUTES:		Optional, specifies game time in minutes
	 * 		-p|--players PLAYER1,PLAYER2[,PLAYER3[,PLAYER4]]:
	 * 								List of participating players in reverse order of death (first is the winner). 
	 * 								Can list from 2 to 6 players
	 * 		-r|--rank RANK1,RANK2[,RANK3[,RANK4]]:
	 * 								Optional, used to specify the rank of the players in the game.
	 * 								Useful to specify if two players died together and ended up tied.
	 * 								must have the same number of parameters as -p and there can only be a single
	 * 								1 value, if 2 or more player die together at the end of the game they'll simply
	 * 								all count as a tied 2' place. If not provided, the program will imply that you
	 * 								already listed the players in the correct order
	 *		-d|--decks DECK1,DECK2[,DECK3[,DECK4]]:
	 *								Decks used by the players in the game. Must be the same number of players specified
	 *								and in the same order
	 */
	fprintf(stderr, "\033[33mNot fully implemented\033[0m\n");

	if (!players || !decks) {
		fprintf(stderr, "game.c: add_game: players and decks must not be NULL\n");
		return -1;
	}
	if (count == 0) {
		fprintf(stderr, "game.c: add_game: count is 0\n");
		return -1;
	}

	/*
	fprintf(stderr, "game.c: add_game:\n");
	fprintf(stderr, "\tCount: %hu\n", count);
	fprintf(stderr, "\tTime: %hu\n", duration);
	fprintf(stderr, "\tTimestamp: %lld\n", timestamp);

	for (unsigned short int i = 0; i < count; i++) {
		const char *pstr = "(null)";
		const char *dstr = "(null)";
		const char *rstr = "(null)";

		if (players && players[i]) pstr = players[i];
		if (decks  && decks[i])  dstr = decks[i];
		if (ranks  && ranks[i])  rstr = ranks[i];

		fprintf(stderr, "\tPlayer[%hu]: %s\n", i, pstr);
		fprintf(stderr, "\tDeck[%hu]: %s\n", i, dstr);
		fprintf(stderr, "\tRank[%hu]: %s\n", i, rstr);
	}
	*/

	sqlite3 *db = NULL;
	sqlite3_stmt* stmt = NULL;
	int rc;

	rc = sqlite3_open(get_db_path(), &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return rc;
	}

	// Games
	const char* sql_game = 
		"INSERT INTO Games (date_played, duration_min, comment) "
		"VALUES (?, ?, NULL);";

	rc = sqlite3_prepare_v2(db, sql_game, -1, &stmt, NULL);
	if (rc != SQLITE_OK){
		fprintf(stderr, "Failed to prepare Games insert: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sqlite3_bind_int64(stmt, 1, timestamp);
	sqlite3_bind_int(stmt, 2, duration);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE){
		fprintf(stderr,"Failed to execute Games insert: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return -1;
	}
	sqlite3_finalize(stmt);

	sqlite3_int64 game_id = sqlite3_last_insert_rowid(db);

	// GamePlayers
	const char* sql_gameplayers = 
		"INSERT INTO GamePlayers (game_id, player_id, deck_id, rank)"
		"VALUES (?, "
		" (SELECT player_id FROM Players WHERE name = ?), "
		" (SELECT deck_id FROM Decks WHERE title = ?), "
		" ? "
		");";

	rc = sqlite3_prepare_v2(db, sql_gameplayers, -1, &stmt, NULL);
	if (rc != SQLITE_OK){
		fprintf(stderr, "Failed to prepare GamePlayers insert: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	for (int i=0; i<count; i++){
		sqlite3_bind_int64(stmt, 1, game_id);
		sqlite3_bind_text(stmt, 2, players[i], -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, decks[i], -1, SQLITE_STATIC);

		if(ranks != NULL){
			sqlite3_bind_int(stmt, 4, atoi(ranks[i]));
		} else {
			sqlite3_bind_int(stmt,4, i+1);
		}

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE){
			fprintf(stderr, "Failed to insert GamePlayers entry: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
		}
		sqlite3_reset(stmt);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int delete_game(){
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

int list_games(){
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

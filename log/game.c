#include <stdio.h>
#include <sqlite3.h>

int add_game(){
	/**********************
	 * edh game add [-l MINUTES] -p Mario,Luigi[,Peach[,Browser\ Jr]] [-r 1,2,3,3] -d Marwyn\ Ramp,Marchesa[,Funny\ Artifact\ Boy[,Dinasours]]
	 * 		-l|--lenght MINUTES: 	Optional, specifies game duration in minutes
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
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

int delete_game(){
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

int list_games(){
	fprintf(stderr,"Not yet implemented\n");
	return -1;
}

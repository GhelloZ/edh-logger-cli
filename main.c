#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stats_calc.h"

#include "log/utils.h"
#include "log/game.h"
#include "log/player.h"
#include "log/deck.h"

void game(const char* subcommand){
	// Checking if the subcommand for game is a valid one and calling the appropriate function
	if(strcmp(subcommand,"add") == 0){
		add_game();
	}else if(strcmp(subcommand,"delete") == 0){
		delete_game();
	}else if(strcmp(subcommand,"list") == 0){
		list_games();
	}else{
		fprintf(stderr,"\"%s\" is not a valid subcommand\n", subcommand);
	}
}

int main(int argc, char* argv[]){
	char* command = argv[1];

	// Checks if the database exists and if it doesnt it initializes it
	FILE *database = fopen("database.db", "r");
	if (database){
		fclose(database);
	}else{
		printf("Database not present\n");
		init_db("database.db");
	}

	if(strcmp(command, "game") == 0){
		// Checking if the user provided a subcommand
		if(argc < 3){
			fprintf(stderr,"Please provide a subcommand\n");
			return 1;
		}else if(argv[2][0] == '-'){
			fprintf(stderr,"Please provide a subcommand before the parameters\n");
			return 1;
		}

		game(argv[2]);
	}else if(strcmp(command, "player") == 0){
		printf("This command also exists\n");
	}else if(strcmp(command, "deck") == 0){
		printf("penis\n");
	}else{
		printf("Not a valid command\n");
		return 1;
	}

	return 0;
}

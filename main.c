#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats_calc.h"

#include "log/utils.h"
#include "log/game.h"
#include "log/player.h"
#include "log/deck.h"

void game(const char* subcommand){
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

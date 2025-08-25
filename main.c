#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_utils.h"
#include "stats_calc.h"


int main(int argc, char* argv[]){
	char* command = argv[1];

	FILE *database = fopen("database.db", "r");
	if (database){
		fclose(database);
	}else{
		printf("Database not present\n");
		init_db("database.db");
	}

	if(strcmp(command, "log") == 0){
		printf("This command exists\n");
	}else if(strcmp(command, "stats") == 0){
		printf("This command also exists\n");
	}else{
		printf("Not a valid command\n");
		return 1;
	}

	return 0;
}

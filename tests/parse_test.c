#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.h"

int main(){
	FILE *json_file = fopen("./resp.json", "r");
	long lSize;
	char *json_string;
	DeckInfo *info;

	if( !json_file ) perror("./resp.json"),exit(1);

	fseek( json_file , 0L , SEEK_END);
	lSize = ftell( json_file );
	rewind( json_file );

	/* allocate memory for entire content */
	json_string = calloc( 1, lSize+1 );
	if( !json_string ) fclose(json_file),fputs("memory alloc fails",stderr),exit(1);

	/* copy the file into the json_string */
	if( 1!=fread( json_string , lSize, 1 , json_file) )
		fclose(json_file),free(json_string),fputs("entire read fails",stderr),exit(1);

	printf("json content: %.200s\n", json_string);
	info = parse_deck_info(json_string);

	printf("Title: %s\n", info->title);
	printf("Title: %x\n", info->title);
	printf("Commander: %s\n", info->commander);
	printf("Commander: %x\n", info->commander);
	printf("Partner: %s\n", info->partner);
	printf("Partner: %x\n", info->partner);
	printf("Companion: %s\n", info->companion);
	printf("Companion: %x\n", info->companion);
	printf("Card list: %.200s\n", info->cardlist);
	printf("Card list: %x\n", info->cardlist);

	fclose(json_file);
	free(json_string);

	return 0;
}

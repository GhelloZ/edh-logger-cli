#include <stdio.h>
#include <stdlib.h>
#include "lib/utils.h"

int main(){
	ApiResponse resp = fetch_api("https://naas.isalman.dev/no");
	printf("response: %s\n", resp.output);
	free(resp.output);

	return 0;
}

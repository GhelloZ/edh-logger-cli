#ifndef UTILS_H
#define UTILS_H
#include <sqlite3.h>

typedef enum {
	DB_EXISTS = 0,
	DB_NO_PATH,
	DB_NOT_FOUND
} CheckDBResult;

// Regex validation return codes
typedef enum {
	REGEX_OK = 0,
	REGEX_NULL_PARAM,
	REGEX_COMPILE_ERR,
	REGEX_NO_MATCH,
	REGEX_UNDEFINED_ERR
} ValidateResult;

char* get_db_path();
sqlite3 *open_db();
int init_db();
int check_if_db_exists();
int validate_regex(const char* string, const char* pattern);
char *fetch_api(const char *uri);

#endif

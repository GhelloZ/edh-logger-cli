package main

/*
#cgo CFLAGS: -I./lib
#cgo LDFLAGS: -L. -lliblib
#include <stdlib.h>
#include "game.h"
#include "deck.h"
#include "player.h"
#include "utils.h"
#include "stats_calc.h"
*/
import "C"
import(
	"fmt"
	"edh/cmd"
	"os"
	"path/filepath"
	"unsafe"
)

func main(){
	// File path for database
	home, err := os.UserHomeDir()
	if err != nil {
		fmt.Println("Cannot determine user home directory")
		os.Exit(1)
	}
	dbPath := filepath.Join(home, ".config", "edh", "database.db")
	fmt.Println("main.go log: ", dbPath)

	// Creates file path if missing
	os.MkdirAll(filepath.Dir(dbPath), 0755)

	// Checks if the database already exists
	dbCheck := C.check_if_db_exists()
	if dbCheck == 0 {
		fmt.Println("Database not found, initializing...")
		cPath := C.CString(dbPath)
		defer C.free(unsafe.Pointer(cPath))
		C.init_db(cPath)
	}

	cmd.Execute()
}

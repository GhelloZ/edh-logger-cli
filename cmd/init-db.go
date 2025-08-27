package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../lib -lliblib -lsqlite3
#include "utils.h"
#include <stdlib.h>
*/
import "C"

import (
	"unsafe"

	"github.com/spf13/cobra"
)

var(
	dbName	string
)

func init(){
	// parent: "init"

	initDbCmd := &cobra.Command{
		Use:	"init-db",
		Short:	"Initializes the database",
		Long:	`Initializes the database to store all the game/player/deck data. 
		The program should initialize a database automatically, but this is here
		in case there are some issues.`,
		Run:	func (cmd *cobra.Command, args []string){
			runInitDb()
		},
	}

	initDbCmd.Flags().StringVarP(&dbName, "name", "n", "database.db", "Name for the database file. The program will only consider files named \"database.db\" so use this just for something, idk, it's just an option.")

	rootCmd.AddCommand(initDbCmd)
}

func runInitDb(){
	cName := C.CString(dbName)
	defer C.free(unsafe.Pointer(cName))

	C.init_db(cName)
}

package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../lib -lliblib -lsqlite3
#include "player.h"
#include <stdlib.h>
*/
import "C"

import (
	"fmt"
	"strings"
	"unsafe"

	"github.com/spf13/cobra"
)

var(
	playerName	string
	longFlag	int
)

func init(){
	// Parent: "player"

	playerCmd := &cobra.Command{
		Use:	"player",
		Short:	"Manage players",
	}
	RootCmd.AddCommand(playerCmd)

	addPlayerCmd := &cobra.Command{
		Use:	"add",
		Short:	"Add a new player to the database",
		Run:	func (cmd *cobra.Command, args []string){
			runAddPlayer()
		},
	}

	addPlayerCmd.Flags().StringVarP(&playerName, "name", "n", "", "Name of the player")
	addPlayerCmd.MarkFlagRequired("name")

	addPlayerCmd.Flags().IntVarP(&longFlag, "z-absurdly-long-flag-because-i-wanted-to-test-how-cobra-handles-it", "z", 0, "random flag i made to see how cobra handles displaying really long flags and description because why not. Did you know that otters have a favourite rock? it may be that it's not otters that have favourite rocks but it's some other animal, but english is not my first language so that's probably he reason and i'm mistranslating from my language")

	playerCmd.AddCommand(addPlayerCmd)
}

func runAddPlayer(){
	name := strings.TrimSpace(playerName)

	if name == "" {
		fmt.Printf("Player name is empty, please provide one")
		return
	}

	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))

	rc := C.add_player(cName)
	if int(rc) != 0 {
		fmt.Printf("Failed to store player into the database\n")
		return
	}

	fmt.Printf("Added %s to database\n", name)
}

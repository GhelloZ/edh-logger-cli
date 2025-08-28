package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../lib -lliblib -lsqlite3
#include "game.h"
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
	time	int
	playersCsv	string
	ranksCsv	string
	decksCsv	string
)

func init(){
	// parent: "game"

	gameCmd := &cobra.Command{
		Use:	"game",
		Short:	"Manage Games",
	}
	rootCmd.AddCommand(gameCmd)

	addCmd := &cobra.Command{
		Use:	"add",
		Short:	"Add a new game to the log",
		Run:	func (cmd *cobra.Command, args []string){
			runAddGame()
		},
	}

	addCmd.Flags().IntVarP(&time, "time", "t", 0, "Game duration in minutes")
	addCmd.Flags().StringVarP(&playersCsv, "players", "p", "", "Comma separated list of players")
	addCmd.Flags().StringVarP(&ranksCsv, "ranks", "r", "", "Optional. Comma separated ranks for the players")
	addCmd.Flags().StringVarP(&decksCsv, "decks","d","","Comma separated listof decks")
	addCmd.MarkFlagRequired("players")
	addCmd.MarkFlagRequired("decks")

	gameCmd.AddCommand(addCmd)
}

func runAddGame(){
	playerList := strings.Split(playersCsv,",")
	deckList   := strings.Split(decksCsv,",")
	var rankList []string
	if ranksCsv != "" {
		rankList = strings.Split(ranksCsv,",")
	}

	if len(playerList) != len(deckList) {
		fmt.Println("Number of players must match number of decks")
		return
	}

	// Convert []string to []*C.char
	cPlayers := make([]*C.char, len(playerList))
	for i, s := range playerList {
		cPlayers[i] = C.CString(s)
	}

	cDecks := make([]*C.char, len(deckList))
	for i, s := range deckList {
		cDecks[i] = C.CString(s)
	}

	// Handling optional ranks field. If nothing is provided, nil will be
	// provided to C.add_game()
	var cRanks []*C.char
	var cRanksPtr **C.char
	if len(rankList) > 0 {
		cRanks = make([]*C.char, len(rankList))
		for i, s := range rankList {
			cRanks[i] = C.CString(s)
		}
		cRanksPtr = (**C.char)(unsafe.Pointer(&cRanks[0]))
	} else {
		cRanksPtr = nil
	}

	// Calling add_game() from game.h to actually add the game
	C.add_game(
		(**C.char)(unsafe.Pointer(&cPlayers[0])),
		(**C.char)(unsafe.Pointer(&cDecks[0])),
		cRanksPtr,
		C.ushort(len(playerList)),
		C.ushort(time),
	)

	// Freeing memory
	for _, s := range cPlayers {
		C.free(unsafe.Pointer(s))
	}
	for _, s := range cDecks {
		C.free(unsafe.Pointer(s))
	}
	for _, s := range cRanks {
		C.free(unsafe.Pointer(s))
	}

	fmt.Printf("Added game: \n")
	fmt.Printf("\tPlayers:\t%v\n", playerList)
	if len(rankList) > 0 {
		fmt.Printf("\tRanks:\t\t%v\n", rankList)
	}
	fmt.Printf("\tDecks:\t\t%v\n", deckList)
	if time > 0 {
		fmt.Printf("\tTime:\t\t%d minutes\n", time)
	}
}

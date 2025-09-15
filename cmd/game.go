package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../build -ledh -lsqlite3
#include "game.h"
#include <stdlib.h>
*/
import "C"

import (
	"fmt"
	"strings"
	"unsafe"
	timepkg "time"

	"github.com/spf13/cobra"
)

var(
	time		int
	playersCsv	string
	ranksCsv	string
	decksCsv	string
	dateStr		string
)

func init(){
	// parent: "game"

	gameCmd := &cobra.Command{
		Use:	"game",
		Short:	"Manage ggmes",
	}
	RootCmd.AddCommand(gameCmd)

	addGameCmd := &cobra.Command{
		Use:	"add",
		Short:	"Add a new game to the log",
		Run:	func (cmd *cobra.Command, args []string){
			runAddGame()
		},
	}

	addGameCmd.Flags().IntVarP(&time, "time", "t", 0, "Optional. Game duration in minutes")
	addGameCmd.Flags().StringVarP(&playersCsv, "players", "p", "", "Comma separated list of players")
	addGameCmd.Flags().StringVarP(&ranksCsv, "ranks", "r", "", "Optional. Comma separated ranks for the players")
	addGameCmd.Flags().StringVarP(&decksCsv, "decks","d","","Comma separated listof decks")
	addGameCmd.Flags().StringVarP(&dateStr, "date","","","Optional. Defaults to current time. Must be provided in this format: YYYYMMDDHHmm")
	addGameCmd.MarkFlagRequired("players")
	addGameCmd.MarkFlagRequired("decks")

	gameCmd.AddCommand(addGameCmd)
}

func runAddGame() int {
	playerList := strings.Split(playersCsv,",")
	deckList   := strings.Split(decksCsv,",")
	var rankList []string
	if ranksCsv != "" {
		rankList = strings.Split(ranksCsv,",")
	}

	if len(playerList) != len(deckList) {
		fmt.Println("Number of players must match number of decks")
		return -1
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

	// Parsing date
	var timestamp int64
	if dateStr != "" {
		parsed, err := timepkg.Parse("2006-01-02 15:04", dateStr)
		if err != nil {
			fmt.Printf("Invalid date format. Please use 'YYYY-MM-DD HH:MM'\n")
			return -1
		}
		timestamp = parsed.Unix()
	} else {
		timestamp = timepkg.Now().Unix()
		timestamp -= int64(time*60)
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
	rc := int(C.add_game(
		(**C.char)(unsafe.Pointer(&cPlayers[0])),
		(**C.char)(unsafe.Pointer(&cDecks[0])),
		cRanksPtr,
		C.ushort(len(playerList)),
		C.ushort(time),
		C.longlong(timestamp),
	))

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

	if rc != 0 {
		fmt.Printf("Game could not be logged\n")
		return rc
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
	fmt.Printf("\tDate:\t\t%s\n", timepkg.Unix(timestamp, 0).Format("2006-01-02 15:04"))

	return rc
}

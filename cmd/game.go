package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../lib -lliblib
#include "game.h"
*/
import "C"

import (
	"fmt"
	"strings"

	"github.com/spf13/cobra"
)

var(
	length	int
	players	string
	ranks	string
	decks	string
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

	addCmd.Flags().IntVarP(&length, "length", "l", 0, "Game duration in minutes")
	addCmd.Flags().StringVarP(&players, "players", "p", "", "Comma separated list of players")
	addCmd.Flags().StringVarP(&ranks, "ranks", "r", "", "Optional. Comma separated ranks for the players")
	addCmd.Flags().StringVarP(&decks, "decks","d","","Comma separated listof decks")
	addCmd.MarkFlagRequired("players")
	addCmd.MarkFlagRequired("decks")

	gameCmd.AddCommand(addCmd)
}

func runAddGame(){
	playerList := strings.Split(players,",")
	deckList   := strings.Split(decks,",")
	var rankList []string
	if ranks != "" {
		rankList = strings.Split(ranks,",")
	}

	if len(playerList) != len(deckList) {
		fmt.Println("Number of players must match number of decks")
		return
	}

	// TODO: pass actual values into C.add_game when it's defined with arguments
	fmt.Printf("Added game: \n")
	fmt.Printf("\tPlayers:\t%v\n", playerList)
	if len(rankList) > 0 {
		fmt.Printf("\tRanks:\t\t%v\n", rankList)
	}
	fmt.Printf("\tDecks:\t\t%v\n", deckList)
	if length > 0 {
		fmt.Printf("\tLenght:\t\t%d minutes\n", length)
	}

	C.add_game()
}

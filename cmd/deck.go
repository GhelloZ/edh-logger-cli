package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../lib -lliblib -lsqlite3
#include "deck.h"
#include <stdlib.h>
*/
import "C"

import (
	"fmt"
	//"strings"
	//"unsafe"

	"github.com/spf13/cobra"
)

var (
	title		string
	commander1	string
	partner		string
	companion	string
	link		string
	cardList	string
)

func init(){
	// parent: "deck"

	deckCmd := &cobra.Command{
		Use:	"deck",
		Short:	"Manage decks",
	}
	RootCmd.AddCommand(deckCmd)

	addDeckCmd := &cobra.Command{
		Use:	"add",
		Short:	"Add a new deck to the database",
		Run:	func (cmd *cobra.Command, args []string){
			runAddDeck()
		},
	}

	addDeckCmd.Flags().StringVarP(&title, "title","t","","Title for the deck")
	addDeckCmd.Flags().StringVarP(&commander1, "commander","c","","Commander")
	addDeckCmd.Flags().StringVarP(&partner, "partner","p","","Optional. Partner")
	addDeckCmd.Flags().StringVarP(&companion, "companion","","","Optional. Companion")
	addDeckCmd.Flags().StringVarP(&link, "link","l","","Optional. Link for the deck list")
	addDeckCmd.Flags().StringVarP(&cardList, "card-list", "", "", "Optional. Full deck list. If an archidekt link is provided the deck list will be pulled from there by default.")

	addDeckCmd.MarkFlagRequired("title")
	addDeckCmd.MarkFlagRequired("commander")

	deckCmd.AddCommand(addDeckCmd)
}

func runAddDeck(){
	fmt.Println("Not implemented")
}

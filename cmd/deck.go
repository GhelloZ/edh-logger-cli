package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../build -ledh -lsqlite3
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
	commander	string
	partner		string
	companion	string
	link		string
	cardListFilePath	string
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
	addDeckCmd.Flags().StringVarP(&commander, "commander","c","","Commander")
	addDeckCmd.Flags().StringVarP(&partner, "partner","p","","Optional. Partner")
	addDeckCmd.Flags().StringVarP(&companion, "companion","","","Optional. Companion")
	addDeckCmd.Flags().StringVarP(&link, "link","l","","Optional. Link for the deck list")
	addDeckCmd.Flags().StringVarP(&cardListFilePath, "card-list", "", "", "Optional. Full deck list. If an archidekt link is provided the deck list will be pulled from there by default.")

//	addDeckCmd.MarkFlagRequired("title")
//	addDeckCmd.MarkFlagRequired("commander")

	deckCmd.AddCommand(addDeckCmd)
}

func runAddDeck() int {
	fmt.Println("Not fully implemented")

	rc := int(C.add_deck_list_file_path(
		C.CString(title),
		C.CString(commander),
		C.CString(partner),
		C.CString(companion),
		C.CString(link),
		C.CString(cardListFilePath),
	))

	fmt.Printf("%d: ", rc)
	switch rc {
	case 0:
		fmt.Println("Ok.")
	case 1:
		fmt.Println("No deck title provided")
	case 2:
		fmt.Println("Title failed regex validation")
	case 3:
		fmt.Println("No commander provided")
	case 4:
		fmt.Println("Commander failed regex validation")
	case 5:
		fmt.Println("Partner failed regex validation")
	case 6:
		fmt.Println("Companion failed regex validation")
	case 7:
		fmt.Println("Link was incorrect or not from archidekt")
	case 8:
		fmt.Println("Card list failed regex validation")
	case 9:
		fmt.Println("Invalid file path for deck list")
	case 10:
		fmt.Println("Generic validation error")
	}

	return rc
}

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
	"unsafe"

	"github.com/spf13/cobra"
)

var (
	title				string
	commander			string
	owner				string
	partner				string
	companion			string
	link				string
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
	addDeckCmd.Flags().StringVarP(&owner, "owner","o","","Owner of the deck")
	addDeckCmd.Flags().StringVarP(&partner, "partner","p","","Optional. Partner")
	addDeckCmd.Flags().StringVarP(&companion, "companion","","","Optional. Companion")
	addDeckCmd.Flags().StringVarP(&link, "link","l","","Optional. Link for the deck list")
	addDeckCmd.Flags().StringVarP(&cardListFilePath, "card-list", "", "", "Optional. Full deck list. If an archidekt link is provided the deck list will be pulled from there by default.")

	deckCmd.AddCommand(addDeckCmd)
}

func runAddDeck() int {
	cTitle := C.CString(title)
	defer C.free(unsafe.Pointer(cTitle))
	cCommander := C.CString(commander)
	defer C.free(unsafe.Pointer(cCommander))
	cOwner := C.CString(owner)
	defer C.free(unsafe.Pointer(cOwner))
	cPartner := C.CString(partner)
	defer C.free(unsafe.Pointer(cPartner))
	cCompanion := C.CString(companion)
	defer C.free(unsafe.Pointer(cCompanion))
	cLink := C.CString(link)
	defer C.free(unsafe.Pointer(cLink))
	cCardListFilePath := C.CString(cardListFilePath)
	defer C.free(unsafe.Pointer(cCardListFilePath))

	rc := int(C.add_deck_list_file_path(
		cTitle,
		cCommander,
		cOwner,
		cPartner,
		cCompanion,
		cLink,
		cCardListFilePath,
	))

	fmt.Printf("%d: ", rc)
	switch rc {
	case C.ADDDECK_OK:
		fmt.Println("Ok.")
	case C.ADDDECK_NO_TITLE:
		fmt.Println("No deck title provided")
	case C.ADDDECK_INVALID_TITLE:
		fmt.Println("Title failed regex validation")
	case C.ADDDECK_NO_COMMANDER:
		fmt.Println("No commander provided")
	case C.ADDDECK_INVALID_COMMANDER:
		fmt.Println("Commander failed regex validation")
	case C.ADDDECK_NO_OWNER:
		fmt.Println("No owner provided")
	case C.ADDDECK_INVALID_OWNER:
		fmt.Println("Owner not in Players table or failed regex validation")
	case C.ADDDECK_INVALID_PARTNER:
		fmt.Println("Partner failed regex validation")
	case C.ADDDECK_INVALID_COMPANION:
		fmt.Println("Companion failed regex validation")
	case C.ADDDECK_INVALID_LINK:
		fmt.Println("Link was incorrect or not from archidekt")
	case C.ADDDECK_INVALID_CARD_LIST:
		fmt.Println("Card list failed regex validation")
	case C.ADDDECK_INVALID_CARD_LIST_PATH:
		fmt.Println("Invalid file path for deck list")
	case C.ADDDECK_LIST_TOO_LONG:
		fmt.Println("Deck list too long")
	case C.ADDDECK_VALIDATION_ERROR:
		fmt.Println("Generic validation error")
	}

	return rc
}

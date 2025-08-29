package cmd

import (
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

var RootCmd = &cobra.Command{
	Use:	"edh",
	Short:	"EDH Game tracker CLI",
	Long:	`A command line tool to log and manage EDH games, decks, and players.`,
}

func Execute(){
	if err := RootCmd.Execute(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

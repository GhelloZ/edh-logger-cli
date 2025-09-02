package cmd

import (
	"fmt"
	_ "embed"

	"github.com/spf13/cobra"
)

//go:embed version
var version	string

func init(){
	// parent: "version"

	versionCmd := &cobra.Command{
		Use:	"version",
		Short:	"Display version of the CLI",
		Run:	func (cmd *cobra.Command, args []string){
			printVersion()
		},
	}
	RootCmd.AddCommand(versionCmd)
}

func printVersion(){
	fmt.Printf("Go CLI version: %s", version)
}

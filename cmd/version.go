package cmd

/*
#cgo CFLAGS: -I${SRCDIR}/../lib
#cgo LDFLAGS: -L${SRCDIR}/../build -ledh -lsqlite3
#include "cJSON.h"
#include <stdlib.h>
*/
import "C"

import (
	"fmt"
	_ "embed"

	"github.com/spf13/cobra"
)

//go:embed VERSION
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
	cjsonVersion := C.cJSON_Version()
	goCjsonVersion := C.GoString(cjsonVersion)

	fmt.Printf("libedh version: %s", version)
	fmt.Printf("cJSON version: %s\n", goCjsonVersion)
}

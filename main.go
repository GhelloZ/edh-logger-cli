package main

/*
#cgo CFLAGS: -I./lib
#cgo LDFLAGS: -L. -lliblib
#include "game.h"
#include "deck.h"
#include "player.h"
#include "utils.h"
#include "stats_calc.h"
*/
import "C"
import "edh/cmd"

func main(){
	cmd.Execute()
}

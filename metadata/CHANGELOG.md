## What's new
- [x] Players can be renamed with the `player rename` command
## Fixes
- [x] Added check to `rename_player()` that checks if a user with the given current name actually exists
- [ ] Fix ON DELETE actions (for some reason they don't work. It doesn't really affect how the library works since i don't want the removal of players or decks to automatically delete rows from other tables, but it's still weird and probably worth to check out)
## GitHub stuff
- [ ] Fix workflows
## TODO
### Libraries stuff
- [ ] Add `delete_deck()` function
- [ ] Add `update_deck()` function
- [x] Add `rename_player()` function
- [ ] Add `delete_player()` function
- [ ] Add `list_games()` function
- [ ] Add `delete_dame()` function
### Logging commands
- [ ] Add `player rename` command
- [x] Add `player delete` command
- [ ] Add `deck update` command
- [ ] Add `deck delete` command
- [ ] Add `game list` command
- [ ] Add `game delete` command
### Stats stuff
I'll think about it when the logging commands are finished

## Contributors
Just me for now! If you're interested, fork the project and make all the pull requests you want and you'll be listed here!

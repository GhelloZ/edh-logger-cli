<!-- GitAds-Verify: K5ELW3H2XRUO1Q34LJ73XEA5FRMBMHJM -->
## GitAds Sponsored
[![Sponsored by GitAds](https://gitads.dev/v1/ad-serve?source=ghelloz/edhstats@github)](https://gitads.dev/v1/ad-track?source=ghelloz/edhstats@github)
> Id the ad bothers you, just install and turn on an adblocker, or turn it off to give me some small money

Simple Go CLI tool to log MTG Commander games and get stats for both decks and players. 
When it'll be complete I'll worry about writing a proper README.md file. 

# Dependencies
This package has no runtime dependencies aside `glibc`, but it has some dependencies for compiling:
- gcc
- go
- make
You must install these manually, otherwise the package can't be compiled

# Installation
For now the package is only available in the AUR so use you're AUR helper of choice to install it
```bash
yay -S edh-logger-cli
```
# Build
1. First of all, clone the repo (use the release branch for a probably stable version)
2. `cd` into it
3. run `make`. the `edh` binary will be built in `./build/edh`, you may move it wherever you'd like in your PATH to make it accessible everywhere
```bash
git clone https://github.com/GhelloZ/edh-logger-cli/tree/releases.git
cd edh-logger-cli
make
# Optional but highly encouraged
# use sudo if it doesn't work
mv build/edh /usr/bin/
```

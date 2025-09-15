<!-- GitAds-Verify: K5ELW3H2XRUO1Q34LJ73XEA5FRMBMHJM -->
![banner](https://github.com/GhelloZ/edh-logger-cli/blob/42940ebf69a69de04c99710a8b8a72bfe165f345/metadata/banner.png)

## GitAds Sponsored
[![Sponsored by GitAds](https://gitads.dev/v1/ad-serve?source=ghelloz/edhstats@github)](https://gitads.dev/v1/ad-track?source=ghelloz/edhstats@github)

Simple Go CLI tool to log MTG Commander games and get stats for both decks and players. 
When it'll be complete I'll worry about writing a proper README.md file. 

<!--
# Usage
![Demo](link-to-demo.gif)
blah blah use it like this blah blah
-->
# Dependencies
## Compiling dependencies
- [gcc](https://gcc.gnu.org/install/)
- [go](https://go.dev/doc/install/source)
- [make](https://www.gnu.org/software/make/#download)
## Runtime dependencies
- [sqlite](https://sqlite.org/src/doc/trunk/README.md)
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

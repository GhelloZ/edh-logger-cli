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
- [gcc](https://gcc.gnu.org/install/)
- [go](https://go.dev/doc/install/source)
- [make](https://www.gnu.org/software/make/#download)
- [sqlite](https://sqlite.org/src/doc/trunk/README.md)
- [libcurl](https://curl.se/docs/install.html)
You must install these manually, otherwise the package can't be compiled

# Installation
## AUR
For now the package is only available in the AUR so use you're AUR helper of choice to install it
```bash
yay -S edh-logger-cli
```
## Github Release
If you're not an Arch Linux user, you can simply pull the binary from the releases page, you can either do it in a visual way by downloading the file from the web page and placing it in the PATH or do everthing from the terminal
### Terminal
```bash
# Get the latest version number
VERSION=v$(https://raw.githubusercontent.com/GhelloZ/edh-logger-cli/refs/heads/releases/metadata/VERSION)
# Dowload the binary from the latest Github release
wget "https://github.com/GhelloZ/edh-logger-cli/releases/download/${VERSION}/edh
# Move it to the path to be able to use it system-wide
# Use sudo if it doesn't work
mv edh /usr/bin
```
### Graphical way
1. Go to the [releases page](https://github.com/GhelloZ/edh-logger-cli/releases)
2. Download the binary file named `edh` from the latest or desired release
3. Place the downloaded file in a folder in the path (like `/usr/bin`) to use it system-wide
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

#!bin/bash

echo "Downloading binary release v$(cat metadata/VERSION)"
curl -o edh https://github.com/GhelloZ/edh-logger-cli/releases/download/v$(cat metadata/VERSION)/edh

echo "Moving binary to /usr/local/bin"
sudo -v
sudo mv edh /usr/local/bin/
sudo -K
echo "edh-logger-cli is fully installed"

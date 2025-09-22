#!/bin/bash
set -e
TMP_DIR=$(mktemp -d)
git clone --depth=1 https://github.com/DaveGamble/cJSON.git "$TMP_DIR"
cp "$TMP_DIR/cJSON.c" lib/cjson/
cp "$TMP_DIR/cJSON.h" lib/cjson/
cp "$TMP_DIR/cJSON_Utils.c" lib/cjson/
cp "$TMP_DIR/cJSON_Utils.h" lib/cjson/
rm -rf "$TMP_DIR"
echo "cJSON updated!"

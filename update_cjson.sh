#!/bin/bash
set -euo pipefail

# Directory where you want to put cJSON
DEST_DIR="lib"
mkdir -p "$DEST_DIR"

# Temp directory for cloning
TMP_DIR=$(mktemp -d)
trap 'rm -rf "$TMP_DIR"' EXIT

# Clone tags only (no full history)
git clone --quiet --depth=1 --branch "$(git ls-remote --tags --sort=v:refname https://github.com/DaveGamble/cJSON.git | tail -n1 | sed 's/.*refs\/tags\///')" https://github.com/DaveGamble/cJSON.git "$TMP_DIR"

# Copy the files you need
cp "$TMP_DIR/cJSON.c" "$DEST_DIR/"
cp "$TMP_DIR/cJSON.h" "$DEST_DIR/"

cp "$TMP_DIR/cJSON_Utils.c" "$DEST_DIR/"
cp "$TMP_DIR/cJSON_Utils.h" "$DEST_DIR/"

echo "✅ cJSON updated to latest release: $(basename "$(git -C "$TMP_DIR" describe --tags --abbrev=0)")"

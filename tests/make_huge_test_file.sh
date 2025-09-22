#!/bin/bash
OUTPUT_FILE="huge_test_file.txt"
TARGET_SIZE_GB=5

rm -f "$OUTPUT_FILE"

LINES_PER_BLOCK=10000  # adjust for speed
LINE="1x Arcane Signet"

# Estimate roughly how many blocks we need
BLOCK=$(yes "$LINE" | head -n $LINES_PER_BLOCK | tr '\n' '\n') # ensures proper newlines

CURRENT_SIZE=0
TARGET_BYTES=$((TARGET_SIZE_GB * 1024 * 1024 * 1024))

while [ $CURRENT_SIZE -lt $TARGET_BYTES ]; do
    yes "$LINE" | head -n $LINES_PER_BLOCK >> "$OUTPUT_FILE"
    CURRENT_SIZE=$(stat -c%s "$OUTPUT_FILE")
done

echo "File created: $OUTPUT_FILE (~${TARGET_SIZE_GB}GB)"


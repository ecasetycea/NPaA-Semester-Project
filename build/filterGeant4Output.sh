#!/usr/bin/env bash

# Usage:
# ./filter.sh input.txt "SPECIAL_KEY"

INPUT_FILE="$1"
#KEY="$2"
KEY="start_split"

# Temporary output file
TMP_FILE="$(mktemp)"

keep_next=0

while IFS= read -r line; do
    # If we are currently keeping lines
    if [[ $keep_next -gt 0 ]]; then
        echo "$line" >> "$TMP_FILE"
        ((keep_next--))
        continue
    fi

    # If the line contains the special key
    if [[ "$line" == *"$KEY"* ]]; then
        # Do NOT keep this line itself
        # Keep the next 5 lines
        keep_next=1
    fi

done < "$INPUT_FILE"

# Replace original file
mv "$TMP_FILE" "$INPUT_FILE"

echo "Done."

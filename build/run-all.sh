#!/bin/bash

# Check if argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <slabThickness+Unit>"
    exit 1
fi

# Directory containing the .mac files
MACRO_DIR="./macro"
RESULT_DIR="./results"
FORMATTED_DIR="./resultsFinal"
CUSTOM_STR="$1"

# Loop over all .mac files in the directory
for macfile in "$MACRO_DIR"/*.mac; do
    # Extract the base filename without extension
    base=$(basename "$macfile" .mac)

    # Run the command and redirect output to a new file
    ./exampleB1 "$macfile" > "$RESULT_DIR/${base}-$CUSTOM_STR.txt"

    # filter file
    ./filterGeant4Output.sh "$RESULT_DIR/${base}-$CUSTOM_STR.txt"

    # format file
    ./formatOutputFile.sh "$RESULT_DIR/${base}-$CUSTOM_STR.txt" "$FORMATTED_DIR/${base}-$CUSTOM_STR.txt"

    # Optional: print status
    echo "Processed $macfile -> ${base}-$CUSTOM_STR.txt"
done

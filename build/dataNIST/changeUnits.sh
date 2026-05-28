#!/usr/bin/env bash

# Usage: ./script.sh input.txt multiplier

input_file="$1"
factor="$2"

if [[ -z "$input_file" || -z "$factor" ]]; then
  echo "Usage: $0 input.txt multiplier"
  exit 1
fi

# Print first 8 lines unchanged
head -n 8 "$input_file"

# Process remaining lines
tail -n +9 "$input_file" | awk -v f="$factor" '{
  $2 = $2 * f;
  print;
}'

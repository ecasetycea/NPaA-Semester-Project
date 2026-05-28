#!/bin/bash

# Hardcoded header
HEADER="# E(MeV) DEDX(MeV/cm) δDEDX(MeV/cm)"

# Check arguments
if [ $# -lt 1 ]; then
    echo "Usage: $0 <input_file> [output_file]"
    echo "If output_file is omitted, output goes to stdout."
    exit 1
fi

input_file="$1"
output_file="$2"

if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found."
    exit 1
fi

# Output the header
if [ -n "$output_file" ]; then
    echo "$HEADER" > "$output_file"
else
    echo "$HEADER"
fi

# Generate the energy points and pair them with the input lines
awk -v start=0.00009 -v end=10000 -v outfile="$output_file" '
BEGIN {
    # Build the array of energies exactly as in createGeant4Macro.sh
    start_exp = int(log(start)/log(10));
    end_exp   = int(log(end)/log(10));
    idx = 0;
    for (expon = start_exp; expon <= end_exp; expon++) {
        step = 10^expon;
        for (n = 1; n <= 9; n++) {
            energy = n * step;
            if (energy >= start && energy <= end) {
                idx++;
                energy_arr[idx] = energy;
            }
        }
    }
    total = idx;
}
{
    if (FNR <= total) {
        line = sprintf("%g %s", energy_arr[FNR], $0);
        if (outfile != "") {
            print line >> outfile;
        } else {
            print line;
        }
        # Progress every 10 lines (to stderr so it does not mix with data)
        #if (FNR % 10 == 0) {
            printf "Progress: %d / %d (%.3f MeV)\n", FNR, total, energy_arr[FNR] > "/dev/stderr";
        #}
    } else if (FNR == total + 1) {
        printf "Warning: input file has more lines than energy points (%d). Extra lines ignored.\n", total > "/dev/stderr";
    }
}
END {
    if (outfile != "") {
        printf "Output written to '\''%s'\''\n", outfile > "/dev/stderr";
    }
}
' "$input_file"

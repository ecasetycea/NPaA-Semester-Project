#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <particle>"
    echo "Example: $0 proton"
    exit 1
fi

particle="$1"
output_file="${particle}.mac"
start_energy=0.00009 # actually 0.0001, weird rounding error
end_energy=10000

# Write macro header
{
    echo "/run/initialize"
    echo "/control/verbose 0"
    echo "/run/verbose 0"
    echo "/event/verbose 0"
    echo "/tracking/verbose 0"
    echo "/process/verbose 0"
    echo "/vis/verbose errors"
    echo "#"
    echo "# PARTICLE"
    echo "/gun/particle ${particle}"
    echo "#"
} > "$output_file"

# --- Count how many energies will be generated ---
total_steps=$(awk -v start="$start_energy" -v end="$end_energy" '
BEGIN {
    count = 0;
    start_exp = int(log(start)/log(10));
    end_exp   = int(log(end)/log(10));
    for (expon = start_exp; expon <= end_exp; expon++) {
        step = 10^expon;
        for (n = 1; n <= 9; n++) {
            energy = n * step;
            if (energy >= start && energy <= end) count++;
        }
    }
    print count;
}')

echo "Creating $output_file with $total_steps energy points ..."

# --- Generate energies with exact values and write macro ---
awk -v start="$start_energy" -v end="$end_energy" '
BEGIN {
    start_exp = int(log(start)/log(10));
    end_exp   = int(log(end)/log(10));
    for (expon = start_exp; expon <= end_exp; expon++) {
        step = 10^expon;
        for (n = 1; n <= 9; n++) {
            energy = n * step;
            if (energy >= start && energy <= end) {
                printf "%g\n", energy;
            }
        }
    }
}' | {
    i=0
    while read -r energy; do
        i=$((i + 1))
        {
            echo "/control/echo \"run $i\""
            echo "/gun/energy ${energy} MeV"
            echo "/run/beamOn 10000"
        } >> "$output_file"

        #if (( i % 10 == 0 )); then
            printf "Progress: %d / %d (%.3f MeV)\n" "$i" "$total_steps" "$energy" >&2
        #fi
    done
}

echo "" >> $output_file

echo "Done. Macro file '$output_file' created with $total_steps energy steps."

#!/bin/bash

# list of particle names
particles=("e+" "e-" "mu+" "mu-" "pi+" "pi-" "proton" "anti_proton" "alpha" "deuteron" "triton" "kaon+" "kaon-" "kaon0" "anti_kaon0")

for p in "${particles[@]}"; do
    # find files that start with the particle name followed by -
    files=( "$p"-*.txt )

    # skip if no files found
    [ ${#files[@]} -eq 0 ] && continue

    # join files with space
    file_list="${files[*]}"

    # call gnuplot with particle and files
    echo "Plotting $p"
    gnuplot -e "particle='${p}'; files='${file_list}'" plot.gp
done

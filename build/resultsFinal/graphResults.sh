#!/bin/bash

# list of particle names
particles=("e-" "proton" "alpha")

for p in "${particles[@]}"; do
    # find files that start with the particle name followed by -
    files=( "$p"-*um.txt )
    files2=( "$p"-*-extra.txt )

    # skip if no files found
    [ ${#files[@]} -eq 0 ] && continue
    [ ${#files[@]} -eq 0 ] && continue

    # join files with space
    file_list="${files[*]}"
    file_list2="${files2[*]}"

    # call gnuplot with particle and files
    echo "Plotting $p"
    gnuplot -e "particle='${p}'; files='${file_list}'" plot.gp
    gnuplot -e "particle='${p} (extra accuracy)'; files='${file_list2}'" plot.gp
done

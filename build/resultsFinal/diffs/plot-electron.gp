fout = "ESTAR-errors"
title = "Difference between ESTAR data and Geant4 results"

set terminal pngcairo size 1000,700
set output sprintf("%s.png", fout)
set title sprintf("%s", title)
set grid

set logscale x
set logscale y
set xlabel "E (MeV)"
set ylabel "δ(dE/dx)/(dE/dx) (%)"

plot \
    "ESTAR.txt" using 1:(100 * abs($3-$2) / $2) with lines title "τ=0.1μm", \
    "ESTAR.txt" using 1:(100 * abs($4-$2) / $2) with lines title "τ=100μm", \
    31.73 with lines title "σ", \
    4.55 with lines title "2σ", \
    0.27 with lines title "3σ"

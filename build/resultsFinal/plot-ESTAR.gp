fout = "ESTAR-comparison"
title = "Comparison between ESTAR data and Geant4 results (electron)"

set terminal pngcairo size 1000,700
set output sprintf("%s.png", fout)
set title sprintf("%s", title)
set grid

set logscale x
set logscale y
set xlabel "E (MeV)"
set ylabel "dE/dx (MeV/cm)"

plot \
    "ESTAR.txt"         using 1:2   with lines  title "ESTAR", \
    "e--0.1um.txt"   using 1:2:3 with yerror title "τ=0.1μm", \
    "e--1um.txt"     using 1:2:3 with yerror title "τ=1μm", \
    "e--10um.txt"    using 1:2:3 with yerror title "τ=10μm", \
    "e--100um.txt"   using 1:2:3 with yerror title "τ=100μm"

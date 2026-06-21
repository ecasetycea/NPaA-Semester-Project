fout = "ASTAR-comparison-extra"
title = "Comparison between ASTAR data and Geant4 results (extra physics list)"

set terminal pngcairo size 1000,700
set output sprintf("%s.png", fout)
set title sprintf("%s", title)
set grid

set logscale x
set logscale y
set xlabel "E (MeV)"
set ylabel "dE/dx (MeV/cm)"

plot \
    "ASTAR.txt"         using 1:2   with lines  title "ASTAR", \
    "alpha-0.1um-extra.txt"   using 1:2:3 with yerror title "τ=0.1μm", \
    "alpha-1um-extra.txt"     using 1:2:3 with yerror title "τ=1μm", \
    "alpha-10um-extra.txt"    using 1:2:3 with yerror title "τ=10μm", \
    "alpha-100um-extra.txt"   using 1:2:3 with yerror title "τ=100μm"

fout = "PSTAR-comparison"
title = "Comparison between PSTAR data and Geant4 results (proton)"

set terminal pngcairo size 1000,700
set output sprintf("%s.png", fout)
set title sprintf("%s", title)
set grid

set logscale x
set logscale y
set xlabel "E (MeV)"
set ylabel "dE/dx (MeV/cm)"

plot \
    "PSTAR.txt"         using 1:2   with lines  title "PSTAR", \
    "proton-0.1um.txt"   using 1:2:3 with yerror title "τ=0.1μm", \
    "proton-1um.txt"     using 1:2:3 with yerror title "τ=1μm", \
    "proton-10um.txt"    using 1:2:3 with yerror title "τ=10μm", \
    "proton-100um.txt"   using 1:2:3 with yerror title "τ=100μm"

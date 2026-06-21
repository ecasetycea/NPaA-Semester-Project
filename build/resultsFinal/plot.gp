set terminal pngcairo size 1000,700
set output sprintf("%s.png", particle)

set title sprintf("Particle: %s", particle)
set grid
#set key outside

# split the files string into individual files
filearray = files

# gnuplot "for" loop
set logscale x
set logscale y
set xlabel "E (MeV)"
set ylabel "dE/dx (MeV/cm)"
plot for [f in filearray] f using 1:2:3 with yerror title f

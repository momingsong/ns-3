set terminal png
set output "ec-".mpm."-latency.png" 

set xlabel "K"
set ylabel "Latency"
set xrange [0:11]
set yrange [0:3]

set key center at 9,0.8

plot "ec-".mpm."-latency.data" \
using 1:2 title "M=1" with linespoints, \
"" using 1:3 title "M=2" with linespoints, \
"" using 1:4 title "M=3" with linespoints
set terminal x11

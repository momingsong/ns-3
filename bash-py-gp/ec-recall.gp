set terminal png
set output "ec-".mpm."-recall.png" 

set xlabel "K"
set ylabel "Recall"
set xrange [0:11]
set yrange [0:1]

set key center at 9,0.2

plot "ec-".mpm."-recall.data" \
using 1:2 title "M=1" with linespoints, \
"" using 1:3 title "M=2" with linespoints, \
"" using 1:4 title "M=3" with linespoints
set terminal x11

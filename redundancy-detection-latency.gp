set terminal png
set output "redundancy-latency.png" 

set xlabel "Data Amount"
set ylabel "Latency (s)"
set xrange [0:60000]
set yrange [0:3]

set key center at 10000,2.5

plot "redundancy-latency.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 1:3 title "Redundancy=2" with linespoints, \
"" using 1:4 title "Redundancy=3" with linespoints, \
"" using 1:5 title "Redundancy=4" with linespoints, \
"" using 1:6 title "Redundancy=5" with linespoints
set terminal x11

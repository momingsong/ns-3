set terminal png
set output "redundancy_detection-latency.png" 

set xlabel "Data Amount"
set ylabel "Latency (s)"
set xrange [0:60000]
set yrange [0:2]

set key center at 50000,0.5

plot "redundancy_detection-latency.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 1:3 title "Redundancy=2" with linespoints, \
"" using 1:4 title "Redundancy=3" with linespoints, \
"" using 1:5 title "Redundancy=4" with linespoints, \
"" using 1:6 title "Redundancy=5" with linespoints
set terminal x11

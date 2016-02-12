set terminal png
set output "ack-latency.png" 

set xlabel "Timeout time (s)"
set ylabel "Latency (s)"
set xrange [0:1.1]
set yrange [0:25]

set key center at 0.7,7

plot "ack-latency.data" \
using 1:3 title "Max transmitting=1" with linespoints, \
"" using 1:4 title "Max transmitting=2" with linespoints, \
"" using 1:5 title "Max transmitting=3" with linespoints, \
"" using 1:6 title "Max transmitting=4" with linespoints, \
"" using 1:7 title "Max transmitting=5" with linespoints, \
"" using 1:8 title "Max transmitting=6" with linespoints, \
"" using 1:9 title "Max transmitting=7" with linespoints, \
"" using 1:10 title "Max transmitting=8" with linespoints, \
"" using 1:11 title "Max transmitting=9" with linespoints, \
"" using 1:12 title "Max transmitting=10" with linespoints
set terminal x11
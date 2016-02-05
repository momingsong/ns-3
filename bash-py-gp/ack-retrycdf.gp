set terminal png
set output "ack-retrycdf.png" 

set xlabel "Number of transmitting"
set ylabel "CDF"
set xrange [0:10]
set yrange [0:1]


set key center at 7,0.25

plot "ack-retrycdf.data" using 1:2 title "Max transmitting=1" with linespoints, \
"" using 3:4 title "Max transmitting=2" with linespoints, \
"" using 5:6 title "Max transmitting=3" with linespoints, \
"" using 7:8 title "Max transmitting=4" with linespoints, \
"" using 9:10 title "Max transmitting=5" with linespoints, \
"" using 11:12 title "Max transmitting=6" with linespoints, \
"" using 13:14 title "Max transmitting=7" with linespoints, \
"" using 15:16 title "Max transmitting=8" with linespoints, \
"" using 17:18 title "Max transmitting=9" with linespoints, \
"" using 19:20 title "Max transmitting=10" with linespoints
set terminal x11

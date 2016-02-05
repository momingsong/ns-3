set terminal png
set output "collision_avoidance-latency.png" 

set xlabel "Max Backoff Time (s)"
set ylabel "Latency (s)"
set xrange [0:3]
set yrange [0:26]


set key center at 2.5,6

plot "collision_avoidance-latency.data" using 1:2 title "Max MpM=200" with linespoints, \
"" using 1:3 title "Max MpM=400" with linespoints, \
"" using 1:4 title "Max MpM=600" with linespoints, \
"" using 1:5 title "Max MpM=800" with linespoints, \
"" using 1:6 title "Max MpM=1000" with linespoints, \
"" using 1:7 title "No Limits" with linespoints
set terminal x11

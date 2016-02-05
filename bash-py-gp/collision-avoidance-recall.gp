set terminal png
set output "collision_avoidance-recall.png" 

set xlabel "Max Backoff Time (s)"
set ylabel "Recall"
set xrange [0:3]
set yrange [0:1]


set key center at 2.5,0.2

plot "collision_avoidance-recall.data" using 1:2 title "Max MpM=100" with linespoints, \
"" using 1:3 title "Max MpM=200" with linespoints, \
"" using 1:4 title "Max MpM=300" with linespoints, \
"" using 1:5 title "Max MpM=400" with linespoints, \
"" using 1:6 title "Max MpM=500" with linespoints, \
"" using 1:7 title "Max MpM=600" with linespoints, \
"" using 1:8 title "No Limits" with linespoints
set terminal x11

set terminal png
set output "collision_avoidance-message-size-all.png" 

set xlabel "Max Backoff Time (s)"
set ylabel "Message Overall Size (Bytes)"
set xrange [0:3]
set yrange [0:2500000]

set key center at 2.5,500000

plot "collision_avoidance-message.data" using 1:2 title "Max MpM=100" with linespoints, \
"" using 1:3 title "Max MpM=200" with linespoints, \
"" using 1:4 title "Max MpM=300" with linespoints, \
"" using 1:5 title "Max MpM=400" with linespoints, \
"" using 1:6 title "Max MpM=500" with linespoints, \
"" using 1:7 title "Max MpM=600" with linespoints, \
"" using 1:8 title "No Limits" with linespoints
set terminal x11

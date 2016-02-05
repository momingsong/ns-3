#set title "Recall by revceive data packet"
set terminal png

set xlabel "Receive/Send ratio for each message"
set ylabel "CDF"
set xrange [0:8]
set yrange [0:1]

set key center at 1.5,0.8
set output "collision_avoidance-rsratio-hopnonce.png"

plot "collision_avoidance-rsratio-hopnonce.data" using 1:2 title "MPM=100" with linespoints, \
"" using 3:4 title "MPM=200" with linespoints, \
"" using 5:6 title "MPM=300" with linespoints, \
"" using 7:8 title "MPM=400" with linespoints, \
"" using 9:10 title "MPM=500" with linespoints, \
"" using 11:12 title "MPM=600" with linespoints, \
"" using 13:14 title "No Limit" with linespoints
set terminal x11

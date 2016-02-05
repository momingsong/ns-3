#set title "Recall by revceive data packet"
set terminal png

set xlabel "Receive/Send ratio for each message"
set ylabel "CDF"
set xrange [0:8]
set yrange [0:1]

set key center at 1.5,0.8
set output "collision_avoidance-rsratio-hopnonce.png"

plot "collision_avoidance-rsratio-hopnonce.data" using 1:2 title "Backoff Time=0.0" with linespoints, \
"" using 3:4 title "Backoff Time=0.5" with linespoints, \
"" using 5:6 title "Backoff Time=1.0" with linespoints, \
"" using 7:8 title "Backoff Time=1.5" with linespoints, \
"" using 9:10 title "Backoff Time=2.0" with linespoints, \
"" using 11:12 title "Backoff Time=2.5" with linespoints, \
"" using 13:14 title "Backoff Time=3.0" with linespoints
set terminal x11

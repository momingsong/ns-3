#set title "Recall by revceive data packet"
set terminal png

set xlabel "Receive/Send ratio for each message"
set ylabel "CDF"
set xrange [0:8]
set yrange [0:1]

set key center at 1.5,0.8
set output "redundancy_detection-rsratio.png"

plot "redundancy_detection-rsratio-nonce.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 3:4 title "Redundancy=2" with linespoints, \
"" using 5:6 title "Redundancy=3" with linespoints, \
"" using 7:8 title "Redundancy=4" with linespoints, \
"" using 9:10 title "Redundancy=5" with linespoints
set terminal x11

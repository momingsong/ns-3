set terminal png
set output "baseline-recvmsgnew.png" 

set xlabel "Number of new metadata entries carried by each message"
set ylabel "Message Reveived by Consumer(Time order)"
set xrange [0:40]
set yrange [0:3000]

set key center at 30,2500

plot "baseline-recvmsgnew.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 1:3 title "Redundancy=2" with linespoints, \
"" using 1:4 title "Redundancy=3" with linespoints, \
"" using 1:5 title "Redundancy=4" with linespoints, \
"" using 1:6 title "Redundancy=5" with linespoints
set terminal x11

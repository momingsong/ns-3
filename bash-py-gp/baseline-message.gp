set terminal png
set output "baseline-message-size-all.png" 

set xlabel "Data Amount"
set ylabel "Message Overall Size (Bytes)"
set xrange [0:60000]
set yrange [0:2500000]

set key center at 12000,1500000

plot "baseline-msgsize.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 1:3 title "Redundancy=2" with linespoints, \
"" using 1:4 title "Redundancy=3" with linespoints, \
"" using 1:5 title "Redundancy=4" with linespoints, \
"" using 1:6 title "Redundancy=5" with linespoints
set terminal x11

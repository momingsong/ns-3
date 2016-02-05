set terminal png
set output "ec-recall.png" 

set xlabel "Block Size(Bytes)"
set ylabel "Recall"
set xrange [0:1100]
set yrange [0:1]

set key center at 800,0.2

plot "ec-recall.data" \
using 1:2 title "Redundancy rate=10%" with linespoints, \
"" using 1:3 title "Redundancy rate=20%" with linespoints, \
"" using 1:4 title "Redundancy rate=30%" with linespoints, \
"" using 1:5 title "Redundancy rate=40%" with linespoints, \
"" using 1:6 title "Redundancy rate=50%" with linespoints
set terminal x11

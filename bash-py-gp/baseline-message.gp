set terminal png
set output "baseline-msgsize.png" 

set xlabel "Data amount"
set ylabel "Message overall size(Bytes)"
set xrange [0:60000]
set yrange [0:8000000]

set key center at 12000,6000000

plot "baseline-msgsize.data" using 1:2 title "Redundancy=1" with linespoints, \
"" using 1:3 title "Redundancy=2" with linespoints, \
"" using 1:4 title "Redundancy=3" with linespoints, \
"" using 1:5 title "Redundancy=4" with linespoints, \
"" using 1:6 title "Redundancy=5" with linespoints
set terminal x11

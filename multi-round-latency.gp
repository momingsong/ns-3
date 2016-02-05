set terminal png
set output "multi_round-latency.png" 

set xlabel "TH_R"
set ylabel "Latency (s)"
set xrange [0:0.5]
set yrange [0:20]


set key center at 0.4,3

plot "multi_round-latency.data" using 1:2 title "TH_D=0" with linespoints, \
"" using 1:3 title "TH_D=0.1" with linespoints, \
"" using 1:4 title "TH_D=0.2" with linespoints, \
"" using 1:5 title "TH_D=0.3" with linespoints, \
"" using 1:6 title "TH_D=0.4" with linespoints
set terminal x11
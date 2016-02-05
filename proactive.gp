set terminal png
set output "proactive.png" 

set xlabel "Average Number of Messages"
set ylabel "Latency (s)"
set xrange [0:1100]
#set yrange [0:20]

unset key

#set key center at 0.4,3

plot "proactive.data" using 1:2 with linespoints

set terminal x11
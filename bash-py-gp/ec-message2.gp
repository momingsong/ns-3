set terminal png
set output "ec-message-size-all2.png" 

set xlabel "Block Size(Bytes)"
set ylabel "Message Overall Size (Bytes)"
set xrange [0:0.6]
set yrange [0:2500000]

set key center at 0.4,600000

plot "ec-message.data" \
using 1:2 title "Block size=100B" with linespoints, \
"" using 1:3 title "Block size=200B" with linespoints, \
"" using 1:4 title "Block size=300B" with linespoints, \
"" using 1:5 title "Block size=400B" with linespoints, \
"" using 1:6 title "Block size=500B" with linespoints, \
"" using 1:7 title "Block size=600B" with linespoints, \
"" using 1:8 title "Block size=700B" with linespoints, \
"" using 1:9 title "Block size=800B" with linespoints, \
"" using 1:10 title "Block size=900B" with linespoints, \
"" using 1:11 title "Block size= 1KB" with linespoints
set terminal x11

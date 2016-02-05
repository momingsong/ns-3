set terminal png
set output "multi_round-message-num.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set yrange [0:110]
set ylabel "Average Number of Messages (s)"

#unset key

plot "multi_round-overhead.data" using 2:xtic(1) title "Multi Round", \
"" using 4:xtic(1) title "Single Round"

set terminal x11

set terminal png
set output "multi_round-message-size.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set yrange [0:17000]
set ylabel "Size of Messages (Byte)"

#unset key

plot "multi_round-overhead.data" using 3:xtic(1) title "Multi Round", \
"" using 5:xtic(1) title "Single Round"

set terminal x11
set terminal png
set output "redundancy_detection-message-num.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set ylabel "Average Number of Messages (s)"

unset key

plot "redundancy_detection-overhead.data" using 2:xtic(1)

set terminal x11

set terminal png
set output "redundancy_detection-message-size.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set yrange [0:6000]
set ylabel "Size of Messages (Byte)"

unset key

plot "redundancy_detection-overhead.data" using 3:xtic(1)

set terminal x11
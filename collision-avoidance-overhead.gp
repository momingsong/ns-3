set terminal png
set output "collision_avoidance-message-num.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set ylabel "Average Number of Messages (s)"

unset key

plot "collision_avoidance-overhead.data" using 2:xtic(1)

set terminal x11

set terminal png
set output "collision_avoidance-message-size.png" 

set style data histogram
set style histogram clustered gap 1
set style fill solid 0.4 border

set yrange [0:4200]
set ylabel "Size of Messages (Byte)"

unset key

plot "collision_avoidance-overhead.data" using 3:xtic(1)

set terminal x11
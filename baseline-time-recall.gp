set terminal png
set output "baseline-time-recall.png" 

set xlabel "Time (s)"
set ylabel "Recall (%)"
set xrange [0:8]
set yrange [0:1]

set key center at 6.5,0.22

plot \
"baseline_da10000_r1_0.data" using 1:($2/10000) title "DA=10000, R=1", \
"baseline_da10000_r3_0.data" using 1:($2/10000) title "DA=10000, R=3", \
"baseline_da10000_r5_0.data" using 1:($2/10000) title "DA=10000, R=5", \
"baseline_da30000_r1_0.data" using 1:($2/30000) title "DA=30000, R=1", \
"baseline_da30000_r3_0.data" using 1:($2/30000) title "DA=30000, R=3", \
"baseline_da30000_r5_0.data" using 1:($2/30000) title "DA=30000, R=5", \
"baseline_da50000_r1_0.data" using 1:($2/50000) title "DA=50000, R=1", \
"baseline_da50000_r3_0.data" using 1:($2/50000) title "DA=50000, R=3", \
"baseline_da50000_r5_0.data" using 1:($2/50000) title "DA=50000, R=5"
set terminal x11
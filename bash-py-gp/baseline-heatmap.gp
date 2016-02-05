set terminal png
set output "basline-heatmap.png"
#set title "Spacial heat map of message number sent/received by each node"
unset key
#set tic scale 0

# Color runs from white to green
#set palette rgbformula -7,2,-7
set cbrange [0:8]
set cblabel "Average Receive/Send ratio for each message"
#unset cbtics

unset xtics 
unset ytics
set xrange[-0.5:9.5]
set yrange[-0.5:9.5]


set view map
splot 'baseline-heatmap.data' matrix with image

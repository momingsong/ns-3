set terminal png
set output "redundancy_detection-heatmap.png"
#set title "Spacial heat map of message number sent/received by each node"
unset key
#set tic scale 0

# Color runs from white to green
#set palette rgbformula -7,2,-7
set cbrange [0:8]
set cblabel "Receive/Send ratio for each node"
#unset cbtics

unset xtics 
unset ytics
set xrange[-0.5:9.5]
set yrange[-0.5:9.5]


set view map
splot 'redundancy_detection-heatmap.data' matrix with image

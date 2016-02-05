set terminal png
set output "heatmap.png"
set title "Spacial heat map of message number sent/received by each node"
unset key
set tic scale 0

# Color runs from white to green
#set palette rgbformula -7,2,-7
set cbrange [0:3]
set cblabel "Ratio Sent/Recved"
unset cbtics

set xrange [-0.5:9.5]
set yrange [-0.5:9.5]


set view map
splot 'baseline.sr.heatmap.data' matrix with image

import os

def plotRecall(base,varys,keys):
	recalls = "set terminal png\nset output \"%s-recall.png\"\nset xlabel \"%s\"\nset ylabel \"Recall\"\
	\nset yrange [0:1] \n\
	plot \"%s-recall.data\" "%(base,keys[0],base)

	b=eval(varys[keys[1]])
	recalls += "using 1:2 title \"%s=%s\" with linespoints, "%(keys[1],str(b[0]))
	for a in xrange(1,len(b)):
		recalls += "\"\" using 1:%d title \"%s=%s\" with linespoints, "%(a+2,keys[1],str(b[a]))

	recallf=open("./recall.gp",'w')
	recallf.write(recalls)
	recallf.close()

	os.system("gnuplot recall.gp")

def plotHeatMap(base,varys,keys):
	for a in eval(varys[keys[0]]):
		for b in eval(varys[keys[1]]):
			heatmaps="set terminal png\n set output \"%s-heatmap-%s%s_%s%s.png\"\n\
			unset key\nset cbrange [0:8]\nset cblabel \"Average Receive/Send ratio for each message\"\n\
			unset xtics\nunset ytics\nset xrange[-0.5:9.5]\nset yrange[-0.5:9.5]\nset view map\n\
			splot '%s-heatmap-%s%s_%s%s.data' matrix with image"%(base,keys[0],str(a),keys[1],str(b),base,keys[0],str(a),keys[1],str(b))
			hf=open("./heatmap.gp",'w')
			hf.write(heatmaps)
			hf.close()
			os.system("gnuplot heatmap.gp")


def plot(base,varys,number):
    k=[]
    for key in varys:
        k.append(key)

    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    plotRecall(base,varys,k)
    plotHeatMap(base,varys,k)
    os.chdir(current)
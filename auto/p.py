import os

def plotSumCurve(base,varys,title):
	recalls = "set terminal png\nset output \"%s-%s.png\"\nset xlabel \"%s\"\nset ylabel \"%s\"\
	\nplot \"%s-%s.data\" "%(base,title,varys[0][0],title,base,title)

	b=eval(varys[1][1])
	recalls += "using 1:2 title \"%s=%s\" with linespoints, "%(varys[1][0],str(b[0]))
	for a in xrange(1,len(b)):
		recalls += "\"\" using 1:%d title \"%s=%s\" with linespoints, "%(a+2,varys[1][0],str(b[a]))

	recallf=open("./%s.gp"%(title),'w')
	recallf.write(recalls)
	recallf.close()

	os.system("gnuplot %s.gp"%(title))

def plotHeatMap(base,varys):
	for a in eval(varys[0][1]):
		for b in eval(varys[1][1]):
			heatmaps="set terminal png\n set output \"%s-heatmap-%s%s_%s%s.png\"\n\
			unset key\nset cbrange [0:8]\nset cblabel \"Average Receive/Send ratio for each message\"\n\
			unset xtics\nunset ytics\nset xrange[-0.5:9.5]\nset yrange[-0.5:9.5]\nset view map\n\
			splot '%s-heatmap-%s%s_%s%s.data' matrix with image"%(base,varys[0][0],str(a),varys[1][0],str(b),base,varys[0][0],str(a),varys[1][0],str(b))
			hf=open("./heatmap.gp",'w')
			hf.write(heatmaps)
			hf.close()
			os.system("gnuplot heatmap.gp")


def plot(base,varys,number):
    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    plotSumCurve(base,varys,"recall")
    plotSumCurve(base,varys,"latency")
    plotHeatMap(base,varys)
    os.chdir(current)
import os
import sys

def plotSumCurve(base,varys,title):
	paramarray=[]
	for lv in xrange(0,len(varys)-2):
		paramarray.append(0)
	while len(paramarray)==0 or paramarray[0] < len(eval(varys[0][1])):
		paramlist=""
		for lv in xrange(0,len(varys)-2):
			kv=eval(varys[lv][1])
			paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv]])))

		if len(paramarray) > 0:
			paramarray[-1]+=1
		for rv in xrange(len(varys)-3,0,-1):
			if paramarray[rv]>=len(eval(varys[rv][1])):
				paramarray[rv]=0
				paramarray[rv-1]+=1



		recalls = "set terminal png\nset output \"%s-%s%s.png\"\nset xlabel \"%s\"\nset ylabel \"%s\"\
		\nplot \"%s-%s%s.sum\" "%(base,title,paramlist,varys[-2][0],title,base,title,paramlist)

		b=eval(varys[-1][1])
		recalls += "using 1:2 title \"%s=%s\" with linespoints, "%(varys[-1][0],str(b[0]))
		for a in xrange(1,len(b)):
			recalls += "\"\" using 1:%d title \"%s=%s\" with linespoints, "%(a+2,varys[-1][0],str(b[a]))

		recallf=open("./%s.gp"%(title),'w')
		recallf.write(recalls)
		recallf.close()

		os.system("gnuplot %s.gp"%(title))
		if len(paramarray)==0:
			paramarray.append(sys.maxint)

def plotHeatMap(base,varys):
	paramarray=[]
	for lv in xrange(0,len(varys)):
		paramarray.append(0)
	while paramarray[0] < len(eval(varys[0][1])):
		paramlist=""
		for lv in xrange(0,len(varys)):
			kv=eval(varys[lv][1])
			paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv]])))

		if len(paramarray) > 0:
			paramarray[-1]+=1
		for rv in xrange(len(varys)-1,0,-1):
			if paramarray[rv]>=len(eval(varys[rv][1])):
				paramarray[rv]=0
				paramarray[rv-1]+=1
		heatmaps="set terminal png\n set output \"%s-heatmap%s.png\"\n\
		unset key\nset cbrange [0:8]\nset cblabel \"Average Receive/Send ratio for each message\"\n\
		unset xtics\nunset ytics\nset xrange[-0.5:9.5]\nset yrange[-0.5:9.5]\nset view map\n\
		splot '%s-heatmap%s.sum' matrix with image"%(base,paramlist,base,paramlist)
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
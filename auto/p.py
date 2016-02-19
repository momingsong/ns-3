import os
import sys

def plotSumCurve(base,varys,title):
	for filename in os.listdir("./"):
		ix =  filename.find('-')
		if filename[ix+1:].split('_')[0]==title and filename[-3:]=="sum":

			fileprefix = filename[:-4]
			recalls = "set terminal png\nset output \"%s.png\"\nset xlabel \"%s\"\nset ylabel \"%s\"\
			\nplot \"%s.sum\" "%(fileprefix,varys[0][0],title,fileprefix)

			b=eval(varys[1][1])
			recalls += "using 1:2 title \"%s=%s\" with linespoints, "%(varys[1][0],str(b[0]))
			for a in xrange(1,len(b)):
				recalls += "\"\" using 1:%d title \"%s=%s\" with linespoints, "%(a+2,varys[1][0],str(b[a]))

			recallf=open("./%s.gp"%(title),'w')
			recallf.write(recalls)
			recallf.close()

			os.system("gnuplot %s.gp"%(title))


def plotPlatencyCurve(base,varys,title):
	for filename in os.listdir("./"):
		ix =  filename.find('-')
		if filename[ix+1:].split('_')[0]==title and filename[-3:]=="sum":

			fileprefix = filename[:-4]
			recalls = "set terminal png\nset output \"%s.png\"\nset xlabel \"%s\"\nset ylabel \"%s\"\
			\nplot \"%s.sum\" "%(fileprefix,varys[0][0],title,fileprefix)

			b=eval(varys[1][1])
			recalls += "using 1:2 title \"50%-Latency\"  with linespoints, "
			recalls += "\"\" using 1:3 title \"60%-Latency\" with linespoints, "
			recalls += "\"\" using 1:4 title \"70%-Latency\" with linespoints, "
			recalls += "\"\" using 1:5 title \"80%-Latency\" with linespoints, "
			recalls += "\"\" using 1:6 title \"85%-Latency\" with linespoints, "
			recalls += "\"\" using 1:7 title \"90%-Latency\" with linespoints, "
			recalls += "\"\" using 1:8 title \"95%-Latency\" with linespoints, "
			recalls += "\"\" using 1:9 title \"100%-Latency\" with linespoints"

			recallf=open("./%s.gp"%(title),'w')
			recallf.write(recalls)
			recallf.close()

			os.system("gnuplot %s.gp"%(title))

def plotHeatMap(base,varys,title):
	for filename in os.listdir("./"):
		ix =  filename.find('-')
		if filename[ix+1:].split('_')[0] == title and filename[-3:]=="sum":

			fileprefix = filename[:-4]

			heatmaps="set terminal png\n set output \"%s.png\"\n\
			unset key\nset cbrange [0:8]\nset cblabel \"Average Receive/Send ratio for each message\"\n\
			unset xtics\nunset ytics\nset xrange[-0.5:9.5]\nset yrange[-0.5:9.5]\nset view map\n\
			splot '%s.sum' matrix with image"%(fileprefix,fileprefix)
			hf=open("./heatmap.gp",'w')
			hf.write(heatmaps)
			hf.close()
			os.system("gnuplot %(title)s.gp"%vars())


def plot(base,varys,number):
    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    plotSumCurve(base,varys,"recall")
    plotSumCurve(base,varys,"latency")
    plotHeatMap(base,varys,"heatmap")
    plotPlatencyCurve(base,varys,"platency")
    os.chdir(current)


'''
	if(len(varys)==1):
		print "sum curve cannot be drawn"
		return

	paramarray=[]
	for lv in xrange(2,len(varys)):
		paramarray.append(0)
	while len(paramarray)==0 or paramarray[0] < len(eval(varys[2][1])):
		paramlist=""
		for lv in xrange(2,len(varys)):
			kv=eval(varys[lv][1])
			paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-2]])))
		if len(paramarray) > 0:
			paramarray[-1]+=1
		for rv in xrange(len(varys)-1,2,-1):
			if paramarray[rv-2]>=len(eval(varys[rv][1])):
				paramarray[rv-2]=0
				paramarray[rv-3]+=1
	'''

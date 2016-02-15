import os
import sys

def RecallAndLatency(typePrefix,varys,params):
    for avgs in xrange(int(params['avg']),0,-1):
        if(len(varys)==1):
            print "recall and latency cannot be drawn"
            return
        paramarray=[]
        #print len(varys)
        for lv in xrange(2,len(varys)):
            paramarray.append(0)
        #print paramarray
        while len(paramarray)==0 or paramarray[0] < len(eval(varys[2][1])):
            paramlist=""
            for lv in xrange(2,len(varys)):
                kv=eval(varys[lv][1])
                paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-2]])))
                params[varys[lv][0]]=kv[paramarray[lv-2]]
            if len(paramarray) > 0:
                paramarray[-1]+=1
            for rv in xrange(len(varys)-1,2,-1):
                if paramarray[rv-2]>=len(eval(varys[rv][1])):
                    paramarray[rv-2]=0
                    paramarray[rv-3]+=1

            recallf = open("./%(typePrefix)s-recall%(paramlist)s&%(avgs)s.sum"%vars(), "w")
            latencyf = open("./%(typePrefix)s-latency%(paramlist)s&%(avgs)s.sum"%vars(), "w")
            recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
            latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
            recallf.write("#%s %s\n"%(varys[1][0],varys[1][1]))
            latencyf.write("#%s %s\n"%(varys[1][0],varys[1][1]))

            
            
            for a in eval(varys[0][1]):
                params[varys[0][0]] = a
                recallf.write(str(a)+" ")
                latencyf.write(str(a)+" ")
                for b in eval(varys[1][1]):
                    params[varys[1][0]] = b
                    file = open("./%s_%s%s_%s%s%s&%s_0.data"%(typePrefix,varys[0][0],str(a),varys[1][0],str(b),paramlist,avgs))
                    line = file.readlines()[-1].split()
                    recallf.write(str(float(line[1])/float(params['daa']))+" ")
                    latencyf.write(line[0]+" ")
                    file.close()
                recallf.write("\n")
                latencyf.write("\n")
            recallf.close()
            latencyf.close()
            if len(paramarray)==0:
                break

def RSRHeatmap(typePrefix, varys, params):
    for avgs in xrange(int(params['avg']),0,-1):
        paramarray=[]
        #print len(varys)
        for lv in xrange(0,len(varys)):
            paramarray.append(0)
        #print paramarray
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

            sendList = []
            recvList = []
            ratiolist = []
            for i in xrange(100):
                sendList.append([])
                recvList.append([])
                ratiolist.append(0)
            for logcontent in ['SI','SD']:
                file = open("./%s%s&%s_%s.data"%(typePrefix,paramlist,avgs,logcontent))
                for line in file:
                    if(line[0:2] == logcontent):
                        info = line.split(" ")
                        hopnonce = 0
                        for x in info:
                            if x[0:2] == "Ho":
                                hopnonce = int(x.split(":")[1])
                        if hopnonce != 0:
                            sendList[int(info[1])].append(hopnonce)
                file.close()
            for logcontent in ['RI','DRD']:
                file = open("./%s%s&%s_%s.data"%(typePrefix,paramlist,avgs,logcontent))
                for line in file:
                    if(line[0:2] == logcontent or line[0:3] == logcontent):
                        info = line.split(" ")
                        hopnonce = 0
                        for x in info:
                            if x[0:2] == "Ho":
                                hopnonce = int(x.split(":")[1])
                        if hopnonce != 0:
                            recvList[int(info[1])].append(hopnonce)
                file.close()
            for i in xrange(100):
                for x in sendList[i]:
                    recv  = 0
                    for ki in [-11,-10,-9,-1,1,9,10,11]:
                        if (i+ki >99 or i+ki<0):
                            continue
                        elif(i %10 == 0 and (ki == -1 or ki == -11 or ki == 9)):
                            continue
                        elif(i % 10 == 9 and (ki == 1 or ki == 11 or ki == -9)):
                            continue
                        recv += recvList[i+ki].count(x)
                    ratiolist[i] += recv
                
                if (len(sendList[i])==0):
                    ratiolist[i] = 0
                else:
                    ratiolist[i] /= float(len(sendList[i]))
            writefile = open("./%s-heatmap%s&%s.sum"%(typePrefix,paramlist,avgs), "w")
            writefile.write("#%s-heatmap%s&%s.sum\n"%(typePrefix,paramlist,avgs))
            writefile.write("#Data for receive send ratio on each ndoe of the %(typePrefix)s\n"%vars())
            writefile.write("#10x10 grid. Each = R/S ratio for the node.\n")
            for i in xrange(10):
                for j in xrange(10):
                    writefile.write(str(ratiolist[i*10 + j])+"\t")
                writefile.write("\n")
            writefile.close()


def average(params):
    imageclass={}
    for files in os.listdir("./"):
        if files.split('.')[-1]=="sum":
            info = files.split('&')
            if(info[-1].split('.')[0].isdigit()):
                if imageclass.has_key(info[-2]):
                    imageclass[info[-2]].append(info[-1])
                else:
                    imageclass[info[-2]]=[]
                    imageclass[info[-2]].append(info[-1])
    for k in imageclass:
        writefile = open("./%s&avg.sum"%k,'w')
        karray = []
        for s in xrange(len(imageclass[k])):
            karray.append([])
            readfile = open("./%s&%s"%(k,imageclass[k][s]))

            ln = 0
            for lines in readfile:
                if lines[0]!="#":
                    karray[s].append([])
                    for numbers in lines.split():
                        karray[s][ln].append(float(numbers))
                    ln+=1
        for a in xrange(1,len(karray)):
            for b in xrange(len(karray[a])):
                for c in xrange(len(karray[a][b])):
                    karray[0][b][c] += karray[a][b][c]
        for b in xrange(len(karray[0])):
            for c in xrange(len(karray[0][b])):
                karray[0][b][c] /= len(karray)
                writefile.write(str(karray[0][b][c]) + ' ')
            writefile.write('\n')
        writefile.close()


            

    

def process(varys,params,number):
    base=params['t']

    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    RecallAndLatency(base,varys,params)
    RSRHeatmap(base,varys,params)
    average(params)
    os.chdir(current)
    

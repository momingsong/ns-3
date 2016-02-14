import os
import sys

def RecallAndLatency(typePrefix,varys,params):
    paramarray=[]
    #print len(varys)
    for lv in xrange(0,len(varys)-2):
        paramarray.append(0)
    #print paramarray
    while len(paramarray)==0 or paramarray[0] < len(eval(varys[0][1])):
        paramlist=""
        for lv in xrange(0,len(varys)-2):
            kv=eval(varys[lv][1])
            paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv]])))
            params[varys[lv][0]]=kv[paramarray[lv]]

        if len(paramarray) > 0:
            paramarray[-1]+=1
        for rv in xrange(len(varys)-3,0,-1):
            if paramarray[rv]>=len(eval(varys[rv][1])):
                paramarray[rv]=0
                paramarray[rv-1]+=1

        recallf = open("./%(typePrefix)s-recall%(paramlist)s.sum"%vars(), "w")
        latencyf = open("./%(typePrefix)s-latency%(paramlist)s.sum"%vars(), "w")
        recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
        latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
        recallf.write("#%s %s\n"%(varys[1][0],varys[1][1]))
        latencyf.write("#%s %s\n"%(varys[1][0],varys[1][1]))

        
        
        for a in eval(varys[-2][1]):
            params[varys[-2][0]] = a
            recallf.write(str(a)+" ")
            latencyf.write(str(a)+" ")
            for b in eval(varys[-1][1]):
                params[varys[-1][0]] = b
                file = open("./%s%s_%s%s_%s%s_0.data"%(typePrefix,paramlist,varys[-2][0],str(a),varys[-1][0],str(b)))
                line = file.readlines()[-1].split()
                recallf.write(str(float(line[1])/float(params['daa']))+" ")
                latencyf.write(line[0]+" ")
                file.close()
            recallf.write("\n")
            latencyf.write("\n")
        recallf.close()
        latencyf.close()

        if len(paramarray)==0:
            paramarray.append(sys.maxint)

def RSRHeatmap(typePrefix, varys):
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
            file = open("./%s%s_%s.data"%(typePrefix,paramlist,logcontent))
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
            file = open("./%s%s_%s.data"%(typePrefix,paramlist,logcontent))
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
        writefile = open("./%s-heatmap%s.sum"%(typePrefix,paramlist), "w")
        writefile.write("#%s-heatmap%s.sum\n"%(typePrefix,paramlist))
        writefile.write("#Data for receive send ratio on each ndoe of the %(typePrefix)s\n"%vars())
        writefile.write("#10x10 grid. Each = R/S ratio for the node.\n")
        for i in xrange(10):
            for j in xrange(10):
                writefile.write(str(ratiolist[i*10 + j])+"\t")
            writefile.write("\n")
        writefile.close()

def process(varys,params,number):
    base=params['t']

    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    RecallAndLatency(base,varys,params)
    RSRHeatmap(base,varys)
    os.chdir(current)
    

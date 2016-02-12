import os

def RecallAndLatency(typePrefix,varys,keys,amount):
    recallf = open("./%(typePrefix)s-recall.data"%vars(), "w")
    latencyf = open("./%(typePrefix)s-latency.data"%vars(), "w")
    recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
    latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
    recallf.write("#%s %s\n"%(keys[1],varys[keys[1]]))
    latencyf.write("#%s %s\n"%(keys[1],varys[keys[1]]))
    
    for a in eval(varys[keys[0]]):
        recallf.write(str(a)+" ")
        latencyf.write(str(a)+" ")
        for b in eval(varys[keys[1]]):
            file = open("./%s_%s%s_%s%s_0.data"%(typePrefix,keys[0],str(a),keys[1],str(b)))
            line = file.readlines()[-1].split()
            recallf.write(str(float(line[1])/float(amount))+" ")
            latencyf.write(line[0]+" ")
            file.close()
        recallf.write("\n")
        latencyf.write("\n")
    recallf.close()
    latencyf.close()

def RSRHeatmap(typePrefix, varys, keys):
    for a in eval(varys[keys[0]]):
        for b in eval(varys[keys[1]]):
            sendList = []
            recvList = []
            ratiolist = []
            for i in xrange(100):
                sendList.append([])
                recvList.append([])
                ratiolist.append(0)
            for logcontent in ['SI','SD']:
                file = open("./%s_%s%s_%s%s_%s.data"%(typePrefix,keys[0],str(a),keys[1],str(b),logcontent))
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
                file = open("./%s_%s%s_%s%s_%s.data"%(typePrefix,keys[0],str(a),keys[1],str(b),logcontent))
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
                    
                ratiolist[i] /= float(len(sendList[i]))
            writefile = open("./%s-heatmap-%s%s_%s%s.data"%(typePrefix,keys[0],str(a),keys[1],str(b)), "w")
            writefile.write("#%s-heatmap-%s%s_%s%s.data\n"%(typePrefix,keys[0],str(a),keys[1],str(b)))
            writefile.write("#Data for receive send ratio on each ndoe of the %(typePrefix)s\n"%vars())
            writefile.write("#10x10 grid. Each = R/S ratio for the node.\n")
            for i in xrange(10):
                for j in xrange(10):
                    writefile.write(str(ratiolist[i*10 + j])+"\t")
                writefile.write("\n")
            writefile.close()

def process(base,varys,number,amount):
    k=[]
    for key in varys:
        k.append(key)

    current = os.path.dirname(__file__)
    aimdir = current + "/%d-%s"%(number,base)
    os.chdir(aimdir)
    RecallAndLatency(base,varys,k,amount)
    RSRHeatmap(base,varys,k)
    os.chdir(current)
    

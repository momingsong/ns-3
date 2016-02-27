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
                if(varys!='trf'):
                    kv=eval(varys[lv][1])
                    paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-2]])))
                    params[varys[lv][0]]=kv[paramarray[lv-2]]
            if len(paramarray) > 0:
                paramarray[-1]+=1
            for rv in xrange(len(varys)-1,2,-1):
                if paramarray[rv-2]>=len(eval(varys[rv][1])):
                    paramarray[rv-2]=0
                    paramarray[rv-3]+=1

            #read multiple consumer infomation
            consumerlist = []
            listfile=open("./%s_%s%s_%s%s%s&%s_list.data"%(typePrefix,varys[0][0],eval(varys[0][1])[0],varys[1][0],eval(varys[1][1])[0],paramlist,avgs))
            for consumer in listfile:
                if consumer[0]!='#':
                    info=consumer.split(' ')
                    consumerlist.append(info[0])

            for il in consumerlist:
                #write file prepare
                recallf = open("./%(typePrefix)s-recall%(paramlist)s_%(il)s&%(avgs)s.sum"%vars(), "w")
                latencyf = open("./%(typePrefix)s-latency%(paramlist)s_%(il)s&%(avgs)s.sum"%vars(), "w")
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
                        file = open("./%s_%s%s_%s%s%s&%s_%s.data"%(typePrefix,varys[0][0],str(a),varys[1][0],str(b),paramlist,avgs,il))
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

def MessageSize(typePrefix, varys, params):
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

            '''        
            #read multiple consumer infomation
            consumerlist = []
            listfile=open("./%s_%s%s_%s%s%s&%s_list.data"%(typePrefix,varys[0][0],eval(varys[0][1])[0],varys[1][0],eval(varys[1][1])[0],paramlist,avgs))
            for consumer in listfile:
                if consumer[0]!='#':
                    info=consumer.split(' ')
                    consumerlist.append(info[0])
            '''

            wf = open("./%(typePrefix)s-message%(paramlist)s&%(avgs)s.sum"%vars(), "w")
            wf.write("#Data for message overall size of the %(typePrefix)s\n"%vars())
            wf.write("#%s %s\n"%(varys[1][0],varys[1][1]))
            num = 100 # may subject to change by the simulation node number
            for a in eval(varys[0][1]):
                params[varys[0][0]] = a
                wf.write(str(a) + " ")
                for b in eval(varys[1][1]):
                    params[varys[1][0]] = b
                    file = open("./%s_%s%s_%s%s%s&%s_overhead.data"%(typePrefix,varys[0][0],str(a),varys[1][0],str(b),paramlist,avgs))
                    for line in file:
                        if line[0] != "#":
                            numbers = line.split(' ')
                            wf.write(numbers[7]+" ")
                wf.write("\n")
                file.close()
            wf.close()
            if len(paramarray)==0:
                break


def  PLatency(typePrefix, varys, params):
    for avgs in xrange(int(params['avg']),0,-1):
        paramarray=[]
        #print len(varys)
        for lv in xrange(1,len(varys)):
            paramarray.append(0)
        #print paramarray
        while len(paramarray)==0 or paramarray[0] < len(eval(varys[1][1])):
            paramlist=""
            for lv in xrange(1,len(varys)):
                kv=eval(varys[lv][1])
                paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-1]])))
                params[varys[lv][0]]=kv[paramarray[lv-1]]
            if len(paramarray) > 0:
                paramarray[-1]+=1
            for rv in xrange(len(varys)-1,1,-1):
                if paramarray[rv-1]>=len(eval(varys[rv][1])):
                    paramarray[rv-1]=0
                    paramarray[rv-2]+=1

            #read multiple consumer infomation
            consumerlist = []
            listfile=open("./%s_%s%s%s&%s_list.data"%(typePrefix,varys[0][0],eval(varys[0][1])[0],paramlist,avgs))
            for consumer in listfile:
                if consumer[0]!='#':
                    info=consumer.split(' ')
                    consumerlist.append(info[0])

            for il in consumerlist:
                platencyf = open("./%(typePrefix)s-platency%(paramlist)s_%(il)s&%(avgs)s.sum"%vars(), "w")
                for a in eval(varys[0][1]):
                    params[varys[0][0]] = a
                    platencyf.write(str(a)+" ")
                    file = open("./%s_%s%s%s&%s_%s.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs,il))
                    order = 0
                    for line in file:
                        if line[0]=='#':
                            continue
                        else:
                            info = line.split(' ')
                            if float(info[1])/float(params['daa']) >= 0.5 and order < 1:
                                platencyf.write(info[0]+" ") 
                                order = 1
                            if float(info[1])/float(params['daa']) >= 0.6 and order < 2:
                                platencyf.write(info[0]+" ")
                                order = 2
                            if float(info[1])/float(params['daa']) >= 0.7 and order < 3:
                                platencyf.write(info[0]+" ")
                                order = 3
                            if float(info[1])/float(params['daa']) >= 0.8 and order < 4:
                                platencyf.write(info[0]+" ")
                                order = 4
                            if float(info[1])/float(params['daa']) >= 0.85 and order < 5:
                                platencyf.write(info[0]+" ") 
                                order = 5
                            if float(info[1])/float(params['daa']) >= 0.90 and order < 6:
                                platencyf.write(info[0]+" ")
                                order = 6
                            if float(info[1])/float(params['daa']) >= 0.95 and order < 7:
                                platencyf.write(info[0]+" ")
                                order = 7
                            if float(info[1])/float(params['daa']) >= 0.999999 and order < 8:
                                platencyf.write(info[0]+" ")
                                order = 8
                    #platencyf.write(line.split(' ')[0] + " ")
                    platencyf.write("\n")
                    file.close()
                platencyf.close()

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

def RSRatioCDF(typePrefix, varys, params):
    for avgs in xrange(int(params['avg']),0,-1):
        paramarray=[]
        #print len(varys)
        for lv in xrange(1,len(varys)):
            paramarray.append(0)
        #print paramarray
        while len(paramarray)==0 or paramarray[0] < len(eval(varys[1][1])):
            paramlist=""
            for lv in xrange(1,len(varys)):
                kv=eval(varys[lv][1])
                paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-1]])))
                params[varys[lv][0]]=kv[paramarray[lv-1]]
            if len(paramarray) > 0:
                paramarray[-1]+=1
            for rv in xrange(len(varys)-1,1,-1):
                if paramarray[rv-1]>=len(eval(varys[rv][1])):
                    paramarray[rv-1]=0
                    paramarray[rv-2]+=1

            writefile = open("%(typePrefix)s-rsratio%(paramlist)s&%(avgs)s.sum"%vars(), "w")
            backofftime = 2.5 # may subject to change by the data amount wanted to observe
            da=50000
            msgcount = {}
            ratiotemp = {}
            ratio = {}
            for a in eval(varys[0][1]):
                params[varys[0][0]] = a
                msgcount[a] = {}
                for logcontent in ['SI','SD']:
                    file = open("./%s_%s%s%s&%s_%s.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs,logcontent))
                    for line in file:
                        if line[0:2] == logcontent:
                            info = line.split(' ')
                            for x in info:
                                if x[0:2] == "Ho":
                                    nonce = x.split(':')[1]
                                    if msgcount[a].has_key(nonce):
                                        msgcount[a][nonce]["s"] += 1
                                    else:
                                        msgcount[a][nonce] = {}
                                        msgcount[a][nonce]["s"] = 1
                                        msgcount[a][nonce]["r"] = 0
                                        msgcount[a][nonce]["rs"] = 0
                for logcontent in ['RI','DRD']:
                    file = open("./%s_%s%s%s&%s_%s.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs,logcontent))
                    for line in file:
                        if line[0:2] == logcontent or line[0:3] == logcontent:
                            info = line.split(' ')
                            for x in info:
                                if x[0:2] == "Ho":
                                    nonce = x.split(':')[1]
                                    if(msgcount[a].has_key(nonce)):
                                        msgcount[a][nonce]["r"] += 1
                                    else:
                                        print logcontent, a, nonce
                for nonce in msgcount[a]:
                    msgcount[a][nonce]['rs'] = float(msgcount[a][nonce]['r']) / float(msgcount[a][nonce]['s'])
                msg = sorted(msgcount[a].iteritems(), key=lambda s: s[1]['rs'])
                for x in range(len(msg)):
                    ratiotemp[msg[x][1]["rs"]] = float(x+1) / len(msg);
                ratio[a] = sorted(ratiotemp.iteritems())
                ratiotemp.clear()
            length = 0
            for ki in ratio:
                length = max(length,len(ratio[ki]))
            for j in range(length):
                for i in eval(varys[0][1]): 
                    if(len(ratio[i])<=j):
                        writefile.write("null null ")
                    else: 
                        writefile.write(str(ratio[i][j][0])+" "+str(ratio[i][j][1])+ " ")
                writefile.write("\n")
            if len(paramarray)==0:
                break


def TimeRecallAndReceive(typePrefix, varys, params):
    for avgs in xrange(int(params['avg']),0,-1):
        paramarray=[]
        #print len(varys)
        for lv in xrange(1,len(varys)):
            paramarray.append(0)
        #print paramarray
        while len(paramarray)==0 or paramarray[0] < len(eval(varys[1][1])):
            paramlist=""
            for lv in xrange(1,len(varys)):
                kv=eval(varys[lv][1])
                paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv-1]])))
                params[varys[lv][0]]=kv[paramarray[lv-1]]
            if len(paramarray) > 0:
                paramarray[-1]+=1
            for rv in xrange(len(varys)-1,1,-1):
                if paramarray[rv-1]>=len(eval(varys[rv][1])):
                    paramarray[rv-1]=0
                    paramarray[rv-2]+=1

            consumerlist = []
            listfile=open("./%s_%s%s%s&%s_list.data"%(typePrefix,varys[0][0],eval(varys[0][1])[0],paramlist,avgs))
            for consumer in listfile:
                if consumer[0]!='#':
                    info=consumer.split(' ')
                    consumerlist.append([info[0],info[1]])

            for il in consumerlist:
                #write file prepare
                gettime = {}
                getdata = {}
                for a in eval(varys[0][1]):
                    params[varys[0][0]] = a
                    gettime[a]=[]
                    getdata[a]=[]
                    file1 = open("./%s_%s%s%s&%s_%s.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs,il[0]))
                    for lines in file1:
                        if lines[0]!="#":
                            info = lines.split(' ')
                            if info[-1][0]=="d" or info[-1][0]=="I":
                                gettime[a].append([info[0],float(info[1])/float(params['daa'])])
                    filed = open("./%s_%s%s%s&%s_DRD.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs))
                    filer = open("./%s_%s%s%s&%s_WRD.data"%(typePrefix,varys[0][0],str(a),paramlist,avgs))
                    msgall= 0
                    for lines in filed:
                        if lines[0:3]=="DRD":
                            info = lines.split(' ')
                            if info[1]==il[1]:
                                msgall += int(info[-1].strip())
                                getdata[a].append([info[2],msgall])
                    filed.close()
                    filer.close()
                    file1.close()

                pt = il[0]
                recallf = open("./%(typePrefix)s-timerecall%(paramlist)s_%(pt)s&%(avgs)s.sum"%vars(), "w")
                recvf = open("./%(typePrefix)s-timerecvmsg%(paramlist)s_%(pt)s&%(avgs)s.sum"%vars(), "w")
                length = 0
                for ki in gettime:
                    length = max(length,len(gettime[ki]))
                for j in range(length):
                    for i in eval(varys[0][1]): 
                        if(len(gettime[i])<=j):
                            recallf.write("null null ")
                        else: 
                            recallf.write(str(gettime[i][j][0])+" "+str(gettime[i][j][1])+ " ")
                    recallf.write("\n")
                recallf.close()
                length = 0
                for ki in getdata:
                    length = max(length,len(getdata[ki]))
                for j in range(length):
                    for i in eval(varys[0][1]): 
                        if(len(getdata[i])<=j):
                            recvf.write("null null ")
                        else: 
                            recvf.write(str(getdata[i][j][0])+" "+str(getdata[i][j][1])+ " ")
                    recvf.write("\n")

            if len(paramarray)==0:
                break

def RoundNumber(typePrefix,varys,params):
    if not "m" in typePrefix:
        print "This is multiround only"
        return
    for avgs in xrange(int(params['avg']),0,-1):
        if(len(varys)==1):
            print "Round Number cannot be drawn"
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

            #read multiple consumer infomation
            consumerlist = []
            listfile=open("./%s_%s%s_%s%s%s&%s_list.data"%(typePrefix,varys[0][0],eval(varys[0][1])[0],varys[1][0],eval(varys[1][1])[0],paramlist,avgs))
            for consumer in listfile:
                if consumer[0]!='#':
                    info=consumer.split(' ')
                    consumerlist.append([info[0],info[1]])

            for il in consumerlist:
                #write file prepare
                ilix = il[0]
                rnf = open("./%(typePrefix)s-roundnumber%(paramlist)s_%(ilix)s&%(avgs)s.sum"%vars(), "w")
                rnf.write("#Round number for  the %(typePrefix)s\n"%vars())
                rnf.write("#%s %s\n"%(varys[1][0],varys[1][1]))

                for a in eval(varys[0][1]):
                    params[varys[0][0]] = a
                    rnf.write(str(a)+" ")
                    for b in eval(varys[1][1]):
                        params[varys[1][0]] = b
                        file = open("./%s_%s%s_%s%s%s&%s_SI.data"%(typePrefix,varys[0][0],str(a),varys[1][0],str(b),paramlist,avgs))
                        rn = 0
                        for line in file:
                            if line[0:2]=="SI":
                                info = line.split(' ')
                            if info[1] == il[1]:
                                rn += 1
                        rnf.write(str(rn)+" ")
                        file.close()
                    rnf.write("\n")
                rnf.close()
            if len(paramarray)==0:
                break


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
        if(k.split('-')[1].split('_')[0] == "rsratio"):
            continue
        if(k.split('-')[1].split('_')[0] == "timerecall"):
            continue
        if(k.split('-')[1].split('_')[0] == "timerecvmsg"):
            continue
        writefile = open("./%s&avg.sum"%k,'w')
        karray = []
        for s in xrange(len(imageclass[k])):
            karray.append([])
            readfile = open("./%s&%s"%(k,imageclass[k][s]))
	    print k

            ln = 0
            for lines in readfile:
                if lines[0]!="#":
                    karray[s].append([])
                    for numbers in lines.split():
                        #print k
                        if numbers.isdigit():
                            karray[s][ln].append(float(numbers))
                        else:
                            karray[s][ln].append(numbers)
                    ln+=1
        for a in xrange(1,len(karray)):
            for b in xrange(len(karray[a])):
                for c in xrange(len(karray[a][b])):
                    if c>=len(karray[0][b]) or b>=len(karray[0]):
                        continue
                    if type(karray[0][b][c]) is float:
                        karray[0][b][c] += karray[a][b][c]

        for b in xrange(len(karray[0])):
            for c in xrange(len(karray[0][b])):
                if type(karray[0][b][c]) is float:
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
    #RSRHeatmap(base,varys,params)
    #PLatency(base,varys,params)
    #MessageSize(base,varys,params)
    #RSRatioCDF(base,varys,params)
    #TimeRecallAndReceive(base,varys,params)
    average(params)
    os.chdir(current)
    

import sys
import os
#For baseline and redundacy-detecion to prepare message size picture
def MessageSize(typePrefix, directory):
    wf = open("%(typePrefix)s-message.data"%vars(), 'w')
    wf.write("#Suggest Filename: %(typePrefix)s-message.data\n#Data for drawing message overall size"%vars())
    wf.write("#Redundancy 0.1,0.2,0.3,0.4,0.5\n")
    num = 100 # may subject to change by the simulation node number
    for redundacy in [0.1,0.2,0.3,0.4,0.5]:
        wf.write(str(redundacy)+" ")
        for block in [100,200,300,400,500,600,700,800,900,1000]:
            file = open("%(directory)s%(typePrefix)s_b%(block)s_r%(redundacy)s_overhead.data"%vars())
            for line in file:
                if line[0] != "#":
                    numbers = line.split(' ')
                    wf.write(numbers[7]+" ")
        wf.write("\n")
        file.close()
    wf.close()

def RecvToSendRatio(typePrefix, directory):
    writefile = open("%(typePrefix)s-rsratio-nonce.data"%vars(), "w")
    writefile.write("#Suggest Filename: %(typePrefix)s-rsratio.data\n#Data for drawing each package in different Amount/Redundancy\n"%vars())
    writefile.write("#MPM100 ratio MPM200 ratio MPM300 ratio MPM400 ratio MPM500 ratio MPM600 ratio NoLimit ratio\n")
    writefile.write("0 0 0 0 0 0 0 0 0 0 0 0 0 0\n")
    backofftime = 2.5 # may subject to change by the data amount wanted to observe
    msgcount = {}
    ratiotemp = {}
    ratio = {}
    for mms in [100,200,300,400,500,600,-1]:
        msgcount[mms] = {}
        for logcontent in ['SI','SD']:
            file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Pa":
                            nonce = x.split(':')[1]
                            if msgcount[mms].has_key(nonce):
                                msgcount[mms][nonce]["s"] += 1
                            else:
                                msgcout[mms][nonce] = {}
                                msgcount[mms][nonce]["s"] = 1
                                msgcount[mms][nonce]["r"] = 0
                                msgcount[mms][nonce]["rs"] = 0
        for logcontent in ['RI','DRD']:
            file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent or line[0:3] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Pa":
                            nonce = x.split(':')[1]
                            if(msgcount[mms].has_key(nonce)):
                                msgcount[mms][nonce]["r"] += 1
                            else:
                                print logcontent, mms, nonce
        for nonce in msgcount[mms]:
            msgcount[mms][nonce]['rs'] = float(msgcount[mms][nonce]['r']) / float(msgcount[mms][nonce]['s'])
        msg = sorted(msgcount[mms].iteritems(), key=lambda s: s[1]['rs'])
        for x in range(len(msg)):
            ratiotemp[msg[x][1]["rs"]] = float(x+1) / len(msg);
        ratio[mms] = sorted(ratiotemp.iteritems())
        ratiotemp.clear()
    length = max(len(ratio[100]),len(ratio[200]),len(ratio[300]),len(ratio[400]),len(ratio[500]),len(ratio[-1]))
    for j in range(length):
        for i in [100,200,300,400,500,600,-1]: 
            if(len(ratio[i])<=j):
                writefile.write("null null")
            else: 
                writefile.write(str(ratio[i][j][0])+" "+str(ratio[i][j][1]))
        writefile.write("\n")

         
def RecvToSendRatioHopnonce(typePrefix, directory):
    writefile = open("%(typePrefix)s-rsratio-hopnonce.data"%vars(),"w")
    writefile.write("#Suggest Filename: %(typePrefix)s-rsratio.data\n#Data for drawing receive to send ratio\n"%vars())
    writefile.write("#backofftime: 0 0.5 1 1.5 2 2.5 3\n")
    writefile.write("0 0 0 0 0 0 0 0 0 0 0 0 0 0\n")
    #backofftime = 2.5 # may subject to change by the data amount wanted to observe
    mms = -1
    msgcount = {}
    ratiotemp = {}
    ratio = {}
    for backofftime in [0,0.5,1,1.5,2,2.5,3]:
        msgcount[backofftime] = {}
        for logcontent in ['SI','SD']:
            file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Ho":
                            nonce = x.split(':')[1]
                            if msgcount[backofftime].has_key(nonce):
                                msgcount[backofftime][nonce]["s"] += 1
                            else:
                                msgcount[backofftime][nonce] = {}
                                msgcount[backofftime][nonce]["s"] = 1
                                msgcount[backofftime][nonce]["r"] = 0
                                msgcount[backofftime][nonce]["rs"] = 0
        for logcontent in ['RI','DRD']:
            file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent or line[0:3] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Ho":
                            nonce = x.split(':')[1]
                            if(msgcount[backofftime].has_key(nonce)):
                                msgcount[backofftime][nonce]["r"] += 1
                            else:
                                print logcontent, backofftime, nonce
        for nonce in msgcount[backofftime]:
            msgcount[backofftime][nonce]['rs'] = float(msgcount[backofftime][nonce]['r']) / float(msgcount[backofftime][nonce]['s'])
        msg = sorted(msgcount[backofftime].iteritems(), key=lambda s: s[1]['rs'])
        for x in range(len(msg)):
            ratiotemp[msg[x][1]["rs"]] = float(x+1) / len(msg);
        ratio[backofftime] = sorted(ratiotemp.iteritems())
        ratiotemp.clear()
    length = max(len(ratio[0]),len(ratio[0.5]),len(ratio[1]),len(ratio[1.5]),len(ratio[2]),len(ratio[2.5]),len(ratio[3]))
    for j in range(length):
        for i in [0,0.5,1,1.5,2,2.5,3]: 
            if(len(ratio[i])<=j):
                writefile.write("null null")
            else: 
                writefile.write(str(ratio[i][j][0])+" "+str(ratio[i][j][1]) + " ")
        writefile.write("\n")



#Get recall and latency
def RecallAndLatency(typePrefix, directory):
    for mpm in [20,40,60,80,100,120]
        recallf = open("./%(typePrefix)s-%(mpm)s-recall.data"%vars(), "w")
        latencyf = open("./%(typePrefix)s-%(mpm)s-latency.data"%vars(), "w")
        recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
        latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
       
        recallf.write("#M: 1,2,3\n")
        latencyf.write("#M: 1,2,3\n")
        for m in [1,2,3]:
            recallf.write(str(m)+" ")
            latencyf.write(str(m)+" ")
            for k in [2,4,6,8,10]:
                file = open("%(directory)s%(typePrefix)s_k%(k)s_m%(m)s_mpm%(mpm)s_0.data"%vars())
                line = file.readlines()[-1].split()
                recallf.write(str(float(line[1])/10000)+" ")
                latencyf.write(line[0]+" ")
                file.close()
            recallf.write("\n")
            latencyf.write("\n")
        recallf.close()
        latencyf.close()
    #os.system("gnuplot collision-avoidance-recall.gp")
    #os.system("gnuplot collision-avoidance-latency.gp")

def RSRHeatmap(typePrefix, directory):
    backofftime = 2
    mms = -1
    sendList = []
    recvList = []
    ratiolist = []
    for i in xrange(100):
        sendList.append([])
        recvList.append([])
        ratiolist.append(0)
    for logcontent in ['SI','SD']:
        file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
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
        file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
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
    writefile = open("./%(typePrefix)s-heatmap.data"%vars(), "w")
    writefile.write("#Data for receive send ratio on each ndoe of the %(typePrefix)s\n"%vars())
    for i in xrange(10):
        for j in xrange(10):
            writefile.write(str(ratiolist[i*10 + j])+"\t")
        writefile.write("\n")
    writefile.close()
    #os.system("gnuplot collision-avoidance-heatmap.gp")

def RetryTime(typePrefix):
    retrytf = open("./%(typePrefix)s-retrycount.data"%vars(),"w")
    retrytf.write("#Suggest Filename: %(typePrefix)s-retrycount.data\n#Data for drawing number of retries \n"%vars())
    retrycdff =  open("./%(typePrefix)s-retrycdf.data"%vars(),"w")
    retrycdff.write("#Suggest Filename: %(typePrefix)s-retrycdf.data\n#Data for drawing CDF of retries \n"%vars())
    num = 100 # may subject to change by the simulation node number
    cdf = {}

    for retry in [1,2,3,4,5,6,7,8,9,10]:
        m = {}
        retrytf.write(str(retry)+" ")
        for to in [0.4]:
            file1 = open("./%(typePrefix)s_rty%(retry)s_rtt%(to)s_SD.data"%vars())
            if retry != 1:
                file = open("./%(typePrefix)s_rty%(retry)s_rtt%(to)s_T.data"%vars())
                for line in file:
                    info = line.split(' ')
                    key=info[3] + info[5].strip()
                    if m.has_key(key):
                        m[key] += 1
                    else:
                        m[key] = 1
                file.close()
            for line1 in file1:
                if line1[0]=="S":
                    info1 = line1.split(' ')
                    key=info1[3] + info1[5].strip()
                    if m.has_key(key):
                        m[key] += 1
                    else:
                        m[key] = 1
            file1.close()
            c = 0.0
            for k in m:
                c += m[k]
            c /= float(len(m))
            retrytf.write(str(c))
            file2 = open("./%(typePrefix)s_rty%(retry)s_rtt%(to)s_RK.data"%vars())
            t={}
            for line2 in file2:
                info2 = line2.split(' ')
                key=info2[3] + info2[5].strip()
                t[key] = 1
            file2.close()
            for k in m:
                if m[k]==retry:
                    if not t.has_key(k):
                        print "aa"
                        m[k]=10000
            cdft = sorted(m.iteritems(),key=lambda s: s[1])

            cot = {}
            for x in xrange(len(cdft)):
                cot[cdft[x][1]] = float(x+1) / len(cdft);
            cdf[retry]=sorted(cot.iteritems())

        retrytf.write("\n")
    retrytf.close()
    print cdf
    length = max(len(cdf[1]),len(cdf[2]),len(cdf[3]),len(cdf[4]),len(cdf[5]),len(cdf[6]))
    length = max(len(cdf[7]),len(cdf[8]),len(cdf[9]),len(cdf[10]))
    for j in xrange(length):
        for i in [1,2,3,4,5,6,7,8,9,10]:
            if(len(cdf[i])<=j):
                retrycdff.write("null null ")
            elif cdf[i][j][0]==10000:
                retrycdff.write("null null ")
            else: 
                retrycdff.write(str(cdf[i][j][0])+" "+str(cdf[i][j][1])+ " ")
        retrycdff.write("\n")




#MessageSize("baseline", "F:\\Data_baseline\\")
#MessageSize("redundancy_detection", "F:\\Data_redundancy\\")
#RecvToSendRatio("collision_avoidance", "/home/theodore/pecns3/")
#RecallAndLatency("collision_avoidance", "/home/theodore/pecns3/")
#RecvToSendRatioHopnonce("collision_avoidance", "/home/theodore/pecns3/")
#ReceivedConsumer("baseline", "F:\\Data_baseline\\", 44)
#RSRHeatmap("collision_avoidance", "/home/theodore/pecns3/")

#main
if len(sys.argv) <=1:
    print "This python program is the automatic processing for the data log of the ns3 simulator of the pec. \nThis particular for the collision avoidance.\n"
    print "Useage: choose the function by the first parameter."
    print "a. Receive send ratio of each message."
    print "b. Receive send ratio of each message by hop."
    print "c. Receive send ratio heatmap for each node by each message hop."
    print "d. Recall and latency."
    print "Put this file into the same directory of the data files. Then run \"python\" + filename + chiose to get the picture direcely."
else:
    if sys.argv[1] == "a":
        RecvToSendRatio("collision_avoidance", "./")
    elif sys.argv[1] == "b":
        RecvToSendRatioHopnonce("collision_avoidance", "./")
    elif sys.argv[1] == "c":
        RSRHeatmap("collision_avoidance", "./")
    elif sys.argv[1] == "d":
        RecallAndLatency("ec", "./")
    elif sys.argv[1] == "e":
        MessageSize("ec", "./")
    elif sys.argv[1] == "f":
        RetryTime("ack")
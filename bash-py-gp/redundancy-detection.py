import sys
import os
#For baseline and redundacy-detecion to prepare message size picture
def MessageSize(typePrefix, directory):
    wf = open("%(typePrefix)s-msgsize.data"%vars(), "w")
    wf.write("#Suggest Filename: %(typePrefix)s-message.data\n#Data for drawing message overall size in different Amount/Redundancy\n"%vars())
    wf.write("#row: amount(10000, 20000, 30000, 40000, 50000)\n#col: redundancy(1, 2, 3, 4, 5)\n")
    wf.write('#amount\tRedundancy 1\tRedundancy 2\tRedundancy 3\tRedundancy 4\tRedundancy 5\n')
    num = 100 # may subject to change by the simulation node number
    for amount in [10000,20000,30000,40000,50000]:
        wf.write(str(amount) + " ")
        for redundancy in [1,2,3,4,5]:
            file = open("%(directory)s%(typePrefix)s_da%(amount)s_r%(redundancy)s_overhead.data"%vars())
            for line in file:
                if line[0] != "#":
                    numbers = line.split(' ')
                    wf.write(numbers[7]+" ")
        wf.write("\n")
        file.close()

def RecvToSendRatio(typePrefix, directory):
    writefile = open("%(typePrefix)s-rsratio-nonce.data"%vars(), "w")
    writefile.write("#Suggest Filename: %(typePrefix)s-rsratio.data\n#Data for drawing each package in different Amount/Redundancy\n"%vars())
    writefile.write("#MPM100 ratio MPM200 ratio MPM300 ratio MPM400 ratio MPM500 ratio MPM600 ratio NoLimit ratio\n")
    writefile.write("0 0 0 0 0 0 0 0 0 0 \n")
    backofftime = 2.5 # may subject to change by the data amount wanted to observe
    da=50000
    msgcount = {}
    ratiotemp = {}
    ratio = {}
    for redundancy in [1,2,3,4,5]:
        msgcount[redundancy] = {}
        for logcontent in ['SI','SD']:
            file = open("%(directory)s%(typePrefix)s_da%(da)s_r%(redundancy)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Ho":
                            nonce = x.split(':')[1]
                            if msgcount[redundancy].has_key(nonce):
                                msgcount[redundancy][nonce]["s"] += 1
                            else:
                                msgcount[redundancy][nonce] = {}
                                msgcount[redundancy][nonce]["s"] = 1
                                msgcount[redundancy][nonce]["r"] = 0
                                msgcount[redundancy][nonce]["rs"] = 0
        for logcontent in ['RI','DRD']:
            file = open("%(directory)s%(typePrefix)s_da%(da)s_r%(redundancy)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent or line[0:3] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Ho":
                            nonce = x.split(':')[1]
                            if(msgcount[redundancy].has_key(nonce)):
                                msgcount[redundancy][nonce]["r"] += 1
                            else:
                                print logcontent, redundancy, nonce
        for nonce in msgcount[redundancy]:
            msgcount[redundancy][nonce]['rs'] = float(msgcount[redundancy][nonce]['r']) / float(msgcount[redundancy][nonce]['s'])
        msg = sorted(msgcount[redundancy].iteritems(), key=lambda s: s[1]['rs'])
        for x in range(len(msg)):
            ratiotemp[msg[x][1]["rs"]] = float(x+1) / len(msg);
        ratio[redundancy] = sorted(ratiotemp.iteritems())
        ratiotemp.clear()
    length = max(len(ratio[1]),len(ratio[2]),len(ratio[3]),len(ratio[4]),len(ratio[5]))
    for j in range(length):
        for i in [1,2,3,4,5]: 
            if(len(ratio[i])<=j):
                writefile.write("null null")
            else: 
                writefile.write(str(ratio[i][j][0])+" "+str(ratio[i][j][1])+ " ")
        writefile.write("\n")

         
def RecvToSendRatioHopnonce(typePrefix, directory):
    writefile = open("%(typePrefix)s-rsratio-hopnonce.data"%vars())
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
                        if x[0:2] == "Ho":
                            nonce = x.split(':')[1]
                            if msgcount[mms].has_key(nonce):
                                msgcount[mms][nonce]["s"] += 1
                            else:
                                msgcount[mms][nonce] = {}
                                msgcount[mms][nonce]["s"] = 1
                                msgcount[mms][nonce]["r"] = 0
                                msgcount[mms][nonce]["rs"] = 0
        for logcontent in ['RI','DRD']:
            file = open("%(directory)s%(typePrefix)s_mb%(backofftime)s_mms%(mms)s_%(logcontent)s.data"%vars())
            for line in file:
                if line[0:2] == logcontent or line[0:3] == logcontent:
                    info = line.split(' ')
                    for x in info:
                        if x[0:2] == "Ho":
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



#Get recall and latency
def RecallAndLatency(typePrefix, directory):
    recallf = open("./%(typePrefix)s-recall.data"%vars(), "w")
    latencyf = open("./%(typePrefix)s-latency.data"%vars(), "w")
    recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
    latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
    recallf.write("# row: max_backoff(0 0.5 1 1.5 2 2.5 3)\n")
    recallf.write("# col: max_message_size(-1, 200, 400, 600, 800, 1000)\n")
    recallf.write("#MaxBackoff  No Limits 100 200 300 400 500\n")
    latencyf.write("# row: max_backoff(0 0.5 1 1.5 2 2.5 3)\n")
    latencyf.write("# col: max_message_size(-1, 200, 400, 600, 800, 1000)\n")
    latencyf.write("#MaxBackoff  No Limits 100 200 300 400 500\n")
    
    for amount in [10000,20000,30000,40000,50000]:
        recallf.write(str(amount)+" ")
        latencyf.write(str(amount)+" ")
        for redundancy in [1,2,3,4,5]:
            file = open("%(directory)s%(typePrefix)s_da%(amount)s_r%(redundancy)s_0.data"%vars())
            line = file.readlines()[-1].split()
            recallf.write(str(float(line[1])/amount)+" ")
            latencyf.write(line[0]+" ")
            file.close()
        recallf.write("\n")
        latencyf.write("\n")
    recallf.close()
    latencyf.close()
  #  os.system("gnuplot collision-avoidance-recall.gp")
 # os.system("gnuplot collision-avoidance-latency.gp")

def RSRHeatmap(typePrefix, directory):
    amount = 50000
    redundancy = 1
    sendList = []
    recvList = []
    ratiolist = []
    for i in xrange(100):
        sendList.append([])
        recvList.append([])
        ratiolist.append(0)
    for logcontent in ['SI','SD']:
        file = open("%(directory)s%(typePrefix)s_da%(amount)s_r%(redundancy)s_%(logcontent)s.data"%vars())
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
        file = open("%(directory)s%(typePrefix)s_da%(amount)s_r%(redundancy)s_%(logcontent)s.data"%vars())
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
        if(len(sendList[i])) > 0:    
            ratiolist[i] /= float(len(sendList[i]))
        else:
            ratiolist[i] = 0
    writefile = open("./%(typePrefix)s-heatmap.data"%vars(), "w")
    writefile.write("#Data for receive send ratio on each ndoe of the %(typePrefix)s\n"%vars())
    for i in xrange(10):
        for j in xrange(10):
            writefile.write(str(ratiolist[i*10 + j])+"\t")
        writefile.write("\n")
    writefile.close()
  #  os.system("gnuplot collision-avoidance-heatmap.gp")




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
    print "e. Message overall size"
    print "Put this file into the same directory of the data files. Then run \"python\" + filename + chiose to get the picture direcely."
else:
    if sys.argv[1] == "a":
        RecvToSendRatio("redundancy_detection", "./")
    elif sys.argv[1] == "b":
        RecvToSendRatioHopnonce("redundancy_detection", "./")
    elif sys.argv[1] == "c":
        RSRHeatmap("redundancy_detection", "./")
    elif sys.argv[1] == "d":
        RecallAndLatency("redundancy_detection", "./")
    elif sys.argv[1] == "e":
        MessageSize("redundancy_detection", "./")

#Get recall and latency
def RecallAndLatency(typePrefix, directory):
    recallf = open("./%(typePrefix)s-recall.data"%vars(), "w")
    latencyf = open("./%(typePrefix)s-latency.data"%vars(), "w")
    recallf.write("#Data for recall of the %(typePrefix)s\n"%vars())
    latencyf.write("#Data for latency of the %(typePrefix)s\n"%vars())
    recallf.write("# row: th_round(0 0.1 0.2 0.3 0.4 0.5)\n")
    recallf.write("# col: th_data_discovery(0 0.1 0.2 0.3 0.4 0.5)\n")
    recallf.write("#MaxBackoff  0 0.1 0.2 0.3 0.4 0.5\n")
    latencyf.write("# row: th_round(0 0.1 0.2 0.3 0.4 0.5)\n")
    latencyf.write("# col: th_data_discovery(0 0.1 0.2 0.3 0.4 0.5)\n")
    latencyf.write("#MaxBackoff  N0 0.1 0.2 0.3 0.4 0.5\n")
    
    for thr in [0,0.1,0.2,0.3,0.4,0.5]:
        recallf.write(str(thr)+" ")
        latencyf.write(str(thr)+" ")
        for thd in [0,0.1,0.2,0.3,0.4,0.5]:
            file = open("%(directory)s%(typePrefix)s_thr%(thr)s_thd%(thd)s_0.data"%vars())
            line = file.readlines()[-1].split()
            recallf.write(str(float(line[1])/50000)+" ")
            latencyf.write(line[0]+" ")
            file.close()
        recallf.write("\n")
        latencyf.write("\n")
    recallf.close()
    latencyf.close()

RecallAndLatency("multi_round", "./")
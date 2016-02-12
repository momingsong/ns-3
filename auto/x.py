import os
def execute(methods,varys,params,number):

	current = os.path.dirname(__file__)
	os.system("mkdir %d-%s"%(number,params['t']))
	aimdir = current + "/%d-%s"%(number,params['t'])

	for a in eval(varys[0][1]):
		params[varys[0][0]] = a
		for b in eval(varys[1][1]):
			params[varys[1][0]] = b

			exe = './waf --run "pec_grid_immobile --tracePath='+params['t']+'_'+varys[0][0]+str(a)+'_'+varys[1][0]+str(b)+' --dataAmount=%(daa)s --redundancy=%(rdd)s --enableRedundancyDetection=%(er)s --enableCollisionAvoidance=%(ec)s --maxMetadataPerMessage=%(mpm)s --maxBackoff=%(mbo)s --enableMultiRound=%(em)s --multiRoundRoundFinishThreshold=%(thr)s --multiRoundDiscoveryFinishThreshold=%(thd)s --multiRoundSlotSize=%(mss)s --multiRoundWindowSize=%(mws)s --enableRetransmit=%(ea)s --retransmitTimeout=%(rto)s --retransmitRetry=%(rty)s --enableErasureCoding=%(ee)s --erasureCodingK=%(eck)s --erasureCodingM=%(ecm)s --seed=%(sed)s"'%params
			print exe
			os.chdir("/home/sxt/workspace/pec/ns-3/")
			#os.chdir("/home/ubuntu/pecns3/")
			os.system(exe)
			os.system("mv *.data %s"%aimdir)
	os.chdir(current)
	



def baseECC(base):
	if base=="a" or base=="b" or base=="e" or base=="m" or base=="r" or base=="o":
		return base
	else:
		print "Wrong input base"
		sys.exit(100)



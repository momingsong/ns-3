import os
def execute(methods,varys,params,number):
	#setting all variables 
	params['ea']='false'
	params['ec']='false'
	params['ee']='false'
	params['em']='false'
	params['er']='false'

	if methods['a']==True:
		params['ea']= 'true'
	if methods['b']==True:
		params['ec']= 'true'
	if methods['e']==True:
		params['ec'] = 'true'
		params['ee'] = 'true'
	if methods['m']==True:
		params['em'] = 'true'
	if methods['r']==True:
		params['er'] = 'true'

	current = os.path.dirname(__file__)
	os.system("mkdir %d-%s"%(number,params['t']))
	aimdir = current + "/%d-%s"%(number,params['t'])

	k = []
	for key in varys:
		k.append(key)

	for a in eval(varys[k[0]]):
		params[k[0]] = a
		for b in eval(varys[k[1]]):
			params[k[1]] = b

			exe = './waf --run "pec_grid_immobile --tracePath='+params['t']+'_'+k[0]+str(a)+'_'+k[1]+str(b)+' --dataAmount=%(daa)s --redundancy=%(rdd)s --enableRedundancyDetection=%(er)s --enableCollisionAvoidance=%(ec)s --maxMetadataPerMessage=%(mpm)s --maxBackoff=%(mbo)s --enableMultiRound=%(em)s --multiRoundRoundFinishThreshold=%(thr)s --multiRoundDiscoveryFinishThreshold=%(thd)s --multiRoundSlotSize=%(mss)s --multiRoundWindowSize=%(mws)s --enableRetransmit=%(ea)s --retransmitTimeout=%(rto)s --retransmitRetry=%(rty)s --enableErasureCoding=%(ee)s --erasureCodingK=%(eck)s --erasureCodingM=%(ecm)s --seed=%(sed)s"'%params
			print exe
			os.chdir("/home/ubuntu/pecns3/")
			os.system(exe)
			os.system("mv *.data %s"%aimdir)
	os.chdir(current)
	



def baseECC(base):
	if base=="a" or base=="b" or base=="e" or base=="m" or base=="r" or base=="o":
		return base
	else:
		print "Wrong input base"
		sys.exit(100)



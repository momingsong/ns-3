import os
import sys
def execute(methods,varys,params,number):

	current = os.path.dirname(__file__)
	os.system("mkdir %d-%s"%(number,params['t']))
	aimdir = current + "/%d-%s"%(number,params['t'])

	for avgs in xrange(int(params['avg']),0,-1):
		params['sed'] = str(avgs)
		paramarray=[]
		for lv in xrange(len(varys)):
			paramarray.append(0)
		while paramarray[0] < len(eval(varys[0][1])):
			paramlist=""
			for lv in xrange(len(varys)):
				kv=eval(varys[lv][1])
				paramlist+= ("_%s%s"%(varys[lv][0],str(kv[paramarray[lv]])))
				params[varys[lv][0]]=kv[paramarray[lv]]
			print paramlist

			paramarray[-1]+=1
			for rv in xrange(len(varys)-1,0,-1):
				if paramarray[rv]>=len(eval(varys[rv][1])):
					paramarray[rv]=0
					paramarray[rv-1]+=1

		
			exe = './waf --run "pec_grid_immobile --tracePath='+params['t']+paramlist+'-'+str(avgs)+' --dataAmount=%(daa)s --redundancy=%(rdd)s --enableRedundancyDetection=%(er)s --enableCollisionAvoidance=%(ec)s --maxMetadataPerMessage=%(mpm)s --maxBackoff=%(mbo)s --enableMultiRound=%(em)s --multiRoundRoundFinishThreshold=%(thr)s --multiRoundDiscoveryFinishThreshold=%(thd)s --multiRoundSlotSize=%(mss)s --multiRoundWindowSize=%(mws)s --enableRetransmit=%(ea)s --retransmitTimeout=%(rto)s --retransmitRetry=%(rty)s --enableErasureCoding=%(ee)s --erasureCodingK=%(eck)s --erasureCodingM=%(ecm)s --seed=%(sed)s"'%params
					
			print exe
			#os.chdir("/home/sxt/workspace/pec/ns-3/")
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


'''
	for a in eval(varys[0][1]):
		params[varys[0][0]] = a
		for b in eval(varys[1][1]):
			params[varys[1][0]] = b
'''
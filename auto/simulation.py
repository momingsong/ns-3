#!/usr/bin/python

import os
import sys

import x
import d
import p

simlationFile = []
number = 0

#reading all simulation file in the dir
for files in os.listdir("./"):
	if files.split('.')[-1]=="simf":
		simlationFile.append(files)

#running simulation
for file in simlationFile:
	f = open("./"+file)
	#simulation swithes
	base = ""
	item = {}
	item['a'] = False
	item['e'] = False
	item['r'] = False
	item['b'] = False
	item['m'] = False
	#paramteres initialize
	params={}
	params['daa']="0"
	params['rdd']="1"
	params['mpm']="0"
	params['mbo']="0"
	params['thd']="0"
	params['thr']="0"
	params['mss']="0"
	params['mws']="0"
	params['rto']="0"
	params['rty']="0"
	params['eck']="0"
	params['ecm']="0"
	params['sed']="1"

	vary={}

	#read file
	for line in f:
		number += 1
		if line[0]!='#':
			info = line.split(' ')
			for i in info[0]:
				if item.has_key(i):
					item[i]=True
			params["t"]=info[0]
			print info
			for j in xrange(0,len(info)):
				if info[j][0:2]=="--":
					if params.has_key(info[j][2:5]) and info[j][5]=='[':
						vary[info[j][2:5]]=info[j][5:]
						if len(vary)>=3:
							print "too many varys"
							sys.exit(101)
					elif params.has_key(info[j][2:5]) and info[j][5]=='=':
						params[info[j][2:5]]=info[j][6:]
					#else:
					#	sys.exit(103)
			if len(vary) < 2:
				print "too few varys"
				sys.exit(102)
			print vary

			x.execute(item,vary,params,number)
			d.process(params['t'],vary,number,params['daa'])
			p.plot(params['t'],vary,number)
			




# Error code:
# 101 too many varys
# 102 too few varys 
# 103 no such params
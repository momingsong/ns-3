#!/bin/bash
eck=(2 4 6 8 10)
ecm=(1 2 3)
ecmpm=(20 40 60 80 100 120)

for k in ${eck[@]}
  do
  for m in ${ecm[@]}
    do
    	for mpm in ${ecmpm[@]}
    	do
      ./waf --run "pec_grid_immobile  --dataAmount=10000 --redundancy=1 --tracePath=ec_k${k}_m${m}_mpm${mpm} --enableRedundancyDetection=false --enableMultiRound=false --enableCollisionAvoidance=true --maxMetadataPerMessage=${mpm} --maxBackoff=0  --enableRetransmit=true -retransmitRetry=5 --retransmitTimeout=0.5 --enableErasureCoding=true --erasureCodingK=${k} --erasureCodingM=${m}"
  done
  done
done

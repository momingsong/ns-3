#!/bin/bash
max_backoff=(0 0.5 1 1.5 2 2.5 3)
max_message_size=(-1 100 200 300 400 500 600)

for mb in ${max_backoff[@]}
  do
  for mms in ${max_message_size[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=collision_avoidance_mb${mb}_mms${mms} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=false --maxMetadataPerMessage=$mms --maxBackoff=$mb  --enableRetransmit=false --enableErasureCoding=false"
  done
done

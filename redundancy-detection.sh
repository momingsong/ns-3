#!/bin/bash
data_amount=(10000 20000 30000 40000 50000)
redundancy=(1 2 3 4 5)

for da in ${data_amount[@]}
do
  for r in ${redundancy[@]}
  do
    ./waf --run "pec_grid_immobile  --dataAmount=$da --redundancy=$r --tracePath=redundancy_detection_da${da}_r${r} --enableRedundancyDetection=true --enableCollisionAvoidance=false --enableMultiRound=false --enableRetransmit=false --enableErasureCoding=false"
  done
done

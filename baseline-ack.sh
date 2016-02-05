#!/bin/bash
data_amount=(10000 20000 30000 40000 50000)
redundancy=(1 2 3 4 5)

for da in ${data_amount[@]}
do
  for r in ${redundancy[@]}
  do
    ./waf --run "pec_grid_immobile  --dataAmount=$da --redundancy=$r --tracePath=baseline_da${da}_r${r} --enableRedundancyDetection=false --enableCollisionAvoidance=false --enableMultiRound=false --enableRetransmit=true  --retransmitRetry=6 --retransmitTimeout=0.5 --enableErasureCoding=false"
  done
done

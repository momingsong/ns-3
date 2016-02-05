#!/bin/bash
retry=(0 1 2 3 4 5 6 7 8 9 10)
timeout=(0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0)

for rty in ${retry[@]}
  do
  for rtt in ${timeout[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=ack_rty${rty}_rtt${rtt} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=false --maxMetadataPerMessage=-1 --maxBackoff=2  --enableRetransmit=true --retransmitRetry=$rty --retransmitTimeout=$rtt --enableErasureCoding=false"
  done
done

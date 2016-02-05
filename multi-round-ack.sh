#!/bin/bash
timeout=(0 0.1 0.2 0.3 0.4 0.5)
#retry=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20)
#threshold_data_discovery=(0)

for to in ${timeout[@]}
  do
  ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=multi_round_ack_ret20_to${to} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=true --maxMetadataPerMessage=-1 --maxBackoff=1.5 --multiRoundRoundFinishThreshold=0.5 --multiRoundDiscoveryFinishThreshold=0.2 --bloomFilterSizeMax=60000 --bloomFilterSizeMin=10000 --bloomFilterFalsePositive=0.05 --multiRoundSlotSize=0.2 --multiRoundWindowSize=12 --enableRetransmit=true --retransmitRetry=20 --retransmitTimeout=$to"
done

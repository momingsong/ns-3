#!/bin/bash
threshold_round=(0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0)
threshold_data_discovery=(0)

for thr in ${threshold_round[@]}
  do
  for thd in ${threshold_data_discovery[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=multi_round_thr${thr}_thd${thd} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=true --maxMetadataPerMessage=-1 --maxBackoff=1.5 --multiRoundRoundFinishThreshold=$thr --multiRoundDiscoveryFinishThreshold=$thd --bloomFilterSizeMax=60000 --bloomFilterSizeMin=10000 --bloomFilterFalsePositive=0.05 --multiRoundSlotSize=0.2 --multiRoundWindowSize=12 --enableRetransmit=false"
  done
done

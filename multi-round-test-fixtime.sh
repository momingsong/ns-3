#!/bin/bash
windowsize=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25)


threshold_round=(0 0.1 0.2 0.3 0.4 0.5)
threshold_data_discovery=(0 0.1 0.2 0.3 0.4 0.5)

for ws in ${windowsize[@]}
#for((ws=1;ws<=25;ws++))
  do
  q=`echo "scale=6;2.5/$ws"|bc` 
  for thr in ${threshold_round[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=multi_round_thr${thr}_ws${ws} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=true --maxMetadataPerMessage=-1 --maxBackoff=2.5 --multiRoundRoundFinishThreshold=$thr --multiRoundDiscoveryFinishThreshold=0 --bloomFilterSizeMax=60000 --bloomFilterSizeMin=10000 --bloomFilterFalsePositive=0.01 --multiRoundSlotSize=$q --multiRoundWindowSize=$ws"
  done
done

for((ws=1;ws<=25;ws++))
  do
    q=`echo "scale=6;2.5/$ws"|bc` 
  for thd in ${threshold_data_discovery[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=multi_round_thd${thd}_ws${ws} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=true --maxMetadataPerMessage=-1 --maxBackoff=2.5 --multiRoundRoundFinishThreshold=0 --multiRoundDiscoveryFinishThreshold=$thd --bloomFilterSizeMax=60000 --bloomFilterSizeMin=10000 --bloomFilterFalsePositive=0.01 --multiRoundSlotSize=$q --multiRoundWindowSize=$ws"
  done
done

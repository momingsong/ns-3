#!/bin/bash
threshold_round=(0 0.1 0.2 0.3 0.4 0.5)
threshold_data_discovery=(0 0.1 0.2 0.3 0.4 0.5)

    #q =`echo "scale=2;$ws / 5.0" | bc` 
for thr in ${threshold_round[@]}
    do
        ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=multi_round_thr${thr}_ws12 --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=true --maxMetadataPerMessage=-1 --maxBackoff=2.5 --multiRoundRoundFinishThreshold=$thr --multiRoundDiscoveryFinishThreshold=0 --bloomFilterSizeMax=60000 --bloomFilterSizeMin=10000 --bloomFilterFalsePositive=0.01 --multiRoundSlotSize=0.2 --multiRoundWindowSize=12"
done

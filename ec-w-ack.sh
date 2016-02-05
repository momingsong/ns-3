#!/bin/bash
block=(100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000)
redundancy=(0.1 0.2 0.3 0.4 0.5)

for b in ${block[@]}
  do
  for r in ${redundancy[@]}
    do
      ./waf --run "pec_grid_immobile  --dataAmount=50000 --redundancy=1 --tracePath=ec_b${b}_r${r} --enableRedundancyDetection=true --enableCollisionAvoidance=true --enableMultiRound=false --maxMetadataPerMessage=-1 --maxBackoff=2  --enableRetransmit=true --retransmitRetry=5 --retransmitTimeout=0.5 --enableErasureCoding=true --erasureCodingRedundancyRate=$r --erasureCodingBlockSize=$b"
  done
done

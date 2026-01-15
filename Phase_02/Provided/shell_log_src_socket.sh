#!/bin/bash

SOCKET_PATH="./telemetry.sock"
DURATION=60         
INTERVAL=0.1        

START_TIME=$(date +%s)

while (( $(date +%s) - START_TIME < DURATION )); do
    VALUE=$(awk -v r=$RANDOM 'BEGIN { printf "%.2f", r/32767*100 }')

    echo "$VALUE"

    echo "$VALUE" | nc -U  $SOCKET_PATH
    
    sleep "$INTERVAL"
done
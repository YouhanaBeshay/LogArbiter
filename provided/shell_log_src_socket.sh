#!/bin/bash

SOCKET_PATH="./telemetry.sock"
DURATION=60         
INTERVAL=1

rm -f "$SOCKET_PATH"
trap "rm -f $SOCKET_PATH" EXIT

(
    START_TIME=$(date +%s)
    while (( $(date +%s) - START_TIME < DURATION )); do
        VALUE=$(awk -v r=$RANDOM 'BEGIN { printf "%.2f", r/32767*100 }')
        echo "$VALUE"
        sleep "$INTERVAL"
    done
) | nc -lkU "$SOCKET_PATH"
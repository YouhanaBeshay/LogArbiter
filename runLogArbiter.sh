#!/bin/bash

INTERFACE=enp8s0
MULTICAST=224.224.224.245

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

BINARY="$SCRIPT_DIR/build/LogArbiter"
CONFIG="$SCRIPT_DIR/src/sources/client.json"

# Use pkexec (will show GUI password prompt if needed)
if ! ip route | grep -q $MULTICAST; then
    pkexec ip route add $MULTICAST dev $INTERFACE
fi

export VSOMEIP_CONFIGURATION="$CONFIG"


"$BINARY"
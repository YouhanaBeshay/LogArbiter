#!/bin/bash

INTERFACE=eth0
MULTICAST=224.224.224.245

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

BINARY="$SCRIPT_DIR/build_pi/vSomeipServer_GpuUsage"
CONFIG="$SCRIPT_DIR/server.json"

# Add multicast route only if not exists
ip route | grep -q $MULTICAST || sudo ip route add $MULTICAST dev $INTERFACE

export VSOMEIP_CONFIGURATION="$CONFIG"
export VSOMEIP_APPLICATION_NAME=vSomeipServer_GpuUsage


"$BINARY"
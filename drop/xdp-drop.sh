#!/bin/bash
source ../_param.sh
XDP_MODE='xdp|xdpgeneric|xdpoffload|xdpdrv'

PROG=xdp-drop
PIN=/sys/fs/bpf/$PROG
rm $PROG.o
# Compile XDP program with clang
clang -O2 -Wall -Wno-compare-distinct-pointer-types \
    -target bpf -c $PROG.c -o $PROG.o

echo "DEV=$DEV interface selected."
echo "If it's not desired, please specify \$DEV."

# Attach XDP to interface
if [ -z "$BPFTOOL"  ]; then
    ip link set dev $DEV xdp obj ./$PROG.o sec xdp1
else
    bpftool prog load ./$PROG.o $PIN
    bpftool net attach xdp pinned $PIN dev $DEV
fi

MODE=$(ip link show $DEV | head -1 | egrep -o $XDP_MODE)
echo "XDP Attached mode : $MODE"

watch -d -n 1 "ethtool -S $DEV | grep rx_xdp_drop"

# Detach XDP from interface
if [ -z "$BPFTOOL"  ]; then
    ip link set dev $DEV xdp off
else
    bpftool net detach xdp dev $DEV
    rm -rf $PIN
fi

echo '==========================================='
printf "${RED}ethtool XDP DROP metric may vary${NC}\n"
echo '(ex. mellanox XDP - rx_xdp_drop)'
echo '(ex. netronome XDP offload - bpf_app1_pkts)'
# if your nic does not support ethtool metric,
# try ./xdp_rxq_info from linux/samples/bpf'

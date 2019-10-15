#!/bin/bash
source ../_param.sh

echo "DEV=$DEV interface selected."
echo "If it's not desired, please specify \$DEV."

# Add tc ingress qdisc and filter to it (parent ffff:)
tc qdisc add dev $DEV ingress
tc filter add dev $DEV parent ffff: protocol ip u32 \
    match ip dst $IP match ip dport $PORT 0xffff \
    action drop

# Watch filter match count
watch -d -n 1 "tc -s filter show ingress dev $DEV"

tc qdisc del dev $DEV ingress

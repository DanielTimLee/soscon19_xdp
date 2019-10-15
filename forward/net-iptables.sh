#!/bin/bash
source ../_param.sh

# Add iptable rule for destination nat and watch rule match count
iptables -t nat -A PREROUTING -d $IP -p udp --dport $PORT \
    -j DNAT --to-destination 10.1.1.2:$PORT
watch -d -n 1 'iptables -t nat -vxnL PREROUTING'

# Clear rule
iptables -t nat --flush

echo "==================================================================="
echo "Counting will be increase 1, since the DNAT is cached to conntrack."
echo "For a percise number, count packet from the forwarded host."

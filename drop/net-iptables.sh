#!/bin/bash
source ../_param.sh

# Add iptable rule and watch rule match count
iptables -A INPUT -d $IP -p udp --dport $PORT -j DROP
watch -d -n 1 'iptables -vxnL INPUT'

# Clear rule
iptables --flush

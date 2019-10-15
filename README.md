# SOSCON19 XDP


### Packet Drop
 
1. Userspace - UDP socket
2. Netfilter - iptables DROP
3. TC - ingress qdisc filter DROP 
4. XDP - XDP_DROP

### Packet Forward

1. Userspace - NGINX stream
2. Netfilter - iptables DNAT
3. TC - ingress qdisc filter mirred redirect 
4. XDP - XDP_REDIRECT


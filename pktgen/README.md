### Kernel pktgen script

Pktgen source copied from linux/samples/pktgen. (net-next 40f843e)
`pktgen_sample04_many_flows.sh` renamed to `pktgen.sh`

### Usage:

`pktgen.sh -i <$DEV> -m <$DST_MAC> -d <$DEST_IP> -t <$CPU_NUM>`


### Example

```bash
soscon19_xdp/pktgen$ arp 10.1.0.2
Address                  HWtype  HWaddress           Flags Mask           Iface
10.1.0.2                 ether   90:e2:ba:c5:05:89   C                    enp1s0f0
soscon19_xdp/pktgen$ ./pktgen.sh -i enp1s0f0 -d 10.1.0.2 -m 90:e2:ba:c5:05:89 -t 4
Running... ctrl^C to stop
^CDevice: enp1s0f0@0
Result: OK: 6161764(c6158131+d3633) usec, 21720084 (60byte,0frags)
3524977pps 1691Mb/sec (1691988960bps) errors: 0
Device: enp1s0f0@1
Result: OK: 6189928(c6175113+d14815) usec, 21986660 (60byte,0frags)
3552005pps 1704Mb/sec (1704962400bps) errors: 0
Device: enp1s0f0@2
Result: OK: 6109786(c6105479+d4306) usec, 21518043 (60byte,0frags)
3521897pps 1690Mb/sec (1690510560bps) errors: 0
Device: enp1s0f0@3
Result: OK: 6149731(c6147595+d2136) usec, 21741946 (60byte,0frags)
3535430pps 1697Mb/sec (1697006400bps) errors: 0 
```

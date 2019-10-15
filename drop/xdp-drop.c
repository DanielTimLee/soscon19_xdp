#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/bpf.h>
#include <linux/udp.h>
#include <arpa/inet.h>
#include "../bpf_helpers.h"

#define _htonl __builtin_bswap32

SEC("xdp1")
int xdp_prog1(struct xdp_md *xdp)
{
	void *data_end = (void *)(long)xdp->data_end;
	void *data = (void *)(long)xdp->data;
	struct ethhdr *eth = data;
	struct iphdr *iph;
	struct udphdr *uh;
	/* DROP UDP 10.1.0.2:1234 */
	__u32 dst = 0xa010002;
	__u16 dport = 1234;

	if (eth + 1 > data_end)
		return XDP_DROP;

	if (eth->h_proto == htons(ETH_P_IP)) {
		iph = data + sizeof(*eth);
		if (iph + 1 > data_end)
			return XDP_DROP;

		if (iph->daddr == _htonl(dst)) {
			if (iph->protocol == IPPROTO_UDP) {
				uh = data + sizeof(*eth) + sizeof(*iph);
				if (uh + 1 > data_end)
					return XDP_DROP;

				if (uh->dest == htons(dport))
					return XDP_DROP;
			}
		}
	}

	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";

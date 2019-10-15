#include <string.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include "../bpf_helpers.h"

#define _htonl __builtin_bswap32

/* since this file is compiled outside of linux source tree,
 * can't include net/ip.h directly. Below function does identical
 * to the ip_decrease_ttl from include/net/ip.h */
static __always_inline int ip_decrease_ttl(struct iphdr *iph)
{
	__u32 check = (__u32)iph->check;
	check += (__u32)htons(0x0100);
	iph->check = (__sum16)(check + (check >= 0xFFFF));
	return --iph->ttl;
}

SEC("xdp_fwd")
int xdp_fwd_prog(struct xdp_md *xdp)
{
	void *data_end = (void *)(long)xdp->data_end;
	void *data = (void *)(long)xdp->data;
	struct bpf_fib_lookup fib;
	struct ethhdr *eth = data;
	struct iphdr *iph;
	struct udphdr *uh;
	int rc;

	if (eth + 1 > data_end)
		return XDP_DROP;

	if (eth->h_proto == htons(ETH_P_IP)) {
		iph = data + sizeof(*eth);
		if (iph + 1 > data_end)
			return XDP_DROP;

		if (iph->daddr == _htonl(0xa010002))
			if (iph->protocol == IPPROTO_UDP) {
				uh = data + sizeof(*eth) + sizeof(*iph);
				if (uh + 1 > data_end)
					return XDP_DROP;

				if (uh->dest == htons(1234))
					iph->daddr = _htonl(0xa010102);
			}

		__builtin_memset(&fib, 0, sizeof(fib));
		fib.family	= AF_INET;
		fib.tos		= iph->tos;
		fib.l4_protocol	= iph->protocol;
		fib.tot_len	= ntohs(iph->tot_len);
		fib.ipv4_src	= iph->saddr;
		fib.ipv4_dst	= iph->daddr;
		fib.ifindex	= xdp->ingress_ifindex;
	} else {
		return XDP_PASS;
	}

	rc = bpf_fib_lookup(xdp, &fib, sizeof(fib), 0);

	if (rc == BPF_FIB_LKUP_RET_SUCCESS) {
		ip_decrease_ttl(iph);			/* from include/net/ip.h */
		memcpy(eth->h_dest, fib.dmac, ETH_ALEN);
		memcpy(eth->h_source, fib.smac, ETH_ALEN);
		return bpf_redirect(fib.ifindex, 0);
	}

	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";

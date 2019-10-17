# SOSCON19 XDP

[Slide Deck](SOSCON2019_keynote_XDP.pdf)

[Demo](https://drive.google.com/file/d/1B_l2AOKWbWjmVrjKhXrPs3VOvGe65D-5/view?usp=drivesdk)

Repo for the performance test from the slide

**Disclaimer: scripts may not work with your environment, since this test is performed in specific environment**

### Prerequisite
- bpftool (run `./install-bpftool.sh`)
- iproute2 (TC command)
- clang, llvm (BPF compile)
- Kernel upper than 4.14 (XDP_REDIRECT)  
(depend on the driver XDP support, required version may vary. find [here][1])
- **NIC support with XDP, XDP_REDIRECT**  
(if not, you can still try GENERIC XDP)

### How to test?

Host to test - Run scripts under each directory `drop/`, `forward/`  
Host to generate packet - Run script under `pktgen/`  
(Regarding with pktgen, follow the instruction from '-h' option)

ex)  
Host A: `# ./pktgen/pktgen.sh -i enp1s0f0 -d 10.1.0.2 -m 90:e2:ba:c5:05:89 -t 4`  
Host B: `# ./drop/xdp-drop.sh 10.1.0.2 1234 eth0`  

### Packet Drop

Environment:  
XDP - Mellanox ConnectX-4 (mlx5)  
XDP OFFLOAD - Netronome Agilio (nfp)  

1. Userspace - UDP socket
2. Netfilter - iptables DROP
3. TC - ingress qdisc filter DROP 
4. XDP - XDP_DROP
5. XDP OFFLOAD - XDP_DROP

### Packet Forward

Environment:  
XDP - Mellanox ConnectX-4 (mlx5)  

1. Userspace - NGINX stream
2. Netfilter - iptables DNAT
3. TC - ingress qdisc filter mirred redirect 
4. XDP - XDP_REDIRECT

### To find out more

- [IOVISOR XDP](https://www.iovisor.org/technology/xdp)
- [BPF and XDP Reference Guide](https://cilium.readthedocs.io/en/latest/bpf/)
- [KERNEL BPF(XDP) SAMPLES](https://git.kernel.org/pub/scm/linux/kernel/git/davem/net-next.git/tree/samples/bpf)
- [XDP TUTORIAL](https://github.com/xdp-project/xdp-tutorial)
- [Latest BPF SOURCE bpf-next](https://github.com/xdp-project/bpf-next.git)


[1]: https://github.com/iovisor/bcc/blob/master/docs/kernel-versions.md

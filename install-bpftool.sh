#!/bin/bash

if [ "x$(id -u)" != x0 ]; then
    echo "You might have to run it as root user."
    echo "Please run it again with 'sudo'."
    exit
fi

distro=$(grep "^ID=" /etc/os-release | cut -d\= -f2 | sed -e 's/"//g')
tmp=/tmp/linux

case $distro in
    "ubuntu" | "debian")
        apt-get install -y build-essential binutils-dev libelf-dev ;;
    "fedora")
        dnf groupinstall -y "Development Tools"
        dnf install -y binutils-devel elfutils-libelf-devel ;;
    "rhel" | "centos")
        yum groupinstall -y "Development Tools"
        yum install -y binutils-devel elfutils-libelf-devel ;;
    *)
        echo "\"$distro\" is not supported distro, so please install packages manually." ;;
esac

git clone --depth=1 https://github.com/xdp-project/bpf-next.git $tmp
cd $tmp
make -C tools/bpf/bpftool/ -j 4
make -C tools/bpf/bpftool/ install


#!/bin/bash
if [ "$1" == "-h" ]; then
  echo "Usage: `basename $0` [IP] [PORT] [DEV]"
  echo "* [DEV] only needed for tc, xdp"
  exit 0
fi

if [ "x$(id -u)" != x0 ]; then
    echo "You might have to run it as root user."
    echo "Please run it again with 'sudo'."
    exit
fi

IP=${1:-10.1.0.2}
PORT=${2:-1234}

# Fetch potential default interface
_DEV=$(route | grep '^default' | grep -o '[^ ]*$' | head -1)
DEV=${3:-$_DEV}

# Misc..
BPFTOOL=$(which bpftool)
RED='\033[0;31m'
NC='\033[0m'

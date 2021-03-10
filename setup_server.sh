#!/bin/bash
mkdir -p bin
make
name="server_`date +%y%m%d-%H%M%S`"
pidof tcpdump | kill -9
pidof server | kill -9
tcpdump -i any -p -s 100 -w $name.pcap udp &
mkdir -p log
./bin/server > ./log/$name &

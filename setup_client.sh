#!/bin/bash
mkdir -p bin
make
name="client_`date +%y%m%d-%H%M%S`"
pidof tcpdump | kill -9
pidof client | kill -9
tcpdump -i any -p -s 100 -w $name.pcap udp &
mkdir -p log
bash script/getGps.sh log
./bin/client > ./log/$name &

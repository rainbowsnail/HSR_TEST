#!/bin/bash
dir=$1
datename=$dir/capture_$(date +%Y%m%d-%H%M%S).txt
echo $datename
while [ true ];do
echo $(curl  http://192.168.10.1:8888/gpsInfo) >> $datename
#curl  http://192.168.10.1:8888/gpsInfo -o $datename

sleep 1
done


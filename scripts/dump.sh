#!/bin/bash

BUS=$1
ADDR=$2

echo "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f"

for i in {0..255}; do 
	if ! (( $i % 16)); then
		printf '%02x ' $i;
	fi
	val=`i2ccl $BUS w $ADDR $i r $ADDR 1`	
	printf " %s" ${val:2:2}
	if ! (( $((i+1)) % 16)); then 
		echo
	fi; 
done

echo ""


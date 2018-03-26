#!/bin/bash

if [ ! -e assn_3 ];then
	gcc ./assn_3.c -o assn_3
fi

if [ ! -e input.bin ];then
	echo "input.bin not exist."
	exit
fi

echo "test basic:"
./assn_3  --basic ./input.bin ./sort.bin

diff ./sort.bin ./sort.bin.standard
rm -rf input.bin.* sort.bin

echo "test multistep:"
./assn_3 --multistep ./input.bin ./sort.bin
diff ./sort.bin ./sort.bin.standard
rm -rf input.bin.* sort.bin

echo "test replacement:"
./assn_3  --replacement ./input.bin ./sort.bin
diff ./sort.bin sort.bin.standard
rm -rf input.bin.* sort.bin



#/bin/bash

if [ ! -e assn_4 ];then
	gcc ./assn_4.c -o assn_4
fi

./assn_4 index.bin 4 < ./inputLarge.txt > outputL

diff ./outputL outputLarge.txt

rm outputL index.bin assn_4

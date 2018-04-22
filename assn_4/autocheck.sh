#/bin/bash

if [ ! -e assn_4 ];then
	gcc ./assn_4.c -o assn_4
fi

./assn_4 index.bin 4 < ./input-01.txt > output-01.tmp

./assn_4 index.bin 4 < ./input-02.txt > output-02.tmp

diff ./output-01.txt output-01.tmp

diff ./output-02.txt output-02.tmp

rm output-01.tmp output-02.tmp index.bin assn_4

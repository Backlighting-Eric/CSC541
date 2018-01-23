#!/bin/bash


if [ ! -e assn_1} ];then
	gcc assn_1.c -o assn_1
fi

files=(output_mem_lin output_mem_bin output_disk_lin output_disk_bin)

i="0"

while [ $i -lt 4 ]
do
	[ -e ${files[i]} ] && rm ${files[i]}
	i=$[$i+1]
done

./assn_1 --mem-lin key.db seek.db > output_mem_lin
./assn_1 --mem-bin key.db seek.db > output_mem_bin
./assn_1 --disk-lin key.db seek.db > output_disk_lin
./assn_1 --disk-bin key.db seek.db > output_disk_bin

i="0"
while [ $i -lt 4 ]
do
	echo ${files[i]} 
	diff output.txt ${files[i]}
	i=$[$i+1]
done

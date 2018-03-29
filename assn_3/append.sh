#!/bin/bash

i="0"

while [ $i -lt 10 ]
do
	cat ./input.bin >> input.bin4

	i=$[$i+1]

done

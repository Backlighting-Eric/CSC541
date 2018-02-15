#!/bin/bash

if [ ! -e assn_2 ];then
	gcc assn_2.c -o assn_2
fi

inputfiles=(input-01.txt input-02.txt)

if [ -e student.db ];then
	rm -rf student.*
fi

./assn_2 --first-fit student.db < input-01.txt > my-output-01-first.txt
./assn_2 --first-fit student.db < input-02.txt > my-output-02-first.txt
echo "difference in first fit"
diff ./my-output-01-first.txt ./output-01-first.txt
diff ./my-output-02-first.txt ./output-02-first.txt


if [ -e student.db ];then
	rm -rf student.*
fi

./assn_2 --best-fit student.db < input-01.txt > my-output-01-best.txt
./assn_2 --best-fit student.db < input-02.txt > my-output-02-best.txt
echo "difference in best fit"
diff ./my-output-01-best.txt ./output-01-best.txt
diff ./my-output-02-best.txt ./output-02-best.txt

if [ -e student.db ];then
	rm -rf student.*
fi

./assn_2 --worst-fit student.db < input-01.txt > my-output-01-worst.txt
./assn_2 --worst-fit student.db < input-02.txt > my-output-02-worst.txt
echo "difference in worst fit"
diff ./my-output-01-worst.txt ./output-01-worst.txt
diff ./my-output-02-worst.txt ./output-02-worst.txt


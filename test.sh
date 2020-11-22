#!/bin/bash

#while read line; do 
#	echo "Text read from file is $line"
#done < temp.txt
	#cp test.txt '1'
	#cd '1' && g++ *.cpp -pthread &&(./a.out <<< test.txt) > temp.txt
	#cd && cd Desktop
	#cd '2' && gcc *.c -pthread && (./a.out < test.txt) > temp.txt
	#cd && cd Desktop
	#cd '3' && g++ *.cpp -pthread && (./a.out < test.txt) > temp.txt
for file in HW3/*/
do 
	cp test.txt "$file"
	cd "$file"
	g++ *.cpp -pthread && (./a.out <<< test.txt) > temp.txt
	gcc *.c -pthread && (./a.out <<< test.txt) > temp.txt
	cd
	cd Desktop
done





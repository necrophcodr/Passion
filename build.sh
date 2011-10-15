#!/bin/bash
printf "=== BUILDING ===\n";
cmake .;
make;
if [[ "$?" == "0" ]]; then
	printf "=== RUNNING ===\n";
	cd passion/bin;
	./passion
	printf "=== DONE === \n";
	exit 0;
else
	exit 1;
fi

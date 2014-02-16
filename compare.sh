#!/bin/sh

./plingtheory testall -b -o updates.out -r updates.root -v 6
diffuse updates.out ref.out 
./plingtheory compare ref.root updates.root 

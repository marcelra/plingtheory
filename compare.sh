#!/bin/sh

./plingtheory testall -b -o update.out -r update.root -v 6
diffuse update.out ref.out 
./plingtheory compare ref.root update.root -v 6

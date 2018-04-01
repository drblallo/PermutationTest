#!/bin/bash

max=10000
for (( i=0; i < $max; ++i ))
do
    echo $i
    matlab -nodesktop generateTestCase.m -r "try, run ('generateTestCase.m'); end; quit"
    cd generationDir/
    ./testProject
    cd ../
done

cd generationDir
python3 test.py
cd ../

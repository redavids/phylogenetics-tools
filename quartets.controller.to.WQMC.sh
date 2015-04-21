#!/bin/bash

#Goal: this script converts a list of quartet files in a folder that are outputs of quartet-controller.sh to quartetfiles suitable for input to wQMC

#for i in {1..100}; do
    #cat BS.$i.quartets | sed s/"(("//g | sed s/"),("/"|"/g | sed s/")); "/":"/g | sed '/|/!d' > wQMC.BS.$i.quartets
#done

#mkdir Cyano.wQMC.mlbs.quartets
for i in {2..100}; do
    mv wQMC.BS.$i.quartets Cyano.wQMC.mlbs.quartets/
done

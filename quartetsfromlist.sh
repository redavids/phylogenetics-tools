#!/bin/bash

for i in {1..1000}; do
    tail -n+$i $1 | head -n1 > doody
    python getridofedgeweights.py doody newdoody 
    #echo head -1 tmpfile
    /Users/ruthdavidson/code/treemeth/treequar.sh -e /Users/ruthdavidson/code/treemeth/newdoody /Users/ruthdavidson/code/hgt-data/model.50.2000000.0.000001.0/01/true.equartets.$i
done

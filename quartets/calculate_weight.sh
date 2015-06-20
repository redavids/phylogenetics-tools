#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo USAGE: $0 [genetreesfilename] [speciestreefilename]
fi

sh quartet_count.sh $1 | perl summarize_quartets_stdin.pl > $1.quartets

cat $1.quartets | sed s/"(("//g | sed s/"),("/"|"/g | sed s/")); "/":"/g | sed '/|/!d' > $2

rm $1.quartets

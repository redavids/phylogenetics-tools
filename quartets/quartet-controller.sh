#!/bin/sh

quartetcounts=${2}

if [ ! -s $quartetcounts ]
then
    sh ~/phylogenetics/quartets/quartet_count.sh $1 | perl ~/phylogenetics/quartets/summarize_quartets_stdin.pl  | sed s/"(("//g | sed s/"),("/"|"/g | sed s/")); "/":"/g | sed '/|/!d' > $2
fi


#sh quartet_count.sh $1 | perl summarize_quartets_stdin.pl > $2

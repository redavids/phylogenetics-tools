import dendropy
import numpy as np
import sys


#filename is a string that is path to your file 
def listparse(filename):
    L = dendropy.TreeList.get_from_path(filename,'newick')
    #labels = L.taxon_set.get_taxa(labels)
    taxa = L.taxon_set.get_taxa(labels=L.taxon_set.labels())
    labels = sorted([n.label for n in taxa])
    labeldict = {}
    for i in range(len(taxa)):
        labeldict[taxa[i].label] = labels.index(taxa[i].label)+1
        taxa[i].label = str(labels.index(taxa[i].label)+1)
    newnameslist = [L[i].as_newick_string()+';' for i in range(len(L))]
    return newnameslist,labeldict
        
        


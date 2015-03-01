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
       
#takes a specified directory with tree files (each file may contain multiple trees), standardizes leaf labels in each tree and returns a dictionary for all trees for translating between new labels and original taxon names
#new_tree_list contains all the trees with new labels in the original tree format form = 'newick', 'fasta', 'phylip', or 'nexus'
#currently all trees in the directory have to be of the same form
def fileswap(tree_directory,form):
    file_list = os.listdir(tree_directory)
    #concatenates all the tree files into a single file called combined_tree_list
    with open(tree_directory+'combined_tree_list', 'w') as combined_tree_list:
        for fname in file_list:
            with open(tree_directory+fname) as infile:
                for line in infile:
                    combined_tree_list.write(line)
                    
    combined_L = dendropy.TreeList.get_from_path(tree_directory+'combined_tree_list',form)
    combined_taxa = combined_L.taxon_set.get_taxa(labels=combined_L.taxon_set.labels())
    combined_labels = sorted([n.label for n in combined_taxa])
    labeldict = {}

    #creates taxon key for all trees in the directory and relabels all trees in combined_tree_list
    for i in range(len(combined_taxa)):
        labeldict[combined_taxa[i].label] = combined_labels.index(combined_taxa[i].label)+1
        combined_taxa[i].label = str(combined_labels.index(combined_taxa[i].label)+1)
    combined_L.write_to_path(tree_directory+'combined_tree_list_relabeled',form)

    H = open(tree_directory+'/combined_tree_list.taxon_key', 'w')
    H.write(str(labeldict))
    H.close()
   
  

import dendropy
import sys
import os

#treefilename must include path if it's not where you're running this from
treefilename = str(sys.argv[1])

H = open(treefilename,'r')

treestring = H.read()

H.close()

header = str('/////////////////////////////////////////////////////////////////////////////////////\n' +
'//                                                                                 //\n'+
'//  INDELible V1.03 control file - made by makeindeliblcondtrolfile.py             //\n'+
'//                                                                                 //\n'+
'//      A basic introduction to the structure of the INDELible control file.       //\n'+
'//                                                                                 //\n'+
'/////////////////////////////////////////////////////////////////////////////////////\n'+

'// It is useful to know that anything on a line after two forward slashes is ignored.\n'

'/*\n\t'+
   'Another useful thing to know is that anything after a forward slash and star\n'+
   'is ignored until INDELible sees a star followed by a forward slash later on.\n'
+'*/ \n')     

type = '[TYPE] NUCLEOTIDE 1 \n'
         

model = str('[MODEL]    ruthmodel        //  Evolutionary models are defined in [MODEL] blocks\n') +
  str('[submodel]     JC         //  Here the substitution model is simply set as JC69.\n')+
 str(' [indelmodel]   NB  0.4 1  //  Geometric indel length distribution (q=0.4, r=1)\n')+
  str('[insertrate]   0.08       //  insertion rate = 0.08 relative to substitution rate of 1\n')+
  str('[deleterate]   0.12       //  deletion rate = 0.12 relative to substitution rate of 1 \n')
  
tree = '[TREE] ' + ' ' + treefilename + ' ' + treestring +    str(' //  User trees are defined here\n')

partitions = '[PARTITIONS] ruthpartitionname             //  [PARTITIONS] blocks say which models go with \n\t
  [+'treefilename +' ruthmodel 1000]            //  which trees and define the length of the
                                       //  sequence generated at the root (1000 here).'

evolver = '[EVOLVE] partitionname 2 outputname  //  This will generate 2 replicate datasets 
                                       //  from the [PARTITIONS] block named above.

// The true alignment will be output in a file named outputname_TRUE.phy
// The unaligned sequences will be output in a file named outputname.fas
// To learn how to implement more complicated simulations (or different 
// models) please consult the manual or the other example control files.'

controlfile = os.getcwd()+'/control.txt'
G = open(controlfile,'w')

G.write(header)

G.write(type)

G.write(model)

G.write(tree)

G.write(partitions)

G.write(evolver)

G.close()




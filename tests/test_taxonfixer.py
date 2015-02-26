import dendropy
import unittest
import numpy as np
import taxonfixer

class TestTreeReWrite(unittest.TestCase):
    def test_tree_rewrite1(self):
        treelist = ['((fee,fi),(fo,fum));','((fee,fum),(fi,fa));','(((foo,fa),fi),fo);']
        taxondict = {'fee':1, 'fi':2, 'fo':3, 'fum':4, 'fa':5, 'foo':6}
        newlist = ['((1,2),(3,4));','((1,4),(2,5));','(((6,5),2),3);']
        n = taxonfixer.listparse(treelist)[0]
        d = taxonfixer.listparse(treelist)[1]
        self.assertEqual(n,newlist)
        self.assertEqual(d,taxondict)
        
        

import dendropy
import unittest
import numpy as np
import taxonfixer

class TestTreeReWrite(unittest.TestCase):
    def test_tree_rewrite1(self):
        treelist = dendropy.TreeList.get_from_path('/Users/ruthdavidson/code/phylogenetics-tools/tests/feefifofum','newick')
        taxondict = {'fa': 1, 'fee': 2, 'fi' : 3, 'fo': 4, 'foo': 5, 'fum': 6}
        newlist = ['((2,3),(4,6));', '((2,6),(3,1));', '(((5,1),3),4);']
        n,d = taxonfixer.listparse('/Users/ruthdavidson/code/phylogenetics-tools/tests/feefifofum')
        self.assertEqual(n,newlist)
        self.assertEqual(d,taxondict)
        
        

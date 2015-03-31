import ast
import dendropy
import unittest
import numpy as np
import taxonfixer

class TestDictFirstReWrite(unittest.TestCase):
    def test_rewrite1(self):
        taxondict = {'a':'1', 'b':'2','c_6':3, 'd_1':4}
        treelist = dendropy.TreeList.get_from_path('/Users/ruthdavidson/code/phylogenetics-tools/tests/underscorestrees','newick')
        taxonfixer.dictswap(taxondict,'/Users/ruthdavidson/code/phylogenetics-tools/tests/underscorestrees','newick','/Users/ruthdavidson/code/phylogenetics-tools/tests/newunderscorestrees')
        newlist = dendropy.TreeList.get_from_path('/Users/ruthdavidson/code/phylogenetics-tools/tests/newunderscorestrees','newick')
        L = [newlist[i].as_newick_string() for i in range(len(newlist))]
        self.assertEqual(L,['((3:1,4:2):0.4,1:0.1,2:5)','((1:3,4:1):0.2,2:0.1,3:7)'])


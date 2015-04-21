import ast
import dendropy
import unittest
import numpy as np
import makeindeliblecontrol

class TestFileFromTree(unittest.TestCase):
    def test_1(self):
        tree = '(A:0.1,B:0.1);'
        filename = 'treetest'


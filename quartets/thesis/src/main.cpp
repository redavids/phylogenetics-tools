#include <iostream>
#include <vector>
#include <ctime>
#include <string.h>
#include <algorithm>
#include "int_stuff.h"
#include "rooted_tree.h"
#include "unrooted_tree.h"
#include "output_handler.h"
#include "newick_parser.h"
#include "triplet_calc.h"
#include "quartet_calc.h"
#include "naive_calc.h"
#include "soda13_calc.h"
#include "hdt_factory.h"
#include "hdt.h"
#include "fancy_time.h"

#ifndef _MSC_VER
	#define _stricmp strcasecmp
#endif

#ifdef _DEBUG
	#include "nvwa/debug_new.h"
#endif

using namespace std;

// Global vars :x
RootedTree *rt1 = NULL, *rt2 = NULL;
UnrootedTree *ut1 = NULL, *ut2 = NULL;
NewickParser *parser = NULL;
NaiveCalc *naiveCalc;
TripletCalc *tripletCalc;
QuartetCalc *quartetCalc;

void usage(char *programName)
{
	cerr << endl;
	cerr << "Usage of the program is as follows:" << endl;
	cerr << programName << endl
			<< "\t" << " (calcTripDist | calcQuartDist | printTriplets |" << endl
			<< "\t" << " printQuartets | printTree | parseTree | fancy | " << endl
			<< "\t" << "HDT | HDTPrint | genTree <i> | genTreeArb <i>)" << endl
			<< "\t" << " [filename1 filename2]" << endl;
	cerr << endl;

	cerr << "calcTripDist" << "\t" << "Calculates the triplet distance between the two input files." << endl
			<< "\t\t" << "Thus requires two filenames!" << endl << endl;

	cerr << "calcQuartDist" << "\t" << "Calculates the quartet distance between the two input files." << endl
			<< "\t\t" << "Thus requires two filenames!" << endl << endl;

	cerr << "printTriplets" << "\t" << "Prints the triplets of a tree" << endl
			<< "\t\t" << "(buyer beware: This is going to be huge and take some time!)." << endl
			<< "\t\t" << "Last param is the tree used." << endl << endl;

	cerr << "printQuartets" << "\t" << "Prints the quartets of a tree" << endl
			<< "\t\t" << "(buyer beware: This is going to be huge and take some time!)." << endl
			<< "\t\t" << "Last param is the tree used." << endl << endl;

	cerr << "printTree" << "\t" << "Prints the parsed tree (last param!) as dot format." << endl << endl;

	cerr << "printRootedTree" << "\t" << "Roots the tree parsed tree (last param!) and prints it as dot format." << endl << endl;

	cerr << "parseTree" << "\t" << "Only parses the input file (the last param)." << endl
			<< "\t\t" << "Can be used for seeing how fast (or slow?) the parsing is (e.g.)." << endl << endl;

	cerr << "fancy" << "\t\t" << "Use the fancy calculator (i.e. the (hopefully fast) soda13 one)" << endl
			<< "\t\t" << "(i.e. the not naive one). If not specified the naive implementation is used!" << endl << endl;

	cerr << "HDT" << "\t\t" << "Do a HDT of the tree (input 1)" << endl << endl;

	cerr << "HDTPrint" << "\t" << "Do a HDT of the tree (input 1) and print it as dot format." << endl << endl;

	cerr << "genTree <i>" << "\t" << "Generates a random binary tree with <i> leafs (named 1 to <i>)." << endl << endl;

	cerr << "genTreeArb <i>" << "\t" << "Generates a random non-binary tree with <i> leafs (named 1 to <i>)." << endl << endl;

	cerr << endl;
	cerr << "Filenames are expected to point to files in the Newick fileformat." << endl;
	cerr << endl;
}

/*
	Print debug stuff if we're in debug mode
	Free stuff (if allocated)
	Etc.
*/
unsigned int programStart;
void program_exiting()
{
	if (rt1 != NULL) delete rt1->factory;
	if (rt2 != NULL) delete rt2->factory;
	if (ut1 != NULL) delete ut1;
	if (ut2 != NULL) delete ut2;
	if (parser != NULL) delete parser;
	if (tripletCalc != NULL) delete tripletCalc;
	if (quartetCalc != NULL) delete quartetCalc;
	if (naiveCalc != NULL) delete naiveCalc;

	#ifdef _MSC_VER
		#ifdef _DEBUG
			_CrtDumpMemoryLeaks();
			system("pause");
		#endif
	#endif
}

void genRandomTreeRec(vector<int> leafs, unsigned int maxDegreeThis, unsigned int maxDegree, bool printedRoot)
{
	if (leafs.size() <= maxDegreeThis)
	{
		if (leafs.size() > 1) cout << "(";
		for(unsigned int i = 0; i < leafs.size(); i++)
		{
			cout << leafs[i];
			if (i != (leafs.size() - 1)) cout << ",";
		}
		if (leafs.size() > 1) cout << ")";
		return;
	}

	vector<int> *children = new vector<int>[maxDegreeThis];
	vector<int>::iterator i = leafs.begin();
	children[0].push_back(*i++);
	children[1].push_back(*i++);
	int biasHere = rand() % maxDegreeThis;
	for(; i != leafs.end(); i++)
	{
		int putHere = rand() % maxDegreeThis;
		if (rand() < RAND_MAX / 2) putHere = biasHere;
		children[putHere].push_back(*i);
	}
	
	bool hasPrintedFirst = false;
	cout << "(";
	for(unsigned int i = 0; i < maxDegreeThis; i++)
	{
		if (children[i].size() == 0) continue;

		if (hasPrintedFirst) cout << ",";
		//if (children[i].size() > 1) cout << "(";
		genRandomTreeRec(children[i], 2 + rand() % (maxDegree - 1), maxDegree, true);
		//if (children[i].size() > 1) cout << ")";
		hasPrintedFirst = true;
	}
	cout << ")";
	delete[] children;
}

/*
int ds[6] = {0};
void countDs(RootedTree *t)
{
	ds[t->numChildren]++;
	for(TemplatedLinkedList<RootedTree*> *i = t->children; i != NULL; i = i->next)
	{
		countDs(i->data);
	}
	return;
}
*/

void genRandomTree(int i, unsigned int maxDegree)
{
	srand(FancyTime::preciseClock());

	vector<int> data;
	for(int j = 1; j <= i; j++)
	{
		data.push_back(j);
	}
	random_shuffle(data.begin(), data.end());

	if (maxDegree == 2)
		genRandomTreeRec(data, 2, maxDegree, false);
	else
		genRandomTreeRec(data, 2 + rand() % (maxDegree - 1), maxDegree, false);
	cout << ";";
}

bool isNum(char* s)
{
	while (*s != 0)
	{
		if (*s < 48 || *s > 57) return false;
		s++;
	}
	return true;
}

int main(int argc, char** argv)
{
	// Register exit-handler (for debug printing etc)
	atexit(program_exiting);
	
	bool calcTripDist = false;
	bool calcQuartDist = false;
	bool printTriplets = false;
	bool printQuartets = false;
	bool printTree = false;
	bool printRootedTree = false;
	bool parseTree = false;
	bool useFancy = false;
	bool hdt = false;
	bool hdtPrint = false;
	bool genTree = false;
	bool genTreeArb = false;
	int numInput = 0;
	char *filename1 = NULL, *filename2 = NULL;

	// Parse commandline
	for(int i = 1; i < argc; i++)
	{
		calcTripDist = calcTripDist || (_stricmp(argv[i], "calcTripDist") == 0);
		calcQuartDist = calcQuartDist || (_stricmp(argv[i], "calcQuartDist") == 0);
		printTriplets = printTriplets || (_stricmp(argv[i], "printTriplets") == 0);
		printQuartets = printQuartets || (_stricmp(argv[i], "printQuartets") == 0);
		printTree = printTree || (_stricmp(argv[i], "printTree") == 0);
		printRootedTree = printRootedTree || (_stricmp(argv[i], "printRootedTree") == 0);
		parseTree = parseTree || (_stricmp(argv[i], "parseTree") == 0);
		useFancy = useFancy || (_stricmp(argv[i], "fancy") == 0);
		hdt = hdt || (_stricmp(argv[i], "hdt") == 0);
		hdtPrint = hdtPrint || (_stricmp(argv[i], "hdtPrint") == 0);
		genTree = genTree || (_stricmp(argv[i], "genTree") == 0);
		genTreeArb = genTreeArb || (_stricmp(argv[i], "genTreeArb") == 0);

		if (isNum(argv[i])) numInput = atoi(argv[i]);
	}

	if (!calcTripDist && !calcQuartDist && !printTriplets && !printQuartets && !printTree
		&& !printRootedTree && !parseTree && !hdt && !hdtPrint && !genTree && !genTreeArb)
	{
		// No (reasonable) commands given!
		usage(argv[0]);
		return -1;
	}

	// If calculating distance, check that the last 2 inputs are files
	if (calcTripDist || calcQuartDist)
	{
		if (argc < 4)
		{
			// Error!
			cerr << "Not enough parameters for calculating distance! (" << argc << " < 4)" << endl;
			usage(argv[0]);
			return -1;
		}
		filename1 = argv[argc-2];
		filename2 = argv[argc-1];
	}

	// If parsing or printing, require 1 input file!
	if (printTriplets || printQuartets || printTree || printRootedTree || parseTree || hdt || hdtPrint)
	{
		if (argc < 3)
		{
			// Error!
			cerr << "Not enough parameters for parsing or printing! (" << argc << " < 3)" << endl;
			usage(argv[0]);
			return -1;
		}
		filename2 = argv[argc-1];
	}

	// Parse the input files
	parser = new NewickParser();
	if (filename1 != NULL)
	{
		ut1 = parser->parseFile(filename1);
		if (ut1 == NULL || parser->isError())
		{
			cerr << "Parsing of file \"" << filename1 << "\" failed." << endl;
			cerr << "Aborting!" << endl;
			return -1;
		}
	}

	if (filename2 != NULL)
	{
		ut2 = parser->parseFile(filename2);
		if (ut2 == NULL || parser->isError())
		{
			cerr << "Parsing of file \"" << filename2 << "\" failed." << endl;
			cerr << "Aborting!" << endl;
			return -1;
		}
	}

	// Initialize calculators
	if (useFancy)
	{
		if (calcTripDist) tripletCalc = new Soda13Calc();
		if (calcQuartDist) quartetCalc = new Soda13Calc();
	}
	else
	{
		if (calcTripDist) tripletCalc = new NaiveCalc();
		if (calcQuartDist) quartetCalc = new NaiveCalc();
	}
	if (printTriplets || printQuartets)
	{
		naiveCalc = new NaiveCalc();
	}

	// Do as told!
	INTTYPE_N4 lastResult = -1;
	if (calcTripDist)
	{
		if (rt1 == NULL) rt1 = ut1->convertToRootedTree(NULL);
		if (rt2 == NULL) rt2 = ut2->convertToRootedTree(rt1->factory);
		
		//HDT *hdt = HDT::constructHDT(rt1, rt1->maxDegree);
		//RootedTree *hopefull = hdt->goBack();
		//delete hdt;
		//hopefull->toDot();
		//rt2->toDot();
		//lastResult = tripletCalc->calculateTripletDistance(rt2, hopefull);
		//hopefull->deleteTreeExceptLeafs();
		lastResult = tripletCalc->calculateTripletDistance(rt1, rt2);
		cout << lastResult << endl;

		#ifdef doCounter
		cout << "Counter = " << Soda13Calc::runtimeThingy << endl;
		#endif
	}
	if (calcQuartDist)
	{
		// Smallest D as t1
		if (ut1->maxDegree <= ut2->maxDegree)
			lastResult = quartetCalc->calculateQuartetDistance(ut1, ut2);
		else
			lastResult = quartetCalc->calculateQuartetDistance(ut2, ut1);
		cout << lastResult << endl;

		#ifdef doCounter
		cout << "Counter = " << Soda13Calc::runtimeThingy << endl;
		#endif
	}
	if (printTriplets)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		naiveCalc->printTriplets(rt2);
	}
	if (printQuartets)
	{
		naiveCalc->printQuartets(ut2);
	}
	if (printTree)
	{
		ut2->toDot();
	}
	/*
	if (parseTree)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		countDs(rt2);
		for (int i = 0; i < 6; i++)
		{
			cout << i << "\t" << ds[i] << endl;
		}
	}
	*/
	if (printRootedTree)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		rt2->toDot();
	}
	if (hdt)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		long long rootedTreeSize = rt2->factory->getSizeInRam();
		cout << "Size of RootedTree: " << rootedTreeSize << " bytes" << endl;

		HDT *h = HDT::constructHDT(rt2, 0, NULL); // doesn't need degree here!
		long long HDTSize = h->factory->getSizeInRam();
		cout << "Size of HDT: " << HDTSize << " bytes" << endl;
		delete h->factory;

		cout << HDTSize / rootedTreeSize << endl;
	}
	if (hdtPrint)
	{
		rt2 = ut2->convertToRootedTree(NULL);
		HDT *h = HDT::constructHDT(rt2, 0, NULL); // doesn't need degree here!
		h->toDot();
		delete h->factory;
	}
	if (genTree)
	{
		genRandomTree(numInput, 2);
	}
	if (genTreeArb)
	{
		genRandomTree(numInput, 10);
	}

	return 0;
}

#include <algorithm>
#include "naive_calc.h"

INTTYPE_REST NaiveCalc::calculateTripletDistance(RootedTree *t1, RootedTree *t2)
{
	oh = new OutputHandler(true);
	vector<string> *res1, *res2;

	calcTriplets(t1);
	res1 = oh->extractLinesAndReset();
	calcTriplets(t2);
	res2 = oh->extractLinesAndReset();

	delete oh;
	INTTYPE_REST result = compareVectors(res1, res2);

	delete res1;
	delete res2;

	return result;
}

INTTYPE_N4 NaiveCalc::calculateQuartetDistance(UnrootedTree *t1, UnrootedTree *t2)
{
	oh = new OutputHandler(true);
	vector<string> *res1, *res2;

	calcQuartets(t1);
	res1 = oh->extractLinesAndReset();
	calcQuartets(t2);
	res2 = oh->extractLinesAndReset();

	delete oh;
	INTTYPE_N4 result = compareVectors(res1, res2);

	delete res1;
	delete res2;

	return result;
}

void NaiveCalc::printTriplets(RootedTree *t)
{
	oh = new OutputHandler(false);
	calcTriplets(t);
	delete oh;
}

void NaiveCalc::printQuartets(UnrootedTree *t)
{
	oh = new OutputHandler(false);
	calcQuartets(t, true);
	delete oh;
}

INTTYPE_N4 NaiveCalc::compareVectors(vector<string> *resOne, vector<string> *resTwo)
{
	if (resOne->size() != resTwo->size())
	{
		// Not the same output size?!? Can't compare!
		return -1;
	}

	// Compare by sorting both, running through both lists simultaneously
	sort(resOne->begin(), resOne->end());
	sort(resTwo->begin(), resTwo->end());

	unsigned int i = 0;
	unsigned int j = 0;
	INTTYPE_N4 equals = 0;
	while(i < resOne->size() && j < resOne->size())
	{
		string s1 = resOne->at(i);
		string s2 = resTwo->at(j);
		int comp = s1.compare(s2);
		if (comp == 0)
		{
			i++;
			j++;
			equals++;
		}
		else if (comp < 0)
		{
			i++;
		}
		else // comp > 0
		{
			j++;
		}
	}
	return resOne->size() - equals;
}

void NaiveCalc::calcTriplets(RootedTree *t)
{
	string easySorter[3];

	vector<RootedTree*> *list = t->getList();
	for (unsigned int i1 = 0; i1 < list->size()-2; i1++)
	{
		RootedTree *t1 = list->at(i1);
		for (unsigned int i2 = i1+1; i2 < list->size()-1; i2++)
		{
			RootedTree *t2 = list->at(i2);
			for (unsigned int i3 = i2+1; i3 < list->size(); i3++)
			{
				RootedTree *t3 = list->at(i3);

				RootedTree *p1 = t1->getParent();
				RootedTree *p2 = t2->getParent();
				RootedTree *p3 = t3->getParent();

				while ((p1 != p2 && p1 != p3 && p2 != p3) || (p1 == p2 && p1->level < p3->level) || (p1 == p3 && p1->level < p2->level) || (p2 == p3 && p2->level < p1->level))
				{
					if (p1->level == p2->level && p2->level == p3->level)
					{
						if (p1->getParent() != NULL)
							p1 = p1->getParent();
						if (p2->getParent() != NULL)
							p2 = p2->getParent();
						if (p3->getParent() != NULL)
							p3 = p3->getParent();
					}
					else
					{
						unsigned int max = RootedTree::max(p1, p2, p3);
						if (max == p1->level)
							p1 = p1->getParent();
						if (max == p2->level)
							p2 = p2->getParent();
						if (max == p3->level)
							p3 = p3->getParent();
					}
				}

				if (p1 == p2 && p2 == p3)
				{
					easySorter[0] = t1->name;
					easySorter[1] = t2->name;
					easySorter[2] = t3->name;
					sort(easySorter, easySorter+3);
					oh->out << easySorter[0] << " " << easySorter[1] << " " << easySorter[2];
					oh->commit();
				}
				else
				{
					if (p1 == p2)
					{
						easySorter[0] = t3->name;
						easySorter[1] = t1->name;
						easySorter[2] = t2->name;
					}
					else if (p1 == p3)
					{
						easySorter[0] = t2->name;
						easySorter[1] = t1->name;
						easySorter[2] = t3->name;
					}
					else if (p2 == p3)
					{
						easySorter[0] = t1->name;
						easySorter[1] = t2->name;
						easySorter[2] = t3->name;
					}
					else
					{
						cout << "WTF?!?" << endl;
					}
					sort(easySorter+1,easySorter+3);

					oh->out << easySorter[0] << " | " << easySorter[1] << " " << easySorter[2];
					oh->commit();
				}
			}
		}
	}
	delete list;
}

void NaiveCalc::calcQuartets(UnrootedTree *ut, bool printAnchor)
{
	string easySorter[4];
	RootedTree *t = ut->convertToRootedTree(NULL);

	vector<RootedTree*> *list = t->getList();
	for (unsigned int i1 = 0; i1 < list->size()-3; i1++)
	{
		RootedTree *t1 = list->at(i1);
		for (unsigned int i2 = i1+1; i2 < list->size()-2; i2++)
		{
			RootedTree *t2 = list->at(i2);
			for (unsigned int i3 = i2+1; i3 < list->size()-1; i3++)
			{
				RootedTree *t3 = list->at(i3);
				for(unsigned int i4 = i3+1; i4 < list->size(); i4++)
				{
					RootedTree *t4 = list->at(i4);

					RootedTree *p1 = t1->getParent();
					RootedTree *p2 = t2->getParent();
					RootedTree *p3 = t3->getParent();
					RootedTree *p4 = t4->getParent();
					while (((p1 != p2 && p1 != p3 && p1 != p4 && p2 != p3 && p2 != p4 && p3 != p4) ||
						 (p1 == p2 && (p1->level < p3->level || p1->level < p4->level)) ||
						 (p1 == p3 && (p1->level < p2->level || p1->level < p4->level)) ||
						 (p1 == p4 && (p1->level < p2->level || p1->level < p3->level)) ||
						 (p2 == p3 && (p2->level < p1->level || p2->level < p4->level)) ||
						 (p2 == p4 && (p2->level < p1->level || p2->level < p3->level)) ||
						 (p3 == p4 && (p3->level < p1->level || p3->level < p2->level))) &&
						 !((p1 == p2 && p3 == p4) || (p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3))
						 )
					{
						if (p1->level == p2->level && p2->level == p3->level && p3->level == p4->level)
						{
							if (p1->getParent() != NULL)
								p1 = p1->getParent();
							if (p2->getParent() != NULL)
								p2 = p2->getParent();
							if (p3->getParent() != NULL)
								p3 = p3->getParent();
							if (p4->getParent() != NULL)
								p4 = p4->getParent();
						}
						else
						{
							unsigned int max = RootedTree::max(p1, p2, p3, p4);
							if (max == p1->level)
								p1 = p1->getParent();
							if (max == p2->level)
								p2 = p2->getParent();
							if (max == p3->level)
								p3 = p3->getParent();
							if (max == p4->level)
								p4 = p4->getParent();
						}
					}

					if ((p1 == p2 && (p2 == p3 || p2 == p4)) ||
						(p3 == p4 && (p3 == p1 || p3 == p2)))
					{
						easySorter[0] = t1->name;
						easySorter[1] = t2->name;
						easySorter[2] = t3->name;
						easySorter[3] = t4->name;
						sort(easySorter, easySorter+4);
						
						if (printAnchor && (p1 == p2 && (p2 == p3 || p2 == p4)))
						{
							if (p1 == p2 && p2 == p3 && p3 == p4)
								oh->out << p1->name << ", delta: ";
							else
								oh->out << p1->name << ", epsilon: ";
						}
						else if (printAnchor)
						{
							oh->out << p3->name << ", epsilon: ";
						}
						//if (!printAnchor)
						{
							oh->out << easySorter[0] << " " << easySorter[1] << " " << easySorter[2] << " " << easySorter[3];
							oh->commit();
							//cout << easySorter[0] << " " << easySorter[1] << " " << easySorter[2] << " " << easySorter[3] << endl;
						}
					}
					else
					{
						if (p1 == p2 || p3 == p4)
						{
							easySorter[0] = t1->name;
							easySorter[1] = t2->name;
							easySorter[2] = t3->name;
							easySorter[3] = t4->name;
						}
						else if (p1 == p3 || p2 == p4)
						{
							easySorter[0] = t1->name;
							easySorter[1] = t3->name;
							easySorter[2] = t2->name;
							easySorter[3] = t4->name;
						}
						else if (p1 == p4 || p2 == p3)
						{
							easySorter[0] = t1->name;
							easySorter[1] = t4->name;
							easySorter[2] = t2->name;
							easySorter[3] = t3->name;
						}
						else
						{
							cout << "WTF?!?" << endl;
						}

						if (printAnchor)
						{
							bool isAlphaBool = false;
							while ((!((p1 == p2 && (p2 == p3 || p2 == p4)) || (p3 == p4 && (p3 == p1 || p3 == p2)))) ||
								(p1 == p2 && (p2 == p3 || p2 == p4) && (p1->level < p3->level || p1->level < p4->level)) ||
								(p3 == p4 && (p3 == p1 || p3 == p2) && (p3->level < p1->level || p3->level < p2->level))
								)
							{
								isAlphaBool = isAlphaBool || isAlpha(p1, p2, p3, p4);
								if (p1->level == p2->level && p2->level == p3->level && p3->level == p4->level)
								{
									if (p1->getParent() != NULL)
										p1 = p1->getParent();
									if (p2->getParent() != NULL)
										p2 = p2->getParent();
									if (p3->getParent() != NULL)
										p3 = p3->getParent();
									if (p4->getParent() != NULL)
										p4 = p4->getParent();
								}
								else
								{
									unsigned int max = RootedTree::max(p1, p2, p3, p4);
									if (max == p1->level)
										p1 = p1->getParent();
									if (max == p2->level)
										p2 = p2->getParent();
									if (max == p3->level)
										p3 = p3->getParent();
									if (max == p4->level)
										p4 = p4->getParent();
								}
							}

							if (isAlphaBool)
							{
								oh->out << p1->name << ", alpha: ";
							}
							else if (p1 == p2 && p2 == p3 && p3 == p4)
							{
								oh->out << p1->name << ", beta: ";
							}
							else
							{
								oh->out << p3->name << ", gamma: ";
							}
						}

						sort(easySorter,easySorter+2);
						sort(easySorter+2,easySorter+4);
						if (easySorter[0] < easySorter[2])
							oh->out << easySorter[0] << " " << easySorter[1] << " | " << easySorter[2] << " " << easySorter[3];
						else
							oh->out << easySorter[2] << " " << easySorter[3] << " | " << easySorter[0] << " " << easySorter[1];
						oh->commit();
					}
				}
			}
		}
	}

	delete t->factory;
	delete list;
}

bool NaiveCalc::isAlpha(RootedTree *p1, RootedTree *p2, RootedTree *p3, RootedTree *p4)
{
	// Has 3 or more met?
	if ((p1 == p2 && (p2 == p3 || p2 == p4)) ||
		(p3 == p4 && (p3 == p1 || p3 == p2)))
		return false;

	if (!(p1->level == p2->level && p2->level == p3->level && p3->level == p4->level)) return false;

	unsigned int meets = 0;
	if (p1 == p2 || p1 == p3 || p1 == p4) meets++;
	if (p2 == p3 || p2 == p4) meets++;
	if (p3 == p4) meets++;

	return meets == 2;
}
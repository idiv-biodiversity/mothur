#ifndef DOTAXONOMY_H
#define DOTAXONOMY_H

/*
 * phylotree.h
 *  
 *
 *  Created by Pat Schloss on 6/17/09.
 *  Copyright 2009 Patrick D. Schloss. All rights reserved.
 *
 */

#include "mothur.h"
#include "mothurout.h"

/**************************************************************************************************/

struct TaxNode {
	vector<string> accessions;	//names of seqs in this branch of tree
	map<string, int> children;  //childs name to index in tree
	int parent, childNumber, level;
	string name, heirarchyID;
	
	TaxNode(string n) : name(n), level(0), parent(-1) {		}
	TaxNode(){}
};

/**************************************************************************************************/

class PhyloTree {

public:
	PhyloTree();
	PhyloTree(string);  //pass it a taxonomy file and it makes the tree
	PhyloTree(ifstream&);  //pass it a taxonomy file and it makes the train.tree
	~PhyloTree() {};
	int addSeqToTree(string, string);
	void assignHeirarchyIDs(int);
	void print(ofstream&);
	void printTreeNodes(string); //used by bayesian to save time
	vector<int> getGenusNodes();
	vector<int> getGenusTotals();	
	void binUnclassified();
		
	TaxNode get(int i)				{	return tree[i];							}
	TaxNode get(string seqName)		{	return tree[name2Taxonomy[seqName]];	}
	int getIndex(string seqName)	{	return name2Taxonomy[seqName];			}
	string getName(int i)			{	return tree[i].name;					}
	string getFullTaxonomy(string);	 //pass a sequence name return taxonomy
	int getMaxLevel()				{	return maxLevel;						}
	int getNumSeqs()  {  return numSeqs;  }
	
private:
	string getNextTaxon(string&);
	vector<TaxNode> tree;
	vector<int> genusIndex; //holds the indexes in tree where the genus level taxonomies are stored
	vector<int> totals; //holds the numSeqs at each genus level taxonomy
	map<string, int> name2Taxonomy;  //maps name to index in tree
	map<int, int> uniqueTaxonomies;  //map of unique taxonomies
	void print(int, ofstream&);
	int numNodes;
	int numSeqs;
	int maxLevel;
	bool calcTotals;
	MothurOut* m;
};

/**************************************************************************************************/

#endif



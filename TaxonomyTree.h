//******************************************************************************//
// Copyright 2011 Rui Chen, Noman Mohammed, and Benjamin C. M. Fung             //
//                                                                              //
// Licensed under the Apache License, Version 2.0 (the "License");              //
// you may not use this file except in compliance with the License.             //
// You may obtain a copy of the License at                                      //
//                                                                              //
//      http://www.apache.org/licenses/LICENSE-2.0                              //
//                                                                              //
// Unless required by applicable law or agreed to in writing, software          //
// distributed under the License is distributed on an "AS IS" BASIS,            //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     //
// See the License for the specific language governing permissions and          //
// limitations under the License.                                               //
//******************************************************************************//

#ifndef TAXONOMYTREE_H
#define TAXONOMYTREE_H

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "TaxonomyTreeNode.h"

using namespace std;

class TaxonomyTree{
private:
    TaxonomyTreeNode* root; // suppose to be the root, but it seems to be the first child of the root?
    int domainSize; // the total number of leaf nodes
	//TaxonomyTreeNode* firstLeaf;

public:
    TaxonomyTree(int); //the input is the total number of items (that is, leaf nodes)
    ~TaxonomyTree();

	TaxonomyTreeNode* buildTaxonomyTree();
    //bool readLine(ifstream& in, int& iid); Do we really need this? We can give the maxID directly 
    inline TaxonomyTreeNode* getRoot(){ return root; };
	inline int getNumOfInternalNodes(){ return root->getId()-domainSize;}; //get the total internal node of the taxonomy tree
	
	void print();

private:
	//helper function: prints all nodes starting from the given node
	void print(TaxonomyTreeNode*);
};



#endif

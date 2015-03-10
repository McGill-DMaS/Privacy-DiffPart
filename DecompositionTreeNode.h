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

#ifndef DECOMPOSITIONTREENODE_H
#define DECOMPOSITIONTREENODE_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "TaxonomyTreeNode.h"

using namespace std;

class DecompositionTreeNode{
private:
	vector<TaxonomyTreeNode*>* hierarchyCut; //the hierarchy cut of this node
	double count; //the noisy count of this hierarchy cut
    vector<DecompositionTreeNode*>* children;
    DecompositionTreeNode* nextNode; // points to the next node on the same level
    DecompositionTreeNode* parentNode; 
    
public:
    DecompositionTreeNode(vector<TaxonomyTreeNode*>*, double);
	DecompositionTreeNode(vector<TaxonomyTreeNode*>*);
    ~DecompositionTreeNode();

	//setters and getters
	inline DecompositionTreeNode* getNext(){return nextNode;};
	inline vector<DecompositionTreeNode*>* getChildren() {return children;};
    inline DecompositionTreeNode* getParent(){ return parentNode;};
	inline double getCount(){ return count;};

	inline void setNext(DecompositionTreeNode* inext) {nextNode = inext;};
    inline void setParent(DecompositionTreeNode* parent) {parentNode=parent;};
	inline void setChildren(vector<DecompositionTreeNode*>* ichildren){children = ichildren;};
	inline void setCount(double icount){count=icount;};

    void print();
};


#endif

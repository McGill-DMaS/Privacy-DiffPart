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

#ifndef DECOMPOSITIONTREE_H
#define DECOMPOSITIONTREE_H

#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "DecompositionTreeNode.h"
#include "TaxonomyTreeNode.h"

using namespace std;

class DecompositionTree{

private:
    DecompositionTreeNode* root;

public:
    DecompositionTree();
    ~DecompositionTree();

	inline DecompositionTreeNode* getRoot(){ return root; };
	inline void setRoot(DecompositionTreeNode* iroot){root = iroot;};

private:
	//helper function: prints all nodes starting from the given node
	//void print(DecompositionTreeNode*);
};



#endif
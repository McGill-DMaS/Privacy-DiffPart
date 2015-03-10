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

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
//#include <sys/time.h>
#include <cmath>
#include <assert.h>
//#include "Global.h"
#include "TaxonomyTreeNode.h"

using namespace std;

TaxonomyTreeNode::TaxonomyTreeNode(int imin, int imax, int iid, int ilevel, vector<TaxonomyTreeNode*>* ichildren, int inumChildren, int inumLeaves){
    min = imin;
    max = imax;
    id = iid;
    level = ilevel;
    numChildren = inumChildren;
    numLeaves = inumLeaves;
    children = ichildren;
    nextNode = NULL;
	parentNode = NULL;
}

TaxonomyTreeNode::~TaxonomyTreeNode(){
    delete children;
}

TaxonomyTreeNode* TaxonomyTreeNode::getFirstChild(){
	if(children == NULL)
		return NULL;
	else
		return (*children)[0];
}

void TaxonomyTreeNode::print(){
    cout << id << " ";
}
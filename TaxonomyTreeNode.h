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

#ifndef TAXONOMYTREENODE_H
#define TAXONOMYTREENODE_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>

using namespace std;

class TaxonomyTreeNode{
private:
	int level;  // leaf level is 0;
    int numChildren; // nodes in the leaf level have 0 child
    int min;  //this, together with max, is used to determine if the node is a specialization/generalization of another node
    int max;  //this, together with min, is used to determine if the node is a specialization/generalization of another node
    int id;
    vector<TaxonomyTreeNode*>* children;
    int numLeaves; //nodes in the leaf level have 1 leaf, that is, itself
	int numInternalNodes; //number of internal nodes in the subtree rooted at this node. In our definition, this node
	                      //is also an internal node of the subtree. An exception: leaf nodes have 0 internal nodes.

    TaxonomyTreeNode* nextNode; // points to the next node on the same level
    TaxonomyTreeNode* parentNode; 
    
public:
	//int min, int max, int id, int level, vector<TaxonomyTreeNode*>* children, int numChildren, int numLeaves
    TaxonomyTreeNode(int, int, int, int, vector<TaxonomyTreeNode*>*, int, int);
    ~TaxonomyTreeNode();

	//setters and getters
	inline void setID(int i){id = i;};
	inline void setNext(TaxonomyTreeNode* inext) {nextNode = inext;};
	inline TaxonomyTreeNode* getNext(){return nextNode;};
	inline int getMin(){return min;};
	inline int getMax(){return max;};
    inline int getNumOfLeaves() { return numLeaves; };
    inline int getLevel() { return level; };
    inline int getId() { return id; };
	inline vector<TaxonomyTreeNode*>* getChildren() {return children;};
    inline void setParent(TaxonomyTreeNode* parent) {parentNode=parent;};
    inline TaxonomyTreeNode* getParent(){ return parentNode;};
	inline int getNumOfChildren() { return numChildren;};

	inline void setNumOfInternalNodes(int numInNode){numInternalNodes = numInNode;};	
	inline int getNumOfInternalNodes() {return numInternalNodes;};

	TaxonomyTreeNode* getFirstChild();
    void print();
};


#endif

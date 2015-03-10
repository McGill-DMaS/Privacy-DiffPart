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
#include <set>
#include <queue>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "Global.h"
#include "TaxonomyTree.h"
#include "TaxonomyTreeNode.h"

using namespace std;

TaxonomyTree::TaxonomyTree(int idomainSize){
	domainSize = idomainSize;
	root = buildTaxonomyTree();
}

TaxonomyTree::~TaxonomyTree(){
}

TaxonomyTreeNode* TaxonomyTree::buildTaxonomyTree(){
	// to construct the leaf level of the taxonomy tree
	TaxonomyTreeNode* dummy = new TaxonomyTreeNode(0,0,0,0,NULL,0,0);
	TaxonomyTreeNode* previousNode = dummy;

	for(int i=1; i<=domainSize; i++){
		//TaxonomyTreeNode(int imin, int imax, int iid, int ilevel, vector<TaxonomyTreeNode*>* ichildren, int inumChildren, int inumLeaves)
		TaxonomyTreeNode* newNode = new TaxonomyTreeNode(i, i, i, 0, NULL, 0, 1);
		//for nodes in the leaf level, they have 0 internal nodes
		newNode->setNumOfInternalNodes(0);
		previousNode->setNext(newNode);
		previousNode = newNode;
	}
	previousNode->setNext(NULL);

	//to construct the upper levels of the taxonomy tree
	vector<TaxonomyTreeNode*>* children = NULL;
	TaxonomyTreeNode* newDummy = NULL;
	int currentLevel = 1;
	//every node will get an ID in ascending order
	int currentId = domainSize;

	while(true){
		newDummy = new TaxonomyTreeNode(0,0,0,currentLevel, NULL, 0, 0);
		previousNode = newDummy;

		//get the first node in Level currentLevel-1
		TaxonomyTreeNode* newNode = dummy->getNext();
		int i=0, numLeaves = 0;
		int min, max, numChildren;

		for(; newNode!=NULL; newNode=newNode->getNext(), i++){
			//if newNode is the first node in the subtree
			if(i%FAN_OUT ==0){
				numLeaves = 0;
				min = newNode->getMin();
				children = new vector<TaxonomyTreeNode*>;
			}

			children->push_back(newNode);
			numLeaves += newNode->getNumOfLeaves();
			max = newNode->getMax();

			//if newNode is the last node in the subtree, we need to create the parent node
			if(i%FAN_OUT == FAN_OUT-1){ 
				max = newNode->getMax();
				numChildren = FAN_OUT;
				TaxonomyTreeNode* tmpNode = new TaxonomyTreeNode(min, max, ++currentId, currentLevel, children, numChildren, numLeaves);
				previousNode->setNext(tmpNode);
				previousNode = tmpNode;
				
				//set the parent node for all nodes in children and calculate the internal node of this node
				int numOfInternalNode = 0;
				for(int j=0; j<numChildren; j++){
					numOfInternalNode += (*children)[j]->getNumOfInternalNodes();
					(*children)[j]->setParent(tmpNode);
				}
				// we need to add 1 to numOfInternalNode to make it work
				tmpNode->setNumOfInternalNodes(++numOfInternalNode);
			}
		}

		//if i%FAN_OUT !=0, there must be some leftovers. Then we create a subtree for them
		if(i%FAN_OUT!=0){
			numChildren = i%FAN_OUT;
			TaxonomyTreeNode* tmpNode = new TaxonomyTreeNode(min, max, ++currentId, currentLevel, children, numChildren, numLeaves);
			previousNode->setNext(tmpNode);
			previousNode = tmpNode;

			//set the parent node for all nodes in children and calculate the internal node of this node
			int numOfInternalNode = 0;
			for(int j=0; j<numChildren; j++){
				numOfInternalNode += (*children)[j]->getNumOfInternalNodes();
				(*children)[j]->setParent(tmpNode);
			}
			// we need to add 1 to numOfInternalNode to make it work
			tmpNode->setNumOfInternalNodes(++numOfInternalNode);
		}

		previousNode->setNext(NULL);

		//to determine if we should continue building higher level nodes
		if(i <= FAN_OUT)
			break;

		dummy = newDummy;
		currentLevel ++;
	}

	return newDummy->getNext();
}

//this is only for debugging purpose
void TaxonomyTree::print(){
	if(root == NULL)
	{
		cout << "Empty Taxonomy Tree!" << endl;
	}
	else
	{
		//to get all first nodes of all levels
		queue<TaxonomyTreeNode*>* nodes = new queue<TaxonomyTreeNode*>;
		TaxonomyTreeNode* currentNode = root;

		do{
			nodes->push(currentNode);
			currentNode = currentNode->getFirstChild();
		}while(currentNode!=NULL);
		
		while(!nodes->empty())
		{
			print(nodes->front());
			nodes->pop();
		}
	}
}

void TaxonomyTree::print(TaxonomyTreeNode* node){
	do{
		cout << node->getId() << " " << node->getNumOfInternalNodes() << " |";

		// use the following to get more detailed information of each node
		//node->print();
		//cout << " | ";

		node = node->getNext();
	}while(node!=NULL);

	cout << endl;
}
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
#include "Transaction.h"
#include "TaxonomyTreeNode.h"

using namespace std;

void Transaction::print(){
	cout << tid << " : ";

    for (int i=0; i<items->size(); i++)
		cout << (*items)[i] << " ";

    cout << endl;
}

bool Transaction::isInHierarchyCut(vector<TaxonomyTreeNode*>* hierarchyCut){
	// this is for the purpose of not misclassfying, for example, a1b1 to ABC
	int matchedItem = 0;
	for(int i=0; i<items->size(); i++)
	{
		if(isInHierarchyCut((*items)[i], hierarchyCut))
			matchedItem++;
		else
			return false;
	}

	if(matchedItem != items->size())
		return false;

	return true;
}

bool Transaction::isInHierarchyCut(int item, vector<TaxonomyTreeNode*>* hierarchyCut){
	TaxonomyTreeNode* currentNode;

	for(int i=0; i<hierarchyCut->size(); i++)
	{
		currentNode = (*hierarchyCut)[i];
		if(item>=currentNode->getMin() &&  item<=currentNode->getMax())
			return true;
	}
	
	return false;
}

Transaction::Transaction(int itid, vector<int>* iitems){
	tid = itid;
    items = iitems;
}

Transaction::~Transaction(){
    delete items;
}

int Transaction::hasItemInRange(int min, int max){
	for(int i=0; i<items->size(); i++){
		int item = (*items)[i];
		if(item<=max && item>=min)
			return 1;
	}

	return 0;
}

int Transaction::hasItemInRange(TaxonomyTreeNode* node){
	return hasItemInRange(node->getMin(), node->getMax());
}
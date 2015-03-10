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

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
//#include <sys/time.h>
#include <cmath>
#include <assert.h>
#include <set>
#include "TaxonomyTreeNode.h"

using namespace std;

class Transaction{
private:
	int tid;  //transaction id
	vector<int>* items; //the ids of the set of items of the transaction
    
public:
	Transaction(int tid, vector<int>* items);
	~Transaction();

	// to print the transaction in the following format
	// tid : item1 item2 item3 ...
	void print(); 

	inline int getTid(){return tid;};
	inline vector<int>* getItems(){return items;};

	// to check if a transaction has an item falling into a taxonomy tree node
	int hasItemInRange(int min, int max);
	int hasItemInRange(TaxonomyTreeNode* node);
	// to check if a transaction falls into a hierarchy cut
	bool isInHierarchyCut(vector<TaxonomyTreeNode*>*);

private:
	// to check if a single item falls into a hierarchy cut
	bool isInHierarchyCut(int, vector<TaxonomyTreeNode*>*);
};

#endif

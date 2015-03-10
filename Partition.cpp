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
#include <stack>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "Global.h"
#include "TaxonomyTree.h"
#include "TaxonomyTreeNode.h"
#include "Partition.h"
#include "Transaction.h"
#include "PPInterface.h"

using namespace std;

TaxonomyTree* Partition::taxonomyTree = NULL;
int Partition::scale = 0;


Partition::Partition(vector<TaxonomyTreeNode*>* ihierarchyCut, vector<Transaction*>* itransactions, double unusedPrivacyBudget){
	hierarchyCut = ihierarchyCut;
	transactions = itransactions;
	privacyBudget = unusedPrivacyBudget;
	partitionable = true;
	isLeave = false;
	noisyCount = 0.0;
	epsilon = 0.0;
	numInNodes = 0;
}


Partition::Partition(vector<TaxonomyTreeNode*>* ihierarchyCut, double unusedPrivacyBudget){
	hierarchyCut = ihierarchyCut;
	privacyBudget = unusedPrivacyBudget;
	partitionable = true;
	isLeave = false;
	transactions = new vector<Transaction*>;
	noisyCount = 0.0;
	epsilon = 0.0;
	numInNodes = 0;
}


Partition::~Partition(){
	delete hierarchyCut;
	delete transactions;
}


bool Partition::readLine(ifstream& in, vector<int>* items){
	char line[ITEM_ID_LENGTH];
	char* token;
	char delimiter[] = " ";
	int item;

	while(!in.eof()){
		in.getline(line, sizeof(line));
		token = strtok(line, delimiter);

		while(token!=NULL)
		{
			item = atoi(token);
			items->push_back(item);
			token = strtok(NULL, delimiter);
		}	
		return true;
	}

	return false;
}


vector<Transaction*>* Partition::createTransactions(){
	int tid = 0;
	string filePath = RAWDATAFILE;

	ifstream in(filePath.c_str());

	if(!in.is_open()){
		cout << "Failed to open raw data file " << endl;
	}

	vector<Transaction*>* transactions = new vector<Transaction*>;
	vector<int>* items = new vector<int>;

	//read a new line
    while(readLine(in, items)) {
		tid++;
		Transaction* transaction = new Transaction(tid, items);
		transactions->push_back(transaction);
		items = new vector<int>;
    }

    in.close();
		
	cout << "Total number of transactions: " << transactions->size() << endl; 
	return transactions;
}

void Partition::createPartitions(){
	TaxonomyTree* taxTree = new TaxonomyTree(TOTAL_LEAF_NODES);
	Partition::taxonomyTree = taxTree;
	//used to specify different stop conditions for different levels
	Partition::scale = taxonomyTree->getNumOfInternalNodes();

	//the total number of transactions in the published data set
	int totalNumber = 0;

	vector<Transaction*>* transactions = Partition::createTransactions();

	//construct the first partition
	vector<TaxonomyTreeNode*>* hierarchyCut = new vector<TaxonomyTreeNode*>;
	hierarchyCut->push_back(taxonomyTree->getRoot());

	// the initial partition has all transactions and hierarchyCut {ALL}
	Partition* partition = new Partition(hierarchyCut, transactions, PRIVACY_BUDGET);

	//calculate the epsilon for the next level
	//PRIVACY_BUDGET/2 is reserved for the leaf level 
	double e = PRIVACY_BUDGET/2/scale;
	partition->setEpsilon(e);
	partition->setTotalNumInternalNodes(scale);
	
	//we assume that the total number of transactions is known
	//actually this assumption is not critical for now
	partition->setNoisyCount(partition->getTransactions()->size());

	//stack to hold all unpartitioned and isPartitionable partitions
	stack<Partition*>* unpartitioned = new stack<Partition*>;

	unpartitioned->push(partition);
	
	//keep partitioning until no partition is left
	while(unpartitioned->size()>0){
		partition = unpartitioned->top();
		unpartitioned->pop();

		//generate subpartitions from partition
		vector<Partition*>* subpartitions = partition->createSubpartitions();

		//partition would never be used again, however, the transactions in the
		//partition can NOT be destroyed
		delete partition;

		if(subpartitions->size()==0){
			delete subpartitions;
			continue;
		}

		for(int i=0; i<subpartitions->size(); i++){
			Partition* subpartition = (*subpartitions)[i];

			//if the subpartition is still partitionable, we add it to the stack
			if(subpartition->isPartitionable())
				unpartitioned->push(subpartition);
			else //either the subpartition is empty or it is the leaf partition
			{
				if(subpartition->getLeave() && subpartition->getNoisyCount()>=1){
					//OUTPUT
					subpartition->print();
					totalNumber+=subpartition->getNoisyCount();
				}

				delete subpartition;
			}
		}

		delete subpartitions;
	}

	cout << "The total noisy number is " << totalNumber << endl;
	cout<< "Bye!" << endl;
}

vector<Partition*>* Partition::createSubpartitions(){
	//for return
	vector<Partition*>* subpartitions = new vector<Partition*>;
	//it stores all non-empty subpartitions
	map<int, Partition*>* spm = new map<int, Partition*>;

	vector<TaxonomyTreeNode*>* highestPartitionableNodes = getHighestPartitionableNodes();
	assert(highestPartitionableNodes != NULL);

	bool isLeaf = false;
	int level = getHighestPartitionableLevel();
	//cout << "LEVEL:" << level;
	int size = highestPartitionableNodes->size();

	if(level==1 && size==1)
		isLeaf = true;

	//randomly select a node to specialize
	TaxonomyTreeNode* selectedNode = PPInterface::noisySelectNode(highestPartitionableNodes);
	//get the index of selectedNode in hierarchyCut
	int pos = findIndexInHierarchyCut(selectedNode);
	//newHierarchyCut contains all other nodes, except the one selected
	vector<TaxonomyTreeNode*>* newHierarchyCut = new vector<TaxonomyTreeNode*>(hierarchyCut->begin(), hierarchyCut->end());
	newHierarchyCut->erase(newHierarchyCut->begin()+pos);

	//get all the children of the selected node
	vector<TaxonomyTreeNode*>* children = selectedNode->getChildren();

	map<int, Partition*>::iterator it;

	//generate all non-empty subpartitions
	for(int i=0; i<transactions->size(); i++){
		Transaction* t = (*transactions)[i];
		vector<TaxonomyTreeNode*>* tmpHierarchy = new vector<TaxonomyTreeNode*>(newHierarchyCut->begin(), newHierarchyCut->end());

		//index is used to determine where to put this transaction
		int index = 0;

		for(int j=0; j<children->size(); j++){
			TaxonomyTreeNode* child = (*children)[j];
			int in = t->hasItemInRange(child); //in equals either 1 or 0
			index=index*2+in;

			if(in)
				tmpHierarchy->push_back(child);
		}

		it = spm->find(index);

		//if this is a new partition
		if (it == spm->end()){
			Partition* p = new Partition(tmpHierarchy, privacyBudget);
			p->getTransactions()->push_back(t);
			spm->insert(pair<int, Partition*>(index, p));
		}
		else
			it->second->getTransactions()->push_back(t);
	} //END OF OUTER FOR

	//an array used to generate empty partitions
	int* helpArr = new int[SUBSET_SIZE];
	//number of empty subpartitions that we need to check
	int maxIndex = pow(2.0, (double)children->size());

	for(int i=1; i<maxIndex; i++)
		helpArr[i] = i;

	double e = 0.0;

	//if spm is not empty
	if(!spm->empty()){
		//calculate the noisy count of non-empty subpartitions
		for(it=spm->begin(); it!=spm->end(); it++){
			Partition* p = it->second;
			helpArr[it->first] = helpArr[--maxIndex];
			//if p is in the leaf level
			if(isLeaf){
				//we can use all unused privacy budget
				double noisyCount = round(PPInterface::noisyCount(p->getTransactions()->size(), p->getPrivacyBudget()));

				if(noisyCount >= FACTOR2*sqrt(2.0)/p->getPrivacyBudget())
					p->setNoisyCount(noisyCount);
				else
					p->setNoisyCount(0.0);

				p->setPartitionable(false);
				p->setIsLeave(true);
	
				subpartitions->push_back(p);
			}
			else{
				//epsilon is from the upper level
				double noisyCount = round(PPInterface::noisyCount(p->getTransactions()->size(), epsilon));
				p->updatePrivacyBudget(epsilon);
				p->setNoisyCount(noisyCount);

				int totalNumInternalNodes = getTotalNumInternalNodes(p->getHierarchyCut());
				e = (privacyBudget-PRIVACY_BUDGET/2)/(totalNumInternalNodes-1);
				assert(e>0);			
				p->setEpsilon(e);
				p->setTotalNumInternalNodes(totalNumInternalNodes);

				//check if we need to further specialize this subpartition (stop condition)
				//here it could be improved in the following way:
				//if it really can not be specialized, that is, it has a size of 0, we could
				//use the entire unused privacy budget to check, not only epsilon
				//cout << "stop condition " << sqrt(2.0)/(epsilon*subpartitions->size()) << endl;
				if(noisyCount < FACTOR1*sqrt(2.0)/epsilon*level)	
					p->setPartitionable(false);


				subpartitions->push_back(p);
			}
		} //END OF FOR
	}
	
	//calculate the noisy count of empty subpartitions
	//only if the noisy count of an empty subpartition exceeds the stop condition, it will be generated	
	
	int numEmpty = maxIndex-1;

	for(int i=0; i< numEmpty; i++){
		//if this is the leaf level
		if(isLeaf){
			double noisyCount = round(PPInterface::noisyCount(0, privacyBudget));

			if(noisyCount > FACTOR2*sqrt(2.0)/privacyBudget){
				int selected = helpArr[rand()%(maxIndex-1)+1];
				helpArr[selected] = helpArr[--maxIndex];

				vector<TaxonomyTreeNode*>* cut =  new vector<TaxonomyTreeNode*>(newHierarchyCut->begin(), newHierarchyCut->end());
				vector<TaxonomyTreeNode*>* tmp = generateHierarchyCut(selected, children);
				cut->insert(cut->begin(), tmp->begin(), tmp->end());

				Partition* ep = new Partition(cut, 0.0);
				
				ep->setIsLeave(true);
				ep->setPartitionable(false);
				ep->setNoisyCount(noisyCount);

				subpartitions->push_back(ep);
			}
		}
		else{
			double noisyCount = round(PPInterface::noisyCount(0, epsilon));
			if(noisyCount > FACTOR1*sqrt(2.0)/epsilon*level){
				//randomly select a hierarchy cut
				int selected = helpArr[rand()%(maxIndex-1)+1];
				helpArr[selected] = helpArr[--maxIndex];

				vector<TaxonomyTreeNode*>* cut =  new vector<TaxonomyTreeNode*>(newHierarchyCut->begin(), newHierarchyCut->end());
				vector<TaxonomyTreeNode*>* tmp = generateHierarchyCut(selected, children);
				cut->insert(cut->begin(), tmp->begin(), tmp->end());

				Partition* ep = new Partition(cut, privacyBudget-epsilon);
				ep->setNoisyCount(noisyCount);

				int totalNumInternalNodes = getTotalNumInternalNodes(ep->getHierarchyCut());
				ep->setEpsilon((ep->getPrivacyBudget()-PRIVACY_BUDGET/2)/(totalNumInternalNodes-1));
				ep->setTotalNumInternalNodes(totalNumInternalNodes);

				subpartitions->push_back(ep);
			}
		}
	}
	

	delete[] helpArr;
	delete spm;
	delete newHierarchyCut;

	return subpartitions;
}


int Partition::generateEmptyPartition(int* helpArr, int max)
{
	int r = rand()%max+1;

	while(helpArr[r]==1) //it has been selected
		r = rand()%max+1;
	
	helpArr[r] = 1; //make it selected

	return r;
}


vector<TaxonomyTreeNode*>* Partition::generateHierarchyCut(int selected, vector<TaxonomyTreeNode*>* children)
{
	int* arr = new int[FAN_OUT];
	for(int i=0; i<FAN_OUT; i++)
		arr[i] = 0;

	vector<TaxonomyTreeNode*>* ret = new vector<TaxonomyTreeNode*>; 
	int size = children->size()-1;
	int cindex = size;

	while(selected>0){
		arr[cindex] = selected%2;
		selected/=2;
		cindex--;
	}

	for(int i=0; i<=size; i++)
		if(arr[i]==1)
			ret->push_back((*children)[i]);

	delete[] arr;
	return ret;
}


int Partition::getTotalNumInternalNodes(vector<TaxonomyTreeNode*>* cut){
	int ret = 0;

	for(int i=0; i<cut->size(); i++)
		ret += (*cut)[i]->getNumOfInternalNodes();

	return ret;
}

int Partition::findIndexInHierarchyCut(TaxonomyTreeNode* node){
	for(int i=0; i<hierarchyCut->size(); i++){
		if((*hierarchyCut)[i] == node)
			return i;
	}

	assert(0);
}

vector<TaxonomyTreeNode*>* Partition::getHighestPartitionableNodes(){
	int highestLevel = getHighestPartitionableLevel();

    if (highestLevel == 0 || highestLevel == -1)
		return NULL;
  
    vector<TaxonomyTreeNode*>* ret = new vector<TaxonomyTreeNode*>;
    for(int i=0; i<hierarchyCut->size(); i++){
		if( (*hierarchyCut)[i]->getLevel() == highestLevel)
			ret->push_back( (*hierarchyCut)[i] );
    }
    assert (ret->size() > 0 );
 
    return ret;
}

int Partition::getHighestPartitionableLevel(){
    int highestLevel = -1;
    for(int i=0; i<hierarchyCut->size(); i++){
		if( (*hierarchyCut)[i]->getLevel() > highestLevel)
			highestLevel = (*hierarchyCut)[i]->getLevel();
    }
    assert(highestLevel != -1);
    return highestLevel;
}


double Partition::round(double n){
	if(n<0)
		return 0;

	double t =n-floor(n);
	
	if(t>=0.5)
		return ceil(n);
    else 
        return floor(n);
}

void Partition::print(){
	for(int i=0; i<hierarchyCut->size(); i++)
		(*hierarchyCut)[i]->print();

	cout << ": " << noisyCount << endl;
}


int main()
{
	time_t start,end;
    double AATime;
    time (&start);
	srand((unsigned)time(NULL));
	
	cout << "Starting to anonymize..." << endl;

	Partition::createPartitions();

	time (&end);
    AATime = difftime (end,start);
    cout << endl << "partition elapse time = " << AATime << endl;
}



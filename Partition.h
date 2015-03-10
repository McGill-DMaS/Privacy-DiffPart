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

#ifndef PARTITION_H
#define PARTITION_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "Global.h"
#include "TaxonomyTreeNode.h"
#include "TaxonomyTree.h"
#include "Transaction.h"
#include "PPInterface.h"

using namespace std;

class Partition{
private:
    vector<TaxonomyTreeNode*>* hierarchyCut; // the associated hierarchyCut of the partition
    vector<Transaction*>* transactions; // the transactions belong to this partition

	double privacyBudget;
	bool partitionable; //indicate if the partition is partitionable
	double noisyCount;
	bool isLeave;
	double epsilon; //it is the privacy budget that should be used in its subpartitions
	int numInNodes;

	static int scale;
	static TaxonomyTree* taxonomyTree; 

public:
    Partition(vector<TaxonomyTreeNode*>*, vector<Transaction*>*, double);
	Partition(vector<TaxonomyTreeNode*>*, double);
    ~Partition();

	inline bool getLeave(){return isLeave;};
	inline double getNoisyCount(){ return noisyCount;};
	inline bool isPartitionable(){ return partitionable;};
	inline double getPrivacyBudget(){return privacyBudget;};
	inline vector<TaxonomyTreeNode*>* getHierarchyCut(){return hierarchyCut;};
	inline vector<Transaction*>* getTransactions(){return transactions;};

	inline void setTotalNumInternalNodes(int n){numInNodes = n;};
	inline void setEpsilon(double e){epsilon = e;};
	inline void setIsLeave(bool iisLeave){isLeave = iisLeave;};
	inline void setNoisyCount(double count){noisyCount = count;};
	inline void setPartitionable(bool par) {partitionable = par;};
	inline void setHierarchyCut(vector<TaxonomyTreeNode*>* ihierarchyCut){hierarchyCut = ihierarchyCut;};
	inline void setTransactions(vector<Transaction*>* itransactions){transactions = itransactions;};
	inline void setPrivacyBudget(double budget){privacyBudget = budget;};

    static vector<Transaction*>* createTransactions(); //create transactions from the raw dataset	
	static void createPartitions(); //create all partitions and construct the decomposition tree
	static bool readLine(ifstream&, vector<int>*);
	static double round(double);
	static void createPartitionsDwork();
	
	int getTotalNumInternalNodes(vector<TaxonomyTreeNode*>*);

	void updatePrivacyBudget(double e){privacyBudget-=e;};
	vector<Partition*>* createSubpartitions(); //generate the subpartitions of this partition
	int generateEmptyPartition(int*, int);
	vector<TaxonomyTreeNode*>* generateHierarchyCut(int, vector<TaxonomyTreeNode*>*);

	void print();

private:
	int findIndexInHierarchyCut(TaxonomyTreeNode*);
	vector<TaxonomyTreeNode*>* getHighestPartitionableNodes();
	int getHighestPartitionableLevel();
};

#endif

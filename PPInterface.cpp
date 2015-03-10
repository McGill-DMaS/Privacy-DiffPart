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
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <assert.h>
#include "PPInterface.h"
#include "TaxonomyTreeNode.h"

using namespace std;
using namespace System;

PPInterface::PPInterface(){
}

PPInterface::~PPInterface(){}

double PPInterface::noisyCount(double value, double epsilon)
{
	//assert(epsilon > 0);
	int d = rand();

	if(d == 0)
		d++;
	else if(d == RAND_MAX)
		d--;

	double uniform = (double)d/RAND_MAX- 0.5;
	return value + 1.0 / epsilon * Math::Sign(uniform) * Math::Log(1 - 2.0 * Math::Abs(uniform));
}


TaxonomyTreeNode* PPInterface::noisySelectNode(vector<TaxonomyTreeNode*>* candidateNodes)
{
	int position = rand()%candidateNodes->size();

	return (*candidateNodes)[position];
}
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

#ifndef GLOBAL_H
#define GLOBAL_H

// the fanout of the taxonomy tree
#define FAN_OUT 10
#define SUBSET_SIZE 1024 //it should be 2^FAN_OUT
#define TOTAL_LEAF_NODES 17
#define ITEM_ID_LENGTH 1024
//privacy budget
#define PRIVACY_BUDGET 1.0
#define FACTOR1 6.75
#define FACTOR2 2.75

// the location of raw data file
#define RAWDATAFILE "C:\\dataset\\good\\msnbc_5.dat"
#define ITEM_VAL_DELIMETER           TCHAR(' ')

#endif
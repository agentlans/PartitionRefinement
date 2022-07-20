/*
   Copyright 2022 Alan Tseng

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _PARTITION_REFINEMENT
#define _PARTITION_REFINEMENT

typedef int pare_item;

typedef struct pare_partition_refinement {
  int *member;
  int *place;
  int *begin;
  int *end;
  pare_item *data;
  int n;          // number of elements
  int next_split; // ID of the next split set we create
  int *split;     // temporary workspace
} pare_t;

/*
  Every time a set S is split by elements of set X,
  it gives two subsets: (S intersect X) and (S - X)
  as represented by this struct.

  The (S intersect X) is [begin,mid)
  The (S - X) is [mid,end)
*/
typedef struct pare_split_set {
  pare_item *begin;
  pare_item *mid;
  pare_item *end;
} pare_split_set;

// Keeps track of sets splitted during partition refinement
typedef struct pare_results {
  int n;
  pare_split_set *sets;
} pare_results;

void pare_init(pare_t *pr, pare_item *x, int n);
void pare_clear(pare_t *pr);
void pare_results_clear(pare_results *res);
void pare_results_init(pare_results *res, int n);

void pare_subset(pare_item **x, int *n, const pare_t *pr, int i);
int pare_num_subsets(pare_t *pr);

void pare_refine(pare_t *pr, pare_results *result, const pare_item *xs, int n);

#endif
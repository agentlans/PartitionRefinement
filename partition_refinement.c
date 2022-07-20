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

#include <stdlib.h>
#include <string.h>
#include "partition_refinement.h"

/* Initializes a partition refinement data structure
   that can hold the items in x. */
void pare_init(pare_t* pr, pare_item* x, int n) {
    #define CALLOC calloc(n, sizeof(int))
    int* member = CALLOC; // Initialized to 0 already
    int* place = CALLOC;
    int* begin = CALLOC; // all 0
    int* end = CALLOC; // all 0
    int* split = CALLOC;
    #undef CALLOC
    pare_item* data = calloc(n, sizeof(pare_item));
    if (member && place && begin && end && data) {
        *pr = (pare_t) {
            member, place, begin, end, data, n, 1, split};
        // Copy data into the struct
        memmove(pr->data, x, sizeof(pare_item)*n);
        for (int i = 0; i < n; ++i) {
            pr->place[x[i]] = i;
        }
        pr->end[0] = n;
    } else {
        pare_clear(pr);
    }
}

void pare_clear(pare_t* pr) {
    if (pr) {
        free(pr->member);
        free(pr->place);
        free(pr->begin);
        free(pr->end);
        free(pr->split);
        free(pr->data);
    }
}

// For the results
void pare_results_init(pare_results* res, int n) {
    res->sets = calloc(n, sizeof(pare_split_set));
}

void pare_results_clear(pare_results* res) {
    res->n = 0;
    free(res->sets);
    res->sets = NULL;
}

// For querying the subsets
int pare_num_subsets(pare_t* pr) {
    return pr->next_split;
}

/* 
Returns information about subset i.
Output in x and n.
After calling this function, 
the size of subset i is stored in n
and the pointer to its first element is stored in x.
*/
void pare_subset(pare_item** x, int* n, const pare_t* pr, int i) {
    *x = pr->data + pr->begin[i];
    if (n) {
        *n = pr->end[i] - pr->begin[i];
    }
}

/*
Splits each set in the partition refinement by the elements of xs.
The results of the splitting are stored in the result variable.
result can be NULL.
xs is an array of size n.
*/
void pare_refine(pare_t* pr, pare_results* result, const pare_item* xs, int n) {
    // Clear the workspace
    for (int i = 0; i < pr->n; ++i) {
        pr->split[i] = -1;
    }
    int num_new_splits = 0;
    // For each input element
    for (int i = 0; i < n; ++i) {
        pare_item x = xs[i];
        int s = pr->member[x]; // Find item's set
        int ss; // ID of (set s)'s split set
        if (pr->split[s] != -1) {
            // Split set already exists
            ss = pr->split[s];
        } else {
            // Create a new split
            ss = (pr->next_split)++;
            pr->split[s] = ss;
            pr->begin[ss] = pr->begin[s];
            pr->end[ss] = pr->begin[s];
            num_new_splits++;
        }
        // Add item to split set
        pr->member[x] = ss;
        // Put x at the end of the split set
        // by swapping the pointers to the data
        int a = pr->place[x];
        int b = pr->end[ss];
        pr->place[pr->data[b]] = a;
        pr->place[pr->data[a]] = b;
        // and swapping the data
        int temp = pr->data[a];
        pr->data[a] = pr->data[b];
        pr->data[b] = temp;
        // Increase split set's size
        pr->end[ss]++;
    }
    // Update the bounds of sets that have been split off
    for (int s = 0; s < pr->n; ++s) {
        if (pr->split[s] == -1) continue;
        pr->begin[s] = pr->end[pr->split[s]];
    }
    // Report results if requested
    if (result) {
        result->n = num_new_splits;
        int i = 0;
        for (int s = 0; s < pr->n; ++s) {
            if (pr->split[s] == -1) continue;
            // Get pointers to both parts of the split set
            int ss = pr->split[s];
            int ss_size, s_size;
            pare_subset(&result->sets[i].begin, &ss_size, pr, ss);
            pare_subset(&result->sets[i].mid, &s_size, pr, s);
            result->sets[i].end = result->sets[i].mid + s_size;
            i++;
        }
    }
}

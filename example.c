#include <stdio.h>
#include "partition_refinement.h"

void print_ints(const int* x, int n) {
    printf("{");
    for (int i = 0; i < n; ++i) {
        printf("%d", x[i]);
        if (i != n-1) printf(",");
    }
    printf("}");
}

void print_results(pare_results* res) {
    int n = res->n;
    for (int i = 0; i < n; ++i) {
        pare_split_set* s = res->sets + i;
        printf("S[%d] intersect X: ", i);
        print_ints(s->begin, s->mid - s->begin);
        printf("\n");
        printf("S[%d] \\ X: ", i);
        print_ints(s->mid, s->end - s->mid);
        printf("\n");
    }
    printf("\n");
}

int main() {
    pare_t pr;
    int arr[] = {0,1,2,3,4,5,6,7,8,9};
    pare_init(&pr, arr, 10);
    printf("Initial set: ");
    print_ints(pr.data, 10);
    printf("\n\n");

    pare_results res;
    pare_results_init(&res, 10);

    printf("After refining with X = {2,3,5,7}, the split sets are\n");
    int xs[] = {2,3,5,7};
    pare_refine(&pr, &res, xs, 4);
    print_results(&res);

    printf("After refining with X = {2,4,6,8}, the split sets are\n");
    int evens[] = {2,4,6,8};
    pare_refine(&pr, &res, evens, 4);
    print_results(&res);

    printf("After refining with X = {2}, the split sets are\n");
    int two[] = {2};
    pare_refine(&pr, &res, two, 1);
    print_results(&res);

    printf("Here are all the subsets after refining:\n");
    for (int i = 0; i < pare_num_subsets(&pr); ++i) {
        pare_item* x;
        int n;
        pare_subset(&x, &n, &pr, i);
        print_ints(x, n);
        printf(" ");
    }
    printf("\n");

    pare_results_clear(&res);
    pare_clear(&pr);
}
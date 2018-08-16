#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[100000];

int calc_time (int *, int);
void quicksort (int *,int);
int reorderarray (int *, int); 


int main () {
    char c;
    char tempstr[100];
    int a, b, d, e;
    int *crossing_times;
    int cases;
    int times;
    int n;

    a=0;
    b=0;
    cases=0;
    times=0;

    crossing_times = (int *) calloc (1000, sizeof(int));

    n = scanf("%d",&cases);

    for (a=0; a<cases ; a++) {

        n = scanf("%d",&times);

        for (b=0 ; b<times ; b++) {
            n = scanf("%d",&d);
            (*(crossing_times+b)) = d;
        }

        quicksort(crossing_times, times);

        calc_time(crossing_times, times);

        if (a<(cases-1))
            printf("\n");
    }

    exit(0);
}


#define A xct[0]
#define B xct[1]
#define C xct[i-2]
#define D xct[i-1]

int calc_time (int *xct, int num_times) {
    int i;
    int total_time;
    int buffer_idx;

    i = num_times;
    total_time = 0;
    buffer_idx = 0;

    while (i>0) {

        if (i>=4) {
            if (B+B < A+C) {
                buffer_idx += sprintf(buffer+buffer_idx,"%d %d\n%d\n%d %d\n%d\n", A, B, A, C, D, B);
                total_time += B + A + D + B;
            } else {
                buffer_idx += sprintf(buffer+buffer_idx,"%d %d\n%d\n%d %d\n%d\n", A, D, A, A, C, A);
                total_time += D + A + C + A;
            }
            i-=2;
        }

        if (i==3) {
            buffer_idx += sprintf(buffer+buffer_idx,"%d %d\n%d\n", A, D, A);
            total_time += D + A;
            i=2;
        }

        if (i==2) {
            buffer_idx += sprintf(buffer+buffer_idx,"%d %d\n", A, B);
            total_time += B;
            i=0;
        }

        if (i==1) {
            buffer_idx += sprintf(buffer+buffer_idx,"%d\n", A);
            total_time += A;
            i=0;
        }

    }

    printf("%d\n%s", total_time, buffer);

    return 0;
}


void quicksort (int *array,int long_array) {
	int punto_medio;

	if (long_array > 1) {
		punto_medio = reorderarray(array,long_array);
		quicksort (array,punto_medio);
		quicksort (array+punto_medio+1,long_array-punto_medio-1);
	}

	return;
}


int reorderarray (int *array , int long_array) {
	int l,r,pivot,temp;

	if (long_array < 2)
		return 0;
	
	l = 0;
	r = long_array-1;
	pivot = long_array-1;

	while (l<r) {
		while (l<r && array[l]<array[pivot])
			l++;
		while (l<r && array[r]>=array[pivot])
			r--;
		if (l<r) {
			temp=array[r];
			array[r]=array[l];
			array[l]=temp;
		}
	}
	temp=array[r];
	array[r]=array[pivot];
	array[pivot]=temp;

	return r;
}

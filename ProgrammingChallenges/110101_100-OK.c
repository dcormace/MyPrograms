#include <stdio.h>
#include <stdlib.h>

long int calcular_maximo (long int, long int);
long int calcular (long int);

int main() {
	long int i , j;

	while (scanf("%ld %ld",&i,&j)!=EOF)
		printf("%ld %ld %ld\n",i,j,calcular_maximo(i,j));

	exit(0);
}


long int calcular_maximo (long int i, long int j) {
	long int max;
	long int c;
	long int n;

	if (i>j) {
		c=i;
		i=j;
		j=c;
	}

	c=0;
	max=0;
	for (n=i;n<=j;n++) 
		if ((c=calcular(n))>max) max=c;
	
	return max;
}


long int calcular(long int n) {
	long int a;

	a=1;
	while (n != 1) {
		if (n & 1) 
			n = n *3 + 1;
		else	
			n = n / 2;
		a++;
	}

	return a;
} 

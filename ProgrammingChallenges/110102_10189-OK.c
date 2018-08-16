#include <stdio.h>
#include <stdlib.h>

int soli[102*102];

int main() {
	long int fil , col;
	int a;
	int b;
	char tempstr[102][102];
	long int field=0;

	while (scanf("%ld %ld",&fil,&col)!=EOF && fil>0 && col>0) {

		if (field>0) printf("\n");
		field++;

		for (a=0;a<fil;a++)
			scanf("%s",tempstr[a]);

		solve_mine(tempstr,fil,col,field);
	}

	exit(0);
}

int solve_mine (char mine_field[102][102],int fil,int col,long int field) {
	int f,c;

	for (f=0;f<(fil+2)*(col+2);f++)
		soli[f]=0;

	for (f=0;f<fil;f++)
		for (c=0;c<col;c++)
			if (mine_field[f][c]=='*') {
				int a,b;

				for (a=f-1;a<=f+1;a++)
					for (b=c-1;b<=c+1;b++)
						soli[(a+1)*(col+2)+(b+1)]++;
			}
			
	printf("Field #%ld:\n",field);
	for (f=0;f<fil;f++) {
		for (c=0;c<col;c++)
			if (mine_field[f][c]=='*')
				printf("%c",mine_field[f][c]);
			else 	printf("%1d",soli[(f+1)*(col+2)+c+1]);
		printf("\n");
	}

	return 0;
}


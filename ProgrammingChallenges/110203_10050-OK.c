#include <stdio.h>
#include <stdlib.h>

int main () {
	int a;
	int b;
	char tempstr[100];
	char c;
	int num_test_cases;
	int num_of_days;
	int num_of_parties;
	int hartals[100];

	a=0;
	b=0;
	while ((c=fgetc(stdin))!=EOF) {
		if (c=='\n') {
			tempstr[a]='\0';
			a=0;
			switch (b) {
				case 0: num_test_cases = atoi(tempstr);
					b++;
					break;
				case 1: num_of_days = atoi(tempstr);
					b++;
					break;
				case 2: num_of_parties = atoi(tempstr);
					b++;
					break;
				default: if ((b-3)<num_of_parties)
						hartals[b-3] = atoi(tempstr);
					 b++;
					 if ((b-3)==num_of_parties) {
					 	b=1;
						compute_test_case(num_of_days,num_of_parties,hartals);
					}

			}
		} else tempstr[a++]=c;
	}

	return 0;
}


int compute_test_case (int days , int parties , int hartals[100]) {
	int a;
	int b;
	int days_lost;
	int dow;

	days_lost=0;
	for (b = 1 ; b <= days ; b++) {
		dow = b%7;
		if (dow>0 && dow<6) {
			for (a=0 ; a<parties && b%hartals[a]!=0 ; a++);
			if (a!=parties)
				days_lost++;
		} else b++;
	}

	printf("%d\n",days_lost);

	return days_lost;
}


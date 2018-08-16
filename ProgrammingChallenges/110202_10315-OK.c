#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int analize_hand (char [5][2] , long int *);
int rank_hand (int [13] , int [4] , long int *);
int read_hands (char [2][5][2]);
int next_token (char ** , char * , int );
char convert_set (char );
char convert_suit (char );

int main() {
	long int white , black;
	int ret_val;

	char hands[2][5][2];

	while (read_hands(hands)==0) {
        white = black = 0;
		analize_hand(hands[0],&black);
		analize_hand(hands[1],&white);

		if (white > black)
			printf ("White wins.\n");
		else if (white < black)
			printf ("Black wins.\n");
		else
			printf ("Tie.\n");
	}

	return 0;
}


int read_hands (char hands[2][5][2]) {
	int a;
	char tempstr[50];

    if (fgets(tempstr,40,stdin)==NULL)
        return -1;
    
    for (a=0; a<5 ; a++) {
        hands[0][a][0] = convert_set(tempstr[a*3+0]);
        hands[0][a][1] = convert_suit(tempstr[a*3+1]);
        hands[1][a][0] = convert_set(tempstr[(5+a)*3+0]);
        hands[1][a][1] = convert_suit(tempstr[(5+a)*3+1]);
    }

	return 0;
}


char convert_set (char pset) {

    if (pset >= '2' && pset <='9')
        return pset - '2';

    switch (pset) {
        case 'T': return 8;
        case 'J': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        case 'A': return 12;
    }

    return -1;
}


char convert_suit (char psuit) {
    switch (psuit) {
        case 'C': return 0;
        case 'D': return 1;
        case 'H': return 2;
        case 'S': return 3;
    }

    return -1;
}


int analize_hand (char hand[5][2] , long int *rank) {
	int counter_value[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	int counter_suit[4] = {0,0,0,0};
	int a;

	for (a=0 ; a<5 ; a++) {
        counter_value[(int)hand[a][0]]++;
        counter_suit[(int)hand[a][1]]++;
	}

	return rank_hand (counter_value,counter_suit,rank);
}


int rank_hand (int value[13] , int suit[4] , long int *rank) {
	int a,b,d;
	unsigned short int is_straight , is_flush;
	unsigned short int test_array[5] = {0,0,0,0,0};

	/**************************************
	 * inicializo las variables
	 * ***************************************/
	is_straight = 0;
	is_flush = 0;

	for (a=0 ; a<4 ; a++)
		if (suit[a]==5) 
			is_flush = 1;

	for (a=0 ; a<13 ; a++)
		test_array[value[a]]++;

	if (test_array[1]==5) {
		for (a=0 ; value[a]==0 ; a++);
		for (b=0 ; (a+b)<13 && value[a+b]==1 ; b++);
		if (b==5) 
			is_straight = 1;
	}

	if (is_straight && is_flush)
		*rank = 9;
	else if (test_array[4]==1)
		*rank = 8;
	else if (test_array[3]==1 && test_array[2]==1)
		*rank = 7;
	else if (is_flush)
		*rank = 6;
	else if (is_straight)
		*rank = 5;
	else if (test_array[3]==1)
		*rank = 4;
	else if (test_array[2]==2)
		*rank = 3;
	else if (test_array[2]==1)
		*rank = 2;
	else
		*rank = 1;

	for (d=4 ; d>=1 ; d--)
        if (test_array[d]!=0)
    		for (a=12 ; a>=0 ; a--)
	    		if (value[a]==d)
		    		for (b=0 ; b<d ; b++)
                        *rank = *rank*100+a;

	return 0;
}


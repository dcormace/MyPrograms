#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENTENCE_LENGTH 100
#define MAX_WORD_LENGTH 17

struct word_list {
	char original[20];
    char code[20];
	short int alternativas[1001];
};

char *codify_word (char *, char *);

int main() {
	char dictionary[1000][2][20];
	char tempstr[MAX_SENTENCE_LENGTH];
	int num_of_words;
	int a , b;
    char c;

	a = 0;
	fgets(tempstr,10,stdin);
	num_of_words = atoi(tempstr);

	if (num_of_words > 1000)
        return -1;

	for (a=0 ; a<num_of_words ; a++) {
		for (b=0 ; (c=fgetc(stdin))!='\n' && b<19 ; b++)
			dictionary[a][0][b]=c;
        dictionary[a][0][b]='\0';
        codify_word(dictionary[a][0],dictionary[a][1]);
	}

	while (c!=EOF) {
        for (b=0 ; (c=fgetc(stdin))!='\n' && c!=EOF && b<MAX_SENTENCE_LENGTH ; b++)
            tempstr[b]=c;
        tempstr[b]='\0';
		if (c!=EOF) {
			decrypt(tempstr,dictionary,num_of_words);
        }
	}

	return 0;
}


int decrypt (char *psentence , char pdictionary [1000][2][20] , int num_of_words) {
	char code[20];
	int a,b,d;
	struct word_list *word_list;
    int word_list_counter;
    unsigned int alphabet[26];
    unsigned char flag;
    unsigned char flag_solvable;


    /********************************************************************
     * Divide the sentence in words and create a word list.
     ********************************************************************/
    word_list_counter = divide_sentence_into_tokens (psentence, &word_list);

    flag_solvable = 1;
	for (a=0 ; a < word_list_counter && flag_solvable!=0; a++) {
        flag_solvable = 0;
		for (b=0 ; b < num_of_words ; b++) {
			if (strcmp((word_list+a)->code,pdictionary[b][1])==0) {
                for (d=0 ; (word_list+a)->alternativas[d] != -2 ; d++);
                (word_list+a)->alternativas[d] = b;
                (word_list+a)->alternativas[d+1] = -2;
                flag_solvable++;
			}
		}
	}

    if (flag_solvable!=0) {
        /* set up alphabet */
        for (a=0 ; a<26 ; a++)
            alphabet[a] = 0xFFFFFFFF;

        flag_solvable = solve (alphabet, word_list, word_list_counter, pdictionary);
    }


    for (a=0 ; psentence[a]!='\0' ; a++) {
        if (psentence[a]!=' ') {
            if (flag_solvable!=0)
                printf("%c",'a'+dacgetpos(alphabet[psentence[a]-'a']));
            else printf("*");
        } else printf(" ");
    }
    printf("\n");

    free (word_list);

	return 0;
}

#define NOT_SOLVABLE 0
#define SOLVED 1
#define MULTIPLE_ALTERNATIVES 2

int solve (unsigned int palphabet[26] , struct word_list *pword_list , int pword_list_counter, char pdictionary[1000][2][20]) {
    int a,b,i,d;
    unsigned char flag;
    unsigned char delete_flag;
    int num_of_alternatives;
    int ret_value;
    unsigned int copia_alphabet[26];
    int copia_alternativas[1000];

    ret_value = SOLVED;

    do {
        flag = 0;

        update_alphabet(palphabet , pword_list , pword_list_counter, pdictionary);

        num_of_alternatives = 0;
        for (i=0 ; i<pword_list_counter ; i++) {
            for (a=0 ; (pword_list+i)->alternativas[a] != -2 ; a++) {
                delete_flag=0;
                for (b=0 ; (pword_list+i)->original[b]!='\0' && delete_flag==0; b++) {
                    if ((palphabet[(pword_list+i)->original[b]-'a'] & (0x1<<(pdictionary[(pword_list+i)->alternativas[a]][0][b]-'a'))) == 0) {
                        for (d=a ; (pword_list+i)->alternativas[d] != -2 ; d++)
                            (pword_list+i)->alternativas[d] = (pword_list+i)->alternativas[d+1];
                        flag = 1;
                        delete_flag = 1;
                        a--;
                    }
                }
            }
            num_of_alternatives += a;

            if (a==0)
                return NOT_SOLVABLE;
        }

    } while (flag == 1);

    if (num_of_alternatives > pword_list_counter) {
        ret_value = MULTIPLE_ALTERNATIVES;
        for (i=0 ; i<pword_list_counter && ret_value!=SOLVED; i++)
            if ((pword_list+i)->alternativas[1] != -2) {

                for (a=0 ; (pword_list+i)->alternativas[a] != -2 ; a++)
                    copia_alternativas[a] = (pword_list+i)->alternativas[a];
                copia_alternativas[a] = -2;

                (pword_list+i)->alternativas[1] = -2;
                for (a=0 ; copia_alternativas[a]!=-2 && ret_value!=SOLVED; a++) {
                    (pword_list+i)->alternativas[0] = copia_alternativas[a];

                    for (a=0 ; a<26 ; a++)
                        copia_alphabet[a] = 0xFFFFFFFF;

                    ret_value = solve (copia_alphabet , pword_list , pword_list_counter, pdictionary);
                }
            }

        if (ret_value == SOLVED)
            for (b=0;b<26;b++)
                palphabet[b] = copia_alphabet[b];
    } 

    return ret_value;
}


int divide_sentence_into_tokens (char *psentence, struct word_list **pword_list) {
    int a;
    int iguales;
	char *word;
    int counter;
    char tmp_word_array[50][MAX_WORD_LENGTH];
    char sentence_copy[MAX_SENTENCE_LENGTH];

    strcpy(sentence_copy , psentence);

    counter = 0;
	word = strtok(sentence_copy," ");
	while (word!=NULL) {
        iguales = 1;
		for (a=0 ; a<counter && iguales!=0 ; a++)
		    iguales = strcmp(tmp_word_array[a],word);
		if (iguales!=0) {
            strcpy(tmp_word_array[a],word);
            counter++;
		}
		word = strtok(NULL," ");
	}

    *pword_list = (struct word_list *) calloc (counter,sizeof(struct word_list));
    for (a=0 ; a < counter ; a++) {
		strcpy((*pword_list+a)->original, tmp_word_array[a]);
	    codify_word((*pword_list+a)->original,(*pword_list+a)->code);
        (*pword_list+a)->alternativas[0] = -2;
    }

    return counter;
}


char *codify_word (char *word , char *code) {
	int a;
	int b;
	char temp[20];

    memset(temp,'\0',20);

	for (a=0 ; word[a]!='\0'; a++) {
		for (b=0 ; temp[b]!='\0' && word[a]!=temp[b] ; b++);
        temp[b] = word[a];
		code[a] = 'a' + b;
	}
	code[a]='\0';

	return code;
}


int update_alphabet (unsigned int palphabet[26], struct word_list *pword_list, int pword_list_counter, char pdictionary[1000][2][20]) {
    int a , b , i;
    unsigned int letter_pattern;

    for (i=0 ; i<pword_list_counter ; i++) {
        for (a=0; (pword_list+i)->original[a]!='\0'; a++) {

            letter_pattern = 0;

            for (b=0 ; (pword_list+i)->alternativas[b] != -2 ; b++) 
                letter_pattern |= (0x1 << (pdictionary[(pword_list+i)->alternativas[b]][0][a]-'a'));

            palphabet[(pword_list+i)->original[a]-'a'] &= letter_pattern;
        }
    }

    optimize_alphabet(palphabet);

    return 0;
}


int optimize_alphabet (unsigned int palphabet[26]) {
    int a , b;
    int flag=1;
    unsigned int patterns[26];
    unsigned int counter[26];
    unsigned int bits[26];
    int num_of_patterns;
    int iguales;


    while (flag==1) {
        flag=0;
        num_of_patterns = 0;

        for (a=0 ; a<26 ; a++) {
            iguales = 0;
            for (b=0 ; b<num_of_patterns && iguales==0; b++)
                if (palphabet[a]==patterns[b])
                    iguales = counter[b]++;
            if (b<=num_of_patterns && b<26 && iguales==0) {
                patterns[b]=palphabet[a];
                counter[b]=1;
                bits[b] = bit_count(patterns[b]);
                num_of_patterns++;
            }
        }

        for (a=0 ; a<num_of_patterns ; a++)
            if (counter[a] == bits[a])
                for (b=0 ; b<26 ; b++)
                    if (palphabet[b] != patterns[a] && (palphabet[b]&patterns[a])!=0) {
                        palphabet[b] &= ~patterns[a];
                        flag = 1;
                    }
    }

    return 0;
}


int bit_count (unsigned int pattern) {
    int a , b;

    b=0;
    for (a=(1<<25) ; a>0 ; a>>=1)
        if (a&pattern) b++;

    return b;
}


int dacgetpos(unsigned int xpar) {
    int a;

    if (xpar!=0) 
        for (a=0 ; !(xpar & 0x1) ; a++)
            xpar >>= 1;

    return a;
}


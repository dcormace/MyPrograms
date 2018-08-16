#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
char *gkey = "the quick brown fox jumps over the lazy dog";

int main() {
    char c=0;
    int num_cases;
    int a , b , f;
    char case_text[200][MAX_LINE_LENGTH];
    int line_number;
    char alphabet[26];
    int key_line;
    char key_code[44];

    scanf("%d",&num_cases);
    c=getchar();
    c=getchar();

    codify_line(gkey,key_code);

    while (c!=EOF && num_cases) {
        line_number = 0;
        key_line = -1;
        do {
            for (b=0 ; (c=getchar())!='\n' && c!=EOF ; b++)
                case_text[line_number][b]=c;
            case_text[line_number][b]='\0';

            if (b==43 && key_line<0)
                if (test_line(case_text[line_number],key_code)==0) {
                    key_line = line_number;
                    setup_alphabet(alphabet,case_text[key_line]);
                }
            line_number++;
        } while (b>0 && c!=EOF);

        if (key_line>=0) {
            line_number--;
            for (a=0 ; a<line_number ; a++) {
                for (f=0 ; case_text[a][f]!='\0' ; f++)
                    if (case_text[a][f]!=' ')
                        putchar(alphabet[case_text[a][f]-'a']);
                    else putchar(' ');
                putchar('\n');
            }
        } else
            printf("No solution.\n");

        if (b==0 && c!=EOF && num_cases>1)
            putchar('\n');

        num_cases--;
    }

    while (num_cases--)
        printf("\nNo solution.\n");

    exit(0);
}


int test_line (char *pline, char *pkey_code) {
    char code[44];
    
    codify_line(pline,code);
    return strcmp(pkey_code,code);
}


int codify_line (char *pline , char *code) {
    int a;
    int b;
    char letters[26];
    int index;
    char alpha = 'a';

    memset(letters,'\0',26);

    code[0]='\0';
    for (a=0 ; pline[a]!='\0' ; a++) {
        if (pline[a]!=' ') {
            index = pline[a]-'a';
            if (letters[index]==0)
                letters[index]=alpha++;
            code[a]=letters[index];
        } else
            code[a] = ' ';
    }
    code[a]='\0';

    return 0;
}


int setup_alphabet(char *alphabet , char *line) {
    int a;

    for (a=0 ; line[a]!='\0' ; a++) 
        if (line[a]!=' ')
            alphabet[line[a]-'a'] = gkey[a];

    return 0;
}
            




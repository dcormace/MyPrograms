#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_SIZE 1000

struct word {
    int flag;
    short int len;
    char word[20];
    struct word *next;
};

int main() {
    char c;
    int b=0;
    int a;
    struct word **dict2;
    struct word **lastWord;
    struct word *temp;
    char tempStr[20];
    int numOfWords;
    int pairOfWords = 0;


    dict2 = (struct word **) calloc (16,sizeof(struct word *));
    lastWord = (struct word **) calloc (16,sizeof(struct word *));

    for (a=0 ; a<16 ; a++)
        dict2[a] = NULL;

    numOfWords = 0;
    do {
        for (b=0 ; (c=getchar())!='\n' && c!=EOF ; b++) 
            tempStr[b] = c;
        tempStr[b] = '\0';

        if (b!=0) {
            if (dict2[b-1] == NULL) {
                dict2[b-1] = (struct word *) calloc (1,sizeof(struct word));
                temp = dict2[b-1];
            } else {
                temp = lastWord[b-1];
                temp->next = (struct word *) calloc (1,sizeof(struct word));
                temp = temp->next;
            }
            strcpy(temp->word , tempStr);
            temp->flag=0;
            temp->next = NULL;
            lastWord[b-1] = temp;
            numOfWords++;
        }
    } while (b!=0);

    do {
        char words[2][20];
        char c;
        struct word *tempdict;
        int counter;
        struct word *word1;
        struct word *word2;

        a=0;
        for (b=0 ; (c=getchar())!='\n' && c!=EOF ; b++)
            if (c==' ') {
                words[a][b]='\0';
                a++;
                b=-1;
            } else words[a][b]=c;

        if (c!=EOF) {
            words[a][b]='\0';

            if (pairOfWords++!=0)
                printf("\n");
            
            if (strlen(words[0])!=strlen(words[1])) {
                printf("No solution.\n");
                continue;
            }

            counter=0;
            for (tempdict = dict2[strlen(words[0])-1] ; tempdict!=NULL && counter!=2; tempdict=tempdict->next) {
                if (strcmp(words[0],tempdict->word)==0) {
                    counter++;
                    word1 = tempdict;
                }
                if (strcmp(words[1],tempdict->word)==0) {
                    counter++;
                    word2 = tempdict;
                }
            }
            build_graph (word1 , word2 , dict2[strlen(words[0])-1] , numOfWords);
        }
    } while (!feof(stdin));

    exit(0);
}


int are_doublets (char *str1, char *str2) {
    int diff = 0;
    int a;

    for (a=0 ; str1[a]!='\0' && diff<=1 ; a++)
        if (str1[a]!=str2[a])
            diff++;

    if (str1[a]=='\0' && str2[a]!='\0')
        diff = 0;

    return (diff==1 ? 1 : 0);
}


struct node {
    struct word *link;
    struct node *precedent;
    struct node *doublets;
    struct node *next;
};

int build_graph (struct word *from, struct word *to , struct word *dict , int xnumOfWords) {
    struct node *graph;
    struct node **queue;
    int i;
    int a;
    int fromindex , toindex;
    int newtoindex;
    int flag;

    graph = (struct node *) calloc (1,sizeof(struct node));
    graph->link = from;
    graph->precedent = NULL;
    graph->doublets = NULL;
    graph->next = NULL;
    graph->link->flag = 1;

    queue = (struct node **) malloc ((xnumOfWords+1)*sizeof(struct node *));

    *(queue+0) = graph;
    fromindex = 0;
    toindex = 1;
    newtoindex = 1;
    *(queue+1) = NULL;
    do {
        flag=0;
        for (i=fromindex ; i<toindex && newtoindex>0; i++)
            newtoindex = find_doublets(*(queue+i),queue,newtoindex,dict,to);
        
        if (toindex == newtoindex) {
            printf("No solution.\n");
            flag=1;
        } else if (newtoindex<0)
            flag=1;

        fromindex = toindex;
        toindex = newtoindex;
    } while (flag==0);

    for (a=0 ; *(queue+a)!=NULL ; a++)
        (*(queue+a))->link->flag=0;

    free(queue);

    free_graph(graph);

    return 0;
}


int free_graph (struct node *pnode) {
    struct node *temp;
    struct node *temp2;

    if (pnode==NULL)
        return 0;

    temp=pnode->doublets;
    while (temp!=NULL) {
        temp2 = temp->next;
        free_graph(temp);
        temp = temp2;
    }
    free(pnode);

    return 0;
}


int find_doublets (struct node *pword , struct node **pqueue , int plastIndex, struct word *dict, struct word *pwordTo) {
    int a;
    struct word *temp2;
    struct node *tempdoublets;

    for (temp2 = dict ; temp2!=NULL ; temp2 = temp2->next) {
        if (temp2->flag==0 && are_doublets(pword->link->word,temp2->word)) {
            if (pword->doublets == NULL) {
                pword->doublets = (struct node *) calloc (1,sizeof(struct node));
                tempdoublets = pword->doublets;
            } else {
                for (tempdoublets = pword->doublets ; tempdoublets->next!=NULL ; tempdoublets = tempdoublets->next);
                tempdoublets->next = (struct node *) calloc (1,sizeof(struct node));
                tempdoublets = tempdoublets->next;
            }
            tempdoublets->link = temp2;
            tempdoublets->precedent = pword;
            tempdoublets->doublets = NULL;
            tempdoublets->next = NULL;
            tempdoublets->link->flag = 1;
            *(pqueue + plastIndex++) = tempdoublets;
            *(pqueue + plastIndex) = NULL;

            if (strcmp(tempdoublets->link->word,pwordTo->word)==0) {
                mostrar_camino(tempdoublets);
                return -1;
            }
        }
    }

    return plastIndex;
}


mostrar_camino (struct node *pnode) {
    if (pnode->precedent!=NULL)
        mostrar_camino (pnode->precedent);
    printf("%s\n",pnode->link->word);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1000
#define MAX_NAME_LENGTH 100

struct link {
    short int flag;
    struct autores *coautor;
    struct link *next;
};

struct autores {
    char name[MAX_NAME_LENGTH];
    int erdos;
    struct link *coautores;
    struct autores *next;
};


int read_input_file ();
int agregar_autor (struct autores **, char *);
int agregar_link(struct autores *, char *, struct autores *);
int build_graph (int, struct autores **, int , char *);
int calcular_erdos (struct autores *);


int main() {

    read_input_file ();

	return 0;
}


int read_input_file () {
	int a , i , b , o;
	char tempstr[MAX_LINE_LENGTH];
    char *str;
    int scenarios;
    int cant_obras;
    int cant_autores;
    int counter;
    struct autores **xobras;
    char c;
    char *array_autores;


    scanf("%d",&scenarios);

    for (i=1 ; i<=scenarios ; i++) {
        printf("Scenario %d\n",i);

        scanf("%d %d\n",&cant_obras,&cant_autores);
        xobras = (struct autores **) calloc (cant_obras , sizeof(struct autores *));
        
        for (o=0 ; o<cant_obras ; o++) {
            char tempstr2[MAX_LINE_LENGTH];
            int b;

            for (b=0 ; b<(MAX_LINE_LENGTH-1) && (tempstr[b]=getchar())!='\n' ; b++);
            tempstr[b]='\0';

            xobras[o] = NULL;

            a=0;
            b=0;
            counter=0;
            while (tempstr[a]!=':') {
                if (tempstr[a]==',') 
                    if (counter==0) counter++;
                    else if (counter==1) {
                        tempstr2[b]='\0';
                        agregar_autor (&xobras[o],tempstr2);

                        a+=2;
                        b=0;
                        counter=0;
                    }
                tempstr2[b++]=tempstr[a++];
           }
           tempstr2[b]='\0';
           agregar_autor (&xobras[o],tempstr2);
        }

        array_autores = (char *) calloc (cant_autores * MAX_NAME_LENGTH , sizeof(char));

        for (b = 0 ; b < cant_autores ; b++) {
            for (a=0 ; a<(MAX_NAME_LENGTH-1) && (c=getchar())!='\n' && c!=EOF ; a++)
                tempstr[a]=c;
            if (c!=EOF) {
                tempstr[a]='\0';
            } 
            strcpy(array_autores+b*MAX_NAME_LENGTH,tempstr);
        }
        build_graph(cant_obras,xobras,cant_autores,array_autores);
        free(array_autores);
        
        /*Libero la memoria de la lista de obras*/
        for (o=0 ; o<cant_obras ; o++) {
            while (xobras[o]!=NULL) {
                struct autores *tempcoautores;
                tempcoautores = xobras[o]->next;
                free(xobras[o]);
                xobras[o] = tempcoautores;
            }
        }
        free(xobras);
    }

	return 0;
}


int build_graph (int pcant_obras, struct autores **pxobras, int pcant_autores , char *parray_autores) {
    struct autores *graph;
    struct autores *tempgraph;
    int a;
    int o;

    graph = NULL;
    for (o=0 ; o<pcant_obras ; o++) {
        struct autores *tempcoautores;

        tempcoautores = pxobras[o];
        while (tempcoautores!=NULL) {

            if (graph == NULL) {
                graph = (struct autores *) calloc (1, sizeof(struct autores));
                strcpy(graph->name,tempcoautores->name);
                graph->erdos = INT_MAX;
                graph->coautores = NULL;
                graph->next = NULL;
            } else {
                struct autores *ant;
                tempgraph = graph;
                while (tempgraph!=NULL && strcmp(tempgraph->name,tempcoautores->name)!=0) {
                    ant = tempgraph;
                    tempgraph = tempgraph->next;
                }
                if (tempgraph == NULL) {
                    tempgraph = ant;
                    tempgraph->next = (struct autores *) calloc (1, sizeof(struct autores));
                    tempgraph = tempgraph->next;
                    strcpy(tempgraph->name, tempcoautores->name);
                    tempgraph->erdos = INT_MAX;
                    tempgraph->coautores = NULL;
                    tempgraph->next = NULL;
                }
            }

            tempcoautores = tempcoautores->next;
        }

    }


    tempgraph = graph;
    while (tempgraph!=NULL) {

        for (o=0 ; o<pcant_obras ; o++) {
            struct autores *tempaut;

            tempaut = pxobras[o];
            while (tempaut!=NULL && strcmp(tempgraph->name,tempaut->name)!=0)
                tempaut = tempaut->next;

            if (tempaut!=NULL) {
                tempaut=pxobras[o];
                while (tempaut!=NULL) {
                    if (strcmp(tempgraph->name,tempaut->name)!=0)
                        agregar_link(tempgraph, tempaut->name, graph);
                    tempaut = tempaut->next;
                }
            }
        }

        tempgraph = tempgraph->next;
    }

    tempgraph = graph;
    while (tempgraph!=NULL && strcmp(tempgraph->name,"Erdos, P.")!=0)
        tempgraph = tempgraph->next;
    if (tempgraph!=NULL) {
        tempgraph->erdos = 0;
        calcular_erdos(tempgraph);
    }

    for (a=0 ; a<pcant_autores ; a++) {
        for (tempgraph = graph ; tempgraph != NULL && strcmp(tempgraph->name,parray_autores+a*MAX_NAME_LENGTH) ; tempgraph = tempgraph->next);
        if (tempgraph!=NULL) {
            if (tempgraph->erdos < INT_MAX)
                printf("%s %d\n",tempgraph->name, tempgraph->erdos);
            else printf("%s infinity\n",tempgraph->name);
        } else {
            if (strcmp(parray_autores+a*MAX_NAME_LENGTH,"Erdos, P.")==0)
                printf("%s 0\n",parray_autores+a*MAX_NAME_LENGTH);
            else printf("%s infinity\n",parray_autores+a*MAX_NAME_LENGTH);
        }
    }

    return 0;
}


int calcular_erdos (struct autores *pgraph) {
    struct link *templink;
        
    templink = pgraph->coautores;
    while (templink!=NULL) {
        if (templink->coautor->erdos > pgraph->erdos+1) {
            templink->flag=1;
            templink->coautor->erdos = pgraph->erdos+1;
            templink = templink->next;
        } else if (templink->coautor->erdos+1 < pgraph->erdos) {
            pgraph->erdos = templink->coautor->erdos+1;
            templink = pgraph->coautores;
        } else 
            templink = templink->next;
    }

    templink = pgraph->coautores;
    while (templink!=NULL) {
        if (templink->flag==1) {
            templink->flag=0;
            calcular_erdos (templink->coautor);
        }
        templink = templink->next;
    }

    return 0;
}


int agregar_link(struct autores *pnodo, char *pcoautor, struct autores *pgraph) {
    struct link *templink;

    if (pnodo->coautores == NULL) {
        pnodo->coautores = (struct link *) calloc (1, sizeof (struct link));
        pnodo->coautores->flag=0;
        pnodo->coautores->coautor = NULL;
        pnodo->coautores->next=NULL;
        templink = pnodo->coautores;
    } else {
        templink = pnodo->coautores;
        while (templink->next != NULL)
            templink = templink->next;
        templink->next = (struct link *) calloc (1, sizeof(struct link));
        templink = templink->next;
        templink->flag = 0;
        templink->coautor = NULL;
        templink->next = NULL;
    }

    while (pgraph!=NULL && strcmp(pcoautor,pgraph->name)!=0)
        pgraph = pgraph->next;

    if (pgraph!=NULL)
        templink->coautor = pgraph;

    return 0;
}


int agregar_autor (struct autores **pxobras, char *autor) {
    struct autores *tempcoautores;

    tempcoautores = NULL;

    if (*pxobras == NULL) {
        *pxobras = (struct autores *) calloc (1,sizeof(struct autores));
        (*pxobras)->next = NULL;
        tempcoautores = *pxobras;
    } else {
        tempcoautores = *pxobras;
        while (tempcoautores->next!=NULL) 
            tempcoautores = tempcoautores->next;
        tempcoautores->next = (struct autores *) calloc (1, sizeof(struct autores));
        tempcoautores = tempcoautores->next;
        tempcoautores->next = NULL;
    }

    tempcoautores->erdos=INT_MAX;
    strcpy (tempcoautores->name,autor);

    return 0;
}


    

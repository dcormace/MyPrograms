#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CHANCE 6
#define THREEOFAKIND 7
#define FOUROFAKIND 8
#define FIVEOFAKIND 9
#define SHORTSTRAIGHT 10
#define LONGSTRAIGHT 11
#define FULLHOUSE 12

struct puntos {
    int score;
    int flag;
    int fila;
    int colu;
    struct puntos *next;
 };


int main () {
    char c;
    char tempstr[100];
    int partida[13][5];
    int a;
    int b;


    a=0;
    b=0;
    while ((c=getchar()) != EOF) {
        if (c == '\n') {
            tempstr[a] = '\0';
            a=0;
            sscanf(tempstr,"%d %d %d %d %d",partida[b],partida[b]+1,partida[b]+2,partida[b]+3,partida[b]+4);
            b++;
            if (b==13) {
                b=0;
                analizar_partida(partida);
            }
        } else tempstr[a++]=c;
    }


    exit(0);
}


int analizar_partida (int ppartida[13][5]) {
    int a;
    int b;
    int c;
    short int *scorecard;
    short int *scorelist;
    short int *maxarray;
    short int *maxscorelist;
    int first_six_sum;
    int flag2;
    int flag3;
    int flag_str;


    scorelist = (short int *) calloc (13,sizeof(short int));
    scorecard = (short int *) calloc (13*13,sizeof(short int));
    maxscorelist = (short int *) calloc (((0x1<<13)-1)*13,sizeof(short int));
    maxarray = (short int *) malloc(((0x1<<13)-1)*sizeof(short int));
    for (a=0;a<((0x1<<13)-1);a++)
        *(maxarray+a)=-1;

    for (a=0; a<13 ; a++) {
        flag2 = 0;
        flag3 = 0;

        for (b=0; b<5; b++) {
            *(scorecard+(a*13)+ppartida[a][b]-1) += ppartida[a][b];
            *(scorecard+(a*13)+CHANCE) += ppartida[a][b];
        }


        for (b=0;b<6;b++) {
            if (*(scorecard+a*13+b)/(b+1) == 2)
                flag2++;

            if (*(scorecard+a*13+b)/(b+1) == 3)
                flag3++;

            if (*(scorecard+a*13+b)/(b+1) >= 3)
                *(scorecard+a*13+THREEOFAKIND) = *(scorecard+a*13+CHANCE);

            if (*(scorecard+a*13+b)/(b+1) >= 4)
                *(scorecard+a*13+FOUROFAKIND) = *(scorecard+a*13+CHANCE);

            if (*(scorecard+a*13+b)/(b+1) == 5) {
                *(scorecard+a*13+FIVEOFAKIND) = 50;
                flag2++;
                flag3++;
            }
        }

        flag_str=0;
        for (b=0 ; b<3 && flag_str!=4 ; b++) {
            for (c=0;c<4;c++)
                if (*(scorecard+a*13+b+c) > 0)
                    flag_str++;
            if (flag_str == 4)
                *(scorecard+a*13+SHORTSTRAIGHT) = 25;
            else flag_str = 0;
        }

        flag_str=0;
        for (b=0; b<2 && flag_str!=5; b++) {
            for (c=0;c<5;c++)
                if (*(scorecard+a*13+b+c) > 0)
                    flag_str++;
            if (flag_str == 5)
                *(scorecard+a*13+LONGSTRAIGHT) = 35;
            else flag_str = 0;
        }

        if (flag2>0 && flag3>0)
            *(scorecard+a*13+FULLHOUSE) = 40;
    }


    
    /*printf("--------------------------------------------\n");
    for (a=0;a<13;a++) {
        for (b=0;b<5;b++)
            printf("%d ",ppartida[a][b]);
        for (b=0;b<13;b++)
            printf("%2d ",*(scorecard+(a*13)+b));
        printf("\n");
    }
    printf("--------------------------------------------\n");
   */ 
    
    c = analize_scorecard(scorecard, maxarray, 12, 0, scorelist, maxscorelist);

    first_six_sum = 0;
    for (a=0 ; a<13 ; a++) {
        printf("%d ",scorelist[a]);
        if (a<=5)
            first_six_sum += scorelist[a];
    }
    printf ("%d %d\n",((first_six_sum >= 63) ? 35 : 0) , c);

    free (scorelist);
    free (scorecard);
    free (maxarray);
    free (maxscorelist);

    return 0;
}


int analize_scorecard (short int *pscorecard, short int *pmaxarray, int pcat, int pattern, short int *pscorelist, short int *pmaxscorelist) {
    int a;
    int max = 0;
    short int *maxscore;


    if (pcat >= 0) {

        if (*(pmaxarray+pattern) != -1) {
            for (a=0 ; a<=pcat ; a++)
                *(pscorelist+a) = *(pmaxscorelist+pattern*13+a);
            return *(pmaxarray+pattern);
        }
        
        maxscore = (short int *) calloc (13, sizeof(short int));

        for (a=0 ; a<pcat ; a++)
            *(maxscore+a) = *(pscorelist+a);
        *(maxscore+a) = 0;

        for (a=0;a<13;a++) 
            if (!(pattern & (1<<a))) {
                int v = *(pscorecard+a*13+pcat) + analize_scorecard(pscorecard, pmaxarray, pcat-1, (pattern | (1<<a)), pscorelist, pmaxscorelist);
                if (v > max) {
                    max = v;
                    *(pscorelist+pcat) = *(pscorecard+a*13+pcat);
                    for (v=0 ; v<=pcat ; v++)
                        maxscore[v]=*(pscorelist+v);
                }
            }

        if (pcat==5 && max>=63)
            max += 35;

        *(pmaxarray+pattern) = max;
        for (a=0 ; a<=pcat ; a++) {
            *(pmaxscorelist+pattern*13+a) = maxscore[a];
            *(pscorelist+a) = *(maxscore+a);
        }
        
        free (maxscore);
    }

    return max;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    char ch;
    char str[257];
    int cases;
    int a,b;
    char *xfragment;
    int *xindex;
    int counter;

    scanf("%d\n",&cases);

    xfragment = (char *) calloc (2*256*257,sizeof(char));
    xindex = (int *) calloc (256,sizeof(int));

    ch = 0;
    while (ch!=EOF && cases>0) {
        
        counter=0;
        b=0;
        
        do {

            for (a=0; (ch=getchar())!='\n' && ch!=EOF ; a++)
                str[a]=ch;
            str[a]='\0';

            if (a>0) {
                b+=a;
                if ((*(xindex+a-1))==0 || ((*(xindex+a-1))==1 && strcmp((xfragment+0*256*257+a*257),str)!=0)) {
                    strcpy((xfragment+(*(xindex+a-1))*256*257+(a-1)*257),str);
                    (*(xindex+a-1))++;
                }
                counter++;
            }
        } while (a>0);

        char answer[257];
        answer[0]='\0';
        if (solve(xfragment,xindex,0,b*2/counter,answer)==0) {
            printf("%s\n",answer);
            if (cases>1)
                printf("\n");
        }
        
        memset(xindex,0,256*sizeof(int));
        cases--;
    }

    return 0;
}


int solve (char *pfragments , int *pxindex , int pstart , int ptotalen , char *xfullstring) {
    int a;
    int i1,i2;
    char fullstring[257];
    char *frag1;
    char *frag2;

    for (a=pstart ; *(pxindex+a)==0 && a<(ptotalen>>1) ; a++);

    for (i1=0 ; a<(ptotalen>>1) && i1<*(pxindex+a) ; i1++) {
        frag1 = (pfragments+i1*256*257+a*257);
        for (i2=0 ; i2<*(pxindex+ptotalen-a-2) ; i2++) { 

            if (pstart==((ptotalen>>1)-1) && i1==i2 && (ptotalen%2)==0)
                continue;

            frag2 = (pfragments+i2*256*257+(ptotalen-a-2)*257);

            sprintf(fullstring,"%s%s",frag1,frag2);
            if (pstart==0)
                strcpy(xfullstring,fullstring);
            if (strcmp(fullstring,xfullstring)==0) {
                if (solve(pfragments,pxindex,a+1,ptotalen,fullstring)==0) {
                    return 0;
                }
            } 

            sprintf(fullstring,"%s%s",frag2,frag1);
            if (pstart==0)
                strcpy(xfullstring,fullstring);
            if (strcmp(fullstring,xfullstring)==0)
                if (solve(pfragments,pxindex,a+1,ptotalen,fullstring)==0) {
                    return 0;
                }
        }
    }

    if (a<(ptotalen>>1))
        return 1;

    return 0;
}



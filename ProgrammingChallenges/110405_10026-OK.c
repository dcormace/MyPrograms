#include <stdio.h>
#include <stdlib.h>

struct btree {
	int job;
	float ratio;
	struct btree *left;
	struct btree *right;
};

int btree_insert_node (int , float , struct btree **);
int btree_display (struct btree *);
int btree_delete (struct btree *);

char buffer[10000];
int ind;


int main() {
	int a,b;
	int cases, jobs, time, penalty;
	struct btree *xbtree = NULL;

	scanf("%d",&cases);
	for(a=0; a<cases; a++) {
		scanf("%d",&jobs);
		for (b=0; b<jobs; b++) {
			scanf("%d %d", &time, &penalty);
			btree_insert_node (b+1, (float)penalty/(float)time, &xbtree);
		}

		ind=0;
		btree_display (xbtree);
		buffer[ind-1]='\0';
		puts(buffer);
		btree_delete (xbtree);
		xbtree = NULL;

		if (a<(cases-1)) printf("\n");
	}

	exit(0);
}


int btree_insert_node (int job, float ratio, struct btree **xbtree) {

	if (*xbtree == NULL) {
		*xbtree = (struct btree *) malloc (sizeof(struct btree));
		(*xbtree)->job = job;
		(*xbtree)->ratio = ratio;
		(*xbtree)->left = NULL;
		(*xbtree)->right = NULL;
	} else if (ratio > (*xbtree)->ratio)
		btree_insert_node (job, ratio, &(*xbtree)->left);
	else
		btree_insert_node (job, ratio, &(*xbtree)->right);

	return 0;
}


int btree_display (struct btree *xbtree) {

	if (xbtree != NULL) {
		btree_display (xbtree->left);
		ind += sprintf(buffer+ind, "%d ",xbtree->job);
		btree_display (xbtree->right);
	}

	return 0;
}


int btree_delete (struct btree *xbtree) {

	if (xbtree != NULL) {
		btree_delete (xbtree->left);
		btree_delete (xbtree->right);
		free(xbtree);
	}

	return 0;
}


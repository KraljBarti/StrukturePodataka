#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct node node;
typedef struct node* position;
struct node {
	int el;
	position left;
	position right;


};
int inorder(position root, FILE* file) {
	if (root != NULL) {
		inorder(root->left, file);
		printf("%d ", root->el);

		inorder(root->right, file);
	}
}

position createnode(int el);
position insert(position root, int el);

int replace(position root);


int main() {
	position root = NULL;
	position rootRand = NULL;

	srand(time(0));

	int i;

	for (i = 0; i < 10; i++) {
		int randomValue = rand() % 81 + 10;
		root = insert(root, randomValue);
	}

	printf("Originalno stablo (inorder): ");
	inorder(root, stdout);
	printf("\n");


	replace(root);


	printf("Transformirano stablo (inorder): ");
	FILE* outFile = fopen("ispis.txt", "w");
	if (outFile == NULL) {
		perror("Greška pri otvaranju datoteke");
		return 1;
	}
	inorder(root, outFile);
	fclose(outFile);
	printf("\n");

	return 0;
}
position createnode(int el) {
	position newnode = NULL;
	newnode = (position)malloc(sizeof(struct node));
	if (newnode == NULL) {
		printf("Greska pri alociranju memorije!\n");
		return NULL;
	}
	newnode->el = el;
	newnode->left = NULL;
	newnode->right = NULL;

	return newnode;
}
position insert(position root, int el) {
	if (root == NULL) {
		root = createnode(el);
	}
	else if (el < root->el) {
		root->left = insert(root->left, el);
	}
	else if (el > root->el) {
		root->right = insert(root->right, el);
	}
	return root;
}
int replace(position root) {
	if (root == NULL) {
		return NULL;
	}

	replace(root->left);
	replace(root->right);


	int leftSum = (root->left != NULL) ? root->left->el : 0;
	int rightSum = (root->right != NULL) ? root->right->el : 0;
	root->el += leftSum + rightSum;


}

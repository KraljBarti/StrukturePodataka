/*8. Napisati program koji omogućava rad s binarnim stablom pretraživanja.
Treba omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i level order),
brisanje i pronalaženje nekog elementa*/

#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

struct node;
typedef struct node* Position;
typedef struct node
{
	int number;
	Position right;
	Position left;


}Node;


Position  CreateNode(int value);
int preorder(Position root);
int postorder(Position root);
int inorder(Position root);
Position insert(Position root, int value);


int main()
{

	int menuchoose = 0;

	Position root = NULL;

	printf("enter root value");
	int value = 0;
	scanf("%d", &value);
	root = insert(root, value);//postavlljanje vrijednosti na root stabla

	while (1)
	{

		puts("\n..... Menu opened .....\n");
		printf("1 - Insert node\n");
		printf("2 - Print preorder\n");
		printf("3 - Print inorder\n");
		printf("4 - Print postorder\n");
		printf("7 - exit\n");
		puts("\n enter number - "); scanf("%d", &menuchoose);
		switch (menuchoose)
		{
		case 1://obicni unos elementa, koji se postavlja gdje triba u stablu
			puts("\n enter number "); scanf("%d", &value);
			insert(root, value); break;

		case 2: puts("preorder sort - "); preorder(root); break;
		case 3:puts("inorder sort - "); inorder(root); break;
		case 4: puts("postorder sort - "); postorder(root); break;
		case 5: puts("exit mode - "); return 0; break;

		default:
			break;
		}

	}

}


Position insert(Position root, int value) {
	if (root == NULL)
		return CreateNode(value);

	if (value < root->number)
		root->left = insert(root->left, value);
	else if (value >= root->number)
		root->right = insert(root->right, value);

	return root;
}

Position  CreateNode(int value)
{
	Position node = NULL;
	node = (Position)malloc(sizeof(Node));
	if (node == NULL)
	{
		puts("can not allocate memory for new node...");
		return NULL;
	}

	node->number = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

int preorder(Position root)
{
	if (root != NULL)
	{

		//korijen, lijeva strana, desna strana
		printf("%d", root->number);
		preorder(root->left);
		preorder(root->right);
	}
	return 0;
}
int postorder(Position root)
{
	if (root != NULL)
	{
		//lijeva strana, desna strana, korijen
		preorder(root->left);
		preorder(root->right);
		printf("%d", root->number);
	}
	return 0;
}
int inorder(Position root)
{
	if (root != NULL)
	{
		//lijeva strana, root, desna strana
		preorder(root->left);
		printf("%d", root->number);
		preorder(root->right);

	}
	return 0;
}
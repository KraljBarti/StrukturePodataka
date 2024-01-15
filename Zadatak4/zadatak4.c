/*
4. Napisati program za zbrajanje i množenje polinoma.Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE (1024)
#define MALLOC_ERROR (NULL)
#define FILE_ERROR (-1)
#define EMPTY_LIST (2)
#define FILE_SHAPE_ERROR (3)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int Exp;
	int Coeff;
	Position Next;
}Element;

Position initialization(Position);
int readFile(Position,Position);
int printPoly(Position);
int scanInList(Position, int, int);
int sumPoly(Position,Position,Position);
int multiPoly(Position, Position, Position);
int deleteAll(Position);

int main()
{
	Position headPoly1 = NULL;
	Position headPoly2 = NULL;
	Position headPolySum = NULL;
	Position headPolyMulti = NULL;
	
	//Postavlja head liste na pocetne vrijednosti
	headPoly1=initialization(headPoly1);
	headPoly2=initialization(headPoly2);
	headPolySum=initialization(headPolySum);
	headPolyMulti=initialization(headPolyMulti);

	readFile(headPoly1,headPoly2);
	printf("First polynome: ");
	printPoly(headPoly1->Next);
	printf("Second polynome: ");
	printPoly(headPoly2->Next);

	sumPoly(headPoly1,headPoly2,headPolySum);
	printf("\nSum of polynomials: ");
	printPoly(headPolySum->Next);

	multiPoly(headPoly1->Next, headPoly2->Next, headPolyMulti);
	printf("Product of polynomials:");
	printPoly(headPolyMulti->Next);
	
	deleteAll(headPoly1);
	deleteAll(headPoly2);
	deleteAll(headPolySum);
	deleteAll(headPolyMulti);
	
	return EXIT_SUCCESS;
}

Position initialization(Position p)
{
	p = (Position)malloc(sizeof(Element));
	if (p==NULL)
	{
		printf("ERROR.Unable to allocate memory.\n");
		return MALLOC_ERROR;
	}

	p->Coeff = 0;
	p->Exp = 0;
	p->Next = NULL;

	return p;
}

int readFile(Position headPoly1, Position headPoly2)
{
	char buffer[MAX_LINE] = {0};
	int tempBuff = 0, tempExp=0,tempCoef=0, bufferCounter=0;
	FILE* filePointer = NULL;
	filePointer = fopen("polynomials.txt","r");
	//Provjera je li file uspjesno otvoren
	if (filePointer == NULL)
	{
		printf("ERROR.Unable to open file.\n");
		return FILE_ERROR;
	}
	//Spremanje prvog polinoma
	fgets(buffer, MAX_LINE,filePointer);
	while (sscanf(buffer + tempBuff, "%dx^%d %n", &tempCoef, &tempExp, &bufferCounter) == 2)
	{
		//tempBuff sluzi za pomjeranje pokazivaca i citanje iduceg clana polinoma sve daok ne dodemo do kraja
		tempBuff += bufferCounter;
		scanInList(headPoly1,tempCoef,tempExp);
	}
	//Spremanje drugog polinoma
	fgets(buffer, MAX_LINE, filePointer);
	tempBuff = 0;
	while (sscanf(buffer + tempBuff, "%dx^%d %n", &tempCoef, &tempExp, &bufferCounter) == 2)
	{
		tempBuff += bufferCounter;
		scanInList(headPoly2, tempCoef, tempExp);
	}

	fclose(filePointer);
	return EXIT_SUCCESS;
}

int scanInList(Position head, int coef, int exp)
{
	Position newElement = NULL,temp=NULL;
	//Spremanje po velicini(od najveceg prema najmanjem)
	while (head->Next != NULL && head->Next->Exp>exp) {
		head = head->Next;
	}
	if (head->Next==NULL || head->Next->Exp<exp) {
		//Ucitavanje novog elementa u listi i povezivanje s ostatkom liste
		newElement = initialization(newElement);
		newElement->Coeff = coef;
		newElement->Exp = exp;
		newElement->Next = head->Next;
		head->Next = newElement;
	}
	else
	{
		//Ako vec postoji clan s unesenim eksponentom
		head->Next->Coeff += coef;
		//Ako se dogodi da zbroj koeficijenta bude 0 brise se taj element
		if (head->Next->Coeff == 0)
		{
			temp = head->Next->Next;
			free(head->Next);
			head->Next=temp;
		}
	}

	return EXIT_SUCCESS;
}

int printPoly(Position firstElement)
{
	//Provjera je li lista prazna
	if (firstElement == NULL)
	{
		printf("ERROR. Empty list.");
		return EMPTY_LIST;
	}

	while (firstElement->Next!= NULL)
	{
		if (firstElement->Coeff < 0) {
			//Ako je koeficijen negativan stavljamo ga u zagrade
			printf("(%dx^%d)+", firstElement->Coeff, firstElement->Exp);
			firstElement = firstElement->Next;
		}
		else if (firstElement->Exp == 0)
		{
			//Ako je eksponent jednak 0 ispisujemo samo koeficijen jer je x^0=1
			printf("%d+", firstElement->Coeff);
			firstElement = firstElement->Next;
		}
		else
		{
			//Inace ispisujemo u normalnom obliku do predzadnjeg clana
			printf("%dx^%d+", firstElement->Coeff, firstElement->Exp);
			firstElement = firstElement->Next;
		}
	}
	//zadnji clan ispisujemo posebno kako ne bismo imali + na kraju polinoma
	printf("%dx^%d\n", firstElement->Coeff, firstElement->Exp);
	return EXIT_SUCCESS;
}
int sumPoly(Position Poly1, Position Poly2, Position PolySum)
{
	Position newElement = NULL;

	while (Poly1->Next != NULL && Poly2->Next!=NULL)
	{
		if (Poly1->Next->Exp > Poly2->Next->Exp)
		{
			newElement = initialization(newElement);
			newElement->Coeff = Poly1->Next->Coeff;
			newElement->Exp = Poly1->Next->Exp;
			//"Dodaj na kraj liste"
			while (PolySum->Next !=NULL)
				PolySum = PolySum->Next;
			newElement->Next = PolySum->Next;
			PolySum->Next = newElement;
			//p1 pomjeramo za jedno mjesto jer smo upisali element iz p1
			Poly1 = Poly1->Next;
		}
		else if (Poly1->Next->Exp < Poly2->Next->Exp)
		{
			newElement = initialization(newElement);
			newElement->Coeff = Poly2->Next->Coeff;
			newElement->Exp = Poly2->Next->Exp;
			while (PolySum->Next != NULL)
				PolySum = PolySum->Next;
			newElement->Next = PolySum->Next;
			PolySum->Next = newElement;
			//p2 pomjeramo za jedno mjesto jer smo upisali element iz p2
			Poly2 = Poly2->Next;
		}
		else if(Poly1->Next->Exp == Poly2->Next->Exp)
		{
			newElement = initialization(newElement);
			newElement->Coeff = Poly2->Next->Coeff+Poly1->Next->Coeff;
			newElement->Exp = Poly2->Next->Exp;
			while (PolySum->Next != NULL)
				PolySum = PolySum->Next;
			newElement->Next = PolySum->Next;
			PolySum->Next = newElement;
			//Pomjeramo i p1 i p2 jer smo upisali oba elementa
			Poly1 = Poly1->Next;
			Poly2 = Poly2->Next;
		}
	}
	//Petlja se izvodi u slucaju ako je p2 kraci od p1
	if (Poly1->Next != NULL)
	{
		while (Poly1->Next != NULL)
		{
			newElement = initialization(newElement);
			newElement->Coeff = Poly1->Next->Coeff;
			newElement->Exp = Poly1->Next->Exp;
			while (PolySum->Next != NULL)
				PolySum = PolySum->Next;
			newElement->Next = PolySum->Next;
			PolySum->Next = newElement;
			Poly1 = Poly1->Next;
		}
	}
	//Petlja se izvodi u slucaju ako je p1 kraci od p2
	if (Poly2->Next != NULL)
	{
		while (Poly2->Next != NULL)
		{
			newElement = initialization(newElement);
			newElement->Coeff = Poly2->Next->Coeff;
			newElement->Exp = Poly2->Next->Exp;
			while (PolySum->Next != NULL)
				PolySum = PolySum->Next;
			newElement->Next = PolySum->Next;
			PolySum->Next = newElement;
			Poly2 = Poly2->Next;
		}
	}
	return EXIT_SUCCESS;
}

int deleteAll(Position head)
{
	Position temp = NULL;
	while (head ->Next!= NULL)
	{
		temp = head->Next;
		head->Next = temp->Next;
		free(temp);
	}
	free(head);

	return EXIT_SUCCESS;
}

int multiPoly(Position Poly1, Position Poly2, Position Result)
{
	Position temp = NULL;
	temp = Poly2;
	//Mnozenje "svaki sa svakim"
	while (Poly1 != NULL)
	{
		Poly2 = temp;
		while (Poly2 != NULL) {
			//Sortirani unos pomocu vec napisane funkcije
			scanInList(Result, Poly1->Coeff * Poly2->Coeff, Poly1->Exp+Poly2->Exp);
			Poly2 = Poly2->Next;
		}
		Poly1 = Poly1->Next;
	}

	return EXIT_SUCCESS;
}

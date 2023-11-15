/*  5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste. */

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_LENGHT (1024)
#define FILE_ERROR (-1)
#define MALLOC_ERROR (NULL)
#define OPERATION_ERROR (999888777)

struct _stack;
typedef struct _stack* Position;
typedef struct _stack {
	int Element;
	Position Next;
}stack;

Position initialization(Position);
int readFromFile(Position, char*);
int calculate(Position);
int stackLogic(Position, char*);
int push(Position, int);
int doOperation(Position, char);
int pull(Position);
int delete(Position);


int main()
{
	Position head = NULL;
	int result = 0;
	head = initialization(head);

	result = calculate(head);
	//Provjera postoji li nepredviden clan u postfiksu
	if (result == OPERATION_ERROR)
		return OPERATION_ERROR;

	printf("%d", result);
	free(head);
	return EXIT_SUCCESS;
}

Position initialization(Position newElement)
{
	newElement = (Position)malloc(sizeof(stack));
	if (newElement == NULL) {
		printf("ERROR.Unable to allocate memory.\n");
		return MALLOC_ERROR;
	}
	newElement->Element = 0;
	newElement->Next = NULL;
	return newElement;
}

int calculate(Position head)
{
	int result = 0;
	int check = 0;
	char buffer[MAX_LENGHT] = { 0 };
	readFromFile(head, buffer);
	check = stackLogic(head, buffer);
	if (check == OPERATION_ERROR)
		return OPERATION_ERROR;
	//result = element prvog clana niza
	result = head->Next->Element;
	//Postavljamo head->Next na NULL ptr
	head->Next = head->Next->Next;
	//Oslobadamo "prvi pravi" clan 
	delete(head);
	return result;
}

int readFromFile(Position head, char* buffer)
{
	FILE* filePointer = NULL;
	filePointer = fopen("postfix.txt", "r");
	//Provjera je li datoteka otvorena
	if (filePointer == NULL)
	{
		printf("ERROR. Unable to open file.");
		return FILE_ERROR;
	}
	//Ucitavnje posfiks izraza u buffer
	fgets(buffer, MAX_LENGHT, filePointer);

	//printf("%s", buffer);
	fclose(filePointer);

	return EXIT_SUCCESS;
}

int stackLogic(Position head, char* buffer)
{
	int bufferCounter = 0;
	int tempNumber = 0;
	int check = 0;
	char tempChar = 0;
	char* tempBuffer = NULL;
	tempBuffer = buffer;
	while (strlen(tempBuffer) != 0)
	{
		if (sscanf(tempBuffer, "%d %n", &tempNumber, &bufferCounter) == 1)
		{
			//dodajemo element na stog
			push(head, tempNumber);
		}
		else
		{
			sscanf(tempBuffer, "%c %n", &tempChar, &bufferCounter);
			//izvodimo zadanu operaciju
			check = doOperation(head, tempChar);
			if (check == OPERATION_ERROR) {
				return OPERATION_ERROR;
			}
		}
		//pomjeramo za procitani element
		tempBuffer += bufferCounter;
	}

	return EXIT_SUCCESS;
}

int push(Position head, int tempNumber)
{
	Position newStackElement = NULL;
	newStackElement = initialization(newStackElement);
	//dodaj na pocetak liste
	newStackElement->Element = tempNumber;
	newStackElement->Next = head->Next;
	head->Next = newStackElement;

	return EXIT_SUCCESS;
}

int doOperation(Position head, char mathOperation)
{
	int ele1 = 0, ele2 = 0;
	//ele1 i ele2 primaju vrijednosti iz stoga
	ele1 = pull(head);
	ele2 = pull(head);
	//oblik: ele2 'operacija' ele1, zbog redoslijeda dolazenja podataka na stog
	switch (mathOperation) {
	case '+':
		ele1 = ele2 + ele1;
		push(head, ele1);
		break;
	case '-':
		ele1 = ele2 - ele1;
		push(head, ele1);
		break;
	case '*':
		ele1 = ele2 * ele1;
		push(head, ele1);
		break;
	case '/':
		ele1 = ele2 / ele1;
		push(head, ele1);
		break;
	default:
		printf("ERROR. Wrong operation!\n");
		delete(head);
		return OPERATION_ERROR;
	}
	return EXIT_SUCCESS;
}

int pull(Position head)
{
	int tempVal = 0;
	Position temp = NULL;
	temp = head->Next;
	tempVal = temp->Element;
	head->Next = temp->Next;
	free(temp);

	return tempVal;
}
int delete(Position head)
{
	Position temp = NULL;

	while (head->Next != NULL)
	{
		temp = head->Next;
		head->Next = temp->Next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

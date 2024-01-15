/*10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki čvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo dr�ava sortirano po nazivu. Svaki čvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGHT (1024)
#define FILE_ERROR (-3)

typedef struct _city* cityPosition;
typedef struct _city {
	char name[MAX_LENGHT];
	int population;
	cityPosition next;
	cityPosition left;
	cityPosition right;
}City;

typedef struct _country* countryPosition;
typedef struct _country {
	char name[MAX_LENGHT];
	cityPosition citiesOfTheCountry;
	countryPosition next;
	countryPosition left;
	countryPosition right;
}Country;

countryPosition initializationOfCountry(countryPosition);
countryPosition creatingTheListOfCountry(countryPosition, char*, char*);
int find(countryPosition);
int treePrint(cityPosition, int);

cityPosition creatingTheTreeOfCities(cityPosition, char*, int);
cityPosition initializationOfCity(cityPosition);
int printCountryList(countryPosition);
int printfCityTree(cityPosition);

countryPosition creatingTheTreeOfCountries(countryPosition, char*, char*);
cityPosition creatingTheListOfCities(cityPosition, char*, int);
int printCountryTree(countryPosition);
int printCityList(cityPosition);

int main()
{
	countryPosition head = NULL;
	head = initializationOfCountry(head);
	FILE* filePointer = NULL;
	filePointer = fopen("drzave.txt", "r");
	if (filePointer == NULL) {
		printf("ERROR. Unable to open file.\n");
		return FILE_ERROR;
	}

	char countryName[MAX_LENGHT] = { 0 };
	char countryFile[MAX_LENGHT] = { 0 };

	while (!feof(filePointer))
	{
		fscanf(filePointer, "%s %s", countryName, countryFile);
		head = creatingTheListOfCountry(head, countryName, countryFile);
		head->left = creatingTheTreeOfCountries(head->left, countryName, countryFile);
	}
	printf("a)\n");
	printCountryList(head->next);
	printf("\n______________\nb)\n");
	printCountryTree(head->left);

	find(head);


	fclose(filePointer);

	return EXIT_SUCCESS;
}

countryPosition initializationOfCountry(countryPosition newElement)
{
	newElement = (countryPosition)malloc(sizeof(Country));
	if (newElement == NULL) {
		printf("ERROR. Unable to allocate memory.\n");
		return NULL;
	}
	newElement->citiesOfTheCountry = NULL;
	strcpy(newElement->name, "0");
	newElement->next = NULL;
	newElement->left = NULL;
	newElement->right = NULL;

	return newElement;
}
countryPosition creatingTheListOfCountry(countryPosition head, char* countryName, char* cityFileName)
{
	FILE* filePointer = NULL;
	countryPosition currentCountry = NULL, newCountry = NULL;
	char cityName[MAX_LENGHT] = { 0 };
	int cityPopulation = 0;
	currentCountry = head;
	filePointer = fopen(cityFileName, "r");
	if (filePointer == NULL) {
		printf("ERROR. Unable to open file.\n");
		return NULL;
	}
	while (currentCountry->next != NULL && strcmp(currentCountry->next->name, countryName) < 0)
		currentCountry = currentCountry->next;
	newCountry = initializationOfCountry(newCountry);
	strcpy(newCountry->name, countryName);
	while (!feof(filePointer))
	{
		fscanf(filePointer, " %s %d", cityName, &cityPopulation);
		cityName[strlen(cityName) - 1] = '\0';
		newCountry->citiesOfTheCountry = creatingTheTreeOfCities(newCountry->citiesOfTheCountry, cityName, cityPopulation);
	}
	newCountry->next = currentCountry->next;
	currentCountry->next = newCountry;
	fclose(filePointer);
	return head;
}
cityPosition creatingTheTreeOfCities(cityPosition node, char* cityName, int cityPopulation)
{
	if (node == NULL) {
		node = initializationOfCity(node);
		strcpy(node->name, cityName);
		node->population = cityPopulation;
		return node;
	}
	else if (cityPopulation >= node->population)
		node->right = creatingTheTreeOfCities(node->right, cityName, cityPopulation);
	else if (cityPopulation < node->population)
		node->left = creatingTheTreeOfCities(node->left, cityName, cityPopulation);
	return node;
}
cityPosition initializationOfCity(cityPosition newCity)
{
	newCity = (cityPosition)malloc(sizeof(City));
	if (newCity == NULL) {
		printf("ERROR. Unable to allocate memory.\n");
		return NULL;
	}
	newCity->left = NULL;
	newCity->right = NULL;
	newCity->next = NULL;
	strcpy(newCity->name, "0");
	newCity->population = 0;
	return newCity;
}
int printCountryList(countryPosition head)
{
	while (head != NULL)
	{
		printf("\n%s", head->name);
		printfCityTree(head->citiesOfTheCountry);
		printf("\n");
		head = head->next;
	}
	return EXIT_SUCCESS;
}
int printfCityTree(cityPosition root)
{
	if (root != NULL)
	{
		printfCityTree(root->left);
		printf("\n%s - %d", root->name, root->population);
		printfCityTree(root->right);
	}
	return EXIT_SUCCESS;
}
countryPosition creatingTheTreeOfCountries(countryPosition node, char* countryName, char* countryFile)
{
	FILE* filePointer = NULL;
	if (node == NULL) {
		char cityName[MAX_LENGHT] = { 0 };
		int cityPopulation = 0;
		node = initializationOfCountry(node);
		strcpy(node->name, countryName);
		filePointer = fopen(countryFile, "r");
		if (filePointer == NULL) {
			printf("ERROR. Unable to open file.\n");
			return NULL;
		}
		while (!feof(filePointer))
		{
			fscanf(filePointer, " %s %d", cityName, &cityPopulation);
			cityName[strlen(cityName) - 1] = '\0';
			node->citiesOfTheCountry = creatingTheListOfCities(node->citiesOfTheCountry, cityName, cityPopulation);
		}
		fclose(filePointer);
		return node;
	}
	else if (strcmp(node->name, countryName) < 0)
		node->right = creatingTheTreeOfCountries(node->right, countryName, countryFile);
	else if (strcmp(node->name, countryName) >= 0)
		node->left = creatingTheTreeOfCountries(node->left, countryName, countryFile);
	return node;
}
cityPosition creatingTheListOfCities(cityPosition head, char* cityName, int cityPopulation)
{
	cityPosition currentCity = NULL, newCity = NULL;
	currentCity = head;
	if (currentCity == NULL)
	{
		currentCity = initializationOfCity(currentCity);
		strcpy(currentCity->name, cityName);
		currentCity->population = cityPopulation;
		return currentCity;
	}
	else if (currentCity->population > cityPopulation)
	{
		newCity = initializationOfCity(newCity);
		strcpy(newCity->name, cityName);
		newCity->population = cityPopulation;
		newCity->next = currentCity;
		return newCity;
	}
	else
		while (currentCity->next != NULL && (currentCity->next->population < cityPopulation))
			currentCity = currentCity->next;
	newCity = initializationOfCity(newCity);
	strcpy(newCity->name, cityName);
	newCity->population = cityPopulation;
	newCity->next = currentCity->next;
	currentCity->next = newCity;
	return head;
}
int printCountryTree(countryPosition root)
{
	if (root != NULL)
	{
		printCountryTree(root->left);
		printf("\n%s\n", root->name);
		printCityList(root->citiesOfTheCountry);
		printCountryTree(root->right);
	}
	return EXIT_SUCCESS;
}
int printCityList(cityPosition head)
{
	while (head != NULL)
	{
		printf("%s - %d\n", head->name, head->population);
		head = head->next;
	}
	return EXIT_SUCCESS;
}
int find(countryPosition head)
{
	char countryName[MAX_LENGHT] = { 0 };
	int findPopulation = 0;
	printf("\nUnesi koju drzavu zelite pretraziti: ");
	scanf(" %s", countryName);
	printf("Unesite broj populacije: ");
	scanf("%d", &findPopulation);

	//PRETRAGA PREKO LISTE DRZAVA
	countryPosition current = NULL;
	current = head->next;
	while (current != NULL && strcmp(current->name, countryName))
		current = current->next;
	if (current != NULL) {
		cityPosition rootCity = NULL;
		rootCity = current->citiesOfTheCountry;
		printf("\nPopis gradova s vecom populacijom od %d:", findPopulation);
		treePrint(rootCity, findPopulation);
	}
	else
		printf("\nPogresan unos, molimo pripazite na veliko i malo slovo!\n");
	return EXIT_SUCCESS;
}
int treePrint(cityPosition nodeCity, int findPopulation)
{
	if (nodeCity != NULL) {
		treePrint(nodeCity->left, findPopulation);
		if (nodeCity->population > findPopulation)
			printf("\n%s - %d", nodeCity->name, nodeCity->population);
		treePrint(nodeCity->right, findPopulation);
	}
	return EXIT_SUCCESS;
}

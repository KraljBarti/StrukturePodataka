#include<stdio.h>
#include<stdlib.h>
/*1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci.+
Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) +
i učitati iz datoteke sve zapise.
Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/


#define VELICINA 50
#define max_br_bodova 10
typedef struct student
{
	char ime[VELICINA];
	char prezime[VELICINA];
	int bodovi;
} stud;

int main()
{
	int brojstudenata = 0;
	stud* s;

	FILE* fp = NULL;
	fp = fopen("studenti.txt", "r");
	if (fp == NULL) {
		printf("Ne mogu otvoriti datoteku studenti.txt.\n");
		return 1;
	}

	
	fscanf(fp, "%d", &brojstudenata);
	printf("Imamo %d studenata na popisu.\n\n", brojstudenata);

	
	s = (stud*)malloc(sizeof(stud) * brojstudenata);
	if (s == NULL) {
		printf("Neuspjelo alociranje memorije.\n");
		return 1;
	}

	for (int i = 0; i < brojstudenata; i++) {
		if (fscanf(fp, "%s %s %d", s[i].ime, s[i].prezime, &s[i].bodovi) != 3) {
			printf("Nije moguće pročitati podatke za studenta %d.\n", i + 1);
			break;
		}
	}

	int relbr = 0;
	for (int i = 0; i < brojstudenata; i++)
	{
		relbr = (s[i].bodovi * 100) / max_br_bodova;

		printf("student %d. \n %s %s %d, rel br bod: %d \n", i + 1, s[i].ime, s[i].prezime, s[i].bodovi, relbr);


	}

	fclose(fp);
	free(s);
	return 0;
}
/*1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/


#include<stdio.h>
#include<stdlib.h>

struct Student {
	char ime[50];
	char prezime[50];
	int bodovi;
};

int main() {

	FILE* datoteka;
	int brojStudenata;
	double max_br_bodova = 100.0; // Maksimalan broj bodova (u ovom slučaju 100)

	//Otvaranje datoteke za čitanje
	datoteka = fopen("studenti.txt", "r");

	//Provjera uspjesnosti otvaranja datoteke
	if (datoteka == NULL) {
		printf("Ne mogu otvoriti datoteku!\n");
		return 1;
	}

	//Čitanje broja redaka iz datoteke
	fscanf(datoteka, "%d", &brojStudenata);

	//Dinamička alokacija prostora za niz struktura studenata
	struct Student* studenti = (struct Student*)malloc(brojStudenata * sizeof(struct Student));

	//Čitanje podataka o studentima iz datoteke
	for (int i = 0; i < brojStudenata; i++) {
		fscanf(datoteka, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);

	}
	//Zatvaranje datoteke
	fclose(datoteka);
	//Ispisivanje podataka o studentima i izračun relativnih bodova
	for (int i = 0; i < brojStudenata; i++) {
		double relativni_br_bodova = ((double)studenti[i].bodovi / max_br_bodova) * 100;
		printf("Ime: %s, Prezime: %s, Apsolutni bodovi: %d, Relativni bodovi: %.2lf%%\n", studenti[i].ime,studenti[i].prezime,studenti[i].bodovi,relativni_br_bodova);

	}

	//Oslobađanje alociranog prostora
	free(studenti);
	return 0;

}
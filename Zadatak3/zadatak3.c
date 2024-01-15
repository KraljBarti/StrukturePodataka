/*
Prethodnom zadatku dodati funkcije:
A. dinamicki dodaje novi element iza odredenog elementa,
B. dinamicki dodaje novi element ispred odredenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. cita listu iz datoteke.
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct osoba;
typedef struct osoba* Position;

struct osoba {
	char ime[10];
	char prezime[10];
	int g_r;
	Position Next;
};

void Insert(Position);
void Print(Position);
Position FindLast(Position);
Position Find(Position);
Position FindPrev(Position);
void Delete(Position);
void SortList(Position);
void WriteToFile(Position);
void ReadFromFile(Position);


void main() {
	struct osoba head, * q;
	char izbor = 0;

	head.Next = NULL;
	while (izbor != 'K' && izbor != 'k') {
		printf("\nUnesi:");
		printf("\n\t0 - unos elementa na pocetak");
		printf("\n\t1 - ispis liste");
		printf("\n\t2 - unos elementa na kraj");
		printf("\n\t3 - trazenje elementa u listi po prezimenu");
		printf("\n\t4 - brisanje");
		printf("\n\t5 - unos elementa iza odredenog elementa");
		printf("\n\t6 - unos elementa ispred odredenog elementa");
		printf("\n\t7 - sortiranje liste po prezimenima");
		printf("\n\t8 - upisivanje liste u datoteku");
		printf("\n\t9 - citanje liste iz datoteke");
		printf("\n\tk - kraj\n\n\t");
		scanf(" %c", &izbor);

		switch (izbor) {
		case '0':
			Insert(&head);
			break;
		case '1':
			Print(head.Next);
			break;
		case '2':
			q = FindLast(&head);
			Insert(q);
			break;
		case '3':
			q = Find(head.Next);
			if (NULL == q) {
				printf("\n Osoba ne postoji u listi!");
			}
			else {
				printf("\n\tOsoba je: %s %s, %d", q->ime, q->prezime, q->g_r);
			}
			break;
		case '4':
			Delete(&head);
			break;
		case '5':
			q = Find(head.Next);
			if (NULL == q)
				printf("\n Osoba ne postoji u listi!");
			else
				Insert(q);
			break;
		case '6':
			q = FindPrev(&head);
			if (NULL == q)
				printf("\n Osoba ne postoji u listi!");
			else
				Insert(q);
			break;
		case '7':
			SortList(&head);
			break;
		case '8':
			WriteToFile(head.Next);
			break;
		case '9':
			ReadFromFile(&head);
			break;
		case 'k':
		case 'K':
			printf("\nKraj programa!!!\n");
			break;
		default:
			printf("\nKrivi unos!!!\n");
		}
	}
	// Oslobodi memoriju prije završetka programa
	while (head.Next != NULL) {
		Position temp = head.Next;
		head.Next = temp->Next;
		free(temp);
	}


}


void Insert(Position P) {
	Position temp;

	temp = (Position)malloc(sizeof(struct osoba));
	printf("\nUnesite podatke o osobi: ");
	scanf("%s %s %d", temp->ime, temp->prezime, &temp->g_r);
	temp->Next = P->Next;
	P->Next = temp;

}

void Print(Position P) {

	if (P == NULL) {
		printf("\nLista je prazna!\n");
	}
	else {
		printf("\nU listi se nalaze:");
		while (P != NULL) {
			printf("\n\t %s %s %d", P->ime, P->prezime, P->g_r);
			P = P->Next;
		}

	}
}
Position FindLast(Position P) {
	while (NULL != P->Next)
		P = P->Next;
	return P;

}

Position Find(Position P) {
	char prez[10];

	printf("\nUnesite prezime osobe koju zelite pronaci: ");
	scanf(" %s", prez);
	while (P != NULL && strcmpi(P->prezime, prez) != 0)
		P = P->Next;
	return P;
}
Position FindPrev(Position P) {
	char prez[10];
	if (NULL == P->Next) {
		P = NULL;
	}
	else {
		printf("\n Unesi prezime osobe koju zelis pronaci. ");
		scanf(" %s", prez);
		while (strcmpi(P->Next->prezime, prez) != 0 && P->Next->Next != NULL)
			P = P->Next;
		if (strcmpi(P->Next->prezime, prez) != 0)
			P = NULL;
	}
	return P;
}
void Delete(Position P) {
	Position prev;

	prev = FindPrev(P);

	if (NULL != prev) {
		P = prev->Next;
		prev->Next = P->Next;
		printf("\nOsoba %s %s %d je obrisana.", P->ime, P->prezime, P->g_r);
		free(P);
	}
	else {
		printf("\nOsoba ne postoji u listi.");
	}
}

void InsertAfter(Position P) {
	if (P == NULL) {
		printf("Ne možete dodati iza NULL elementa.");
		return;
	}

	Position temp;
	temp = (Position)malloc(sizeof(struct osoba));

	if (temp == NULL) {
		printf("Neuspješna alokacija memorije.");
		return;
	}

	printf("\nUnesite podatke o osobi koju zelite dodati iza %s %s: ", P->ime, P->prezime);
	scanf("%s %s %d", temp->ime, temp->prezime, &temp->g_r);

	temp->Next = P->Next;
	P->Next = temp;
}

void InsertBefore(Position P, Position head) {
	if (P == NULL || P == head) {
		printf("Ne možete dodati ispred NULL elementa ili ispred glave liste.");
		return;
	}

	Position temp;
	temp = (Position)malloc(sizeof(struct osoba));

	if (temp == NULL) {
		printf("Neuspješna alokacija memorije.");
		return;
	}

	printf("\nUnesite podatke o osobi koju želite dodati ispred %s %s: ", P->ime, P->prezime);
	scanf("%s %s %d", temp->ime, temp->prezime, &temp->g_r);

	Position prev = FindPrev(P);
	temp->Next = prev->Next;
	prev->Next = temp;
}

void SortList(Position head) {
	if (head->Next == NULL) {
		printf("Lista je prazna, nema što sortirati.\n");
		return;
	}

	int swapped;
	Position temp;
	Position end = NULL;
	Position prev = NULL;

	do {
		swapped = 0;
		temp = head;

		while (temp->Next != end) {
			if (strcmp(temp->prezime, temp->Next->prezime) > 0) {
				// Zamijeni pozicije cvorova
				Position current = temp->Next;
				temp->Next = current->Next;
				current->Next = temp;

				if (prev == NULL) {
					head = current;
				}
				else {
					prev->Next = current;
				}

				prev = current;
				swapped = 1;
			}
			else {
				prev = temp;
				temp = temp->Next;
			}
		}
		end = temp;
	} while (swapped);
}

void WriteToFile(Position P)
{
	// ova funkcija dodaje elemente iz datoteke 
	// iza pokazivaca koji se posalje

	FILE* dat;
	char imeDat[10];

	printf("\nUnesi ime datoteke u koju zelis spremiti listu: ");
	scanf(" %s", imeDat);

	dat = fopen(imeDat, "w");

	if (NULL == dat)
		printf("\n Greska u otvaranju datoteke");
	else
	{
		while (P != NULL)
		{
			fprintf(dat, "\n %s %s %d", P->ime, P->prezime, P->g_r);
			P = P->Next;
		}

		fclose(dat);
	}
}


void ReadFromFile(Position P) {
	FILE* dat;
	char imeDat[100];
	Position temp;

	printf("\nUnesi ime datoteke iz koje zelis procitati listu: ");
	scanf(" %s", imeDat);

	dat = fopen(imeDat, "r");

	if (dat == NULL) {
		printf("\nNe postoji datoteka s tim imenom");
		return;
	}

	char line[100];
	while (fgets(line, sizeof(line), dat) != NULL) {
		temp = (Position)malloc(sizeof(struct osoba));

		if (temp == NULL) {
			printf("\nGreška u alokaciji memorije");
			fclose(dat);
			return;
		}

		if (sscanf(line, " %s %s %d", temp->ime, temp->prezime, &temp->g_r) == 3) {
			temp->Next = P->Next;
			P->Next = temp;
			P = temp;
		}
		else {
			free(temp);
		}
	}

	fclose(dat);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode_function.h"

// Cette fonction remplace par 0 ou par 1 le bit de poids faible dans
// un nombre binaire donnée
// nb : nombre binaire , bits: 0 ou 1
unsigned char bitFaible(unsigned char nb, int bits)
{
	unsigned char resultat = 0x00;
	unsigned char un = 0x01; // 0000 0001(2)
	unsigned char ze = 0xFE; // 1111 1110(2)

	if (bits)
		// met a 1 le bit de poids faible
		resultat = nb | un; // opérateur OU
	else
		// met a 0 le bit de poids faible
		resultat = nb & ze; // opérateur ET

    return resultat;
}

// ouverture fichier + extraction
void extractionFichier(unsigned char *c)
{
	int i =0;
    int caractere = 0;
    FILE* fichier = NULL;
    fichier = fopen("gulliver.txt", "r");

    if (fichier != NULL)
    {
			while (caractere != EOF)
			{
				caractere = fgetc(fichier);
				c[i] = caractere;
				i++;
			}
    }
    else
    {
        // On affiche un message d'erreur
        printf("Impossible d'ouvrir le fichier gulliver.txt");
    }

	fclose(fichier);
}

void ecritureRGB(unsigned char *fichier,unsigned char *r, unsigned char *g, unsigned char *b, int *m, int *f, long taille)
{
	// masque pour extraire les bits des valeurs RGB une par une  UTILISATION D'UN (ET) LOGIQUE
	// Ces masques serviront a extraire chaque bits d'un caractère
	unsigned char masq[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

	// On considére que le bits 2^0 du caractére ira en premier dans le bit de poids faible de R. L'ordre est le suivant R->G->B
	int bits=0;

	//printf("m : %d, f : %d \n", *m, *f);

	// Test qui permet d'arréter l'encodage quand tous les cartère on été encodé
	if(*f < taille)
	{
		/*-------------------------------------------*/
		if (*m == 7) // On a encoder les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		if ((masq[*m]&fichier[*f]) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
			bits = 1;
		else // si tous les bits sont a zero c'est que le bits rechercher vaut 0
			bits = 0;

		// On enregistre le bit 2^0 du caractère dans le nombre binaire R (RGB)
		*r = bitFaible(*r,bits);
		*m = *m + 1;

		/*---------------------------------------------*/
		if (*m == 7) // On a encoder les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		if ((masq[*m]&fichier[*f]) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
			bits = 1;
		else
			bits = 0;

		*g = bitFaible(*g,bits);
		*m = *m + 1;

		/*----------------------------------------*/
		if (*m == 7) // On a encoder les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		if ((masq[*m]&fichier[*f]) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
	            bits = 1;
	    else
	            bits = 0;

		*b = bitFaible(*b,bits);
		*m = *m + 1;
	}
}

long sizeFile(char *nom)
{
    FILE *fichier;
    long size;

    fichier=fopen(nom,"rb");

    if(fichier)
    {
            fseek(fichier, 0, SEEK_END);
            size=ftell(fichier);
            fclose (fichier);
    }
    return size;
}

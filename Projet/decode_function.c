#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode_function.h"

// Cette fonction remplace par 0 ou par 1 le bit de poids faible dans
// un nombre binaire donnée
// nb : nombre binaire , bits: 0 ou 1
void bitChange(unsigned char *tabCarac, int bits, int rang, int f)
{
	unsigned char un[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40}; // 0000 0001(2)
	unsigned char ze[7] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF}; // 1111 1110(2)

	if (bits)
		// met a 1 le bit de rang n
		tabCarac[f] = (tabCarac[f])|(un[rang]); // opérateur OU
	else
		// met a 0 le bit de rang n
		tabCarac[f] = (tabCarac[f])&(ze[rang]); // opérateur ET
}

void lectureRGB(unsigned char *tabCarac,unsigned char *r, unsigned char *g, unsigned char *b, int *m, int *f)
{
	// masque pour extraire les bits des valeurs RGB une par une  UTILISATION D'UN (ET) LOGIQUE
	// Ces masques serviront a extraire chaque bits d'un caractère
	unsigned char masq = 0x01;

	int bits=0;
	
	if (*f < 708064)
	{
		if (*m == 7) // On a extrait les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		
		if ((masq & *r) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
			bits = 1;
		else // si tous les bits sont a zero c'est que le bits rechercher vaut 0
			bits = 0;
	
		// On enregistre le bit 2^m dans le tableau tabCarac
		bitChange(tabCarac, bits, *m, *f);
		*m = *m + 1;
	//*******************************************************************************************************************
		if (*m == 7) // On a extrait les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		if ((masq & *g) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
			bits = 1;
		else
			bits = 0;
	
		// On enregistre le bit 2^m dans le tableau tabCarac
		bitChange(tabCarac, bits, *m, *f);
		*m = *m + 1;
	//*******************************************************************************************************************
		if (*m == 7) // On a extrait les 7 bits du caractère
		{
			*m = 0;
			*f = *f + 1; // on passe au caractère suivant
		}
		if ((masq & *b) != 0)// si le nombre vaut une valeure différente de zero c'est que le bits restant vaut 1
	            bits = 1;
	    else
	            bits = 0;
		
		// On enregistre le bit 2^m dans le tableau tabCarac
		bitChange(tabCarac, bits, *m, *f);
		*m = *m + 1;
	}
}

void writeFile(unsigned char *tabCarac, char *output)
{
	FILE *fichier = NULL;
    fichier=fopen(output,"w+");

    int i=0;

    if (fichier != NULL)
    {
		while(tabCarac[i] != '\0')
		{
			int carac = tabCarac[i];
			fputc(carac, fichier);
			i++;
		}
    }
	else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier output.txt"); //A MODIFIER L AFFICHAGE
    }

	fclose(fichier); // On ferme le fichier qui a été ouvert
}

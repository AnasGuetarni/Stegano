#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include "encode_function.h"

// This fonction replace the last significate bit by 1 or 0 for given binary number
// nb : binary number , bits: 0 or 1
unsigned char bitFaible(unsigned char nb, int bits)
{
	unsigned char resultat = 0x00;
	unsigned char un = 0x01; // 0000 0001(2)
	unsigned char ze = 0xFE; // 1111 1110(2)

	if (bits)
		// set the last significate bit at 1
		resultat = nb | un; // Logical operator OR
	else
		// set the last significate bit at 0
		resultat = nb & ze; // Logical operator AND

    return resultat;
}

// opening file + extraction
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
			// display error message
			printf("Can not open file.\n");
    }

	fclose(fichier);
}

void ecritureRGB(unsigned char *fichier,unsigned char *r, unsigned char *g, unsigned char *b, int *m, int *f, long taille)
{
	// mask to extract bits from RGB values ​​one by one (logical operateur AND)
	// These masks will be used to extract each bits of a character
	unsigned char masq[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

	// It is considered that bit 2 ^ 0 of the character will go first in the last significate bit of R. The order is R-> G-> B
	int bits=0;

	//printf("m : %d, f : %d \n", *m, *f);


	if(*f < taille)
		{
					/*--------------------- R  ---------------------*/
					if (*m == 7) // We encode the 7 bits of the character
					{
						*m = 0;
						*f = *f + 1; // we move on to the next character
					}
					if ((masq[*m]&fichier[*f]) != 0)// if the number != 0 it means that the remaining bit is 1
						bits = 1;
					else
						bits = 0;

					// We save the bit 2 ^ 0 of the character in the R binary number (RGB)
					*r = bitFaible(*r,bits);
					*m = *m + 1;

					/*----------------------  G  -----------------------*/
					if (*m == 7) // We encode the 7 bits of the character
					{
						*m = 0;
						*f = *f + 1; // we move on to the next character
					}
					if ((masq[*m]&fichier[*f]) != 0)// if the number != 0 it means that the remaining bit is 1
						bits = 1;
					else
						bits = 0;

					*g = bitFaible(*g,bits);
					*m = *m + 1;

					/*--------------------  B   --------------------*/
					if (*m == 7) //  We encode the 7 bits of the character
					{
						*m = 0;
						*f = *f + 1; //  we move on to the next character
					}
					if ((masq[*m]&fichier[*f]) != 0)// if the number != 0 it means that the remaining bit is 1
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
    long size = 0;

    fichier=fopen(nom,"rb");

    if(fichier)
    {
            fseek(fichier, 0, SEEK_END);
            size=ftell(fichier);
            fclose (fichier);
    }
		else
					printf("File not loaded\n");

    return size;
}

char *cut_text(char *nom, int min, int max){

  int caractere = 0;
	FILE *fichier;
	fichier=fopen(nom,"rb");
	char *c = malloc(sizeof(char) * (max-min + 1));

  if (nom != NULL)
  {
    for(int i = min; i<=max;i++){
      caractere = fgetc(fichier);
      if (caractere == EOF){
        printf("End file: EOF\n");
				exit(0);
      }
      c[i] = caractere;
    }
  }
  else
  {
    // display error message
    printf("Can not open file");
		exit(0);
		return NULL;
  }
  fclose(fichier);
  return c;
}

unsigned long fsize(char* file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(f);
    fclose(f);
    return len;
}

int add_pow_2(uint8_t *ptr, int exp)
{
	return (*ptr % 2) * pow(2, exp);
}

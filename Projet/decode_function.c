#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode_function.h"

// This function replaces a bit at rank n given by 0 or 1
// nb : binary number , bits: 0 or 1
void bitChange(unsigned char *tabCarac, int bits, int rang, int f)
{
	unsigned char un[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40}; // 0000 0001(2)
	unsigned char ze[7] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF}; // 1111 1110(2)

	if (bits)
		// Set the rank n bit to 1
		tabCarac[f] = (tabCarac[f])|(un[rang]); // Logical operator OR
	else
		// Set the rank n bit to 0
		tabCarac[f] = (tabCarac[f])&(ze[rang]); // Logical operator AND
}

void lectureRGB(unsigned char *tabCarac,unsigned char *r, unsigned char *g, unsigned char *b, int *m, int *f)
{
	// mask to extract bits from RGB values one by one (use AND operator)
	// These masks will be used to extract each bits of a character
	unsigned char masq = 0x01;

	int bits=0;

	if (*f < 708064)
	{
				// *********************************************    R    *************************************************************
		if (*m == 7) // We extracted the 7 bits of the character
		{
			*m = 0;
			*f = *f + 1; // Move to the next character
		}

		if ((masq & *r) != 0)// if the number != 0 it means that the remaining bit is 1
			bits = 1;
		else
			bits = 0;

		// We record the bit 2 ^ m in the tabCarac table
		bitChange(tabCarac, bits, *m, *f);
		*m = *m + 1;
	//*************************************************    G   ************************************************************
		if (*m == 7) // We extracted the 7 bits of the character
		{
			*m = 0;
			*f = *f + 1; // Move to the next character
		}
		if ((masq & *g) != 0)// if the number != 0 it means that the remaining bit is 1
			bits = 1;
		else
			bits = 0;

		// We record the bit 2 ^ m in the tabCarac table
		bitChange(tabCarac, bits, *m, *f);
		*m = *m + 1;
	//********************************************************    B    ***********************************************************
		if (*m == 7)  // We extracted the 7 bits of the character
		{
			*m = 0;
			*f = *f + 1; // Move to the next character
		}
		if ((masq & *b) != 0)// if the number != 0 it means that the remaining bit is 1
	            bits = 1;
	    else
	            bits = 0;

		//  We record the bit 2 ^ m in the tabCarac table
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
        // we get an error message
        printf("Can not open output.txt file");
    }

	fclose(fichier); // Close the file that was opened
}

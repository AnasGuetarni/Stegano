/*
 * @author Group12 - Guetarni Anas - Gay Melvin - Keraim Marwan
 * @date 18 Oct 2017
 * @brief Routines to read and write PPM files.
 *
 * Both binary (P6 type) and plain ASCII (P3 type) PPM file types are supported.
 * The PPM file format is described here: http://netpbm.sourceforge.net/doc/ppm.html
 * To convert a JPG image into a binary PPM file (P6) with ImageMagick:
 * convert image.jpg output.ppm
 * To convert a JPG image into a plain ASCII PPM file (P3) with ImageMagick:
 * convert -compress none image.jpg output.ppm
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "decode_function.h"
#include "ppm.h"

/**
 * Display the program's syntaxe.
 * @param argv program's command line arguments
 */

int convertDecimalToBinary(int n);

void usage(char **argv) {
	fprintf(stderr, "usage: %s [-ascii] input output\n"\
			"Where input is a PPM file and output is a text file and the optional argument\n"\
            "-ascii specifies to write a plain text PPM file.\n", basename(argv[0]));
	exit(EXIT_FAILURE);
}

/**
 * Program entry point.
 * @param argc command line argument count
 * @param argv program's command line arguments
 */
int main(int argc, char **argv) {
	char *input = NULL;
	char *output = NULL;
	enum PPM_TYPE type;
	int m = 0, f = 0;

	// Parse command line
	if (argc == 3) {
		type = PPM_BINARY;
		input = argv[1];
		output = argv[2];
	} else if (argc ==  4) {
		if (strcmp("-ascii", argv[1]) != 0) usage(argv);
		type = PPM_ASCII;
		input = argv[2];
		output = argv[3];
	} else {
		usage(argv);
	}

	// Load image
	img_t *img = load_ppm(input);
	if (!img) {
		fprintf(stderr, "Failed loading \"%s\"!\n", input); // if we can't load picture
		return EXIT_FAILURE;  // return error
	}

	// On affiche la taille de l'image
	/*printf("Height: %d\n", img->height);
	printf("Width: %d\n", img->width);*/

	unsigned char *caracExtrait = NULL;
	caracExtrait = malloc(708064*sizeof(unsigned char));
    if (caracExtrait == NULL) // If the allocation failed
    {
        exit(0);  // We stop the program
    }

	// We go through the entire image and apply the function ecritureRGB
	for (int j = 0; j < img->height; j++) { // We are looking at picture on the height
		for (int i = 0; i < img->width; i++) { // We are looking at picture on the width
			pixel_t *p = &img->pix[j][i]; // we place pixels values in a structure
			lectureRGB(caracExtrait, &p->r, &p->g, &p->b, &m, &f);
		}
	}

	writeFile(caracExtrait, output);

	free(caracExtrait); // the file is released
	free_img(img); // We free space allocated to the image

	return EXIT_SUCCESS; // return success
}

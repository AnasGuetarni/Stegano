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

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "encode_function.h"
#include "ppm.h"
#include <pthread.h>
#include <errno.h>
#include <math.h>

#define NUM_THREADS 20
#define BITS_PER_CHAR 7

/**
 * Display the program's syntaxe.
 * @param argv program's command line arguments
 */

int convertDecimalToBinary(int n);

void usage(char **argv) {
	fprintf(stderr, "usage: %s -- encode text_file input_image output_image thread_count where input_image and output_image are PPM files and thread_count the number of threads to use\n", basename(argv[0]));
	exit(EXIT_FAILURE);
}

void *thread(void *thread_id) {
        int id = *((int *) thread_id);
        printf("Hello from thread %d\n", id);
        return NULL;
}

int max_char_encode(img_t *img){
	return floor((img->height * img->width) * sizeof(pixel_t) / BITS_PER_CHAR);
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

int get_nb_char_img(img_t *img)
{
	uint8_t *test_rgb = &img->raw[0].r;
	int nb_char=0, count=0;

	for (int i = sizeof(int)*8-1; i >= 0; i--)
	{
		nb_char += add_pow_2(test_rgb+i, count);
		count++;
	}

	return nb_char;
}

int main(int argc, char **argv) {
	char *input, *output;
	char *nbT;
	char *fileInput;
	enum PPM_TYPE type;
	int m = 0, f = 0;

	typedef struct intervalle_st {
		int intervalMin[NUM_THREADS];
		int intervalMax[NUM_THREADS];
		img_t **img_out;
	} interval_t;

	typedef struct param_st {
		int thread_id;
		char *text_cut;
		img_t **img_out;
	} param_t;

	interval_t intervalleT;

	if (argc > 5)
	{
		printf("Le nombre d'argument est supérieur au nombre demandé\n");
		exit(EXIT_FAILURE);
	}

	type = PPM_BINARY;
	fileInput = argv[1];
	input = argv[2];
	output = argv[3];
	nbT = argv[4];

	*nbT = *((int *) nbT);

	img_t *img;
	img = load_ppm(input);
	int nb_char = get_nb_char_img(img);
	int max_char = max_char_encode(img);
	long size = sizeFile(fileInput);
	double value = size / NUM_THREADS;
	int reste = 0;
	int old = 7;
	bool end = false;

	pthread_t *threads = malloc(sizeof(pthread_t) * NUM_THREADS);//(pthread_t*)malloc(sizeof(pthread_t)); //malloc(sizeof(pthread_t) * nb_threads);
	param_t *threads_param = malloc(sizeof(param_t) * NUM_THREADS);//(param_t*)malloc(sizeof(param_t));//malloc(sizeof(param_t) * nb_threads);

	if (!img) {
		fprintf(stderr, "Failed loading \"%s\"!\n", input); // Si l'on arrive pas a charger l'image
		return EXIT_FAILURE; // On retourne une erreur
	}

	if (nb_char > max_char){
		printf("Fichier texte trop long pour l'image\n");
		exit(0);
	}

	printf("La valeur pour chaque thread est de: %f\n",value);

	// BOUCLE SUR LES threads
	// Création des intervalles
	for(int i=1;i<NUM_THREADS+1;i++){
		if(i==1)
			intervalleT.intervalMin[i] = value * (i-1);
		else
			intervalleT.intervalMin[i] = value * (i-1) + 1;

		intervalleT.intervalMax[i] = value * i;

		if (i==NUM_THREADS){
				reste = size - intervalleT.intervalMax[i];
				intervalleT.intervalMax[i] += reste;
		}

		printf("intervalMin: %i / intervalMax: %i\n",intervalleT.intervalMin[i],intervalleT.intervalMax[i]);
	}

	//int tab[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
	    //tab[i] = i;
	    int code = pthread_create(&threads[i], NULL, thread, &threads_param[NUM_THREADS]);

			if(pthread_join(threads[i], NULL) != 0) {
		     printf("pthread_join\n");
		     return EXIT_FAILURE;
		  }

	    if (code != 0) {
        fprintf(stderr, "pthread_create failed!\n");
        return EXIT_FAILURE;
	    }
    }
    pthread_exit(NULL);
    return EXIT_SUCCESS;

		// BOUCLE THREAD

		// for (int i = 0; i < nb_threads; i++)
		// {
		// 	int min = round(interval * i) + 0;
		// 	int max = round(interval * (i + 1)) - 1;
		// 	int char_in_interval = max + 1 - min;
		//
		// 	if (char_in_interval > 0)
		// 	{
		// 		threads_param[count_thread].limit = get_limits(min);
		// 		threads_param[count_thread].char_interval = char_in_interval;
		// 		threads_param[count_thread].img = img;
		//
		// 		int code = pthread_create(&threads[count_thread], NULL, thread, &threads_param[count_thread]);
		//
		// 		if (code != 0){
		//             fprintf(stderr, "pthread_create failed!\n");
		//             exit(0);
		// 		}
		//
		// 		count_thread++;
		// 	}
		// }


	unsigned char *fichier = NULL;
	fichier = malloc(size*sizeof(unsigned char));
    if (fichier == NULL) // Si l'allocation a échoué
    {
        exit(0); // On arrête immédiatement le programme
    }

		// On va alors extraire les caractères du fichier
    extractionFichier(fichier);

	// On parcours toute l'image et applique la fonction ecritureRGB
	for (int j = 0; j < img->height && end != true; j++) { // On parcours l'image sur la hauteur
		for (int i = 0; i < img->width && end != true; i++) { // On parcours l'image dans sa largeur
			pixel_t *p = &img->pix[j][i]; //On place dans une structure les valeurs des pixels
			if (m != old)
			{
				old = m;
				ecritureRGB(fichier, &p->r,&p->g,&p->b,&m,&f,size); // On écrit l'encodage sur le fichier en question
			}
			else
			{
				end = true;
			}
		}
	}

	free(fichier); // On libère le fichier

	// Write image
	if (!write_ppm(output, img, type)) {
		fprintf(stderr, "Failed writing \"%s\"!\n", output); // Si l'on arrive pas a écrire sur l'image
		free_img(img); // On libere l'espace alloué a l'image
		return EXIT_FAILURE; // On retourne une erreur
	}

	free_img(img); // On libere l'espace alloué à l'image
	return EXIT_SUCCESS; // On retourne un succès
}

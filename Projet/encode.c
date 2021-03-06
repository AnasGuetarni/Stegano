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

#define NUM_THREADS 10
#define BITS_PER_CHAR 7

/**
 * Display the program's syntaxe.
 * @param argv program's command line arguments
 */

 typedef struct intervalle_st {
	 int intervalMin[NUM_THREADS];
	 int intervalMax[NUM_THREADS];
 } interval_t;

 typedef struct param_st {
	 int intervalMin;
	 int intervalMax;
	 int thread_id;
	 char *text_cut;
	 int m,f;
	 unsigned char *fichier;
	 long size;
	 img_t *img;
 } param_t;

 param_t paramT;
 interval_t intervalleT;

int convertDecimalToBinary(int n);

void usage(char **argv) {
	fprintf(stderr, "usage: %s -- encode text_file input_image output_image thread_count where input_image and output_image are PPM files and thread_count the number of threads to use\n", basename(argv[0]));
	exit(EXIT_FAILURE);
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

int max_char_encode(img_t *img){
	return floor((img->height * img->width) * sizeof(pixel_t) / BITS_PER_CHAR);
}

void *thread(void *para) {
				param_t *p = ((param_t *)para);
				img_t *img = p->img;
				int id = p->thread_id;
				int intervalMin = p->intervalMin;
				int intervalMax = p->intervalMax;
				long size = p->size;
				unsigned char *fichier = p->fichier;
				int m = p->m;
				int f = p->f;

        /*printf("size : %ld\n", size);
				printf("fichier : %s\n", fichier);*/
				printf("id: %i\n", id);
				/*printf("Interval min: %d\n", intervalMin);
				printf("Interval max: %d\n", intervalMax);*/

				for (int j = intervalMin; j < intervalMax; j++) { // We are looking at picture on the height
					for (int i = intervalMin; i < intervalMax; i++) { // We are looking at picture on the width
						pixel_t *p = &img->pix[j][i]; // we place pixels values in a structure
							ecritureRGB(fichier, &p->r,&p->g,&p->b,&m,&f,size); // we encode the file
					}
				}

		free(fichier);

        printf("Hello from thread %d\n", id);
        return NULL;
}

int main(int argc, char **argv) {
	char *input, *output;
	char *nbT;
	char *fileInput;
	enum PPM_TYPE type;
	int m = 0, f = 0;

	if (argc > 5)
	{
				printf("The number of arguments is greater than the requested number\n");
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
	int count_thread=0;
  char *c = malloc(sizeof(char) * NUM_THREADS);

	if (!img) {
			fprintf(stderr, "Failed loading \"%s\"!\n", input); // if we can't load the picture
		return EXIT_FAILURE; // return error
	}

	if (nb_char > max_char){
			printf("Text file too long for the image\n");
		exit(0);
	}

		printf("The value for each thread  : %f\n",value);

    // loop on threads
  	//  creating intervals
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

	unsigned char *fichier = NULL;
	fichier = malloc(size*sizeof(unsigned char));
    if (fichier == NULL) // if allocation fail
    {
        exit(0); // we stop the program
    }

		// We will extract the characters from the file
    extractionFichier(fichier);

		// BOUCLE THREAD
		pthread_t *threads = malloc(sizeof(pthread_t) * NUM_THREADS);
		param_t *threads_param = malloc(sizeof(param_t) * NUM_THREADS);

    // We put into threads_param the different parameters of the structure
		threads_param[count_thread].fichier = fichier;
		threads_param[count_thread].size = size;
		threads_param[count_thread].img = img;

    printf("Avant la boucle\n");
    printf("c avant le cut: %s\n", c);

    // we loop on the variable "i" and we cut the text in a precise interval linking it to the id of the thread that we insert in "c"
    for (int i = 0; i < NUM_THREADS; i++) {
      c[i] = *(cut_text(fileInput,intervalleT.intervalMin[i],intervalleT.intervalMax[i]));
      printf("c:i = %c\n", c[i]);
    }

    // We loop on the variable "o" as long as it is less than the number of threads
		for (int o = 0; o < NUM_THREADS; o++)
		{
        printf("o = %i\n", o);
        printf("Dans l'entree de la boucle\n");

				threads_param[count_thread].thread_id = o;
				threads_param[count_thread].intervalMin = intervalleT.intervalMin[o];
				threads_param[count_thread].intervalMax = intervalleT.intervalMax[o];
        //threads_param[count_thread].text_cut = c[o];
				threads_param[count_thread].m = m;
				threads_param[count_thread].f = f;

				printf("Good initialisaiton\n");

				int code = pthread_create(&threads[count_thread], NULL, thread, &threads_param[count_thread]);

				printf("Code : %i\n", code);

				if (code != 0){
		            fprintf(stderr, "pthread_create failed!\n");
		            exit(0);
				}

				count_thread++;
		}

	for (int i = 0; i < count_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }

	free(fichier); // free file
  free(c);
  free(threads);
  free(threads_param);

  // Write image
	if (!write_ppm(output, img, type)) {
		fprintf(stderr, "Failed writing \"%s\"!\n", output); // if we can't write on in the picture
		free_img(img); // We free space allocated to the image
		return EXIT_FAILURE; // return error
	}

	free_img(img); // We free space allocated to the image
	return EXIT_SUCCESS; // Return success
}

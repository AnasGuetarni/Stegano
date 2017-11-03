#ifndef ENCODE_FONCTION
#define ENCODE_FONCTION

unsigned char bitFaible(unsigned char nb, int bits);
void extractionFichier(unsigned char *c);
long sizeFile(char *nom);
void ecritureRGB(unsigned char* fichier,unsigned char* r, unsigned char *g, unsigned char *b, int *m, int *f, long taille);
void *thread(void *thread_id);

#endif

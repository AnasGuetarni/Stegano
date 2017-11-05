#ifndef ENCODE_FONCTION
#define ENCODE_FONCTION

unsigned char bitFaible(unsigned char nb, int bits);
void extractionFichier(unsigned char *c);
long sizeFile(char *nom);
void ecritureRGB(unsigned char* fichier,unsigned char* r, unsigned char *g, unsigned char *b, int *m, int *f, long taille);
void *thread(void *thread_id);
char *cut_text(char *nom, int min, int max);
unsigned long fsize(char* file);
int add_pow_2(uint8_t *ptr, int exp);

#endif

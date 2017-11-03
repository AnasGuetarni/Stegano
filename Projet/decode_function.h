#ifndef DECODE_FONCTION
#define DECODE_FONCTION

void bitChange(unsigned char *tabCarac, int bits, int rang, int f);
void lectureRGB(unsigned char *tabCarac,unsigned char *r, unsigned char *g, unsigned char *b, int *m, int *f);
void writeFile(unsigned char *tabCarac, char *output);

#endif

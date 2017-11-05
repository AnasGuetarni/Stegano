GCC=gcc -g -Wall -Wextra -std=c99 -Werror
LIBS= -lpthread -lm

all: encode decode

encode: encode.o encode_function.o ppm.o
	$(GCC) $^ -o $@ $(LIBS)

encode.o: encode.c
	$(GCC) $< -c $(LIBS)

encode_function.o: encode_function.c encode_function.h
	$(GCC) $< -c $(LIBS)

decode: decode.o decode_function.o ppm.o
	$(GCC) $^ -o $@ $(LIBS)

decode.o: decode.c
	$(GCC) $< -c $(LIBS)

decode_function.o: decode_function.c decode_function.h
	$(GCC) $< -c $(LIBS)

pm.o: ppm.c ppm.h
		$(GCC) $< -c $(LIBS)

run: encode
	./encode gulliver.txt azores.ppm azores-o.ppm 50

clean:
	rm -f *.o encode
	rm -f *.o decode

GCC=gcc -g -Wall -Wextra -std=c99 -Werror
LIBS= -lpthread -lm

all: encode decode

encode: encode.o encode_function.o ppm.o
	$(GCC) $^ -o $@

encode.o: encode.c
	$(GCC) $< -c

encode_function.o: encode_function.c encode_function.h
	$(GCC) $< -c

decode: decode.o decode_function.o ppm.o
	$(GCC) $^ -o $@

decode.o: decode.c
	$(GCC) $< -c

decode_function.o: decode_function.c decode_function.h
	$(GCC) $< -c

pm.o: ppm.c ppm.h
		$(GCC) $< -c 

run: encode
	./encode gulliver.txt azores.ppm azores-o.ppm 50

clean:
	rm -f *.o encode
	rm -f *.o decode

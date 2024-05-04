all:
	gcc -g -o comp main.c htab.h htab.c
	./comp

adresse:
	gcc -g -o comp main.c htab.h htab.c -fsanitize=leak
	./comp

test:
	gcc -std=c99 -g -o test main.c htab.h htab.c

clean:
	rm -f comp

all: unbracify
	rm -f bootstrap
	rm -f unbracify_braced.c

unbracify: unbracify_braced.c
	$(CC) -o unbracify unbracify_braced.c

unbracify_braced.c: unbracify.c bootstrap
	./bootstrap unbracify.c > unbracify_braced.c

bootstrap: bootstrap.c
	$(CC) -o bootstrap bootstrap.c

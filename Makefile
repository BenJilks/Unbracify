all: unbracify
	rm -f bootstrap
	rm -f unbracify_stage0.c
	rm -f unbracify_stage0
	rm -f unbracify_stage1.c

unbracify: unbracify_stage1.c
	$(CC) -o unbracify unbracify_stage1.c

unbracify_stage1.c: unbracify_stage0
	./unbracify_stage0 unbracify.c > unbracify_stage1.c

unbracify_stage0: unbracify_stage0.c
	$(CC) -o unbracify_stage0 unbracify_stage0.c

unbracify_stage0.c: unbracify.c bootstrap
	./bootstrap unbracify.c > unbracify_stage0.c

bootstrap: bootstrap.c
	$(CC) -o bootstrap bootstrap.c

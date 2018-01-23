CFLAGS=-Wall -std=c99
BINDIR=/usr/local/bin
MANDIR=/usr/local/share/man/man1

codegroups : main.c words.h  words.c
	gcc $(CFLAGS) -o randwords main.c words.c

install : randwords
	cp randwords $(BINDIR)/
	mkdir -p $(MANDIR)
	cp randwords.1 $(MANDIR)/

clean :
	rm -f randwords *.o

distclean :
	rm -f randwords *.o $(BINDIR)/randwords $(MANDIR)/randwords.1


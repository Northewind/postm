OUTF=postm
BIND=bin
SRCD=src

all: compile
	$(BIND)/$(OUTF)

compile:
	gcc -Wall -pedantic -std=c99 $(SRCD)/*.c -o $(BIND)/$(OUTF)

clean:
	rm $(BIND)/$(OUTF)
	cp -f $(BIND)/hd.txt.bak $(BIND)/hd.txt

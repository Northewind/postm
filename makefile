OUTF=postm
BIND=bin
SRCD=src

all: build
	$(BIND)/$(OUTF)

build:
	gcc -Wall -pedantic -std=c89 $(SRCD)/*.c -o $(BIND)/$(OUTF)
	cp -f $(BIND)/hd.txt.bak $(BIND)/hd.txt

clean:
	rm $(BIND)/$(OUTF) $(BIND)/hd.txt

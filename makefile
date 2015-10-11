OUTF=postm
BIND=bin
SRCD=src

all: build
	cd $(BIND) \
	./$(OUTF) rom.txt hd.txt

build:
	gcc -Wall -pedantic -std=c89 $(SRCD)/*.c -o $(BIND)/$(OUTF)

clean:
	rm $(BIND)/$(OUTF)

OUTF=postm
BIND=bin
SRCD=src
TSTD=tests

all: build

build:
	gcc -Wall -pedantic -std=c89 $(SRCD)/*.c -o $(BIND)/$(OUTF)

clean:
	rm $(BIND)/$(OUTF)

t1s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom1s1z.txt hd1.txt

t1z: t1s

t2s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom2s.txt hd2s.txt

t2z: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom2z.txt hd2z.txt

t3s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom3s3z.txt hd3s.txt

t3z: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom3s3z.txt hd3z.txt

t4s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom4s.txt hd4s.txt

t4z: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom4z.txt hd4z.txt

t5s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom5s5z.txt hd5.txt

t5z: t5s

t6s: build
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom6s.txt hd6.txt

t6z: build 
	cd $(TSTD); \
	../$(BIND)/$(OUTF) rom6z.txt hd6.txt


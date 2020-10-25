all: head.o entry.o strap.o
.PHONY: all

head.o: head.S entry.o
	$(GCC) $(CFLAG) -c head.S -o $@

entry.o: entry.S strap.o
	$(GCC) $(CFLAG) -c entry.S -o $@ 

strap.o: strap.c 
	$(GCC) $(CFLAG) -c strap.c -o $@ 
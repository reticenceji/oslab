all: head.o entry.o
.PHONY: all

head.o: head.S entry.o
	$(GCC) $(CFLAG) -c head.S -o $@

entry.o: entry.S
	$(GCC) $(CFLAG) -c $^ -o $@ 

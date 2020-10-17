all: head.o entry.o
.PHONY: all

head.o: head.S
	$(GCC) $(CFLAG) -c $^ -o $@

entry.o: entry.S
	$(GCC) $(CFLAG) -c $^ -o $@ 

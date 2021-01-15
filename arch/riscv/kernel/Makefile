VPATH = ../../../lib
all: head.o entry.o strap.o sched.o vm.o 
.PHONY: all

head.o: head.S entry.o vm.o
	$(GCC) $(CFLAG) -c head.S -o $@

entry.o: entry.S strap.o
	$(GCC) $(CFLAG) -c entry.S -o $@ 

strap.o: strap.c syscall.o
	$(GCC) $(CFLAG) -c strap.c -o $@

sched.o: sched.c rand.o vm.o
	$(GCC) $(CFLAG) -c sched.c -o $@

vm.o: vm.c
	$(GCC) $(CFLAG) -c vm.c -o $@

syscall.o: syscall.c
	$(GCC) $(CFLAG) -c syscall.c -o $@

slub.o: slub.c buddy.o
	$(GCC) $(CFLAG) -c slub.c -o $@

buddy.o: buddy.c 
	$(GCC) $(CFLAG) -c slub.c -o $@

mm.o: mm.c slub.o vma.o vm.o
	$(GCC) $(CFLAG) -c mm.c -o $@

vma.o: vma.c 
	$(GCC) $(CFLAG) -c vma.c -o $@

fault.o: fault.c vma.o
	$(GCC) $(CFLAG) -c fault.c -o $@

fork.o: fork.c slub.o
	$(GCC) $(CFLAG) -c fork.c -o $@
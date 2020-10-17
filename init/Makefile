all: main.o test.o
.PHONY: all
#通过将all伪目标作为默认目标，使之后的main.o和test.o可以正常生成，同时不会生成多余的可执行文件

main.o: main.c
	$(GCC) -c $(CFLAG) -c $^ -o $@  
test.o: test.c
	$(GCC) -c $(CFLAG) -c $^ -o $@  

#%.o: %.c
#$^表示所有的prerequisites;$@表示target

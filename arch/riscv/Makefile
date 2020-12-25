VPATH = kernel:../../init:../../lib
#如果没有指明这个变量，make只会在当前的目录中去找寻依赖文件和目标文件。如果定义了这个变量，那么，make就会在当前目录找不到的情况下，到所指定的目录中去找寻文件了。

all: subdir System.map Image vmlinux
.PHONY: all

subdir:
	$(MAKE) -C kernel
System.map: vmlinux
	nm ../../$^ >../../$@
Image: vmlinux
	$(OBJCOPY) -O binary ../../vmlinux boot/Image --strip-all
vmlinux: put.o rand.o main.o test.o kernel/head.o kernel/entry.o kernel/sched.o kernel/strap.o kernel/vm.o kernel/handler.o kernel/vmlinux.lds
	$(LD) $(word 1,$^) $(word 2,$^) $(word 3,$^) $(word 4,$^) \
		kernel/head.o kernel/entry.o kernel/sched.o kernel/strap.o \
		kernel/vm.o kernel/handler.o -T kernel/vmlinux.lds -o ../../$@


#ld命令 是GNU的连接器，将目标文件连接为可执行程序。
#$(word n,$^)可以获得prereq的第n个依赖文件
#OBJCOPY -O指定生成文件的格式，--strip-all删除符号表
#System.map是内核符号表，nm就是列出符号表
#vmlinux is a ELF format based file which is nothing but the uncompressed version of kernel image which can be used for debugging. 
#可以认为Image是纯粹的vmlinux，删除了调试信息

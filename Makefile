export
CROSS_= riscv64-unknown-elf-
AR=${CROSS_}ar
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy
ISA ?= rv64imafd
ABI ?= lp64
INCLUDE = -I ../include
CF = -O3 -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
CFLAG = ${CF} ${INCLUDE} -DPRIORITY

#CFLAGS and CXXFLAGS are either the name of environment variables or of Makefile variables that can be set to specify additional switches to be passed to a compiler in the process of building computer software.
#ar命令可以用来创建、修改库，也可以从库中提出单个模块。库是一单独的文件，里面包含了按照特定的结构组织起来的其它的一些文件（称做此库文件的member）。原始文件的内容、模式、时间戳、属主、组等属性都保留在库文件中。
#INCLUDE是告诉gcc在哪里寻找头文件，

subsystem:
	$(MAKE) -C lib
	$(MAKE) -C init
	$(MAKE) -C arch/riscv
#$(MAKE) -C subdir 等价于 cd subdir && $(MAKE)


.PHONY: clean run debug
clean:
	find -name "*.o"  | xargs rm -fv && rm -v vmlinux System.map arch/riscv/boot/Image && find -name ".*.swp" | xargs rm -fv
run:
	$(MAKE) -s && qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -initrd hello.bin
debug:
	$(MAKE) -s && qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -S -s -initrd hello.bin
#force and recursive remove all object files.
#.PHONY表示伪目标，伪目标标记后的命令，不会和目录下文件重复。

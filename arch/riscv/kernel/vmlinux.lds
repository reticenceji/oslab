OUTPUT_ARCH(riscv)
ENTRY(_start)
BASE_ADDR = 0x80000000;
SECTIONS
{
  . = BASE_ADDR;

  .text : {
	  *(.text.init)
	  *(.text.entry)
      *(.text)
  }

  .rodata : {
      *(.rodata)
  }

  .data : {
      *(.data)
  }

  _bss_start = .;
  .bss : {
      *(.bss)
  }
  _bss_end = .;

  . += 0x8000;
  stack_top = .;
  _end = .;
}

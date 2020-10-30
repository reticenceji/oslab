BASE_ADDR = 0x80000000;
SECTIONS
{
  . = BASE_ADDR;

  .text : {
	  *(.text.init)
	  *(.text.entry)
      *(.text)
      *(.text.*)
  }

  .rodata : {
      *(.rodata)
      *(.rodata.*)
  }

  .data : {
      *(.data)
      *(.data.*)
  }

  _bss_start = .;
  .bss : {
      *(.sbss)
      *(.sbss.*)
      *(.bss)
      *(.bss.*)
  }
  _bss_end = .;

  . += 0x8000;
  stack_top = .;
  _end = .;
}

/*
 * Minimal linker script for the STM32WL55JC Cortex-M4 core.
 *
 * UNVERIFIED: memory sizes are placeholders taken as 256 KB flash and
 * 64 KB SRAM (SRAM1 + SRAM2). Check them against the STM32WL55JC datasheet
 * and reference manual RM0453 before flashing a board. On the dual-core
 * WL55 the Cortex-M0+ and secure areas may need flash and RAM split off.
 */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  .ARM.exidx :
  {
    *(.ARM.exidx*)
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /* Fail the link if less than 2 KB is left for the stack. */
  ._stack_check (NOLOAD) :
  {
    . = ALIGN(8);
    . = . + 0x800;
  } > RAM
}

/*
 * My attempt at linker script for STM32F071x8 / STM32F071xB Devices
 * Modified from https://electronics.stackexchange.com/questions/224618/minimal-code-required-to-startup-a-stm32f4
*/


TARGET (elf32-littlearm)
OUTPUT_FORMAT (elf32-littlearm)


_Min_Heap_Size = 0x0;        /* required amount of heap   (0 Bytes) */
_Min_Stack_Size = 0x400;     /* required amount of stack (1024 Bytes) */


ENTRY(main) /* For debugger */


/*
 * STM32F071x8 -> 64k Flash
 * STM32F071xB -> 128k Flash
*/

MEMORY
{
    FLASH   (rx)    : ORIGIN = 0x08000000, LENGTH = 64k
    RAM     (rwx)   : ORIGIN = 0x20000000, LENGTH = 16k
}

__stack = ORIGIN(RAM) + LENGTH(RAM);


SECTIONS {

    . = ORIGIN(FLASH);


    .sp_init : ALIGN(4) {
        KEEP(*(.sp_init))
    } > FLASH


    .vectors : ALIGN(4) {
        FILL(0x00)
        __vectors_start__ = ABSOLUTE(.);
        KEEP(*(.vectors))
        . = ALIGN(4);
        __vectors_end__ = ABSOLUTE(.);
    } > FLASH


    .text : ALIGN(4) {
	FILL(0x00)
	__text_start__ = ABSOLUTE(.);
        *(.entry_point)
        *(.text)
        *(.text.*)
        *(.glue*)
        *(.gcc*)

	. = ALIGN(4);
	__text_end__ = ABSOLUTE(.);
    } > FLASH


	.init_array : ALIGN(4) {
	FILL(0x00)
	__init_array_start__ = ABSOLUTE(.);
		KEEP(*(.init_array))
		KEEP(*(.init_array.*))
	. = ALIGN(4);
	__init_array_end__ = ABSOLUTE(.);
	} > FLASH
	

    .rodata : ALIGN(4) {
	FILL(0x00)
	__rodata_start__ = ABSOLUTE(.);
        *(.rodata)
        *(.rodata.*)
	. = ALIGN(4);
	__rodata_end__ = ABSOLUTE(.);
    } > FLASH 



    .data : ALIGN(4) {
        FILL(0x00)
        __data_ROM_start__ = LOADADDR(.data);
	__data_start__ = ABSOLUTE(.);
        *(.data)
        *(.data.*)
        . = ALIGN(4);
	__data_end__ = ABSOLUTE(.);
    } > RAM AT > FLASH




    .bss (NOLOAD) : ALIGN(4) {
	FILL(0x00)
        __bss_start__ = ABSOLUTE(.);
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = ABSOLUTE(.);
    } > RAM


	/DISCARD/ :
	{
		*(.ARM.exidx)
	}


    /*
     * Should throw an error if . moves backwards, i.e. if stack is too small.
    */
    . = __stack;

}


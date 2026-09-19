ENTRY(_kernel_main)

SECTIONS
{
    . = 0x7e00;

    .text :
    {
        *(.text)
    }

    .rdata :
    {
        *(.rdata)
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.pdata)
        *(.xdata)
        *(.eh_frame)
        *(.reloc)
        *(.note*)
        *(.comment)
    }
}

/* Linker script for x86-64 PC hardware kernels

   Cribbed from: http://wiki.osdev.org/64-bit_Higher_Half_Kernel_with_GRUB_2 */

ENTRY(entry)

PHY_BASE = 0x100000;
VIRT_BASE = 0xFFFFC00000000000;

SECTIONS
{
	. = PHY_BASE;
        _start = .;

    kernel_phy = .;
	.boot ALIGN(0x1000) : AT(kernel_phy)
	{
		*(.mboot)
		*(.boot)
		*(.boot.data)
		. = ALIGN(0x1000);
	}
    kernel_phy_end = kernel_phy + SIZEOF(.boot);

	. = VIRT_BASE;

	kernel_virt = kernel_phy_end;
	kernel_text_phy = kernel_phy_end;
	.text ALIGN(0x1000) : AT(kernel_text_phy)
	{
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t*)
		. = ALIGN(0x1000);
	}
	kernel_text_phy_end = kernel_text_phy + SIZEOF(.text);
	
	kernel_data_phy = kernel_text_phy_end;
	.data ALIGN(0x1000) : AT(kernel_data_phy)
	{
		*(.data)
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.d*)
		. = ALIGN(0x1000);
	}
	kernel_data_phy_end = kernel_data_phy + SIZEOF(.data);
	
	kernel_bss = kernel_data_phy_end;
	.bss ALIGN(0x1000) : AT(kernel_bss)
	{
		*(.bss)
	}
	kernel_bss_end = kernel_bss + SIZEOF(.bss);
    
    kernel_virt_end = kernel_bss_end ;
    _end = kernel_virt_end;

	/DISCARD/ :
	{
		*(.init_array)
		*(.comment)
		*(.eh_frame)
	}
}

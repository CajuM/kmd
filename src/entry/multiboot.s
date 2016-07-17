.set MULTIBOOT_HEADER_MAGIC, 0xE85250D6
.set MULTIBOOT_HEADER_ARCHITECTURE, 0
.set MULTIBOOT_HEADER_LENGTH, (mboot_end - mboot)
.set MULTIBOOT_HEADER_CHECKSUM, (-(MULTIBOOT_HEADER_LENGTH + MULTIBOOT_HEADER_ARCHITECTURE + MULTIBOOT_HEADER_MAGIC))

.section .mboot

.global mboot


.align 8
mboot:
mboot_magic:
    .long MULTIBOOT_HEADER_MAGIC
    .long MULTIBOOT_HEADER_ARCHITECTURE
    .long MULTIBOOT_HEADER_LENGTH
    .long MULTIBOOT_HEADER_CHECKSUM
mboot_magic_end:
.align 8
mboot_tag_mbi:
	.short 1
	.short 0
	.long mboot_tag_mbi_end - mboot_tag_mbi
	.long 4
	.long 1
	.long 3
	.long 6
	.long 2
mboot_tag_mbi_end:
.align 8
mboot_tag_end:
	.short 0
	.short 0
	.long 8
mboot_tag_end_end:
mboot_end:



.code32

.section .boot

.global entry
.global stack

.extern build_paging_table, tmp_pml4t


.align 8 * 512
stack_end:
    .rept 4 * 8 * 512
    .byte 0x00
    .endr
stack:

tmp_pml4:
    .rept 8 * 512
    .byte 0x00
    .endr
tmp_pdp:
    .rept 8 * 512
    .byte 0x00
    .endr
tmp_pd:
    .rept 8 * 512
    .byte 0x00
    .endr

entry:
	cli

	movl %ebx, mboot_info
	movl %eax, mboot_magic

	movl $stack, %esp
	movl %esp, %ebp

	movl $tmp_pdp, %edx
	orl  $0xB, %edx
	movl %edx, tmp_pml4

	movl $tmp_pd, %edx
	orl  $0xB, %edx
	movl %edx, tmp_pdp

	movl $tmp_pd, %edx
	movl $0x0000008B, %ecx
tmp_pd_loop:
	movl %ecx, (%edx)
	addl $0x00200000, %ecx
	addl $8, %edx
	cmpl $0x40000000, %ecx
	jl tmp_pd_loop

	movl $tmp_pml4, %eax    # set paging directory
	movl %eax, %cr3


	movl %cr4, %eax         # enable PAE paging
	orl  $(1 << 5), %eax
	movl  %eax, %cr4

	movl $0xC0000080, %ecx   # write efer msr long mode bit
	rdmsr
	orl $(1 << 8), %eax
	wrmsr


	movl %cr0 , %eax          # enable paging
	andl $0x9fffffff, %eax
	orl  $(1 << 31), %eax
	mov %eax, %cr0


	lgdt GDT64.Pointer         # Load the 64-bit global descriptor table.

	jmpl $0x8, $entry64


halt:
	jmp halt

/*
I'm a copy cat I confess, on the other hand, there was not another way to do this,
other than giving rwx to the whole address space and using a linear address model
PS I'm using paging for memmory protection
i*/
GDT64:                           # Global Descriptor Table (64-bit).
    .set GDT64.Null, . - GDT64         # The null descriptor.
    .word 0                         # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0                         # Access.
    .byte 0                         # Granularity.
    .byte 0                         # Base (high).
    .set GDT64.Code, . - GDT64         # The code descriptor.
    .word 0                         # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0b10011010                 # Access (exec/read)
    .byte 0b00100000                 # Granularity.
    .byte 0                         # Base (high).
    .set GDT64.Data, . - GDT64         # The code descriptor.
    .word 0                         # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0b10010010                 # Access (read/write).
    .byte 0b00000000                 # Granularity.
    .byte 0                         # Base (high).
GDT64.Pointer:                    # The GDT-pointer.
    .word . - GDT64 - 1             # Limit.
    .quad GDT64                   # Base.


.code64

.extern kMain

mboot_info: .quad 0
mboot_magic: .quad 0

entry64:
    cli                            # Clear the interrupt flag.
    movq $GDT64.Data, %rax            # Set the A-register to the data descriptor.
    movq %rax, %ds                    # Set the data segment to the A-register.
    movq %rax, %es                    # Set the extra segment to the A-register.
    movq %rax, %fs                    # Set the F-segment to the A-register.
    movq %rax, %gs                    # Set the G-segment to the A-register.
    movq %rax, %ss

	movq $stack, %rsp
	movq %rsp, %rbp

	movq mboot_info, %rdi
	movq $build_paging_table, %rax
	call *%rax
b0:
	movq mboot_info, %rdi
	movq mboot_magic, %rsi
	movabs $kMain, %rax
	call *%rax


halt64:
	jmp halt64

BITS 64

section .text

global jump_to_kernel

jump_to_kernel:
         push rbp
         mov rbp, rsp

         mov cr3, rdi
         mov rsp, 0

         mov rax, 0xFFFF800000000000

         jmp rax

         loop:
            jmp loop

         cli
         hlt

section .note.GNU-stack noalloc noexec nowrite progbits
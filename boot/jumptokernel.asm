BITS 64

section .text

global jump_to_kernel

jump_to_kernel:
         push rbp
         mov rbp, rsp

         mov rax, 5

         loop:
            jmp loop

         cli
         hlt

section .note.GNU-stack noalloc noexec nowrite progbits
BITS 64
DEFAULT REL

global gdt_init

gdt_init:
    push rbp
    mov rbp, rsp

    mov rax, GDT64.Pointer

    lgdt [rax]         ; Load the 64-bit global descriptor table.
    mov ax, GDT64.KernelData
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    mov rax, .csload
    push 0x08
    push rax
    retfq

    .csload:

    mov rsp, rbp
    pop rbp
    ret

section .rodata

GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .KernelCode: equ $ - GDT64   ; The kernel code descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read). (0x9A)
    db 10101111b                 ; Granularity, 64 bits flag, limit 19:16.
    db 0                         ; Base (high).
    .KernelData: equ $ - GDT64   ; The kernel data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write). (0x92)
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
 	.UserCode: equ $ - GDT64     ; The user code descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11111010b                 ; Access (exec/read). (0xFA)
    db 10101111b                 ; Granularity, 64 bits flag, limit 19:16
    db 0                         ; Base (high)
    .UserData: equ $ - GDT64     ; The user data descriptor
    dw 0                         ; Limit (low)
    dw 0                         ; Base (low)
    db 0                         ; Base (middle)
    db 11110010b                 ; Access (read/write) (0xF2)
    db 00000000b                 ; Granularity
    db 0                         ; Base (high)
	.tss1: equ $ - GDT64
	dq 0
	.tss2: equ $ - GDT64
	dq 0
    .Pointer:                    ; GDT pointer
    dw $ - GDT64 - 1             ; Limit
    dq GDT64                     ; Base

    section .note.GNU-stack noalloc noexec nowrite progbits
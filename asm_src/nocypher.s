BITS 64;
section .text
global _nocypher

_nocypher:
    push rdi
    push rsi
    push rdx
    push rbx
    push rcx
    jmp short msg

msg:
    call print
    db '....WOODY....',10,0

print:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    xor rax, rax
    mov dil, 1
    pop rsi
    mov dl, 14
    mov al, 1
    syscall
    mov rcx, 0xBBBBBBBBBBBBBBBB
    lea rbx, [rel _nocypher]
    pop rcx
    pop rbx
    pop rdx
    pop rsi
    pop rdi
    jmp near _nocypher
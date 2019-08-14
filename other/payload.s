BITS 64;
section .text
global _start

_start:
    jmp short msg

print:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    xor rax, rax
    mov dil, 1
    pop rsi
    mov dl, 7
    mov al, 1
    syscall
    jmp lol

msg:
    call print
    db 'coucou',10,0
lol:
    jmp fart
fart:
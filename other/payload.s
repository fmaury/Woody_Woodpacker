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
    mov dl, 14 
    mov al, 1
    syscall
    jmp lol

msg:
    call print
    db '....WOODY....',10,0
lol:
    jmp fart
fart:
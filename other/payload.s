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
    mov rdi, 0x555555555135
;    mov rsi, 369
;loop:
;    mov rax, [rdi]
;    xor rax, 0
;    mov [rdi], rax
;    inc rdi
;    dec rsi
;    cmp rsi, 0
;    jne loop
    jmp msug
poil:
    pop rsi
    mov dl, 14
    mov al, 1
    syscall
    jmp lol

msg:
    call print
    db '....WOODU....',10,0

msug:
    call poil
    db '....WOODY....',10,0

lol:
    jmp fart
fart:
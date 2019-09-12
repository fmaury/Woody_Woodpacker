BITS 64;
section .text
global params
global putwoody

putwoody:
    jmp msg

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
    lea rbx, [rel txtaddr]
    mov rcx, txtlen
    jmp sectxt

msg:
    call print
    db '....WOODY....',10,0

sectxt:
    mov rax, entrypgm
    jmp 0x0000000000001135

params:
txtaddr: dq 0xAABBCCDDAABBCCDD
txtlen: dq 0xFFFFFFFFFFFFFFFF
entrypgm: dq 0xAABBCCDDAABBCCDD
BITS 64;
section .text
global xor42

xor42:
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
    mov rbx, 0xAAAAAAAAAAAAAAAA
    mov rcx, 0xBBBBBBBBBBBBBBBB
    jmp sectxt

msg:
    call print
    db '....WOODY....',10,0

sectxt:
    jmp near xor42
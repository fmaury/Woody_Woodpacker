BITS 64;
section .text
global _rot13

_rot13:
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
    lea rbx, [rel _rot13]
loop:
    xor rax, rax
    mov al, byte [rbx]
    sub al, 13
    mov byte[rbx], al
    inc rbx
    dec rcx
    cmp rcx, 0
    jne loop
    pop rcx
    pop rbx
    pop rdx
    pop rsi
    pop rdi
    jmp near _rot13
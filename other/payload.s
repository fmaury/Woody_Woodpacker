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
    lea rbx, [rel $$] ; 59th byte => -57
    mov rcx, 0x41414141
    mov rdi, 1
    mov rsi, rbx
    mov rdx, 14
    mov rax, 1
    push rbx
    push rcx
    syscall
    pop rcx
    pop rbx
loop:
    ;call msug
    xor byte [rbx], 42
    inc rbx
    dec rcx
    cmp rcx, 0
    jne loop
    jmp lol

msg:
    call print
    db '....WOODY....',10,0

lol:
    jmp ui
ui:
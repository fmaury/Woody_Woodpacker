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
    lea rdi, [rel $$] ; 27th byte => -31
    mov rsi, 369 ; 34th byte => -38
    mov rdx, 7
    mov rax, 10
    syscall
    cmp rax, -1
    je exit
    lea rbx, [rel $$] ; 59th byte => -57
    mov rdi, 1
    mov rsi, rbx
    mov rdx, 14
    mov rax, 1
    syscall
    mov rcx, 369
loop:
    ;call msug
    add byte [rbx], 0
    inc rbx
    dec rcx
    ;cmp rcx, 0
    ;jne loop
    jmp lol
poil:
    pop rsi
    mov rdx, 14
    mov rax, 1
    mov rdi, 1
    syscall
    ret

msg:
    call print
    db '....WOODY....',10,0


msug:
    call poil
    db '....WOODU....',10,0
    ret

exit:
    mov rdi, 1
    mov rax, 60
    syscall

write:

lol:
    jmp $
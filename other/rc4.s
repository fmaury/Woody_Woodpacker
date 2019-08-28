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
    lea rdi, [rel $$] ; 59th byte => -57
    mov rsi, 0x41414141
    jmp key
getkey:
    pop rdx 
    call rc4
    jmp lol

msg:
    call print
    db '....WOODY....',10,0

key:
    call  getkey
    db 'Key', 0

rc4:
push   rbx
mov    r8,rdx
xor    eax,eax
sub    rsp,0x88
lea    rcx,[rsp-0x78]
nop    WORD  [rax+rax*1+0x0]
l24:
mov    BYTE  [rcx+rax*1],al
add    rax,0x1
cmp    rax,0x100
jne    l24
movsx  edx,BYTE  [r8]
xor    esi,esi
test   dl,dl
je     l67
nop    DWORD  [rax+0x0]
l56:
add    rsi,0x1
cmp    BYTE  [r8+rsi*1],0x0
jne    l56
l67:
mov    r9d,0x1
lea    r10,[rcx+0xff]
xor    ebx,ebx
sub    r9,rcx
jmp    l114
nop    WORD  [rax+rax*1+0x0]
l96:
lea    rax,[r9+rcx*1]
xor    edx,edx
add    rcx,0x1
div    rsi
movsx  edx,BYTE  [r8+rdx*1]
l114:
movzx  eax,BYTE  [rcx]
mov    r11d,eax
add    eax,ebx
add    eax,edx
cdq    
shr    edx,0x18
add    eax,edx
movzx  eax,al
sub    eax,edx
mov    ebx,eax
cdqe   
movzx  edx,BYTE  [rsp+rax*1-0x78]
mov    BYTE  [rcx],dl
mov    BYTE  [rsp+rax*1-0x78],r11b
cmp    r10,rcx
jne    l96
movzx  r9d,BYTE  [rdi]
test   r9b,r9b
je     l279
xor    eax,eax
nop    WORD  [rax+rax*1+0x0]
l176:
add    rax,0x1
cmp    BYTE  [rdi+rax*1],0x0
jne    l176
mov    rsi,rdi
lea    r8,[rax+rdi*1]
xor    ecx,ecx
xor    edx,edx
jmp    l212
nop    WORD  [rax+rax*1+0x0]
l208:
movzx  r9d,BYTE  [rsi]
l212:
add    edx,0x1
add    rsi,0x1
movzx  r10d,dl
movzx  eax,BYTE  [rsp+r10*1-0x78]
mov    rdx,r10
add    ecx,eax
movzx  r11d,cl
movzx  ebx,BYTE  [rsp+r11*1-0x78]
mov    rcx,r11
mov    BYTE  [rsp+r10*1-0x78],bl
mov    BYTE  [rsp+r11*1-0x78],al
add    al,BYTE  [rsp+r10*1-0x78]
movzx  eax,al
xor    r9b,BYTE  [rsp+rax*1-0x78]
mov    BYTE  [rsi-0x1],r9b
cmp    r8,rsi
jne    l208
l279:
add    rsp,0x88
mov    rax,rdi
pop    rbx
ret    

lol:
    jmp ui
ui:


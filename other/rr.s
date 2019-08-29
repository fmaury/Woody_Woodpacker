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
   push   rbp
   mov    rbp,rsp
   sub    rsp,0xc0
   mov    QWORD  [rbp-0x128],rdi
   mov    QWORD  [rbp-0x130],rsi
   mov    QWORD  [rbp-0x138],rdx
   mov    DWORD  [rbp-0x8],0x0
   mov    DWORD  [rbp-0xc],0x0
   mov    DWORD  [rbp-0x1c],0x0
   mov    DWORD  [rbp-0x10],0x0
   jmp    l83
l62:
   mov    eax,DWORD  [rbp-0x10]
   mov    edx,eax
   mov    eax,DWORD  [rbp-0x10]
   cdqe   
   mov    BYTE  [rbp+rax*1-0x120],dl
   add    DWORD  [rbp-0x10],0x1
l83:
   cmp    DWORD  [rbp-0x10],0xff
   jle    l62
   mov    DWORD  [rbp-0xc],0x0
   mov    DWORD  [rbp-0x4],0x0
   jmp    l112
l108:
   add    DWORD  [rbp-0x4],0x1
l112:
   mov    eax,DWORD  [rbp-0x4]
   movsxd rdx,eax
   mov    rax,QWORD  [rbp-0x138]
   add    rax,rdx
   movzx  eax,BYTE  [rax]
   test   al,al
   jne    l108
   mov    DWORD  [rbp-0x14],0x0
   jmp    l281
l147:
   mov    eax,DWORD  [rbp-0x14]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   movzx  edx,al
   mov    eax,DWORD  [rbp-0xc]
   lea    ecx,[rdx+rax*1]
   mov    eax,DWORD  [rbp-0x14]
   cdq    
   idiv   DWORD  [rbp-0x4]
   mov    eax,edx
   movsxd rdx,eax
   mov    rax,QWORD  [rbp-0x138]
   add    rax,rdx
   movzx  eax,BYTE  [rax]
   movsx  eax,al
   lea    edx,[rcx+rax*1]
   mov    eax,edx
   sar    eax,0x1f
   shr    eax,0x18
   add    edx,eax
   movzx  edx,dl
   sub    edx,eax
   mov    eax,edx
   mov    DWORD  [rbp-0xc],eax
   mov    eax,DWORD  [rbp-0x14]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   mov    BYTE  [rbp-0x1d],al
   mov    eax,DWORD  [rbp-0xc]
   cdqe   
   movzx  edx,BYTE  [rbp+rax*1-0x120]
   mov    eax,DWORD  [rbp-0x14]
   cdqe   
   mov    BYTE  [rbp+rax*1-0x120],dl
   mov    eax,DWORD  [rbp-0xc]
   cdqe   
   movzx  edx,BYTE  [rbp-0x1d]
   mov    BYTE  [rbp+rax*1-0x120],dl
   add    DWORD  [rbp-0x14],0x1
l281:
   cmp    DWORD  [rbp-0x14],0xff
   jle    l147
   mov    DWORD  [rbp-0x8],0x0
   mov    DWORD  [rbp-0xc],0x0
   mov    DWORD  [rbp-0x4],0x0
   jmp    l321
l317:
   add    DWORD  [rbp-0x4],0x1
l321:
   mov    eax,DWORD  [rbp-0x4]
   movsxd rdx,eax
   mov    rax,QWORD  [rbp-0x128]
   add    rax,rdx
   movzx  eax,BYTE  [rax]
   test   al,al
   jne    l317
   mov    DWORD  [rbp-0x18],0x0
   jmp    l574
l356:
   mov    eax,DWORD  [rbp-0x8]
   lea    edx,[rax+0x1]
   mov    eax,edx
   sar    eax,0x1f
   shr    eax,0x18
   add    edx,eax
   movzx  edx,dl
   sub    edx,eax
   mov    eax,edx
   mov    DWORD  [rbp-0x8],eax
   mov    eax,DWORD  [rbp-0x8]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   movzx  edx,al
   mov    eax,DWORD  [rbp-0xc]
   add    edx,eax
   mov    eax,edx
   sar    eax,0x1f
   shr    eax,0x18
   add    edx,eax
   movzx  edx,dl
   sub    edx,eax
   mov    eax,edx
   mov    DWORD  [rbp-0xc],eax
   mov    eax,DWORD  [rbp-0x8]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   mov    BYTE  [rbp-0x1d],al
   mov    eax,DWORD  [rbp-0xc]
   cdqe   
   movzx  edx,BYTE  [rbp+rax*1-0x120]
   mov    eax,DWORD  [rbp-0x8]
   cdqe   
   mov    BYTE  [rbp+rax*1-0x120],dl
   mov    eax,DWORD  [rbp-0xc]
   cdqe   
   movzx  edx,BYTE  [rbp-0x1d]
   mov    BYTE  [rbp+rax*1-0x120],dl
   mov    eax,DWORD  [rbp-0x8]
   cdqe   
   movzx  edx,BYTE  [rbp+rax*1-0x120]
   mov    eax,DWORD  [rbp-0xc]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   add    eax,edx
   movzx  eax,al
   mov    DWORD  [rbp-0x1c],eax
   mov    eax,DWORD  [rbp-0x18]
   movsxd rdx,eax
   mov    rax,QWORD  [rbp-0x128]
   add    rax,rdx
   movzx  ecx,BYTE  [rax]
   mov    eax,DWORD  [rbp-0x1c]
   cdqe   
   movzx  eax,BYTE  [rbp+rax*1-0x120]
   mov    esi,eax
   mov    eax,DWORD  [rbp-0x18]
   movsxd rdx,eax
   mov    rax,QWORD  [rbp-0x128]
   add    rax,rdx
   xor    ecx,esi
   mov    edx,ecx
   mov    BYTE  [rax],dl
   add    DWORD  [rbp-0x18],0x1
l574:
   mov    eax,DWORD  [rbp-0x18]
   cmp    eax,DWORD  [rbp-0x4]
   jl     l356
   mov    rax,QWORD  [rbp-0x128]
   leave  
   ret    

lol:
   jmp ui
ui:

bits 32
section .boot
global protected_mode
protected_mode:    

    mov eax, 0x1234
    mov ebx, 0x1234

    hlt
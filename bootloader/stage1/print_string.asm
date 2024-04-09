; Read string from bx, terminate at 0 character
print_string:
    pusha

    mov ah, 0x0e

.next_char:
    mov al, [bx]
    inc bx
    test al, al
    jz .done
    int 0x10
    jmp .next_char


.done:
    popa
    ret

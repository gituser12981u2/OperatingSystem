org 0x7c00  ; Set the origin to boot sector address
bits 16     ; Starting in 16-bit real mode

; =============================================================================
; Section: Bootloader Entry Point
; Description: This bootloader sets up a minimal GDT and transitions the system to protected mode. It loads its second stage into memory and jumps to it. 
; Additional responsibilities include checking system requirements.
; =============================================================================

; Initial Setup
jmp real_mode       ; Jump to the start of the real mode code

; Global Descriptor Table (GDT) Definitions
gdt_start:
    dq 0x0          ; Null Descriptor

gdt_code:
    dw 0xFFFF       ; Segment limit 15:0
    dw 0x0          ; Base addr 15:0
    db 0x0          ; Base addr 23:16
    db 10011010b    ; Access byte: present, DPL ring 0, descriptor type, executable, readable
    db 11001111b    ; Granularity: limit 19:16, G=1 (4kb blocks), D=1 (32-bit default size), L=0 (not long mode), AVL=0
    db 0x0          ; Base addr 31:24

gdt_data:
    dw 0xFFFF       ; Segment limit 15:0
    dw 0x0          ; Base addr 15:0
    db 0x0          ; Base addr 23:16
    db 10010010b    ; Access byte: present, DPL ring 0, descriptor type, executable, readable
    db 11001111b    ; Granularity: limit 19:16, G=1 (4kb blocks), D=1 (32-bit default), L=0 (not long mode), AVL=0
    db 0x0          ; Base addr 31:24

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                ; Address of GDT

; Segment Selector Definitions
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

real_mode:
    cli
    lgdt [gdt_descriptor]   ; Load GDT

    ; Check for minimum cpu requirements (80386)
    ; Check for 80386+
    push sp
    pop ax
    cmp ax, sp
    jne not_386         ; If not equal, CPU is not 80386 or newer

    smsw ax             ; Store Machine Status Word
    cmp ax, 0FFF0h      ; Check reserved bits
    jae not_386         ; If above or equal, it's not 80386 or newer

    ; NOT VALID FOR 80386 THOUGH HELPFUL IN THE FUTURE
    ; ; Get memory map, int 0x15 /= 0xe820
    ; mov eax, 0xE820     ; Get memory map
    ; xor ebx, ebx        ; Start with 0
    ; mov ecx, 20         ; Size of the memory map entry structure
    ; mov edx, 0x534D4150 ; 'SMAP' signature
    ; buffer equ 0x7E00   ; Place buffer right above boot sector
    ; mov di, buffer      ; ES:DI points to the buffer to store the memory map
    ; int 0x15
    ; jc disk_error

    ; get_memory_map:
    ;     add di, 20      ; Move buffer pointer to next entry
    ;     mov eax, 0xE820
    ;     mov ecx, 20
    ;     int 0x15
    ;     jc disk_error   ; Carry flag error
    ;     test ebx, ebx
    ;     jnz get_memory_map

    ; Load second part of the bootloader into memory

    ; Retrieve Header from stage2.bin
    mov ah, 0x02        ; read sector function

    mov al, 1           ; Number of sectors to read
    mov dl, 0x80        ; Drive number

    mov ch, 0x0         ; Cylinder number
    mov dh, 0x0         ; Head number
    mov cl, 0x2         ; Sector number

    mov bx, 0x1000      ; Move segment into bx
    mov es, bx          ; Segment (0x1000)
    xor bx, bx          ; Offset (0x0000)

    int 0x13
    jc disk_error

    mov ah, 0x02        ; read sector function

    add bx, 3
    mov al, [es:bx]     ; Number of sectors to read

    mov dl, 0x80        ; Drive number

    mov ch, 0x0         ; Cylinder number
    mov dh, 0x0         ; Head number
    mov cl, 0x2         ; Sector number

    mov bx, 0x1000      ; Move segment into bx
    mov es, bx          ; Segment (0x1000)
    xor bx, bx          ; Offset (0x0000)

    int 0x13
    jc disk_error

    ; Enable A20 Line via Keyboard Controller
    cli         
    in al, 0x64         ; Read status register
    test al, 2          ; Check if input buffer is empty
    jnz -2              ; Wait until it's empty

    mov al, 0xD1        ; Command to write to output port
    out 0x64, al
    in al, 0x64         ; Read status register again
    test al, 2          ; Ensure input buffer is empty
    jnz -2              ; Wait if not

    mov al, 0xDF        ; Enable A20 line
    out 0x60, al
    sti                 ; Re-enable interrupts

    ; BIOS A20 fallback
    mov ax, 0x2401      ; BIOS function to enable A20
    int 0x15
    jc A20_line_not_enabled
    
    ; Transition to Protected mode
    mov eax, cr0
    or eax, 0x1         ; Enable PE bit in cr0
    mov cr0, eax
    
    jmp CODE_SEG:protected_mode_entry

; CPU is below 80386
not_386:
    mov bx, CPU_BELOW_386_MESSAGE
    %include "print_string.asm"
    call print_string
    hlt

; Disk Error Handler
disk_error:
    mov bx, DISK_ERROR_MESSAGE
    call print_string
    hlt

; A20 Line Not Enabled Handler                         
A20_line_not_enabled:
    mov bx, A20_line_error_message
    call print_string
    hlt
CPU_BELOW_386_MESSAGE db "80386 or below", 0

DISK_ERROR_MESSAGE db "Disk Error", 0

A20_line_error_message db "A20 line cannot be enabled", 0

; =============================================================================
; Protected Mode Code
; =============================================================================
bits 32
protected_mode_entry:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Jump to Stage 2 entry point
    cli
    jmp CODE_SEG:0x10000

; =============================================================================
; Utilities and Helpers
; =============================================================================
;%include "print_string.asm"

; =============================================================================
; Boot Signature
; =============================================================================
times 510-($-$$) db 0   ; Align to 510 bytes by padding
dw 0xAA55               ; Add magic number as the last two bytes (aligned now to 512 bytes--one sector)
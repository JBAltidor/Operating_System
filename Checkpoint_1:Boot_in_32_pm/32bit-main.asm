[org 0x7c00] ; bootloader offset.Place le bootloader a l'adresse 0x7c00
    mov bp, 0x9000 ; set the stack. La pile grandit en descendant, donc nous la placons a une adresse ou elle ne pourra pas "overwrite"/impacter nos donnees
    mov sp, bp ; pour mettre bp dans sp

    mov bx, MSG_REAL_MODE
    call print ; Message indicant l'entree en mode 16 bit reel

    call switch_to_pm; passage en mode protege
    jmp $ 

%include "boot_sect_print.asm"
%include "32bit-gdt.asm"
%include "32bit-print.asm"
%include "32bit-switch.asm"

[bits 32]
BEGIN_PM: ; Nous sommes maintenant en mode 32 bits protege.
    mov ebx, MSG_PROT_MODE
    call print_string_pm ; Affichage du message de passage en 32 bits.
    jmp $

MSG_REAL_MODE db "  >Nou komanse an 16-bit Real Mode", 0
MSG_PROT_MODE db "  >Nou pase an 32-bit Protected Mode", 0

; bootsector
times 510-($-$$) db 0
dw 0xaa55

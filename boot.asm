[bits 16]
[org 0x7c00]

global _start
_start:
    cli                         ; Disabilita immediatamente le interruzioni
    xor ax, ax                  ; Sincronizza i registri dati a zero
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00              ; Stack provvisorio posizionato in area sicura

    ;; --- SALTO DI SICUREZZA IN MODALITÀ PROTETTA A 32-BIT ---
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1                   ; Imposta il bit protetto (PE)
    mov cr0, eax

    jmp CODE_SEG:init_pm        ; Svuota i registri a 16-bit ed entra nei 32-bit reali

[bits 32]
init_pm:
    mov ax, DATA_SEG            ; Configura i selettori di segmento dati alla GDT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov esp, 0x7C00             ; Stack pointer stabile che cresce all'indietro da 0x7C00
    mov ebp, esp
    
    ;; Rimosso l'extern! Saltiamo direttamente all'indirizzo del Kernel C (0x7E00)
    jmp 0x7e00                  
    
    jmp $                       ; Protezione di blocco

;; --- GLOBAL DESCRIPTOR TABLE (GDT) ---
align 4
GDT_Start: 
    dq 0x0                      
GDT_Code:  
    dw 0xffff, 0x0, 0x9a00, 0x00cf  ; Sezione Codice Piatta 4GB
GDT_Data:  
    dw 0xffff, 0x0, 0x9200, 0x00cf  ; Sezione Dati Piatta 4GB
GDT_End:

gdt_descriptor:
    dw GDT_End - GDT_Start - 1
    dd GDT_Start

CODE_SEG equ GDT_Code - GDT_Start
DATA_SEG equ GDT_Data - GDT_Start

;; FIRMA DI BOOT RICHIESTA DAI PC REALI E AMERICAN MEGATRENDS
times 510 - ($ - $$) db 0   
dw 0xaa55                   

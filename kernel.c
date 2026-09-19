#include "gui.h"

// --- COSTANTI HARDWARE ---
#define PORTA_DATI_TASTIERA 0x60
#define PORTA_STATO_TASTIERA 0x64

// --- VARIABILI GLOBALI (FINESTRE) ---
Finestra explorer;
Finestra notepad;
Finestra browser;

// --- FUNZIONI HARDWARE INTEGRATE ---
unsigned char inb(unsigned short porta) {
    unsigned char risultato;
    __asm__ volatile("inb %1, %0" : "=a"(risultato) : "Nd"(porta));
    return risultato;
}

// --- LOGICA DELL'ESTENSIONE ".RUNX" ---
void esegui_programma_runx(const char* nome_file) {
    // Feedback visivo nel browser per l'app .runx (Caratteri rossi 0x4F)
    for (int i = 0; i < 10; i++) {
        stampa_carattere_vga(10 + i, 16, 'X', 0x4F); 
    }
}

// --- FUNZIONE DI INGRESSO DEL KERNEL ---
void kernel_main() {
    // Forza la disattivazione degli interrupt a livello di CPU Atom
    __asm__ volatile("cli");

    // 1. Inizializza lo sfondo globale del Desktop (Verde/Azzurro VGA = 3)
    disegna_rettangolo(0, 0, 800, 600, 3);

    // 2. Configura EXPLORER
    explorer.x = 40; explorer.y = 48; explorer.larghezza = 320; explorer.altezza = 216;
    explorer.tipo = APP_EXPLORER; explorer.titolo = "Explorer"; explorer.visibile = 1;
    disegna_finestra(explorer);

    // 3. Configura NOTEPAD
    notepad.x = 400; notepad.y = 48; notepad.larghezza = 340; notepad.altezza = 240;
    notepad.tipo = APP_NOTEPAD; notepad.titolo = "Notepad"; notepad.visibile = 1;
    disegna_finestra(notepad);

    // 4. Configura BROWSER
    browser.x = 80; browser.y = 312; browser.larghezza = 500; browser.altezza = 216;
    browser.tipo = APP_BROWSER; browser.titolo = "Browser"; browser.visibile = 1;
    disegna_finestra(browser);

    // 5. Inizializza l'orologio testuale
    aggiorna_orologio(15, 30);

    // 6. Configura il buffer di scrittura del Notepad usando le variabili di gui.h
    testo_notepad[0] = '\0';
    indice_testo = 0;

    // --- LOOP DI POLLING HARDWARE DELLA TASTIERA ---
    while(1) {
        // Riaffermiamo il CLI ad ogni ciclo per ignorare i segnali elettrici della scheda Asus
        __asm__ volatile("cli"); 

        if (inb(PORTA_STATO_TASTIERA) & 1) {
            unsigned char scancode = inb(PORTA_DATI_TASTIERA);
            char carattere_premuto = 0;

            // Mappatura codici tastiera standard
            if (scancode == 0x1E) carattere_premuto = 'A';
            else if (scancode == 0x30) carattere_premuto = 'B';
            else if (scancode == 0x2E) carattere_premuto = 'C';
            else if (scancode == 0x20) carattere_premuto = 'D';
            else if (scancode == 0x12) carattere_premuto = 'E';
            else if (scancode == 0x39) carattere_premuto = ' '; // Spazio

            if (carattere_premuto != 0 && indice_testo < 20) {
                testo_notepad[indice_testo] = carattere_premuto;
                
                // Calcola la colonna testuale all'interno del Notepad bianco dell'Asus
                stampa_carattere_vga(42 + indice_testo, 5, carattere_premuto, 0xF0); // Testo nero su fondo bianco
                indice_testo++;
                testo_notepad[indice_testo] = '\0';
            }

            // Se l'utente preme il tasto 'E', l'OS esegue l'estensione ".runx"
            if (scancode == 0x12) {
                esegui_programma_runx("app_test.runx");
            }
        }
    }
}

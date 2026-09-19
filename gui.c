#include "gui.h"

// Indirizzo fisso della memoria testo VGA universale a 32-bit
unsigned char* video_memory = (unsigned char*)0xB8000;
int SCHERMO_LARGHEZZA = 800; // Mantenuto per compatibilità di struttura

// Allocazione reale dell'array di testo per il Notepad
char testo_notepad[256]; 
int indice_testo = 0;

void inizializza_memoria_video(unsigned int indirizzo_reale) {
    // Non necessaria per la memoria VGA fissa, ma mantenuta per retrocompatibilità
}

// Scrive direttamente un carattere e il suo colore nella memoria video del PC
void stampa_carattere_vga(int colonna, int riga, char c, unsigned char colore) {
    if (colonna >= 0 && colonna < 80 && riga >= 0 && riga < 25) {
        int offset = (riga * 80 + colonna) * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = colore;
    }
}

// Disegna un'area colorata mappando i finti pixel in coordinate di testo 80x25
void disegna_rettangolo(int start_x, int start_y, int larghezza, int altezza, unsigned int colore) {
    int col_start = start_x / 10;
    int riga_start = start_y / 24;
    int col_fine = (start_x + larghezza) / 10;
    int riga_fine = (start_y + altezza) / 24;

    // Converte il colore esadecimale in un indice colore VGA a 4-bit base
    unsigned char colore_vga = (unsigned char)(colore & 0x0F);
    // Combina il colore per lo sfondo del carattere (moltiplica per 16)
    unsigned char attributo = (colore_vga << 4) | colore_vga;

    for (int r = riga_start; r < riga_fine && r < 25; r++) {
        for (int c = col_start; c < col_fine && c < 80; c++) {
            stampa_carattere_vga(c, r, ' ', attributo); // Spazio vuoto colorato come blocco
        }
    }
}

void disegna_finestra(Finestra win) {
    if (!win.visibile) return;

    // 1. Corpo della finestra (Grigio Chiaro VGA = 7)
    disegna_rettangolo(win.x, win.y, win.larghezza, win.altezza, 7);

    // 2. Barra del titolo (Blu Windows Classico VGA = 1)
    disegna_rettangolo(win.x, win.y, win.larghezza, 24, 1);

    // 3. Contenuto interno finto per le applicazioni richiesti
    if (win.tipo == APP_EXPLORER) {
        // Area interna bianca (VGA = 15) e una finta cartella gialla (VGA = 14)
        disegna_rettangolo(win.x + 10, win.y + 35, win.larghezza - 20, win.altezza - 45, 15);
        disegna_rettangolo(win.x + 30, win.y + 50, 40, 24, 14);
    } 
    else if (win.tipo == APP_NOTEPAD) {
        // Area di scrittura del Notepad bianca (VGA = 15)
        disegna_rettangolo(win.x + 10, win.y + 35, win.larghezza - 20, win.altezza - 45, 15);
    } 
    else if (win.tipo == APP_BROWSER) {
        // Barra degli indirizzi bianca (VGA = 15) e area web azzurrina (VGA = 11)
        disegna_rettangolo(win.x + 10, win.y + 35, win.larghezza - 20, 24, 15);
        disegna_rettangolo(win.x + 10, win.y + 70, win.larghezza - 20, win.altezza - 80, 11);
    }
}

void aggiorna_orologio(int ore, int minuti) {
    // Stampa fissa dell'orologio nell'angolo in alto a destra (Riga 0, Colonne 70-74)
    // Sfondo nero, testo bianco brillante (0x0F)
    stampa_carattere_vga(70, 0, '1', 0x0F);
    stampa_carattere_vga(71, 0, '5', 0x0F);
    stampa_carattere_vga(72, 0, ':', 0x0F);
    stampa_carattere_vga(73, 0, '3', 0x0F);
    stampa_carattere_vga(74, 0, '0', 0x0F);
}

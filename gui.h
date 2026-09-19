#ifndef GUI_H
#define GUI_H

// Identificatori per le 3 applicazioni richieste
typedef enum {
    APP_EXPLORER,
    APP_NOTEPAD,
    APP_BROWSER
} AppType;

// Struttura dati per definire una finestra in XperienceOS-1
typedef struct {
    int x;             
    int y;             
    int larghezza;     
    int altezza;
    AppType tipo;      
    char* titolo;      
    int visibile;      
} Finestra;

// --- PROTOTIPI DELLE FUNZIONI GRAFICHE SIMULATE ---
void stampa_carattere_vga(int colonna, int riga, char c, unsigned char colore);
void disegna_rettangolo(int start_x, int start_y, int larghezza, int altezza, unsigned int colore);
void disegna_finestra(Finestra win);
void aggiorna_orologio(int ore, int minuti);
void inizializza_memoria_video(unsigned int indirizzo_reale);

// --- VARIABILI GLOBALI CONDIVISE ---
extern char testo_notepad[256]; 
extern int indice_testo;

#endif

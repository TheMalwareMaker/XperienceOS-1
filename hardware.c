// Legge un byte direttamente da una porta hardware della scheda madre
unsigned char inb(unsigned short porta) {
    unsigned char risultato;
    // Esegue l'istruzione Assembly 'in' per leggere dalla porta
    __asm__ volatile("inb %1, %0" : "=a"(risultato) : "Nd"(porta));
    return risultato;
}

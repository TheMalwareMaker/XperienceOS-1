# XperienceOS-1

Un sistema operativo basico a 32-bit scritto in **Assembly NASM** e **C**, progettato per funzionare in modalità testo VGA (`0xB8000`) sia su emulatori (QEMU) sia su hardware reale come l'**Asus Eee PC**.

## Requisiti di compilazione (su Windows)
* NASM
* Suite w64devkit (GCC e LD per bare-metal)

## Come compilare
Esegui la catena di comandi nel prompt:
```cmd
nasm -f bin boot.asm -o boot.bin
gcc -m32 -ffreestanding -O2 -c gui.c -o gui.o
gcc -m32 -ffreestanding -O2 -c kernel.c -o kernel.o
ld -m i386pe --image-base 0x0 -T linker.ld -o kernel.pe gui.o kernel.o
objcopy -O binary kernel.pe kernel.bin
copy /b boot.bin + kernel.bin XperienceOS-1.bin
```

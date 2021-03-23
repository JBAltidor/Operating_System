
**Checkpoint 1: Entrer en mode 32 bit protege**
 
 - Demarrage de l'ordinateur
 - Chargement du BIOS
 - Tests hardware
 - Recherche de disque bootable par analysde de boot sector
 - Chargement du boot loader en memoire
 - Entree en mode 16 bit reel
 - Passage en mode 32 bit protege

Pour lancer,
 - nasm -f bin 32bit-main.asm -o 32bit-main.bin 
 - qemu-system-x86_64 32bit-main.bin



Système d’Exploitation
Projet de fin de cours
Réalisation d’un système d’exploitation


Introduction
Dans le cadre du cours de systèmes d’exploitation, il nous a été demandé de réaliser ou de simuler un système d’exploitation. Nous avons opté pour la réalisation. Par ce document nous présenterons en premier lieu notre marche à suivre théorique, ensuite notre programme en tant que tel et enfin les résultats obtenus.


Introduction	1
L’équipe	3
Énoncé du projet	3
Préparation de l'environnement	4
NASM	4
Qemu/Bochs	5
GNU toolchain	5
Cross compiler	5
Binutils	6
Makefile	6
C	6

Ressources	7
Ecran	7
Mode texte	7
Mode graphique	7
Clavier	8
Processeur	8
Modes	8
Real mode	8
Protective mode	8
Interruptions	9
Registres	9
RAM	9
ROM	10

Réalisation	10
Processus de démarrage	10
L’utilisateur presse le bouton de mise en tension	10
BIOS POST	11
BIOS	11
Bootloader	11
GDT	11
GRUB	12
Kernel Entry	13
Checkpoint 1 : Boot Process	13
Système d’exploitation	14
Kernel	15
IDT	15
ISR	16
IRQ	16
PIT	16
Pilotes	17
Ecran	17
Clavier	17
Checkpoint 2 : Shell 	18
Memory Management/Protection	19
Segmentation	19
Paging	19
Implementation	20
MMU	21
Processus	22
Pending	24
Système de fichiers	24
LoadRoot	24
LoadFAT	24
FindFile	25
LoadFile	25
User Mode	25
Ressources	26





L’équipe
Jean-Bernard Altidor
Kevin Dubuche
Barbara Théodore
Sous la tutelle du professeur : Professeur Carl Darbouze
Lien GitHub : https://github.com/JBAltidor/Operating_System.git
Énoncé du projet 
Comme mentionné tantôt , nous avons à réaliser un mini système d’exploitation en vue de mieux assimiler les notions explorées tout le long de l’année académique. En d’autres termes, nous devons implémenter un programme permettant à un utilisateur de communiquer avec le matériel à travers un interface adéquat, en lui facilitant les interactions tout en lui imposant des contraintes.
Préparation de l'environnement
Le groupe utilisant un environnement Linux, parlons un peu de ce système d'exploitation (OS pour faire court désormais). Il existe un grand nombre de systèmes d'exploitation à travers le monde et le choix peut souvent s'avérer difficile lorsqu'on ne sait pas exactement ce que l'on cherche. Notre choix de l'environnement Linux se traduit par:
La disponibilité au grand public (open source)
La fiabilité du système du point de vue sécurité
Sa rapidité car il est plus léger et la gestion des fichiers est mieux organisée.
Mais, en gros, quel est le rôle d'un système d'exploitation ? L'OS est une application dont le rôle premier est de faciliter la communication entre le matériel électronique et l'utilisateur. Toutes les actions que veut poser un être humain doivent être interprétées par l'appareil. Chacune d'elles doit donc être programmée par le concepteur. 

Pour créer un système d'exploitation et le tester par la suite, il nous faudrait un matériel vierge (sans aucun OS) sur lequel l'installer. Faute d'argent, nous ne saurions investir autant. Heureusement, une autre alternative a été pensée par nos aînés: la machine virtuelle. De ce fait, tout en utilisant notre appareil pour programmer notre OS, nous pouvons parallèlement faire fonctionner ce dernier sans dépenses supplémentaires, sur un matériel connu. Notre choix s'oriente alors vers QEMU.
NASM 
Le NASM (Netwide Assembler) est un assembleur pour l’architecture Intel x86. Contrairement à la plupart des langages d’assemblage existants tels que TASM (Turbo Assembler) ou MASM (Microsoft’s Macro Assembler), le NASM permet de générer des exécutables binaires sur 16 bits. De ce fait, nous orientons notre choix vers ce dernier.
Qemu/Bochs
QEMU est à la fois un émulateur et une machine virtuelle open source. Il est compatible avec tout type de système d'exploitation et de machine. VirtualBox étant une machine virtuelle particulièrement compatible avec les architectures x86 et amd64, il devient plus efficient et efficace d’avoir recours au QEMU qui utilise le kernel KVM de notre environnement. De plus, ce dernier est bien plus rapide (comme l’indique son nom: Quick EMUlator). Et enfin, alors que VirtualBox impose des machines virtuelles ayant des caractéristiques spécifiques, QEMU nous laisse donner libre cours à notre imagination pour paramétrer notre environnement. 
Bien que le programme n'ait pas d'interface graphique, certains outils permettent de manipuler la partie graphique tant convoitée par nous autres, humains. Mais Qemu ne nous donne aucune information sur l’exécution de notre OS. Compte tenu du fait que le comportement par défaut pour toute erreur lors de l'exécution est un “Triple fault” , c’est-à-dire un simple redémarrage, nous utiliserons aussi l'émulateur Bochs (un peu plus difficile à manipuler)  qui offre un log pour le debugging.
L'un des grands avantages apportés par notre choix, c'est que QEMU utilisera notre CPU (un environnement connu) pour exécuter les codes de notre machine virtuelle.
GNU toolchain
GNU toolchain est un terme générique désignant une chaîne de compilation utilisant des logiciels du projet GNU.
Cross compiler
Un Cross compiler est un compilateur capable de générer des codes exécutables pour un environnement autre que celui sur lequel il fonctionne.
Notre environnement de travail étant différent de celui de l’OS réalisé, nous utilisons  GNU Compiler Collection (GCC) pour i386-elf toolchain. Ce GCC permet de compiler les codes d’un langage de programmation de haut niveau en simple assembleur. Nous utiliserons C/C++ pour programmer le haut niveau de notre kernel, et ELF(Executable and linkable) comme format de fichier binaire pour l'enregistrement de code compilé (objets, exécutables, bibliothèques de fonctions).
Binutils
Les Binutils (Binary Utilities) sont un ensemble d’outils binaires permettant de créer et de gérer des fichiers de programme binaires, ainsi que d’assurer la gestion de fichiers objet, de bibliothèques, de données de profil et de code source assembleur.
Nous aurons recours au GNU Linker pour lier nos fichiers objet dans le noyau final et au GNU Assembler pour assembler les instructions dans des fichiers objet contenant nos codes machines.
Makefile
Make est un utilitaire Unix conçu pour démarrer l’exécution d’un makefile. Un makefile est un fichier spécial, contenant des commandes Shell, que l’on crée en le nommant Makefile. Une fois dans le répertoire de ce makefile, il suffit de taper make ou make run et les commandes du makefile seront exécutées. Ceci nous permettra d’appliquer, avec une seule commande, les modifications faites durant le développement. 
Notre système d’exploitation sera réalisé pour un ordinateur avec un processeur de la série IA-32 qui utilise le CISC (Complex Instruction Set Computer). Il aura à gérer les diverses ressources disponibles.

C 
C est un langage de programmation minimaliste indépendant de la plate-forme et ne nécessitant aucun runtime. En raison de ces qualités, il est devenu de loin le langage le plus utilisé dans le domaine du développement d'OS et de la programmation système. Il peut être comparé à un "assemblage portable", tel qu'il a été initialement conçu et utilisé.
Nous utiliserons le cross compiler “i386-elf-gcc” pour compiler nos codes et le linker ‘i386-elf-ld’ pour générer le fichier binaire.


Ressources
Les périphériques I/O (Input/Output)
Ecran 
Clavier
Le processeur
La mémoire RAM
Les mémoires ROM
Ecran
L'écran est le périphérique qui permet d’afficher des informations à l'utilisateur. Il fonctionne en deux modes :
Mode graphique
Mode texte
Mode texte
Le mode texte est un type d'affichage sur écran constitué uniquement de caractères. Le principe réside dans le concept qu'un écran peut être considéré comme un quadrillage de 80*25 carrés (pixels), dans lequel chaque carré permet d'afficher un caractère. Chaque pixel est associé à 16 bits en mémoire à partir de l'adresse 0xB8000 , 8 bits pour un caractère ASCII et 8 bits pour la propriété de ce caractère. Nous utiliserons ce mode.
Mode graphique 
Le mode graphique, par opposition au mode texte, est un type d'affichage sur écran constitué de pixels, au lieu de caractères. Ce mode est plus récent et comprend plusieurs standards : VGA, HDTV, HDTV+ …
Clavier
Le clavier est un périphérique permettant d'écrire du texte et communiquer avec l'ordinateur. Les claviers possèdent une centaine de touches qui émettent un signal à chaque entrée. Ce signal est associé à une interruption qui permet de l’associer à sa fonction.
Processeur
Comme mentionné tantôt, nous utiliserons un processeur de la famille x86 d’Intel. Ce dernier est compatible avec le jeu d’instructions Intel 8086. Nous aurons donc à le commander en premier lieu à travers le langage assembleur, langage très proche du code machine. Le processeur peut fonctionner en plusieurs modes.
Modes
Real mode
Le mode réel est un mode 16 bits simpliste présent sur tous les processeurs x86. Le mode réel a été la première conception du mode x86 et a été utilisé par de nombreux systèmes d'exploitation avant la naissance du mode protégé. À des fins de compatibilité, tous les processeurs x86 commencent à s'exécuter en mode réel.

Protective mode
Le mode protégé est le principal mode de fonctionnement des processeurs Intel modernes. Il permet de travailler avec plusieurs espaces d'adressage virtuels, dont chacun dispose d'un maximum de 4 Go de mémoire adressable; et permet au système d'appliquer une protection stricte de la mémoire et des périphériques d'entrées/sorties.

Virtual mode Real mode
Il s’agit d’une amélioration du mode protégé, facilitant l’exécution d’un système d’exploitation à la fois graphique et multitâche.
64 bit extension mode pour les nouveaux processeur 64 bits
Ce mode permet aux applications 64 bit d’accéder aux instructions associées, sans déranger les programmes 16 et 32 bit.
 Pour rester compatible aux CPU antérieurs,notre processeur a le mode 16-bit réel. Ceci remonte au 8080, qui avait 16 bits de bus d'adresse et pouvait donc accéder à 64 Ko de mémoire. Nous commencerons dans cette environnement 16 bit réel, mais passons au mode 32 bits protégé.
Interruptions
Les interruptions sont un mécanisme qui permet au processeur d'arrêter temporairement ce qu'il fait et d'exécuter d'autres instructions de priorité plus élevée avant de revenir à la tâche d'origine. Une l'interruption peut être déclenchée soit par une instruction logicielle, soit par un périphérique matériel nécessitant une action hautement prioritaire (par exemple, pour lire certaines données du clavier). 
Chaque interruption est représentée par un numéro unique qui est un index de l'interruption vector, une table initialement mise en place par le BIOS au début de la mémoire (c'est-à-dire à l'adresse physique 0x0) qui contient des pointeurs d'adresse pour interrompre les routines de service (ISR). 
Un ISR (Interrupt Service Routine) est simplement une séquence d'instructions machine qui traitent d'une interruption spécifique.
Registres
Tout comme nous utilisons des variables dans les langages de niveau supérieur, il est utile de pouvoir stocker des données lors d’une opération. Tous les processeurs x86 ont quatre registres à usage général (ax, bx, cx et dx)  à cet effet. Aussi, ces registres, qui peuvent chacun contenir un mot (deux octets, 16 bits) de données, peuvent être lus et écrits par le CPU avec un retard négligeable par rapport à l'accès à la mémoire principale.
RAM
La RAM (Random Access Memory) est un type de mémoire vive, en opposition à la mémoire morte. Extrêmement rapide par rapport à la ROM, elle permet au processeur de stocker temporairement les données dont il a besoin pour lancer un programme. Le système d'exploitation lui-même est chargé sur la RAM. 
ROM
La ROM (Read-Only Memory) permet de stocker des données de manière non volatile (onboard hard disk, CD drive,USB drive … ). C’est-à-dire qu’elle n’efface pas le contenu si l’appareil en question est mis hors tension. La ROM est divisée en plusieurs secteurs dont le premier est appelé boot sector.





Réalisation 
Processus de démarrage
L’utilisateur presse le bouton de mise en tension
Une fois ce bouton pressé, les fils conducteurs liés à ce dernier envoient un signal électronique à la carte-mère. De son côté, cette dernière redirige ce signal d’un bit vers le PSU( Power Supply). Lorsque le bit est à 0, l’appareil est hors tension (tout comme la carte-mère) tandis qu’à 1 (valeur traduisant que le signal est actif), il est alimenté. Puisqu’un signal actif est reçu, le PSU en renvoie une partie à l’ensemble du système. Dès que toutes les composantes seront suffisamment alimentées, il sera en mesure de continuer à les fournir plus de tension. De ce fait, il envoie un signal vulgairement appelé power good dans la carte-mère au BIOS (Basic Input Output System).
BIOS POST 
Une fois le power good reçu, le BIOS initialise un processus POST (Power On Self Test). Ce dernier fait alors des tests pour vérifier que la tension est suffisante, que les périphériques (clavier, souris, USB, ports séries, etc...) sont bien installés et que la mémoire n’a pas été corrompue. Si tout est correct, il passe la main au BIOS qui prend la relève. Pour ce faire, il charge le BIOS en mémoire et met une instruction de saut. Le CS:IP, pointeur d’instruction, est mis à 0 pour céder la place au processeur qui  commence à exécuter le BIOS. Ce dernier prend alors la relève.
BIOS 
Le BIOS crée une table de vecteur d’interruption et fournit des services d’interruption de base. Puis, grâce à de nouveaux tests, il vérifie que le matériel est correct. Il fournit également un utilitaire de configuration. Ensuite, il faut que le BIOS trouve un OS. De ce fait, il se base sur le boot order paramétré permettant de trouver un périphérique adéquat.
S’il trouve un disque dont le boot sector fini par 0xAA55,ce dernier est donc chargeable, il sera placé à l'adresse 0x7C00 et l'interruption INT 0x19 sera envoyée. Il passe alors la main au boot loader. Dans le cas où le périphérique est introuvable, il part à la recherche du second dans la liste, et ainsi de suite. Si aucun périphérique n’est détecté, un message d’erreur est affiché et le BIOS arrête le système. 
Bootloader
Le boot sector, étant limité à 512 bytes, est trop petit pour contenir tout notre système d’exploitation. Donc, nous y placerons un morceau de code, le bootloader, responsable de charger le kernel.
Un bootloader est un programme de démarrage qui se charge dans la mémoire de travail de l’appareil après son démarrage. Stocké avec le MBR (Master Boot Record), il se trouve dans le premier secteur du disque. 
Mais avant, nous passons en mode 32 bit protégé. Le mode 32 bits nous permet d'utiliser des registres 32 bits et d’adressage mémoire, mémoire protégée, mémoire virtuelle et autres, mais nous perdrons les interruptions BIOS.
GDT
Le GDT (Global Descriptor Table) définit les segments de mémoire et leurs attributs en mode protégé. Dans l'architecture Intel, et plus précisément en mode protégé, la plupart des routines de gestion de la mémoire et de service d'interruption sont contrôlées par le biais de tables de descripteurs. Chaque descripteur stocke des informations sur un seul objet (par exemple, une routine de service, une tâche, un morceau de code ou de données, etc) dont le CPU pourrait avoir besoin à un moment donné. 
Nous définirons le GDT en langage assembleur. Ce qui nous permettra plus tard de charger un noyau (kernel) qui a été compilé à partir d'un langage de plus haut niveau, C. Ce dernier sera compilé en  instructions 32 bits plutôt que les instructions 16 bits moins efficaces.
GRUB
GRUB, mis pour GRand Unified Bootloader, est un chargeur de démarrage ayant pour principal rôle de préparer le système et lancer le Kernel. Dès le démarrage, il prend donc le relais du BIOS, se charge avant de charger le noyau en mémoire. Il est utilisé par de nombreux systèmes d’exploitations.


Nous avons exploré son fonctionnement, mais nous ne l'utiliserons pas car il fait abstraction de tout le processus de démarrage et permet de charger directement l’OS.
Kernel Entry
Le “Kernel Entry” est le morceau de code qui sera exécuté en premier après le chargement du kernel par le bootloader. Ce morceau de code est écrit en langage assembleur, il permet la configuration d'une nouvelle pile, le chargement d'un nouveau GDT. Pour le moment, il ne fait qu’afficher un message.
Checkpoint 1 : Boot Process
Jusqu’ici, nous avons démarré notre ordinateur, machine virtuelle dans notre cas, ce qui nous a donné accès à l’environnement pré-OS. Ce dernier nous offre alors le BIOS, UEFI (Unified Extensible Firmware Interface) pour les systèmes les plus récents. Nous utiliserons le BIOS. 
Le BIOS, stocké sur une chip ROM, est chargé en RAM et exécute des tests de hardware. Puis, si les tests sont réussis, il y a une itération sur le boot sector de toutes les mémoires ROM (en format supporté) disponibles suivant l'ordre défini par le boot sequence. Pour différencier un boot code de simple données, étant donné qu’ils peuvent tous deux être interprétés comme instructions CPU, il vérifie que les deux derniers bytes du boot sector sont fixées à “0xAA55”. Le premier boot sector vérifiant cette condition est alors chargeable en mémoire.
Ce dernier est chargé (copié en mémoire vive) à l'adresse 0x7C00 et le pointeur d'instruction y est aussi placé. Compte tenu que le boot sector n'est limité qu'à (en général) 512 bytes, seul le bootloader y est présent et ce dernier est responsable de charger à son tour le kernel.


Voir le dossier "Checkpoint _1:Boot_in_32_pm"
Système d’exploitation
Après avoir réussi notre processus de boot, notre ordinateur est prêt à recevoir le système d’exploitation proprement dit. Nous commencerons par le kernel qui en est le pilier.
Quand le bootloader est prêt, il charge et exécute notre noyau C en appelant sa routine de point d'entrée. Étant donné que le programme C suit un format interne spécifique, le bootloader doit savoir comment analyser le fichier et localiser la routine de point d'entrée pour l'appeler. Cela nous permet d'utiliser C pour le noyau et les autres bibliothèques que nous avons créées.

Kernel
Le noyau est le cœur d'un système d'exploitation. Il est, en général,  responsable de la gestion de la mémoire, des E/S, de la gestion des interruptions et de diverses autres choses. Le principal objectif du noyau est de gérer les ressources système et de fournir une interface pour que d'autres programmes puissent accéder à ces ressources. 
L'idée de base est qu’ il faut empêcher à un processus de contrôler directement toutes les ressources de l’ordinateur. Le noyau nous offre cette couche d'abstraction. Nous développerons un noyau monolithique. Il gère par le biais d’un system call les appels système entre les applications et le matériel.

IDT 
La table d'interruption, ou IDT ( Interrupt Descriptor Table), est utilisée afin de montrer au processeur quelle routine de service d'interruption (ISR) appeler pour gérer une exception ou un opcode 'int' (en assembleur). Les entrées IDT sont également appelées par des requêtes d'interruption chaque fois qu'un périphérique a terminé une demande et doit être servi. 
Chaque entrée IDT est similaire à celle d'une entrée GDT. Les deux ont une adresse de base, les deux détiennent un indicateur d'accès et les deux ont une longueur de 64 bits. Les principales différences entre ces deux types de descripteurs résident dans la signification de ces champs. Dans un IDT, l'adresse de base spécifiée dans le descripteur est en fait l'adresse du sous-programme de service d'interruption que le processeur doit appeler lorsque cette interruption est «déclenchée» (appelée).
Il y a des moments où vous souhaitez interrompre le processeur. Vous voulez l'empêcher de faire ce qu'il fait et le forcer à faire quelque chose de différent. Un exemple est  la fin d’un timer où une demande d'interruption de clavier (IRQ) se déclenche. Une interruption est comme un signal POSIX - elle vous indique que quelque chose d'intéressant s'est produit. Le processeur va enregistrer des gestionnaires d'interruptions qui traitent l'interruption, puis revenir au code qui était en cours d'exécution avant son déclenchement. Les interruptions peuvent être déclenchées en externe, via des IRQ, ou en interne, via l'instruction «int n».
ISR
Les routines de service d'interruption, ou ISR, sont utilisées pour enregistrer l'état actuel du processeur et configurer les registres de segments appropriés nécessaires pour le mode noyau avant que le gestionnaire d'interruption de niveau C du noyau ne soit appelé. Nous devons également pointer l'entrée correcte dans l'IDT vers le bon ISR afin de gérer la bonne exception. 
Une exception est un cas particulier que le processeur rencontre lorsqu'il ne peut pas continuer l'exécution normale. 

IRQ 
Les Interrupt Requests ou IRQs sont des interruptions qui sont déclenchées par des périphériques matériels. Certains périphériques génèrent une IRQ lorsqu'ils ont des données prêtes à être lues, ou lorsqu'ils terminent une commande comme l'écriture d'un tampon sur le disque, par exemple. Un périphérique générera une IRQ chaque fois qu'il veut l'attention du processeur. Par exemple, lorsque l’utilisateur tape une touche du clavier, ceci déclenche l’ IRQ1.
PIT
Le Programmable Interval Timer (PIT) est une puce connectée à IRQ0, il donne à notre OS la notion de temps. Il peut interrompre le processeur à une fréquence définie par l'utilisateur. La puce dispose de 3 canaux: le canal 0 est lié à IRQ0, pour interrompre le processeur à des heures prévisibles et régulières, le canal 1 est spécifique au système et le canal 2 est connecté au haut-parleur du système. Nous nous limiterons à l'utilisation du canal 0. Le PIT est la principale méthode utilisée pour implémenter une horloge système et la seule méthode disponible pour implémenter le multitâche (basculer les processus en cas d'interruption). 
Pilotes
Ecran
Pour imprimer à l'écran, nous avons également besoin d'un moyen de gérer le défilement de l'écran au besoin, le déplacement du curseur, l’affichage un caractère et aussi d’un moyen pour l’effacer. Il peut être aussi  intéressant de permettre également des couleurs différentes sur l'écran, grâce à la carte vidéo VGA qui nous donne un morceau de mémoire où nous écrivons à la fois des paires d'octets d'attributs et d'octets de caractères afin d'afficher des informations à l'écran. Le contrôleur VGA se chargera de dessiner automatiquement les modifications mises à jour à l'écran. Le défilement est géré par notre logiciel noyau. 
	Clavier
La gestion du clavier se fait en deux étapes, l'obtention d'une touche lorsqu'elle est enfoncée et la conversion du «scancode» en caractères ASCII standard. Un scancode est simplement un numéro clé. Le clavier attribue un numéro à chaque touche du clavier; c'est le scancode. Ils sont généralement numérotés de haut en bas et de gauche à droite, avec quelques exceptions mineures pour garder les dispositions rétrocompatibles avec les claviers plus anciens. Nous utiliserons un tableau de valeurs (keymap) avec les scancodes comme index dans cette table. Elle sera utilisée pour traduire les scancodes en valeurs ASCII à la suite d’une interruption.
Checkpoint 2 : Shell

A ce stade, notre OS présente à l’utilisateur un interface shell qui peut recevoir et réagir à son entrée. Le rappel du clavier recherche un “ENTER”, puis appelle le kernel, en lui disant que l'utilisateur a saisi quelque chose. 
Donc, jusqu'ici nous avons réordonné nos codes et créé un fichier Makefile pour faciliter la compilation. Nous avons ajouté des pilotes pour l'écran et le clavier. Aussi nous avons défini des fonctions de base en C ,tel strcmp(), strlen()... Enfin nous avons installé nos ISR et ISQ.
Voir le dossier "Checkpoint 2:Shell"
Memory Management/Protection
L'architecture x86 dispose de deux méthodes de protection de la mémoire et de mémoire virtuelle: la segmentation et la pagination. 
Segmentation
Avec la segmentation, chaque accès mémoire est évalué par rapport à un segment. Autrement dit, l'adresse mémoire est ajoutée à l'adresse de base du segment et vérifiée par rapport à la longueur du segment. On peut considérer un segment comme une fenêtre dans l'espace d'adressage - Le processus ne sait pas que c'est une fenêtre, tout ce qu'il voit est un espace d'adressage linéaire commençant à zéro et allant jusqu'à la longueur du segment. 

Paging
Avec la pagination, l'espace d'adressage est divisé en blocs, de généralement 4 Ko, appelés pages. Chaque page peut être mappée dans la mémoire physique, mappée sur ce qu'on appelle un «frame». Ou, il peut être non mappé. Comme ça, vous pouvez créer des espaces de mémoire virtuelle.La pagination est la technique la plus courante utilisée par les processeurs x86 pour activer la mémoire virtuelle. La mémoire virtuelle par pagination signifie que chaque processus aura l'impression que la plage de mémoire disponible est de 0x00000000 - 0xFFFFFFFF même si la taille réelle de la mémoire peut être bien inférieure. Cela signifie également que lorsqu'un processus adresse un octet de mémoire, il utilise une adresse virtuelle (linéaire) au lieu d'une adresse physique. Le code du processus utilisateur ne remarquera aucune différence (à l'exception des retards d'exécution). L'adresse linéaire est traduite en adresse physique par la MMU et la table de pages. Si l’adresse virtuelle n’est pas mappée à une adresse physique, le processeur déclenche une interruption de défaut de page. 
Implémentation 
La pagination en x86 consiste en un répertoire de pages (PDT) qui peut contenir des références à 1024 pages tables (PT), chacune pouvant pointer vers 1024 sections de mémoire physique appelées cadres de page (PF ). Chaque cadre de page mesure 4096 octets. Dans une adresse virtuelle (linéaire), les 10 bits les plus élevés spécifient le décalage d'une entrée de répertoire de page (PDE) dans le PDT actuel, les 10 bits suivants le décalage d'une entrée de table de page (PTE) dans la table de page pointée par cela PDE. Les 12 bits les plus bas de l'adresse correspondent au décalage dans le cadre de page à adresser.
Tous les répertoires de pages, tableaux de pages et cadres de page doivent être alignés sur des adresses de 4096 octets. Cela permet d'adresser un PDT, un PT ou un PF avec seulement les 20 bits les plus élevés d'une adresse de 32 bits, puisque les 12 les plus bas doivent être égaux à zéro.La structure PDE et PTE est très similaire l'une à l'autre: 32 bits (4 octets), où les 20 bits les plus élevés pointent vers un PTE ou PF, et les 12 bits les plus bas contrôlent les droits d'accès et d'autres configurations. 4 octets fois 1024 donnent 4096 octets, donc un répertoire de pages et une table de pages tiennent tous les deux dans un cadre de page.

MMU
Le principal service de la MMU (Memory Management Unit) à l'ordinateur est la traduction de la mémoire. La traduction de mémoire est un processus par lequel les adresses virtuelles sont converties en adresses physiques. On peut dire que les adresses virtuelles sont mappées à l'adresse physique. Cela nous donne la possibilité de créer un modèle de mémoire à notre manière. Autrement dit, nous pouvons réorganiser la façon dont la mémoire semble être ordonnée. Parce que nous pouvons faire apparaître la mémoire comme nous le voulons, nous pouvons faire en sorte que chaque processus semble être le seul processus sur la machine. De plus, comme le processus ne peut voir que la mémoire dont il dispose, il ne peut ni modifier ni copier la mémoire d'une autre application. Nous implémentons donc la protection de la mémoire.

Processus
Un processus est un programme en cours d'exécution. L'exécution d'un processus doit progresser de manière séquentielle. Pour le dire simplement, nous écrivons nos programmes informatiques dans un fichier texte et lorsque nous exécutons ce programme, il devient un processus qui effectue toutes les tâches mentionnées dans le programme.
Lorsqu'un programme est chargé dans la mémoire et qu'il devient un processus, il peut être divisé en quatre sections: stack, heap, text et data. L'image suivante montre une disposition simplifiée d'un processus dans la mémoire principale -



Représentation d’un processus.

Changement de processus.
Pending
Système de fichiers
Notre OS, au début, n’a pas la notion de fichier. Tout n’est qu’un ensemble de bits.
Un système de fichier est donc un ensemble de spécifications qui permet de décrire des fichiers. Il donne une logique à ce qui est disponible sur le disque pour effectuer les opérations d'écriture, de lecture et de modification. L'OS doit obtenir l'adresse de début d’un fichier affiche de le charger et aussi avoir certaines informations sur ce dernier. Car, par exemple, l'OS ne saurait différencier un fichier texte d’un fichier exécutable. En effet, un fichier n’est qu’un ensemble de bit dont le sens dépend de l'interprétation.
Notre disque étant subdivisé en secteurs de 512 bytes, un fichier ne sera pas forcément un multiple entier de cette valeur donc on aura à ajouter un padding, comme pour notre bootloader. Quand un fichier occupe plus d’un sector, on parle de cluster; et quand ces secteurs ne sont pas contigus, on parle de fragmentation.
Il existe de nombreux systèmes de fichiers :FAT12, FAT16, FAT32, NTFS, ext (Linux), GFS ( Google File System)... Nous utiliserons FAT12 , pour sa simplicité.

Structure de  FAT12:
Pour implémenter le système de fichier, nous aurons creer  :
 LoadRoot
  -Charge la Root Directory Table a l'adresse 0x7e00
 LoadFAT 
    -Charge ta table FAT a l’adresse 0x7c00
Parametre : ES:DI => Root Directory Table
 FindFile
   - Recherche le nom du fichier dans la table root 
Paramètre : DS:SI => Nom du fichier
Retourne : AX => File index number in directory table. 
                                 -1 si le fichier n’est pas trouvé
LoadFile
   -Charge le fichier.
Paramètre : ES:SI => Fichier a charger
Paramètre : EBX:BP => Buffer ou placer le fichier
Retourne    : AX => -1 s’il y a erreur
                                                0 si l’operation a reussi

User Mode 
Notre OS , pour le moment, fonctionne avec le processeur en kernel mode. Le mode kernel met à disposition certaines instructions qui seraient généralement refusées à un programme utilisateur, comme la possibilité de désactiver les interruptions ou d'arrêter le processeur. Pour exécuter des programmes utilisateur, nous devrons passer du mode noyau au mode utilisateur, pour restreindre les instructions disponibles. Et également restreindre l'accès en lecture ou en écriture à des zones de mémoire.
Le x86 n’offre  pas de moyen direct de passer en mode utilisateur. La seule façon d'atteindre le mode utilisateur est de revenir à exception qui a commencé en mode utilisateur. La seule méthode pour y arriver en premier lieu est de configurer la pile comme si une exception en mode utilisateur s'était produite, puis d'exécuter une instruction de retour d'exception (IRET).




Ressources
https://wiki.osdev.org/Main_Page
http://littleosbook.github.io/
http://www.osdever.net/bkerndev/index.php

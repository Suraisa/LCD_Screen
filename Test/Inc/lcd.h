#ifndef LCD_H
#define LCD_H
#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"


typedef enum{
    CLEARDISPLAY = 0x1,
    RETURNHOME = 0x2,
    ENTRYMODESET = 0x4,
    DISPLAY = 0x8,
    SHIFT = 0x10,
    FUNCTIONSET = 0x20,
    SETDDRAM = 0x40,
    BUSY = 0x80,
    WRITE = 0x200
}LCD_INSTRUCTION;

typedef struct
{
    unsigned ID:1;
    unsigned S:1;
    unsigned D:1;
    unsigned C:1;
    unsigned B:1;
    unsigned DL:1;
    unsigned N:1;
    unsigned F:1;
    unsigned SC:1;
    unsigned RL:1;
} CONFIG;

typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
} ADRESSE;

/*
Tableau d'ADRESSE :
D0 D1 D2 D3 D4 D5 D6 D7 RW RS E
*/
typedef struct{
    ADRESSE pinout[11];
    LCD_INSTRUCTION instruction;
    char data;
}LCDInterface;


/********************************\
******Fonctions controle LCD******
\********************************/

/*
Modifie la longeur de l'interface, le nombre de ligne et la police des characteres
Demande les configs DL N F
*/
void InstructionFunctionSet(LCDInterface* interface, char config[10]);

/*
Function set, run at start
*/
void InstructionFunctionSetStart(LCDInterface* interface, char config[10]);

/*
Modifie les parametres du visuel : Ecran OFF/ON et curseur clignotte ou non
Demande les configs D C B 
*/
void InstructionDisplay(LCDInterface* interface, char config[10]);


/*
Defini la direction du curseur
Demande les configs I/D S
*/
void InstructionEntryModeSet(LCDInterface* interface, char config[10]);


/*
Nettoie l'ecran et retour au debut de la DDRAM (premiere case de l'ecran)
*/
void InstructionClearDisplay(LCDInterface* interface);


/*
Retour au debut de la DDRAM (premiere case de l'ecran)
*/
void InstructionReturnHome(LCDInterface* interface);

/*
Shift the cursor or the display
Use configs : S/C R/L
*/
void CursorDisplayShift(LCDInterface*, char config[10]);


/*
Positionner le curseur a l'ADRESSE DDRAM,
Sachant que l'ecran a les ADRESSEs dans l'ordre suivante :
Ligne 1 : 00 ---> 0F
Ligne 2 : 40 ---> 4F

Si decalage a gauche :
Ligne 1 : 01 ---> 0F
Ligne 2 : 41 ---> 4F

Inversement

Sachant que l'on peut activer un driver d'extension pour l'affichage
Afin d'avoir 40 characteres par lignes.
*/
void InstructionCurseurPosition(LCDInterface* interface, char DDRAM);



/********************************\
**Fonctions d'initialisation LCD**
\********************************/

/*
Initialisation des ports.
Nommez les ports et pins suivant cette nomenclature:
LCD_XX_GPIO_Port
LCD_XX_Pin
Avec XX : RW, RS, D0--D7
*/
void LCDInterfaceInitialisation(LCDInterface*);


/*
Initialise les ports, pins et prepare l'ecran

Config[0--9] : I/D S D C B DL N F S/C R/L

I/D :
    1 : Incremente DDRAM (decale le curseur a la position suivante apres ecriture)
    0 : Decremente DDRAM (decale le curseur a la position precedente apres ecriture)

S :
    1 : Decale l'affichage vers la droite (I/D == 0), vers la gauche (I/D == 1)
    0 : Ne Decale pas l'affichage

D :
    1 : Allume l'ecran
    0 : Etein l'ecran

C :
    1 : Affiche le curseur (affiche sur la 8eme ligne pour 5x8 char sinon 11 pour 5x10)
    0 : Retire le curseur

B :
    1 : Curseur clignote
    0 : Curseur statique

DL :
    1 : Utilisation de 8 bits
    0 : Utilisation de 4 bits

N :
    1 : 2 lignes (F importe peu ici)
    0 : 1 ligne

F :
    1 : Font : 5x10
    0 : Font : 5x8

S/C | R/L :
0 | 0 : Decale curseur gauche
0 | 1 : Decale curseur droite
1 | 0 : Decale ecran gauche
1 | 1 : Decale ecran droite

*/
void LCDInit(LCDInterface*, char config[10]);


/*
Move screen, if :
Direction == 1 : right
Direction == 0 : left
*/
void MoveScreen(LCDInterface* interface, char direction);


/*
Move cursor, if :
Direction == 1 : right
Direction == 0 : left
*/
void MoveCursor(LCDInterface* interface, char direction);

/********************************\
********Fonctions d'envoie********
\********************************/

/*
Envoie de l'instruction stocke dans l'interface
*/
void LCDSendInstruction(LCDInterface*);

/*
Envoie d'un char
*/
void LCDSendChar(LCDInterface*, char);

/*
Envoie d'un texte contenu dans data (ne pas exceder 32 characteres sans nettoyer l'ecran)
*/
void LCDSendText(LCDInterface* interface, char* data);





/********************************\
******Fonctions de reception******
\********************************/


/*
Change mode, if:
Direction == 1 => output
Direction == 0 => input
*/
void PinChangeMode(GPIO_TypeDef* port, uint16_t pin, char direction);


/*
Wait LCD available
*/
void WaitLCD(LCDInterface* interface);

#endif
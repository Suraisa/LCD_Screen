#ifndef MENU_H
#define MENU_H


#include <stdlib.h>
#include "lcd.h"


/********************************\
****Fonctions de gestion menu*****
\********************************/

/*
Permet de choisir le type du parametre
*/
typedef union{
    int entier;
    uint entierP;
    uint16_t entierP16;
    uint32_t entierP32;
    float flotant;
    double reel;
    char octet;
}Type;


/*
Type menu
*/
typedef struct l_menu{
    char text[16];
    Type parameter;
    char* parameter;
    char haveSub;
    struct l_menu *sub;
    struct l_menu *dad;
    struct l_menu *prev;
    struct l_menu *next;
}*Menu;


/*
Create menu
*/
Menu CreateMenu(char* textMenu);

/*
Add sub menu
*/
void AddSubMenu(Menu menu, char* subText);

/*
Add to a menu a parameter
if a menu have a parameter, it won't have a sub menu
choose a certain parameter with parameter
*/
void AddSubParameter(Menu menu, Type parameter, char*);

/*
Display menu
*/
void DisplayMenu(Menu menu, LCDInterface* interface);

void DisplayParameter(Menu);

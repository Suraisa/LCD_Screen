#include "menu.h"

Menu CreateMenu(char* textMenu){
    Menu menu = calloc(1,sizeof(*menu));
    strcpy(menu->text, textMenu);
}

void AddSubMenu(Menu menu, char* subText){
    menu->sub = CreateMenu(subText);
    menu->haveSub = 1;
    menu->sub->dad = menu;
}

void AddSubParameter(Menu menu, Type para, char* namePara){
    menu->parameter = para;
    menu->haveSub = 0;
}

void DisplayMenu(Menu menu, LCDInterface* interface){
    LCDSendText(interface, menu->text);
    if (menu->haveSub)
        DisplayMenu(menu->sub, interface);
    DisplayParameter(menu);
}

void DisplayParameter(Menu menu){
    
}
#include "lcd.h"


void LCDInterfaceInitialisation(LCDInterface* interface){
    (interface->pinout[0]).port = LCD_D0_GPIO_Port;
    (interface->pinout[0]).pin = LCD_D0_Pin;
    (interface->pinout[1]).port = LCD_D1_GPIO_Port;
    (interface->pinout[1]).pin = LCD_D1_Pin;
    (interface->pinout[2]).port = LCD_D2_GPIO_Port;
    (interface->pinout[2]).pin = LCD_D2_Pin;
    (interface->pinout[3]).port = LCD_D3_GPIO_Port;
    (interface->pinout[3]).pin = LCD_D3_Pin;
    (interface->pinout[4]).port = LCD_D4_GPIO_Port;
    (interface->pinout[4]).pin = LCD_D4_Pin;
    (interface->pinout[5]).port = LCD_D5_GPIO_Port;
    (interface->pinout[5]).pin = LCD_D5_Pin;
    (interface->pinout[6]).port = LCD_D6_GPIO_Port;
    (interface->pinout[6]).pin = LCD_D6_Pin;
    (interface->pinout[7]).port = LCD_D7_GPIO_Port;
    (interface->pinout[7]).pin = LCD_D7_Pin;
    (interface->pinout[8]).port = LCD_RW_GPIO_Port;
    (interface->pinout[8]).pin = LCD_RW_Pin;
    (interface->pinout[9]).port = LCD_RS_GPIO_Port;
    (interface->pinout[9]).pin = LCD_RS_Pin;
    (interface->pinout[10]).port = LCD_E_GPIO_Port;
    (interface->pinout[10]).pin = LCD_E_Pin;
    interface->data = 0;
}

void LCDSendChar(LCDInterface* interface, char c){
    interface->data = c;
    interface->instruction = WRITE;
    LCDSendInstruction(interface);
}

void LCDSendText(LCDInterface* interface, char* data){
    char *c = data;
    while(*c != '\0'){
        LCDSendChar(interface, *c);
        c++;
    }
}

void LCDSendInstruction(LCDInterface* interface){
    int i;
    WaitLCD(interface);
    HAL_GPIO_WritePin((interface->pinout[10]).port,(interface->pinout[10]).pin,1);
    for (i=0; i<10; i++){
        if (1<<i & (interface->instruction | interface->data)){
            HAL_GPIO_WritePin((interface->pinout[i]).port,(interface->pinout[i]).pin,1);
        }
        else{
            HAL_GPIO_WritePin((interface->pinout[i]).port,(interface->pinout[i]).pin,0);
        }
    }
    HAL_GPIO_WritePin((interface->pinout[10]).port,(interface->pinout[10]).pin,0);
    interface->data = 0;
}

void InstructionFunctionSet(LCDInterface* interface, char config[10]){
    interface->instruction = FUNCTIONSET;
    interface->data = (char)config[5]<<4|config[6]<<3|config[7]<<2;
    LCDSendInstruction(interface);
}

void InstructionFunctionSetStart(LCDInterface* interface, char config[10]){
    int i;
    InstructionFunctionSet(interface, config);
    HAL_Delay(5);

    for(i=0; i<3; i++){
        InstructionFunctionSet(interface, config);
        HAL_Delay(1);
    }
}

void InstructionDisplay(LCDInterface* interface, char config[10]){
    interface->instruction = DISPLAY;
    interface->data = (char)config[2]<<2|config[3]<<1|config[4]<<0;
    LCDSendInstruction(interface);
}

void InstructionEntryModeSet(LCDInterface* interface, char config[10]){
    interface->instruction = ENTRYMODESET;
    interface->data = (char)config[1]<<1|config[0]<<0;
    LCDSendInstruction(interface);
}

void InstructionClearDisplay(LCDInterface* interface){
    interface->instruction = CLEARDISPLAY;
    interface->data = 0;
    LCDSendInstruction(interface);
}

void InstructionReturnHome(LCDInterface* interface){
    interface->instruction = RETURNHOME;
    LCDSendInstruction(interface);
}

void LCDInit(LCDInterface* interface, char config[10]){
    LCDInterfaceInitialisation(interface);
    HAL_Delay(20);
    InstructionFunctionSetStart(interface, config);
    HAL_Delay(1);
    InstructionDisplay(interface, config);
    HAL_Delay(1);
    InstructionClearDisplay(interface);
    HAL_Delay(1);
    InstructionReturnHome(interface);
}

void InstructionCurseurPosition(LCDInterface* interface, char ddram){
    interface->instruction = SETDDRAM;
    interface->data = ddram;
    LCDSendInstruction(interface);
}

void PinChangeMode(GPIO_TypeDef* port, uint16_t pin, char direction){
    GPIO_InitTypeDef GPIO_InitStruct;
    if(direction){
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else{
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = pin;
    HAL_GPIO_DeInit(port, pin);
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}


void WaitLCD(LCDInterface* interface){    
    HAL_GPIO_WritePin(interface->pinout[8].port,interface->pinout[8].pin,1);
    HAL_GPIO_WritePin(interface->pinout[9].port,interface->pinout[9].pin,0);
    
    PinChangeMode(LCD_D7_GPIO_Port, LCD_D7_Pin, 0);
    HAL_GPIO_WritePin(interface->pinout[10].port,interface->pinout[10].pin,1);
    while(HAL_GPIO_ReadPin(interface->pinout[7].port,interface->pinout[7].pin)){
    }
    HAL_GPIO_WritePin(interface->pinout[10].port,interface->pinout[10].pin,0);

    PinChangeMode(LCD_D7_GPIO_Port, LCD_D7_Pin, 1);
}

void CursorDisplayShift(LCDInterface* interface, char config[10]){
    interface->instruction = SHIFT;
    interface->data = (char)config[8]<<3|config[9]<<2;
    LCDSendInstruction(interface);
}

void MoveScreen(LCDInterface* interface, char direction){
    CursorDisplayShift(interface, (char[10]){0,0,0,0,0,0,0,0,1,direction});
}

void MoveCursor(LCDInterface* interface, char direction){
    CursorDisplayShift(interface, (char[10]){0,0,0,0,0,0,0,0,0,direction});
}
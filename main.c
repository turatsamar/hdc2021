/*
 * main.c
 *
 *  Created on: 25 Jul 2023
 *      Author: turabek
 */




#include "driverlib.h"
#include "HDC2021.h"
#include <stdio.h>

#define SLAVE_ADDRESS 0x40

void I2C_Init(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    UCB0CTLW0 |= UCSWRST;

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN6 + GPIO_PIN7,
        GPIO_SECONDARY_MODULE_FUNCTION
    );

    GPIO_setAsInputPin(
           GPIO_PORT_P3,
           GPIO_PIN4);

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
                SLAVE_ADDRESS
                );
    PM5CTL0 &= ~LOCKLPM5;  //turn on I/O
    UCB0CTLW0 &= ~UCSWRST; //// turn off soft reset;

    //Set Master in receive mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
                EUSCI_B_I2C_TRANSMIT_MODE
                );

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

}

void clockSet(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    //Set DCO frequency to 1 MHz
        CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);
        //Set external clock frequency to 32.768 KHz
        CS_setExternalClockSource(32768,0);
        //Set ACLK=LFXT
        CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Set SMCLK = DCO with frequency divider of 1
        CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Set MCLK = DCO with frequency divider of 1
        CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
        //Start XT1 with no time out
        CS_turnOnLFXT(CS_LFXT_DRIVE_0);
}

bool uartInit(void)
{

    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P2,
    GPIO_PIN0 + GPIO_PIN1,
    GPIO_SECONDARY_MODULE_FUNCTION
    );

    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_ACLK;
    param.clockPrescalar = 3;
    param.firstModReg = 0;
    param.secondModReg = 0x92;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
        return false;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    return true;
}

void UART_SendByte(uint16_t byte)
{
    while (!(UCA0IFG & UCTXIFG)); // ÑGÑtÑuÑ} ÑsÑÄÑÑÑÄÑrÑ~ÑÄÑÉÑÑÑy ÑtÑ|Ñë ÑÅÑuÑÇÑuÑtÑpÑâÑy
    UCA0TXBUF = byte;
}
void UART_SendString(char* str)
{
    int i = 0;
    while (str[i] != '\0') {
        UART_SendByte(str[i]); // Send each character of the string
        i++;
    }
}



#define Data 0x80

int main(){

    char str[255];

    int16_t Da1;
	uint16_t Da2;
   // HDC2021DEPTErr err;


    clockSet();
    uartInit();
    I2C_Init();




     Hdc2021debt_Init();
     Hdc2021debt_SetInterrupt(HDC2021_INT_EN_DRDY, HDC2021_INT_POL_HIGH);
     Hdc2021debt_Start(HDC2021_CC_5SEC);

       while(1)
       {
    	    uint8_t status;
    	Hdc2021debt_Get_Status(&status);
        if(status & Data)
        {

        	Hdc2021debt_ReadTemp(&Da1);
              sprintf(str,"%d",Da1);
              UART_SendString("Temperature ");
              UART_SendString(str);

              UART_SendString("\n");
              Hdc2021debt_ReadHumidity(&Da2);
              UART_SendString("Humidity   " );
              sprintf(str,"%d",Da2);

              UART_SendString(str);
              UART_SendString("\n");
              UART_SendString("*******************************\n");

          }
       }




}





#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool finished = false;

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig;
    uartConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uartConfig.clockPrescalar = 19;
    uartConfig.firstModReg = 8;
    uartConfig.secondModReg = 0x55;
    uartConfig.parity = EUSCI_A_UART_NO_PARITY;
    uartConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uartConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uartConfig.uartMode = EUSCI_A_UART_MODE;
    uartConfig.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    uartConfig.dataLength = EUSCI_A_UART_8_BIT_LEN;

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION);


    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);


    // TODO: Enable EUSCI_A0
     UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {

        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)==EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
            char rChar = UART_receiveData(EUSCI_A0_BASE);
        }
        else{
            rChar = 0xFF;
        }
        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != 0xFF){
            if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)== EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG){
                UART_transmitData(EUSCI_A0_BASE,rChar);//push char to terminal
            }
            if(rChar == '2' || rChar == '5' || rChar == '3' || rChar == '4'){
            finished = charFSM(rChar);//update FSM
            }
        }

        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.

           // UART_transmitData(EUSCI_A0_BASE,response);
      if(finished == true){
          int len = sizeof(response);
          int i;
          for(i = 0; i<len; i++){
              if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)==EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG){
                  UART_transmitData(EUSCI_A0_BASE,response[i]);
              }
          }
          finished = false;
      }

    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    static bool status = false;
    static int count = 0;
    if(rChar == '2'){
        status = true;
        count = 1;
        return false;
    }
    else if(rChar == '5'){
       if(status == true && count == 1){
           count = 2;
        return false;
       }
       else{
        status = false;
        count = 0;
        return false;
       }
    }
    else if(rChar == '3'){
       if(status == true && count == 2){
           count = 3;
        return false;
       }
       else{
        status = false;
        count = 0;
        return false;
       }
    }
    else if(rChar == '4'){
       if(status == true && count == 3){
           status = false;
           count = 0;
        return true;
       }
       else{
           status = false;
           count = 0;
        return false;
       }
    }

    return false; //this means none of the expected chars
}

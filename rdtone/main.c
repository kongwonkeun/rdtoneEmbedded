/*
 * rdtone.c
 *
 * Created: 2019-03-11 13:30:29
 * Author : kong
 */


#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"

ISR(ADC_vect) {
    g_distance = ADCW;
    ADCSRA |= (1 << ADSC);
    txChar('D');
}

ISR(INT0_vect) {
    g_rotation++;
    txChar('I');
}

ISR(TIMER0_COMP_vect) {
    g_millisec++;
    if (!(g_millisec % 1000)) {
        txChar('T');
        g_sec++;
        if (!(g_sec % 60)) {
            g_minute++;
        }
    }
}

int main(void)
{
    unsigned char c;
    unsigned int  i;

    initConsole();
    initAdc0();
    initInt0();
    initTimer0();
    initBluetooth();
    initStdIo();

    sei();

    while (1)
    {
        i = readAdc0(DISTANCE_CHANNEL);
        txHexInt(i);
        txNewLine();
        i = calculateMilliVolt(i);
        txHexInt(i);
        txNewLine();
        i = calculateCentiMeter(i);
        txDecimalByte(i, 2);
        txString("cm\r\n");
        
        //distance = getDistance(DISTANCE_CHANNEL);
        //txHexInt(distance);
        //txNewLine();

        txString("enter\r\n");
        while (1) {
            if (rxBtCharAvailable()) {
                txChar(rxBtChar());
            }
            if (rxCharAvailable()) {
                c = rxChar();
                if (c == '!') break;
                txBtChar(c);
            }
        }
        txString("exit\r\n");
        c = rxChar();
    }
}



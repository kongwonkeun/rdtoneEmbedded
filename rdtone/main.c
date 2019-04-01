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
}

ISR(INT0_vect) {
    g_rotation++;
    txChar('R');
    txBtChar('R');
}

ISR(TIMER0_COMP_vect) {
    g_millisec++;
    if (!(g_millisec % 1000)) {
        txChar('T');
        txBtChar('T');
        g_sec++;
        if (!(g_sec % 60)) {
            g_minute++;
        }
        g_d = (unsigned char)(getDistance(DISTANCE_CHANNEL) & 0xff);
        txChar('D');
        txDecimalByte(g_d, 2);
        txBtChar('D');
        txBtDecimalByte(g_d, 2);
    }
}

int main(void)
{
    unsigned char c;
    unsigned char d;

    initConsole();
    initAdc0();
    initInt0();
    initTimer0();
    initBluetooth();
    initStdIo();

    sei();

    while (1)
    {
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
        cli();
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
        sei();
    }
}



/*
 * console.h
 *
 * Created: 2019-03-11 13:31:49
 *  Author: kong
 */


#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdio.h>

#define BAUDRATE 115200
#define BAUDRATE_CFG_VALUE(b,f) (((float)(f))/(((float)(b))*16.0)-1.0+0.5)

/* console uart */
#define CON_BAUD_LOW_REG UBRR1L
#define CON_STATUS_REG   UCSR1A
#define CON_CONTROL_REG  UCSR1B
#define CON_DATA_REG     UDR1
#define CON_ENABLE_TX    TXEN1
#define CON_ENABLE_RX    RXEN1
#define CON_TX_COMPLETE  TXC1
#define CON_RX_COMPLETE  RXC1

/* function prototypes */
void initStdIo(void);
int  outChar(char c, FILE *fp);
int  inChar(FILE *fp);

void initConsole(void);
void txChar(char c);
void txNewLine(void);
void txString(char *str);
void txDecimalByte(int data, int digit);
void txHexByte(unsigned char data);
int  rxCharAvailable(void);
unsigned char rxChar(void);

/* functions */
void initStdIo() {
    FILE *fpStdIo = fdevopen(outChar, inChar);
}

int  outChar(char c, FILE *fp) {
    CON_DATA_REG = c;
    while (!(CON_STATUS_REG & (1 << CON_TX_COMPLETE))); //wait until the char is sent
    CON_STATUS_REG |= (1 << CON_TX_COMPLETE); // clear TXC flag
    return 0;
}

int  inChar(FILE *fp) {
    while (!(CON_STATUS_REG & (1 << CON_RX_COMPLETE))); // wait for received char
    return CON_DATA_REG;
}

void initConsole() {
    CON_BAUD_LOW_REG = BAUDRATE_CFG_VALUE(BAUDRATE, F_CPU);
    CON_CONTROL_REG  = (1 << CON_ENABLE_TX) | (1 << CON_ENABLE_RX);
}

void txChar(char c) {
    CON_DATA_REG = c;
    while (!(CON_STATUS_REG & (1 << CON_TX_COMPLETE))); //wait until the char is sent
    CON_STATUS_REG |= (1 << CON_TX_COMPLETE); // clear TXC flag
}

void txNewLine() {
    txChar(0x0d);
    txChar(0x0a);
}

void txString(char *str) {
    char c = 1;
    int  i = 0;
    while (c != 0) {
        c = str[i++];
        if (c != 0) txChar(c);
    }
}

void txDecimalByte(int data, int digit) {
    char dec;
    int  num;
    num = data;
    if ((num >= 100) || (digit >= 3)) {
        dec = 0x30 + (num / 100);
        txChar(dec);
    }
    if ((num >= 10)  || (digit >= 2)) {
        dec = 0x30 + (num / 10);
        txChar(dec);
    }
    dec = 0x30 + (num % 10);
    txChar(dec);
}

void txHexByte(unsigned char data) {
    char hex;
    hex = 0x30 + ((data >> 4) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
    hex = 0x30 + ((data) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
}

void txHexInt(unsigned int data) {
    char hex;
    hex = 0x30 + ((data >> 12) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
    hex = 0x30 + ((data >> 8) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
    hex = 0x30 + ((data >> 4) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
    hex = 0x30 + ((data) & 0x0f);
    if (hex > 0x39) hex += 7;
    txChar(hex);
}

int  rxCharAvailable() {
    return (CON_STATUS_REG & (1 << CON_RX_COMPLETE));
}

unsigned char rxChar() {
    while (!(CON_STATUS_REG & (1 << CON_RX_COMPLETE))); // wait for received char
    return CON_DATA_REG;
}


#endif /* CONSOLE_H_ */
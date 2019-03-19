/*
 * bluetooth.h
 *
 * Created: 2019-03-12 11:49:16
 *  Author: kong
 */


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define BT_BAUDRATE 9600
#define BT_BAUDRATE_CFG_VALUE(b,f) (((float)(f))/(((float)(b))*16.0)-1.0+0.5)

/* bluetooth uart */
#define BT_BAUD_LOW_REG UBRR0L
#define BT_STATUS_REG   UCSR0A
#define BT_CONTROL_REG  UCSR0B
#define BT_DATA_REG     UDR0
#define BT_ENABLE_TX    TXEN0
#define BT_ENABLE_RX    RXEN0
#define BT_TX_COMPLETE  TXC0
#define BT_RX_COMPLETE  RXC0

/* function prototypes */
void initBluetooth(void);
void initBluetoothBaud(unsigned long baud);
void txBtChar(char c);
void txBtNewLine(void);
void txBtString(char *str);
void txBtDecimalByte(int data, int digit);
void txBtHexByte(unsigned char data);
int  rxBtCharAvailable(void);
unsigned char rxBtChar(void);

/* functions */
void initBluetooth() {
    BT_BAUD_LOW_REG = BT_BAUDRATE_CFG_VALUE(BT_BAUDRATE, F_CPU);
    BT_CONTROL_REG  = (1 << BT_ENABLE_TX) | (1 << BT_ENABLE_RX);
}

void initBluetoothBaud(unsigned long baud) {
    BT_BAUD_LOW_REG = BT_BAUDRATE_CFG_VALUE(baud, F_CPU);
    BT_CONTROL_REG  = (1 << BT_ENABLE_TX) | (1 << BT_ENABLE_RX);
}

void txBtChar(char c) {
    BT_DATA_REG = c;
    while (!(BT_STATUS_REG & (1 << BT_TX_COMPLETE))); //wait until the char is sent
    BT_STATUS_REG |= (1 << BT_TX_COMPLETE); // clear TXC flag
}

void txBtNewLine() {
    txBtChar(0x0d);
    txBtChar(0x0a);
}

void txBtString(char *str) {
    char c = 1;
    int  i = 0;
    while (c != 0) {
        c = str[i++];
        if (c != 0) txBtChar(c);
    }
}

void txBtDecimalByte(int data, int digit) {
    char dec;
    int  num;
    num = data;
    if ((num >= 100) || (digit >= 3)) {
        dec = 0x30 + (num / 100);
        txBtChar(dec);
    }
    if ((num >= 10)  || (digit >= 2)) {
        dec = 0x30 + (num / 10);
        txBtChar(dec);
    }
    dec = 0x30 + (num % 10);
    txBtChar(dec);
}

void txBtHexByte(unsigned char data) {
    char hex;
    hex = 0x30 + ((data >> 4) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
    hex = 0x30 + ((data) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
}

void txBtHexInt(unsigned int data) {
    char hex;
    hex = 0x30 + ((data >> 12) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
    hex = 0x30 + ((data >> 8) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
    hex = 0x30 + ((data >> 4) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
    hex = 0x30 + ((data) & 0x0f);
    if (hex > 0x39) hex += 7;
    txBtChar(hex);
}

int  rxBtCharAvailable() {
    return (BT_STATUS_REG & (1 << BT_RX_COMPLETE));
}

unsigned char rxBtChar() {
    while (!(BT_STATUS_REG & (1 << BT_RX_COMPLETE))); // wait for received char
    return BT_DATA_REG;
}


#endif /* BLUETOOTH_H_ */
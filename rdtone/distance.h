/*
 * distance.h
 *
 * Created: 2019-03-11 15:50:37
 *  Author: kong
 */ 


#ifndef DISTANCE_H_
#define DISTANCE_H_

#define DISTANCE_CHANNEL 0

/* function prototypes */
void initAdc0(void);
unsigned int readAdc0(unsigned char channel);
unsigned int calculateMilliVolt(unsigned int value);
unsigned int calculateCentiMeter(unsigned int millivolt);
unsigned int getDistance(unsigned char channel);


/* functions */
void initAdc0() {
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler(128) --> 16MHz/128 = 125KHz
    ADMUX  |= (1 << REFS0); // Ref: AVCC, right adjust, ADC0
    ADCSRA |= (1 << ADEN);  // adc enable
    //ADCSRA |= (1 << ADIE);  // adc interrupt enable
    //ADCSRA |= (1 << ADSC);  // adc start conversion
}

unsigned int readAdc0(unsigned char channel) {
    ADMUX  &= 0xf0;
    ADMUX  |= channel;
    ADCSRA |= (1 << ADSC);  // start the first conversion
    while (ADCSRA & (1 << ADSC)); // wait until the conversion is complete
    return ADCW;
}

unsigned int calculateMilliVolt(unsigned int value) {
    unsigned long maxValue = 1023; // adc max value
    unsigned long maxMilliVolt = 5000; // 5 volt
    return (maxMilliVolt * value) / maxValue;
}

unsigned int calculateCentiMeter(unsigned int millivolt) {
    unsigned long centimeter = 1000;
    //return (centimeter * 27.61) / (millivolt - 0.1696); // y[cm] = 27.61/(x[volt] - 0.1696) 
    //return (centimeter * 27.61) / (millivolt);
    return (centimeter * 23) / (millivolt + 50);
}

unsigned int getDistance(unsigned char channel) {
    unsigned int millivolt = calculateMilliVolt(readAdc0(channel));
    return calculateCentiMeter(millivolt);
}


#endif /* DISTANCE_H_ */
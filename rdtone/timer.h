/*
 * timer.h
 *
 * Created: 2019-03-18 11:49:34
 *  Author: kong
 */ 


#ifndef TIMER_H_
#define TIMER_H_

/* function prototypes */
void initTimer0(void);

/* functions */
void initTimer0() {
    TCCR0 |= (1 << CS02);  // prescaler 64
    TCCR0 |= (1 << WGM01); // ctc mode
    TCNT0  = 0;
    OCR0   = 249; // 1ms
    TIMSK |= (1 << OCIE0);
}


#endif /* TIMER_H_ */
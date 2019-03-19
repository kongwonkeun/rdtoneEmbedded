/*
 * rotation.h
 *
 * Created: 2019-03-11 18:22:10
 *  Author: kong
 */ 


#ifndef ROTATION_H_
#define ROTATION_H_

/* function prototypes */
void initInt0(void);

/* functions */
void initInt0() {
    EICRA |= (1 << ISC01) | (1 << ISC00); // rising edge
    EIMSK |= (1 << INT0);
}


#endif /* ROTATION_H_ */
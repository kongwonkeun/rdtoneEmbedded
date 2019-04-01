/*
 * main.h
 *
 * Created: 2019-03-13 11:46:01
 *  Author: kong
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "console.h"
#include "distance.h"
#include "rotation.h"
#include "bluetooth.h"
#include "timer.h"

/* function prototypes */

/* functions */

/* global variables */
unsigned int g_millisec = 0;
unsigned int g_millisec_save = 0;
unsigned int g_sec = 0;
unsigned int g_sec_save = 0;
unsigned int g_minute = 0;
unsigned int g_rotation = 0;
unsigned int g_ratation_save = 0;
unsigned int g_distance = 0;

unsigned char g_d;


#endif /* MAIN_H_ */
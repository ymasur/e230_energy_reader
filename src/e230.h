/*  e230.h
	  ------
	Definitions part

	YM: 24.12.2020 - adapted from e230_5.ino, from JM Paratte
  29.12.2020 - e230 dynamic alloc RAM:   [=======   ]  65.9% (used 1686 bytes from 2560 bytes)
                                  Flash: [========= ]  90.0% (used 25796 bytes from 28672 bytes)
  30.12.2020 - print(), store(), menu 0, 1, 2, 5 OK
    RAM:   [=======   ]  65.1% (used 1667 bytes from 2560 bytes)
    Flash: [========= ]  93.4% (used 26766 bytes from 28672 bytes)    
*/

#ifndef E230_H
#define E230_H

// Storage class of common variables, always in main module
#ifdef MAIN
  #define CLASS // intern...
#else
  #define CLASS extern
#endif

#define __PROG__ "e230_05 VS-CODE Yun"
#define VERSION "05.016" // program version
/*
  with dynamic alloc of E230_S
*/

#ifndef _WIN32
#include <Arduino.h>
#include <SoftwareSerial.h>
/*
  #include <Bridge.h>
  #include <BridgeServer.h>
  #include <BridgeClient.h>
*/
#include <FileIO.h>
#include <avr/wdt.h>
#include <string.h>
#include <Wire.h>
#include <time.h>
#include <RTClib.h>
// #include <jm_Scheduler.h>
#include <jm_LCM2004A_I2C.h>

#else // _WIN32
#warning "only for test purpose"
#include <stdio.h>
#include <string>
#include <stdlib.h>     /* atof */

#endif // _WIN32 not defined


// -----------------------------------------------------------------------------

// https://www.arduino.cc/reference/en/language/functions/communication/serial/

// Arduino Yún
// -----------

// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

// TX, RX, SWRX - D13, SWTX - D5


#define STREAM_OUT_IS_KEYBOARD (false)
#define STREAM_OUT_ALSO_DEBUG (false)
#define STREAM_IN SwSerial
#define STREAM_OUT Serial

#ifndef _WIN32
#include <SoftwareSerial.h>

#include <AltSoftSerial.h>
CLASS AltSoftSerial SwSerial;
// #warning Alternate serial ON

CLASS Stream * stream_in
#ifdef MAIN
= & STREAM_IN;
#else
;
#endif

CLASS Stream * stream_out
#ifdef MAIN
 = & STREAM_OUT;
#else
;
#endif

#else // WIN32 print out

 // WIN32 Print part, used for simulation/test
#include <stdio.h>

 // Arduino use dtostrf(), unused in WIN32. This macro expand into a compatible sprintf()
#define dtostrf(VAR, UNIT, DECIM, BUF) (sprintf(BUF, "%" #UNIT "." #DECIM "f", VAR))

#define sprintf sprintf_s // Visual Studio 2019 need this implementation

class Print_out
{
    public:
    Print_out(){};
	void print(const char * p){ printf(p); };
    void println(const char *p){ printf(p); printf("\n"); };
};

CLASS Print_out print_out;
CLASS Print_out * stream_out
#ifdef MAIN
 = &print_out;
#else
;
#endif  // WIN32 print out

#endif // ! _WIN32
#define E230_BUF_SZ (514)   // size of input buffer
#include "e230_05.hpp"
#include "e_calc.hpp"
#include "e_menu.h"

  
#define ON 1
#define OFF 0
#define LED_J A2
#define LED_R A3
#define LED_ON(x) digitalWrite(x, 1)
#define LED_OFF(x) digitalWrite(x, 0)
#define LED13 13  // on board LED
// SW on Didel LearnCbot
#define SW1 A0   // SW1 « P1 » port Analog 0 / actif LOW
#define SW2 A1   // SW2 « P2 » port Analog 1 / actif LOW
#define SW_NB 2   // only 2 switches to scan


// time given by RTC, or Unix cmd              1         2
//                           012345678901234567890
// asci format, as          "2019-08-29 22:10:42"
#define DT_LENGHT 20
#define TIME_S 18 // offset where the second digit is, in string dateTimeStr
CLASS char dateTimeStr[DT_LENGHT+1]; 

// LCD
CLASS jm_LCM2004A_I2C * lcd;

// RTC
CLASS RTC_DS3231 rtc;
CLASS DateTime myTime; // used to maintain time operations
CLASS DateTime bootTime; // simple copy to know it

// freemem
#define LOW_SRAM_ALERT 200  // Normal use : 910..965 left

// global vars are defined here

#define NAME_LENGHT 32  // sufficient for path/name
//                          1         2         3
//                0123456789012345678901234567890
CLASS char fname[NAME_LENGHT+1]
#ifdef MAIN
//                          1         2         3
//                012345678901234567890123456789012
               = "/mnt/sd/arduino/www/YYMMengy.txt";
               
//conservative filename 8.3 format
//the four chars 'YYMM' are replaced by year and month, as 2012
#else
;
#endif

#define OFFSET_YYMM 20 // offset used to modify the filename: YYMM

// display def and vars
#define LN0 0x01
#define LN1 0x02
#define LN2 0x04
#define LN3 0x08
#define LN_all 0x0F // for LCD lines to be copied onto Serial

// menu used vars
CLASS char menu;
CLASS char menu_old;
CLASS bool menu_changed;

// time, error, storage vars
CLASS bool err_file;
CLASS byte ser_copy;
CLASS byte tempo_msg;
#define TEMPO_MSG_VAL 15
CLASS bool err_act;
// CLASS char err_msg[21];
CLASS DateTime data_time;
//CLASS char datas_values[21];


//CLASS E230_S *p_e230;


// -----------------------------------------------------------------------------


// Function prototype (VS-CODE)

void setup();
void poll_loop_1_s();
void poll_loop_X_ms();
bool IsSyncTime_15_minutes();

// e230 module
char * get_e230_buf();
void get_all_values(char * p_buf);
// void print_data_values(char * p_buf);
void data_print();


void display_menu();
void menu_select();
//void display_info(String);
void display_info(const char *, u8=10);
#define TEMPO_MSG_VAL 15  //
//void display_info(String info, u8 val=TEMPO_MSG_VAL);
//void display_at_ln(String info, u8 ln=1); // Line 0 is alway used for date/time
void display_clock();
void display_datas();
void display_last_rec(char * rec);

void display_log(const char * log);
void display_vers(const char * vers);
void display_at_ln(const char * info, u8 ln=1); 
void display_full_ln(const char * info, u8 ln=1);

//CLASS char last_rec[21]; 
CLASS char last_log[21];
//CLASS char last_msg[21];

//void log_err(String msg); // with flag err_act set, onto SD, display and serial
//void log_msg(String msg); // onto SD, display and serial
//void log_info(String msg); // onto display for short time and serial
void log_err(const char * msg); // with flag err_act set, onto SD, display and serial
void log_msg(const char * msg); // onto SD, display and serial
void log_info(const char * msg); // onto display for short time and serial

void store_datas(char *fname, DateTime dt);
void log_msg_SD(const char * msg);

// e_time prototype
void timeSyncInit();
void timeSyncStart();
void timeSync();
void tm_to_ascii(DateTime *dt, char *dateTimeStr);
bool IsSyncTime_10_seconds();
bool IsSyncTime_10_minutes();
bool IsSyncTime_03h00();

// utility
int freeMemory();


#endif // e230_h

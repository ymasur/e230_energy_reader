/* 	e_main.cpp
	----------
	YM: 31.12.2020
*/
#ifndef MAIN
#define MAIN

#include <Arduino.h>
#include <Bridge.h>
//#include <BridgeServer.h>
//#include <BridgeClient.h>
#include "e230.h"
//#include "e230_05.hpp"
#include "e_menu.h"


enum proc { wait, ask, rec, calc };
u8 pstate;  // aqu process states
bool time_to_store, store_done; // storage process control
bool print_val; // cmd 'p' active print command
E230_S * p_e230;  // pointer for instance of E230_S object

// -----------------------------------------------------------------------------

// Bridge instances
// BridgeServer server;
// BridgeClient client;

// Schedulers instances
//jm_Scheduler pulse_X_ms;
//jm_Scheduler pulse_1_s;

/* blink(short n=1, short t=1)
   ---------------------------
   Blink n times, with impulse cycle t (1/10 sec)
   *** Used only in setup operations ***
   I/O used: LED13
   return: -
*/
/*
void blink(short n=1, short t=1)
{
  for (short i=0; i<n; i++)
  {
    digitalWrite(LED13, LOW);
    delay(50 * t);
    digitalWrite(LED13, HIGH);
    delay(50 * t);
  }
}
*/

// log the error message 
void log_err(const char * msg)
{
  err_act = true; // make the error flag active (removed by menu)
  //strncpy(err_msg, msg, sizeof(err_msg) );
  log_msg(msg);
}

// log the message and take the last one for reference
#if 0
void log_msg(const char * msg)
{
 
  strncpy(last_log, msg, sizeof(last_log)-1);   // copy the message
  Serial.print("#log msg# "); Serial.println(msg);
  display_at_ln(msg);
  log_msg_SD(msg);
}
#endif

// spare memory: log_msg -> log_info
#define log_msg(MSG) log_info(MSG)

// display an info for a short time, and not memorized
void log_info(const char * msg)
{
  Serial.print(F("##info:")); Serial.println(msg);
  tempo_msg = 10;
  display_at_ln(msg);
  //log_msg_SD(msg);
}

void set_actual_time()
{
  myTime = rtc.now(); // get the RTC time
  tm_to_ascii(&myTime, dateTimeStr);
}

/* ----------------------------------------------------- */
void setup()
{
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_J, OUTPUT);
  pinMode(LED_R, OUTPUT);  

  LED_ON(LED_R);

	menu = 0; // clear menu setting
  ser_copy = 0;

	// init an array of button control
	sw[0] = new Sw(SW1);
	sw[1] = new Sw(SW2);

  // I2C
  Wire.begin();


	// LCD
	lcd = new jm_LCM2004A_I2C();  // Add. 0X27 standard
	lcd->begin();  lcd->clear_display();
	lcd->print(F(__PROG__ " " VERSION));  // on line 0
  lcd->print(F("LCD init done..."));     delay(2000);

	// we use serial for log messages
	Serial.begin(9600); delay(3000);
	Serial.println(F("Serial started..."));	
  

	// RTC, start
	display_at_ln("Start RTC, read.... ",2); //delay(2000);

	if (! rtc.begin()) 
	{
		display_full_ln("Couldn't find RTC",2); delay(3000);
	}

	// timing vars initialized
	set_actual_time();
	timeSyncInit();
	timeSyncStart();

  menu = 0; // clear menu setting

  // Bridge startup
  Bridge.begin();

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  //server.listenOnLocalhost();
  //server.begin();

  FileSystem.begin();

	// set stream_in hardware...
#if defined AltSoftSerial_h
	STREAM_IN.begin(300);
#else
	STREAM_IN.begin(300, SERIAL_7E1);
#endif
  display_full_ln("Serial e230 started",3); 

	// stabilize STREAM_IN and STREAM_OUT RX/TX lines after reset/power up...
	delay(1000);

	err_act = false;  // no error

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
#if 0
	server.listenOnLocalhost();
	server.begin();
#endif

	//e230.begin();	
  //display_full_ln("e230 begin",1); //delay(1000);

  log_msg_SD(__PROG__ " " VERSION );

  time_to_store = false;

  LED_OFF(LED_R);

} // end setup()

/*  void serial_cmd()
    -----------------
    Get key onto serial
    Global var modified:
    - menu
    - ser_copy

    The menu number is modified as the hardware switch[+] or [-].
    Limits and display is managed by e_menu.cpp
    The ser_copy byte use 4 bits as flags to control witch line of display (0..3)
    should be copied onto serial.
*/

void serial_cmd()
{
  int c;
  c = Serial.read();
  if (c == -1)
    return;

  //Serial.println(c);
  menu_changed = true;

  switch(c)
  {
    case '+':
      menu++;
      ser_copy = LN_all;        
    break;

    case '-':
      menu--;
      ser_copy = LN_all;  
    break;

    case '0': // menu base - energie actuelle
      menu = 0;
      ser_copy = LN_all;      
    break;

    case '1': // condensé énergie cons - prod - U - I - P
      menu = 1;
      ser_copy = LN1+LN2+LN3;
    break;

    case '2': // U, I, P des 3 phases
      menu = 2;
      ser_copy = LN1+LN2;      
    break;

    case '3': // dernier enreg.
      menu = 3;
      ser_copy = LN1+LN2;        
    break;

    case '4': // dernier log
      ser_copy = LN1+LN2+LN3;      
      menu = 4;
    break;

    case '5': // dernier boot
      ser_copy = LN1+LN2+LN3;      
      menu = 4;
    break;

    case ' ':
      ser_copy = LN_all;
    break;

    case 'd':
      Serial.println(F("Store datas"));
    break;

    case 'm':
      Serial.print(F("\nmem free:")); Serial.println(freeMemory());
    break;

    case 'p':
      print_val = true;
      //data.print();
    break;

    case 't':
    Serial.println(dateTimeStr);
    break;

    case '?':
    default:
      //ser_copy = 0;
      //cmd = 0;
      Serial.println(F("\ncmd: +, -, menu 1, 2, 3, 4, 5."));
    break;
  }
} // serial_cmd()


/* ------------------------------------------*/

void poll_loop_1_s()
{
  set_actual_time(); // take the actual time

  //Serial.println(dateTimeStr);
/*
  if (IsSyncTime_03h00())
    timeSyncStart();

  timeSync();
*/
  
  Serial.println();
  Serial.print(freeMemory());Serial.print(F(":state:")); Serial.println(pstate);

  if (IsSyncTime_10_seconds() && pstate == wait)
  {
    if (p_e230 == NULL)
    p_e230 = new(E230_S);
    Serial.println(freeMemory());

    if (p_e230)
    {
      p_e230->begin();
      p_e230->start();
      pstate = ask;
      Serial.println(F(" ***Start*** "));
    }

  }
  else if(!p_e230->ready() && pstate == ask)
  {
    pstate = rec;
    Serial.println(F(" get values"));
  }
  else if(p_e230->ready() == true && p_e230->errored() == false && pstate == rec)
  {
    Serial.println(F(" calc values"));
    get_all_values(p_e230->_buf);
    pstate = calc;
  }
  else if(pstate == calc)
  {
    delete(p_e230); p_e230 = NULL;
    Serial.println(F(" ***Wait"));

    pstate = wait;
  }
  else if(pstate == wait)
  {
    if (IsSyncTime_15_minutes())  // Q: time window to store data?
    {                             // A: Yes, set the marker
      if (!time_to_store)
      {
        Serial.println(F(" ***Store"));
        time_to_store = true;         
      }
      LED_ON(LED_R); 
    }
    else                  // A: no, reset marker
    {
      if (time_to_store)  // Q: flag active?
      {
        time_to_store = false; // A: yes, clear flag
        LED_OFF(LED_R);        
      }
    }

    if (print_val == true)
    {
      data.print();
      print_val = false;
    }
  }// psate == wait

  if (pstate)
    LED_ON(LED_J);
  else
    LED_OFF(LED_J);

/* ----------------------------------------------------- */
  if (time_to_store) // Q: is time to store datas?
  {
    if (!store_done)  // Q: already done?
    {                 // A: no, store datas
      data_time = myTime; // copy of the time
      store_datas(fname, data_time); 
      data.energy_diff();    // compute the delta energy
      store_done = true;
    }
  }
  else
  { // out of window time for storage
    store_done = false;
  }

/* ----------------------------------------------------- */

  display_menu(); // rest of display depends of the menu choice

  //digitalWrite(LED13, !digitalRead(LED13)); // life monitoring - blink LED13
}


/*  poll_loop_X_ms()
    ----------------
    Compute the state of switches
    Modified var: intern of object Sw.
    The polling time must be between 10..50 ms
    Return value: -
*/
void poll_loop_X_ms()
{
  // scan all switches
  for(short i = 0; i < SW_NB; i++)
  {
    sw[i]->scan();
  }
    
  menu_select(); // follow user choice
  serial_cmd();
}

/* main loop of Arduino
*/
#define SHORT_CYCLE 20  // 20 ms
#define MAIN_CYCLE 1000  // 1 sec.

void loop()
{
  static unsigned long fastPreviusMs, mainPreviusMs;

  unsigned long currentMs = millis();
  if (currentMs - fastPreviusMs > SHORT_CYCLE)
  {
    fastPreviusMs = currentMs;
    poll_loop_X_ms();
  }

  if (currentMs - mainPreviusMs > MAIN_CYCLE)
  {
    mainPreviusMs = currentMs;
    poll_loop_1_s();
  }  

  //if (p_e230 != NULL)	  p_e230->cycle();
  if (pstate)	  p_e230->cycle();

	yield();
  
}// loop()

#endif // MAIN

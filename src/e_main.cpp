/* 	e_main.cpp
	----------
	YM: 31.12.2020
  01.01.2021: add min stack
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


enum proc { wait, ask, rec, getv, calc };
u8 pstate;  // aqu process states
bool time_to_store, store_done; // storage process control
bool print_val; // cmd 'p' active print command
//E230_S * p_e230;  // pointer for instance of E230_S object

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
  //strncpy(last_log, msg, sizeof(last_log) );
  // log_info(msg);
  // display_at_ln(msg);
  //log_msg_SD(msg);
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

// display an info for a short time, and not memorized
void log_info(const char * msg)
{
  Serial.print(F("##info:")); Serial.println(msg);
  tempo_msg = 10;
  display_at_ln(msg);
}


/* ----------------------------------------------------- */

void setup()
{
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED_J, OUTPUT);
  pinMode(LED_R, OUTPUT);  

  LED_ON(LED_R);

	// init an array of button control
	sw[0] = new Sw(SW1);
	sw[1] = new Sw(SW2);

  min_stack = 9999;
	menu = 0; // clear menu setting
  ser_copy = 0;

  // I2C
  Wire.begin();

	// LCD
	lcd = new jm_LCM2004A_I2C();  // Add. 0X27 standard
	lcd->begin();  lcd->clear_display();
	lcd->print(F(__PROG__ "  " VERSION));  // on line 0
  display_full_ln("LCD init done", 1);     

	// we use serial for log messages
	Serial.begin(9600); 
	display_full_ln("Serial started", 2);

  delay(3000);

  // Bridge startup
  Bridge.begin();

  display_full_ln("Read time...", 3);
  strcpy(dateTimeStr, ("2021")); // assure century
  getTimeStamp(dateTimeStr+2, sizeof(dateTimeStr)-3);

  FileSystem.begin();

	// set stream_in hardware...
#if defined AltSoftSerial_h
	STREAM_IN.begin(300);
#else
	STREAM_IN.begin(300, SERIAL_7E1);
#endif
  display_full_ln("Serial e230 started", 1); 

  p_e230 = new(E230_S);
  if (p_e230 != NULL)
    display_full_ln("e230 instancied", 2);

	// stabilize STREAM_IN and STREAM_OUT RX/TX lines after reset/power up...
	delay(3000);

	err_act = false;  // no error

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
#if 0
	server.listenOnLocalhost();
	server.begin();
#endif

  log_msg_SD( __PROG__ " " VERSION );

  time_to_store = false;

  menu = 0; // clear menu setting

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

    case 's':
    Serial.print(F("\nStack min:")); Serial.println(min_stack);
    break;

    case 't':
    Serial.println(dateTimeStr);
    break;

    case 'x': // force termination
      pstate = getv;
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


  // get the minimum aivable Ram space
  if (min_stack > freeMemory())
      min_stack = freeMemory();

}


static unsigned char rec_count;

void poll_loop_1_s()
{
  getTimeStamp(dateTimeStr+2, sizeof(dateTimeStr)-3);

  Serial.print(dateTimeStr);
  Serial.print(F(" pstate:")); Serial.println(pstate);

  if (IsSyncTime_x_seconds(15) && pstate == wait)
  {
    p_e230->begin();
    p_e230->start();
    rec_count = 0;
    Serial.println(F(" ***Start "));
    LED_OFF(LED_R);
    pstate = ask;
  }

  else if(!p_e230->ready() && pstate == ask)
  {
    Serial.println(F(" get values"));
    pstate = rec;    
  }
    
  else if (pstate == rec )
  {
    if (rec_count++ > 6)
    {
      log_info("Read timeout!");
      rec_count = 0;
      pstate = getv;
      Serial.println("*E*");
    }
    
    Serial.print(p_e230->e_state()); Serial.print(" ! "); 
    Serial.print(p_e230->started()); Serial.print(" ! ");
    if (p_e230->errored())
      { 
        log_info("Read error!");
        err_act = true; 
        pstate = wait;
      }

      if (p_e230->ready())
        pstate = getv;
  }

  else if (p_e230->started() == false && pstate == getv)
  {
    Serial.println(F(" get values"));
    get_all_values(p_e230->_buf);
    err_act = false;
    pstate = calc;
  }
  
  else if(pstate == calc)
  {
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
    }
    else                  // A: no, reset marker
    {
      if (time_to_store)  // Q: flag active?
      {
        time_to_store = false; // A: yes, clear flag   
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


  if (err_act)
    LED_ON(LED_R);
  else
    LED_OFF(LED_R);
  
/* ----------------------------------------------------- */

  if (time_to_store) // Q: is time to store datas?
  {
    if (!store_done)  // Q: already done?
    {                 // A: no, store datas
      store_datas(fname); 
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

}

/* main loop of Arduino
*/
#define SHORT_CYCLE 20  // 20 ms
//#define MAIN_CYCLE 1000  // 1 sec.
#define MAIN_CYCLE 1000

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

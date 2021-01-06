/*
  e_time.cpp
  ----------
  20.12.2020 - ymasur@microclub.ch
  01.01.2021 - YM: sup. RTC (NTP only)
*/
#include <Arduino.h>
#include "e230.h"

// take the Unix time of the Yun
// time given by Unix cmd              1         2
//                           012345678901234567890
// asci format, as          "2020-08-29 22:10:42"

/*  getTimeStamp(char *p, short len)
    --------------------------------
    This function fill a string with the time stamp
    Vars used:
    - *p : pointer to the destination str
    - len: max number of char (with final 0)
    
    returned value:
    - number of chr written
*/
int getTimeStamp(char *p=dateTimeStr+2, short len=19)
{
  short i = 0;
  
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin(F("date"));
  // parameters: for the complete date yy-mm-dd hh:mm:ss
  time.addParameter(F("+%y-%m-%d %T"));
  time.run(); // run the command

  // read the output of the command
  while (time.available() > 0 && i < len)
  { 
    char c;

    c = time.read();
    if (c != '\n')
      p[i] = c;
    i++;

    if (p_e230 != NULL) 
      p_e230->cycle();
  }
  p[i] = '\0'; // end of the collected string

  return i;
}

// convert ASCII values to int (see position of digit)
int second(){return (dateTimeStr[17]-'0')*10 + (dateTimeStr[18]-'0'); }
int minute(){return (dateTimeStr[14]-'0')*10 + (dateTimeStr[15]-'0'); }

bool IsSyncTime_x_seconds(uint8_t s)
{
  if (second() % s == 0)
    return true;
  else
    return false;
}

bool IsSyncTime_15_minutes()  // full minute !
{
  if (minute() % 15 == 0)
    return true;
  else
    return false;
}

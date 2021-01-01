/*
  e_time.cpp
  ----------
  20.12.2020 - ymasur@microclub.ch
*/
#include <Arduino.h>
#include "e230.h"

#define DEBUG 0
#define MIN_CORRECTION 3  // in seconds

// take the Unix time of the Yun
#define TIME_MSG_LEN 20
#define UNIX_START_TIME 180 // time to boot and get Internet in seconds
uint16_t timeSyncStep;

/*  getTimeStamp(char *p, short len)
    --------------------------------
    This function fill a string with the time stamp
    Vars used:
    - *p : pointer to the destination str
    - len: max number of char (with final 0)
    
    returned value:
    - number of chr written
*/
int getTimeStamp(char *p, short len)
{
  short i = 0;
  
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  // parameters: for the complete date yy-mm-dd hh:mm:ss
  time.addParameter("+%y-%m-%d %T");
  log_msg(("Get NTP time...     "));
  time.run(); // run the command

  // read the output of the command
  while (time.available() > 0 && i < len)
  { 
    char c;

    c = time.read();
    if (c != '\n')
      p[i] = c;
    i++;
  }
  p[i] = '\0'; // end of the collected string

  return i;
}

// No usage of Unix time - RTC only
void timeSyncInit()
{
  timeSyncStep = 0;
}

// Start of sync with Unix time, after UNIX_START_TIME
void timeSyncStart()
{
  timeSyncStep = 1;
}

/*  void timeSync()
    ---------------
    The RTC can be (re)synchronised by the Unix networlk time protocol (ntp).
    First, call the time stamp of the Yun.
    The timeSyncStep var is set to 1 at 03:00:00, and the sync process is done.
    Then, it is incremented at each call, each second. After 1 hour (at 04:00:00), it can be zeroed.
    This mechanismus avoid the dobble sync at 03:00:00 in case of rtc is in advance; and at the change of 
    summer/winter time.

    The call take caution of the Unix time: it must be greather than Unix compile time.
    This is not the case if the Yun starts and does'nt get Internet connection.
    Var used:
    - timeSyncStep, used to have a windows of the sync time
    - rtc struct, corrected with the ntp source of time

    return value: -
*/
void timeSync()
{
  char ntpStr[TIME_MSG_LEN+2];  //string to store the ntp time
  long sec_correction = 0L;     // sign + if ntp is greater as rtc time

  if (timeSyncStep == 0)  // Q: Sync not started?
    return;               // A: yes, nothing to do

  if (timeSyncStep++ != UNIX_START_TIME) // Q: Sync done?
    return;               // A: yes, no more to do.
                          // A: no, Sync start from here

  getTimeStamp(ntpStr, TIME_MSG_LEN);
  // convert ascii time to a a DateTime
  // 01234567890123456
  // yy-mm-dd hh:mm:ss
  
  DateTime ntp_time(
    atoi(ntpStr),         // yy
    atoi(ntpStr + 3),     // mm
    atoi(ntpStr + 6),     // dd
    atoi(ntpStr + 9),     // hh
    atoi(ntpStr + 12),    // mm
    atoi(ntpStr + 15) );  // ss

/* Note: 
  After a restart, the yun catch the most recent file to set the clock...
  And this can be from 2 minutes, or more. To be sure that the date is far from yet, 
  you must set in the startup script /etc/rc.local this statement:
  
  date -s 2001-01-01

*/
  if (ntp_time.year() < 2020 )  // Caution! 20 stand for year 2020
  {
#if DEBUG > 0
      log_msg(F("NTP old, not used!"));
#endif  
      return;      
  } 
   
#if DEBUG && DEBUG > 1
  Serial.print(F("NTP time:")); Serial.println(ntp_time.unixtime());
  Serial.print(F("my  time:")); Serial.println(myTime.unixtime());
#endif
  sec_correction = ntp_time.unixtime() - myTime.unixtime();
  // set text for log with the correction value 
#if DEBUG && DEBUG > 1
  //                        0123456789012345678901234567890
  char str_correction[32] = "T Correction: 000000000      \n";

  ltoa(sec_correction, str_correction+14, 10);
  log_msg(str_correction);
#endif  
  // then, use only positive value 
  if (sec_correction < 0L) 
      sec_correction = -sec_correction;

  if (sec_correction <= MIN_CORRECTION)
  { 
#if DEBUG && DEBUG > 1    
    //      012345678901234567890
    log_msg(F("Time NTP = RTC      "));
#endif
    return;
  }
  
  rtc.adjust(ntp_time);   // use ntp time to adjust RTC
  getTimeStamp(ntpStr, TIME_MSG_LEN);  // reload ASCII version
  //      012345678901234567890
  // log_msg("NTP used for RTC    ");

}// end of timeSync()

void tm_to_ascii(DateTime *dt, char *date_time_str = dateTimeStr)
{
sprintf(date_time_str, "%04d-%02d-%02d %02d:%02d:%02d ",
          dt->year(),
          dt->month(),
          dt->day(),
          dt->hour(),
          dt->minute(),
          dt->second()
          );
}

bool IsSyncTime_10_seconds()
{
  if (myTime.second() % 10 == 0)
    return true;
  else
    return false;
}

bool IsSyncTime_15_minutes()  // full minute !
{
  if (myTime.minute() % 15 == 0)
    return true;
  else
    return false;
}

bool IsSyncTime_03h00()
{
  if (myTime.second() == 0 && myTime.minute() == 0 && myTime.hour() == 3)
    return true;
  else
    return false;
}

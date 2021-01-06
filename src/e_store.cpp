/*
 e_store.cpp
 -----------
 20.12.2020 - ymasur@microclub.ch
 01.01.2021 - YM: sup. RTC (NTP only)

 Module store can write datas onto SD/USB storage file
*/
#include <Arduino.h>
#include <FileIO.h>
//#include <string.h>
#include "e230.h"


#define RETRY 3 //number of retry in write to file (0: mean no retry)
#define RETRY_DELAY 5 // nb of millisecond between write try

void convert_datas_to_store(char *buf);


// organisation of datas
/*  store_datas(char *fname, DateTime dt)
    --------------------------------------------------
    Store the actual datass in a file on SD card
    Global vars used:
    - char[] fname, contain the full path
    - DateTime dt: structure of actual date

    Modified global vars:
    - fname, the filename in a 8.3 format. 4 first chars are modified
    - errFile: false if OK; then true if an error occures
*/

void store_datas(char *fname)
{
  short i = 0;
  char datas_str[31];
          
  // open the file.
  // The FileSystem card is mounted at the following "/mnt/SD" and
  // create the name with year and month on 4 digits
  //  01234567890123456789
  // "2020-08-29 22:10:42"
  fname[OFFSET_YYMM + 0] = dateTimeStr[2];
  fname[OFFSET_YYMM + 1] = dateTimeStr[3];
  fname[OFFSET_YYMM + 2] = dateTimeStr[5];
  fname[OFFSET_YYMM + 3] = dateTimeStr[6];

  fname[OFFSET_YYMM + 4] = 'e';
  fname[OFFSET_YYMM + 5] = 'n';
  fname[OFFSET_YYMM + 6] = 'g';
  fname[OFFSET_YYMM + 7] = 'y';   

  convert_datas_to_store(datas_str);

  // open the 'fname' file, 3 try...
  do 
  {
    err_file = true;
    
    File filept = FileSystem.open(fname, FILE_APPEND);
    if (filept)  // if the file is available, write to it:
    {
      filept.print(dateTimeStr); 
      //filept.print("\t");
      filept.println(datas_str);
      filept.close();
      err_file = false;
    } // file pt OK
    else // A: file not availble, retry
    {
      delay(RETRY_DELAY);
    }
  } while(++i < RETRY && err_file == true);

  if (err_file == true) // Q: is the file isn't open?
  {                    // A: yes, pop up an error:
    const char err_msg[] = "Error writing file ";  //$$$

    Serial.print(dateTimeStr);
    Serial.print(err_msg);
    Serial.println(fname);
    err_file = true;   // mark it
    //log_msg(err_msg); // try to log (can be in error, too)
  }
}

/*  log_msg_SD()
    ------------
    Store the message in a logfile on SD card
    The format is: time + TAB + String given + CRLF
*/
void log_msg_SD(const char * msg)
{
  const char flog[] = "/mnt/sd/arduino/www/e230.log";
  // open the file, 3 try...
  short i = 0;

  display_info(msg);
  //dateTime_up_ascii();

  do 
  {
    err_file = true;
    File filept = FileSystem.open(flog, FILE_APPEND);
    if (filept)  // if the file is available, write to it:
    {
      err_file = false;      
      filept.print(dateTimeStr);
      filept.print("\t");
      filept.println(msg);
      filept.close();
    }
    else
    {
      delay(RETRY_DELAY);
    }
  } while(i++ < RETRY && err_file == true);
 
} // log_msg_SD()



void convert_datas_to_store(char *buf)
{
  int pos;
  dtostrf(data.e_consumed.val, 10, 3, buf);

  pos = strlen(buf);
  *(buf + pos) = '\t'; *(buf + pos + 1) = '\0';
  dtostrf(data.e_producted.val, 10, 3, buf + pos + 1);
  
  pos = strlen(buf);
  *(buf + pos) = '\0';
}

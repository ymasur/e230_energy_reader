/*	e230_05.hpp
	-----------
	Objetc part
	YM: 26.11.2020 - adapted from e230_4.ino, from JM Paratte
	YM: 13.12.2020 - report modif 230_5.ino
	YM: 15.12.2020 - supress of DEBUG and commented code
	YM: 18.12.2020 - Only E230_S object left here; other fct are in e230.cpp
*/
#ifndef E230_HPP
#define E230_HPP

#ifdef MAIN
  #define CLASS // intern...
#else
  #define CLASS extern
#endif

#include <Arduino.h>
#include "e230.h"

// -----------------------------------------------------------------------------
// Storage class of common variables, always in main module
// #define E230_BUF_SZ (512)   // size of input buffer

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG
#endif

//#endif //

#define CHR_NUL	'\x00'
#define CHR_STX	'\x02'
#define CHR_ETX	'\x03'
#define CHR_ACK	'\x06'
#define CHR_TAB	'\t'	// '\x09'
#define CHR_LF	'\n'	// '\x0A'
#define CHR_CR	'\r'	// '\x0D'

#define STR_STX	"\x02"
#define STR_ETX	"\x03"
#define STR_ACK	"\x06"
#define STR_TAB	"\t"
#define STR_LF	"\n"
#define STR_CR	"\r"
#define STR_CRLF "\r\n"
// -----------------------------------------------------------------------------

#define OK true
#define ER false

#define _OK_ "=OK="
#define _ER_ "#ER#"

// fct prototype (only used in here)
char * str7e( const char * str );
char chr7e( const char c );
unsigned ms();

/*

// Arduino UNO
// -----------

// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

//// TX, RX, SWRX - D10, SWTX - D11
//
//#include <SoftwareSerial.h>
//SoftwareSerial SwSerial(10, 11, false);

// TX, RX, SWRX - D8, SWTX - D9

#include <AltSoftSerial.h>
AltSoftSerial SwSerial;

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (true)

#define STREAM_IN SwSerial

#define STREAM_OUT Serial

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG
#endif

*/


/*

// Leonardo
// --------

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (true)

#define STREAM_IN Serial1

#if STREAM_OUT_IS_KEYBOARD
#	include <jm_Keyboard_Swiss.h>
#	define STREAM_OUT Keyboard
#else
#	define STREAM_OUT Serial
#endif

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG Serial
#endif

*/


/*

// DUE Programming Port
// --------------------

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (true)

#define STREAM_IN Serial1

#if STREAM_OUT_IS_KEYBOARD
#	include <jm_Keyboard_Swiss.h>
#	define STREAM_OUT Keyboard
#else
#	define STREAM_OUT Serial
#endif

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG
#endif

*/

/*

// ESP32 DEVKIT
// ------------

// TX - 1, RX - 3, RX1 - 9, TX1 - 10, RX2 - 16, TX2 - 17, SDA - 21, SCL - 22

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (true)

#define STREAM_IN Serial2 // pins 16 et 17

#define STREAM_OUT Serial

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG
#endif

*/


/*

// ESP8266 WeMos D1 mini
// ---------------------

// TX, RX, SWRX - D5, SWTX - D4, SDA - D2, SCL - D1

#include <SoftwareSerial.h>
SoftwareSerial SwSerial(D5, D4, false);

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (true)

#define STREAM_IN SwSerial

#define STREAM_OUT Serial

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG
#endif

*/

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

#define DIV_CEIL(a, b) ((a + b - 1)/b)

// -----------------------------------------------------------------------------


#define OS_JIT_TIME 10


// -----------------------------------------------------------------------------


#define WITH_LCD false



// https://www.promotic.eu/en/pmdoc/Subsystems/Comm/PmDrivers/IEC62056_OBIS.htm
// https://www.dlms.com/files/Blue_Book_Edition_13-Excerpt.pdf

struct E230_S {

	bool _begun;
	bool _started;
	bool _ready;
	bool _errored;

	unsigned _baud;
	unsigned _char_ms;

	short _state;		// state of transaction
	unsigned _state_ms;	// associated time

	char _buf[E230_BUF_SZ];		// full input buffer
	char * _ptr;		// char pointer of bufferized string
	char * _str;		// string pointer
	int _num;			// number of string received, with '\0' separated in buffer
	char _bcc;			// parity

	E230_S() :
		_begun(false),
		_started(false),
		_ready(false),
		_errored(false),

		_baud(0),
		_char_ms(0),

		_state(0),
		_state_ms(0),

		_buf{CHR_NUL},
		_ptr((char *) & _buf),
		_str((char *) & _buf),
		_num(0),
		_bcc(0)
	{}

	operator bool ()
	{
		return _begun;
	}

	bool init( bool begin, bool start )
	{
		bool ok = true;

		_begun = begin;
		_started = start;
		_ready = false;
		_errored = false;

		if (begin && start) {
			ok = ok && baud(300);
		}
		else {
			_baud = 0;
			_char_ms = 0;
		}

		_state = 0;
		_state_ms = ms(); //millis();

		_buf[0] = CHR_NUL; // memset(_buf, 0, sizeof(_buf));
		_ptr = (char *) & _buf;
		_str = (char *) & _buf;
		_num = 0;
		_bcc = 0;

		return ok;
	}	// init

	bool baud( unsigned baud )
	{
		if (_baud != baud) {

			_baud = baud;
			_char_ms = DIV_CEIL((1+7+1+1)*1000, baud);

#if defined AltSoftSerial_h
			STREAM_IN.begin(baud);
#else
			STREAM_IN.begin(baud, SERIAL_7E1);
#endif
		}

		return OK;
	}

	unsigned baud()
	{
		return _baud;
	}

	char * identity()
	{
		return (char *) & _buf;
	}

	char identity4()
	{
		return _buf[4];
	}

	bool begin()
	{
		if (_begun == true) return ER;

		return init(true, false);
	}

/*
	bool end()
	{
		if (_begun == false) return ER;

DEBUG_LINE_PRINTF("<end>");

		return init(false, false);
	}
*/
	bool start()
	{
		if (_begun == false) return ER;
		if (_started == true) return ER;

		return init(true, true);
	}

/*
	bool stop()
	{
		if (_begun == false) return ER;
		if (_started == false) return ER;

DEBUG_LINE_PRINTF("<stop>");

		_started = false;

		return OK;
	}
*/
	bool started()
	{
		return _started;
	}

	bool ready()
	{
		return _ready;
	}

	bool errored()
	{
		return _errored;
	}

	int e_state()  {return _state; }

	void cycle()
	{
		if (_begun == false) return;
		if (_started == false) return;
		if (_ready == true) return;
		if (_errored == true) return;

		int c = stream_in->read();

#if defined AltSoftSerial_h
		// strip parity bit...
		if (c >= 0) c &= 0x7F;
#else
		// parity bit is already stripped.
#endif

#if DEBUG_CHAR
		if (c >= 0) {
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (ms() - _state_ms));
		}
#endif

		int state = _state;
		unsigned state_ms = ms();

		switch (state) {

			case -1: // Serial jam, flush received chars

				switch (c) {

					case -1:
						// check <char_ms> timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) {
							// 1 step forward...
							_state++;
							_state_ms = state_ms;
						}
						break;

					default:
						// discard char, stay on step with timing restarted...
						_state_ms = state_ms;
						break;
				}

				break;

			case 0: // Serial ready to start request

				switch (c) {

					case -1:
						// ask equipment...
					#if defined AltSoftSerial_h
						// set parity bit of each chars and write them...
						stream_in->print(str7e("/?!" STR_CRLF));
					#else
						// parity bit set by HW, just write them...
						stream_in->print("/?!" STR_CRLF);
					#endif
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default:
						// discard received char, step backward with timing restarted...
						_state--;
						_state_ms = state_ms;
						break;
				}

				break;

			case 1: // flush sending command (delay)

				switch (c) {

					case -1:
						if ((state_ms - _state_ms) >= (5*_char_ms + OS_JIT_TIME)) {
							// delay elapsed, 1 step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
						}
						break;

					default:
						// discard char, stay on step without restart timing...
						break;
				}

				break;

			case 2: // check '/' 1st char of identity

				switch (c) {

					case -1:
						// check 1s timeout...
						if ((state_ms - _state_ms) > 1000U) { // timeout ?
//							_started = false;
							_errored = true;
							// stop cycle...
							_started = false;
						}
						// stay on step without restart timing...
						break;

					case '/': // 1st char of identity
						*_ptr++ = c;
						// step forward with timing restarted...
						_state_ms = state_ms;
						_state++;
						break;

					default: // wrong 1st char of identity
//						_started = false;
						_errored = true;
						// stop cycle...
						_started = false;
						// stay on step without restart timing...
						break;
				}

				break;

			case 3: // append char to identity str, check <CR>

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) 
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					case CHR_CR: // <CR> end of identity str
						// append <NUL> char str delimiter (also strings separator)
						*_ptr++ = CHR_NUL;
						_num++;

						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default: // any char
						// append char to identity str...
						*_ptr++ = c;
						// stay on step without restart timing...
						_state_ms = state_ms;
						break;
				}

				break;

			case 4: // <CR> received, check <LF>

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (_char_ms + OS_JIT_TIME)) 
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					case CHR_LF:
#if WITH_LCD
						lcd_row++; // print on next line...
#endif
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default: // unexpected char
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 5: // identity <CR><LF> received, delay 200ms and then answer "protocol C"

				switch (c) {

					case -1:
						if ((state_ms - _state_ms) < 200U) { // delay ?
							// stay on step without restart timing...
						}
						else {
							// answer "protocol C"...
						#if defined AltSoftSerial_h
							stream_in->print(str7e(STR_ACK "0")); stream_in->print(chr7e(identity4())); stream_in->print(str7e("0" STR_CRLF));
						#else
							stream_in->print(STR_ACK "0"); stream_in->print(identity4()); stream_in->print("0" STR_CRLF);
						#endif
							// step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
						}
						break;

					default: // any char errored
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}
				break;

			case 6: // flush sending command (delay) and then change baud rate according to protocol C
				switch (c) {

					case -1:
						if ((state_ms - _state_ms) < (6*_char_ms + OS_JIT_TIME)) { // delay ?
							// stay on step without restart timing...
						}
						else {
							// change baud rate according to protocol C...
							baud(300<<(identity4() - '0'));
							// step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
						}
						break;

					default: // any char discarded
						// discard crosstalk/diaphony/interference...
						// discard char, stay on step without restart timing...
						break;
				}

				break;

			case 7: // baud rate changed according to protocol C, check <STX>

				switch (c) {

					case -1:
						// check 2s timeout...
						if ((state_ms - _state_ms) > 2000U)
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						// stay on step without restart timing...
						break;

					case CHR_STX: // <STX>
						// step forward with timing restarted...
						_state_ms = state_ms;
						_state++;
						break;

					default: // unexpected char
						_errored = true;
						// stop cycle...
						_started = false;
						// stay on step without restart timing...
						break;
				}

				break;

			parse_next_line:

				_state = 8;

#if defined AVR
			//	https://developers.redhat.com/blog/2017/03/10/wimplicit-fallthrough-in-gcc-7/
			//	__attribute__ ((fallthrough)); // C and C++03
			//	[[gnu::fallthrough]]; // C++11 and C++14
			//	[[fallthrough]]; // C++17 and above
#endif

			case 8: // parse new line

				_str = _ptr;

				_state++;
				_state_ms = state_ms;

				break;

			case 9:

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) 
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					case ' '...'~': // legal char
						// update <bcc>...
						_bcc ^= c;
						// append char to identity str...
						*_ptr++ = c;
						// stay on step without restart timing...
						_state_ms = state_ms;
						break;

					case CHR_CR: // <CR> end of str
						// update <bcc>...
						_bcc ^= c;
						// append <NUL> char str delimiter (also strings separator)
						*_ptr++ = 0;
						_num++;
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default: // unexpected char
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 10: // <CR> received, check <LF>

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) 
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					case CHR_LF:
						// update <bcc>...
						_bcc ^= c;

						if (_str[0] == '!') 
						{
							// step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
							break;
						}
						else 
						{
							goto parse_next_line;
						}

					default: // unexpected char
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 11: // check <ETX>

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) 
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					case CHR_ETX:
						// update <bcc>...
						_bcc ^= c;
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default: // unexpected char
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 12: // check <bcc>

				switch (c) {

					case -1:
						// check char timeout...
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME))
						{ // timeout ?
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					default: // <bcc> char
						// check bcc...
						if (c == _bcc) 
						{
							_ready = true;
						}
						else 
						{
							_errored = true;
						}
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 13: // <ready> or <errored>
				_errored = true;
				// stop cycle...
				_started = false;
				break;

			default: // wrong state
				_errored = true;
				// stop cycle...
				_started = false;
				break;
		}
	}// cycle()


	/*	parse_line( const char * p, char * str )
		----------------------------------------
		search  elements  in the buffer.
		* p: point into the input buffer to be parsed
		* str: write in the output buffer, where the line is to be copied
		Return value: number of chr written, 0 in case of parse problem.
	*/
#if 0
	int parse_line( const char * p, char * str )
	{
		char * ptr = str;
		int num = 0;

		const char * p1; //OBIS code (A-B:C.D.E*F) terminated by '('
		const char * p2; //data value terminated by '*' or ')' and preceded by '('
		const char * p3; //unity value terminated by ')' and preceded by '*'
		const char * p4; //end of IEC 62056-21 data line (terminated by CRLF)

		p1 = p;

		p2 = strchr(p1, '(');
		if (p2 == NULL) return 0;

		memcpy(ptr, p1, (p2 - p1));
		ptr += (p2 - p1);
		*ptr++ = CHR_NUL;
		num++;

		p3 = strpbrk((p2 + 1), "*)");	// search for '*' or ')'
		if (p3 == NULL) return 0;

		memcpy(ptr, (p2 + 1), (p3 - (p2 + 1)));	// copy info in between
		ptr += (p3 - (p2 + 1));	// adjust pointer at end of datas
		*ptr++ = CHR_NUL;	
		num++;

		if (*p3 == '*') {		// '*' marks the unity of datas
			p4 = strchr(p3 + 1, ')');
			if (p4 == NULL) return 0;

			memcpy(ptr, (p3 + 1), (p4 - (p3 + 1)));	// copy unity info
			ptr += (p4 - (p3 + 1));
			*ptr++ = CHR_NUL;
			num++;
		}
		else {
			p4 = p3;
		}

		if (*(p4 + 1) != CHR_NUL) return 0;	// string malformed

		return num;
	}
#endif

#if 0
	bool stream_out_excel() // return OK
	{
		// stream out <field names>...
		stream_out->println(F("'" "Code" STR_TAB "'" "Valeur" STR_TAB "'" "Unité"));

		// stream out <program signature>...
		stream_out->print('\'');
		stream_out->println(F(__PROG__));

		// stream out <identity>...
		stream_out->print('\'');
		stream_out->println(identity()); // println <identity>

		// stream out <data> and loop...
		int i1;
		char * p1;
		for (	i1 = 1, p1 = ((char *) & _buf + strlen(_buf) + 1);
				i1 < (_num - 1);
				i1++, p1 += (strlen(p1) + 1)
			) { // set i1 and p1, skip 1st field (<identity>) and last (<!>)

			char buf1[64]; // <code> CHR_NUL <valeur> CHR_NUL [<unité> CHR_NUL]
			int n1 = parse_line( p1, buf1 );
			if (n1 == 0) { // line parsing error ?

				// stream out <ER>...
				stream_out->print('\'');
				stream_out->println(F(_ER_));

				return ER;
			}

			// stream out fields...
			char * p = &buf1[0];
			for (int n=0; n<n1; n++) {

				if (n > 0) stream_out->print(CHR_TAB);
				stream_out->print('\'');
				stream_out->print(p);
				p += strlen(p) + 1;
			}

			stream_out->println();
		}

		// stream out <!>...
		stream_out->print('\'');
		stream_out->println(p1); // print <!>

		// skip <!>...
		i1++, p1 += (strlen(p1) + 1);

		// stream out <OK>...
		stream_out->print('\'');
		stream_out->println(F(_OK_));

		return OK;
	}
#endif //stream_out_excel()

#if 1
	bool stream_out_buf() // return OK
	{
		stream_out->println(F("buffer contents"));
	
			// stream out fields...
		stream_out->println("[start]");

		// stream out <data> and loop...
		int i;
		char * p;
		for (	i = 0, p = _buf; p < (_buf + sizeof(_buf)) && *p != '!'; p++)
		{
				if (*p != 0)
					stream_out->print(*p);
				else
					stream_out->print("\r\n");

				i += 1;
		}
		stream_out->print(*p);	// must be '!'	

		// stream out the end
		stream_out->println("\n[end] - nb char: ");
		stream_out->println(i);

		return OK;
	}// stream_out_buf()
#endif

};

CLASS E230_S * p_e230;

// -----------------------------------------------------------------------------
#endif // E230_HPP
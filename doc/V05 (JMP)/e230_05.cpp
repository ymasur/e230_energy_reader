//éèà


// -----------------------------------------------------------------------------


#ifndef UserSettings_h
#warning "platform.local.txt" is probably missing. Adapt "platform.local.txt-set.py" and run it.
#include "UserSettings.h"
#endif // UserSettings_h


// -----------------------------------------------------------------------------


//#define __PROG__ "e230_05.ino"
#define __PROG__ BUILD_PROJECT_NAME


// -----------------------------------------------------------------------------


#define DEBUG false
#define DEBUG_CHAR (DEBUG && false)
#define DEBUG_BUF_SIZE 64

// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros

#if DEBUG
#define DEBUG_LINE_PRINTF( ... ) debug_line_printf(__LINE__, __VA_ARGS__)
#else
#define DEBUG_LINE_PRINTF( ... )
#endif


// -----------------------------------------------------------------------------


#define DIV_CEIL(a, b) ((a + b - 1)/b)
#define DIV_ROUND(a, b) ((a + b/2)/b)
#define DIV_FLOOR(a, b) (a/b)


// -----------------------------------------------------------------------------


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

#define STR_BOM "\xEF\xBB\xBF"  // BOM UTF-8 https://en.wikipedia.org/wiki/Byte_order_mark


// -----------------------------------------------------------------------------


#define OK true
#define ER false

#define _OK_ "=OK="
#define _ER_ "#ER#"


// -----------------------------------------------------------------------------


#define OS_JIT_TIME 10


// -----------------------------------------------------------------------------


#define WITH_LCD true


// -----------------------------------------------------------------------------


unsigned ms()
{
	return unsigned(millis());
}


// -----------------------------------------------------------------------------


char chr7e( const char c )
{
	bool odd = false;
	char c1 = c;
	while (c1) {
		odd = !odd;
		c1 = c1 & (c1 - 1);
	}
	if (!odd) return c; else return (c | 0x80);
}

char * str7e( const char * str )
{
	static char buf[64];
	strcpy( buf, str );
	char * p = (char *) & buf;
	for (; *p != CHR_NUL; p++) *p = chr7e( *p );
	return (char *) & buf;
}


// -----------------------------------------------------------------------------


// https://www.arduino.cc/reference/en/language/functions/communication/serial/




// Arduino Yún with Bridge
// -----------------------

// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

// TX, RX, SWRX - D13, SWTX - D5

#include <AltSoftSerial.h>
AltSoftSerial SwSerial;

// Bridge

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
BridgeServer server;

#define STREAM_OUT_IS_KEYBOARD (false)

#define STREAM_OUT_ALSO_DEBUG (false)

#define STREAM_IN SwSerial

#define STREAM_OUT Serial

#if STREAM_OUT_ALSO_DEBUG
#	define STREAM_DEBUG STREAM_OUT
#else
#	define STREAM_DEBUG Serial
#endif




/*

// Arduino Yún as a Leonardo
// -------------------------

// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

// TX, RX, SWRX - D13, SWTX - D5

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


Stream * stream_in = & STREAM_IN;
Stream * stream_out = & STREAM_OUT;


// -----------------------------------------------------------------------------


#if DEBUG

Stream * stream_debug = & STREAM_DEBUG;

#include <stdio.h>
#include <stdarg.h>

void debug_printf( const char * format, ... )
{
	char buffer[DEBUG_BUF_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
//	perror(buffer); //C++
//	printf(buffer); //C++
	stream_debug->print(buffer); //Stream Arduino
}

void debug_line_printf( int line, const char * format, ... )
{
	char buffer[DEBUG_BUF_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	debug_printf("%5u:%04i:%s\n", unsigned(millis()), line, buffer);
}

#endif


// -----------------------------------------------------------------------------


#if WITH_LCD

#include <Wire.h>

#include <jm_LCM2004A_I2C.h>

jm_LCM2004A_I2C lcd;

int lcd_row;

void lcd_print20( const char * str )
{
	char buf[20+1];
	snprintf(buf, (20+1), "%-20s", str);
	lcd.set_cursor( 0, lcd_row );
	lcd.print(buf);
//	lcd.flush();
}

#endif


// -----------------------------------------------------------------------------


// https://www.promotic.eu/en/pmdoc/Subsystems/Comm/PmDrivers/IEC62056_OBIS.htm
// https://www.dlms.com/files/Blue_Book_Edition_13-Excerpt.pdf

struct E230_S {

	bool _begun;
	bool _started;
	bool _ready;
	bool _errored;

	unsigned _baud;
	unsigned _char_ms;

	short _state;
	unsigned _state_ms;

	char _buf[512];
	char * _ptr;
	char * _str;
	int _num;
	char _bcc;

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
	}

	bool baud( unsigned baud )
	{
		if (_baud != baud) {

			_baud = baud;
			_char_ms = DIV_CEIL((1+7+1+1)*1000, baud);

DEBUG_LINE_PRINTF("<%u_7E1-%u>", _baud, _char_ms);

//			STREAM_IN.end();
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

DEBUG_LINE_PRINTF("<begin>");

		return init(true, false);
	}

	bool end()
	{
		if (_begun == false) return ER;

DEBUG_LINE_PRINTF("<end>");

		return init(false, false);
	}

	bool start()
	{
		if (_begun == false) return ER;
		if (_started == true) return ER;
//		if (_state != 0) return ER;

DEBUG_LINE_PRINTF("<start>");

		return init(true, true);
	}

	bool stop()
	{
		if (_begun == false) return ER;
		if (_started == false) return ER;

DEBUG_LINE_PRINTF("<stop>");

		_started = false;
//		_ready = false;
//		_errored = false;

		return OK;
	}

	bool clear()
	{
		if (_begun == false) return ER;
		if (_started == true) return ER;

DEBUG_LINE_PRINTF("<clear>");

//		_started = false;
		_ready = false;
		_errored = false;

		return OK;
	}

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
DEBUG_LINE_PRINTF("/?!");
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
						// discard crosstalk/diaphony/interference...
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
						// discard char, stay on step without restart timing...
						break;
				}

				break;

			case 2: // check '/' 1st char of identity

				switch (c) {

					case -1:
						// check 1s timeout...
						if ((state_ms - _state_ms) > 1000U) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
							_errored = true;
							// stop cycle...
							_started = false;
						}
						// stay on step without restart timing...
						break;

					case '/': // 1st char of identity
DEBUG_LINE_PRINTF("/ (%u)", (state_ms - _state_ms));
						*_ptr++ = c;
						// step forward with timing restarted...
						_state_ms = state_ms;
						_state++;
						break;

					default: // wrong 1st char of identity
DEBUG_LINE_PRINTF("(%u)", (state_ms - _state_ms));
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
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
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
#if WITH_LCD
						// lcd clear...
						lcd.clear_display();
						// lcd formatted print...
						lcd_row = 0; // print on 1st line...
						// lcd formatted print...
						lcd_print20(_str);
#endif
DEBUG_LINE_PRINTF("%s", _str);
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
						if ((state_ms - _state_ms) >= (_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
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
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_begun = false;
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}

				break;

			case 5: // identity <CR><LF> received, delay 200ms and then answer "protocol C"

//				if ((state_ms - _state_ms) < 200) { // wait ?
//					// stay on step without restart timing...
//					break;
//				}
//
//				// answer "protocol C"...
//DEBUG_LINE_PRINTF("<ACK>0%c0", identity4());
//			#if defined AltSoftSerial_h
//				stream_in->print(str7e(STR_ACK "0")); stream_in->print(chr7e(identity4())); stream_in->print(str7e("0" STR_CRLF));
//			#else
//				stream_in->print(STR_ACK "0"); stream_in->print(identity4()); stream_in->print("0" STR_CRLF);
//			#endif
//
//				// step forward with timing restarted...
//				_state++;
//				_state_ms = state_ms;
//				break;
				switch (c) {

					case -1:
						if ((state_ms - _state_ms) < 200U) { // delay ?
							// stay on step without restart timing...
						}
						else {
							// answer "protocol C"...
DEBUG_LINE_PRINTF("<ACK>0%c0", identity4());
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
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_begun = false;
						_errored = true;
						// stop cycle...
						_started = false;
						break;
				}
				break;

			case 6: // flush sending command (delay) and then change baud rate according to protocol C

//				if ((state_ms - _state_ms) < (6*_char_ms + OS_JIT_TIME)) break;
//
//				baud(300<<(identity4() - '0'));
//
//				_state_ms = state_ms;
//				_state++;
//				break;

				switch (c) {

					case -1:
						if ((state_ms - _state_ms) < (6*_char_ms + OS_JIT_TIME)) { // delay ?
							// stay on step without restart timing...
						}
						else {
							// change baud rate according to protocol C...
DEBUG_LINE_PRINTF("<baud> %u", (300<<(identity4() - '0')));
							baud(300<<(identity4() - '0'));
							// step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
						}
						break;

					default: // any char discarded
						// discard crosstalk/diaphony/interference...
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
						// discard char, stay on step without restart timing...
						break;
				}

				break;

			case 7: // baud rate changed according to protocol C, check <STX>

//				if (c != CHR_STX) {
//					if ((state_ms - _state_ms) > 2000) {
//
//DEBUG_LINE_PRINTF("(%u)", (state_ms - _state_ms));
//
//						_started = false;
//					}
//					break;
//				}
//
//DEBUG_LINE_PRINTF("<STX> (%u)", (state_ms - _state_ms));
//
//				_bcc = 0;

				switch (c) {

					case -1:
						// check 2s timeout...
						if ((state_ms - _state_ms) > 2000U) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
							_errored = true;
							// stop cycle...
							_started = false;
						}
						// stay on step without restart timing...
						break;

					case CHR_STX: // <STX>
DEBUG_LINE_PRINTF("<STX> (%u)", (state_ms - _state_ms));
						// step forward with timing restarted...
						_state_ms = state_ms;
						_state++;
						break;

					default: // unexpected char
//DEBUG_LINE_PRINTF("(%u)", (state_ms - _state_ms));
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_started = false;
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
				[[fallthrough]]; // C++17 and above
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
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
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
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_begun = false;
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
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
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
#if WITH_LCD
						lcd_print20(_str);
						if (strstr(_str, "1.8.0(") != NULL) lcd_row++; // print on next line...
						if (strstr(_str, "2.8.0(") != NULL) lcd_row++; // print on next line...
#endif
DEBUG_LINE_PRINTF("%s", _str);
						if (_str[0] == '!') {
DEBUG_LINE_PRINTF("<!>");
							// step forward with timing restarted...
							_state++;
							_state_ms = state_ms;
							break;
						}
						else {
							goto parse_next_line;
						}

					default: // unexpected char
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_begun = false;
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
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
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
DEBUG_LINE_PRINTF("<ETX>");
						// step forward with timing restarted...
						_state++;
						_state_ms = state_ms;
						break;

					default: // unexpected char
DEBUG_LINE_PRINTF("%c %02X (%u)", (c<'!'||c>'~'?' ':c), int(c), (state_ms - _state_ms));
//						_begun = false;
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
						if ((state_ms - _state_ms) >= (1*_char_ms + OS_JIT_TIME)) { // timeout ?
DEBUG_LINE_PRINTF("<timeout>");
//							_started = false;
							_errored = true;
							// stop cycle...
							_started = false;
						}
						else {
							// stay on step without restart timing...
						}
						break;

					default: // <bcc> char
DEBUG_LINE_PRINTF("<bcc> %02X %02X %i %i", int(c), int(_bcc), int(_ptr - (char *) & _buf), _num);
						// check bcc...
						if (c == _bcc) {
							_ready = true;
DEBUG_LINE_PRINTF("<ready>");
#if WITH_LCD
							lcd_print20(_OK_);
#endif
						}
						else {
							_errored = true;
DEBUG_LINE_PRINTF("<errored>");
#if WITH_LCD
							lcd_print20(_ER_);
#endif
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

//				_started = false;
				_errored = true;
				// stop cycle...
				_started = false;
				break;

			default: // wrong state

DEBUG_LINE_PRINTF("(%u)", (state_ms - _state_ms));

//				_started = false;
				_errored = true;
				// stop cycle...
				_started = false;
				break;
		}
	}

	int parse_line( const char * p, char * str )
	{
		char * ptr = str;
		int num = 0;

//21:50:51.307 -> F.F.0(00000000)
//21:50:51.307 -> 0.0.2(  339648)
//21:50:51.354 -> C.1.0(11860813)
//21:50:51.401 -> C.1.1(        )
//21:50:51.448 -> 1.8.1(050325.575*kWh)

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

		p3 = strpbrk((p2 + 1), "*)");
		if (p3 == NULL) return 0;

		memcpy(ptr, (p2 + 1), (p3 - (p2 + 1)));
		ptr += (p3 - (p2 + 1));
		*ptr++ = CHR_NUL;
		num++;

		if (*p3 == '*') {
			p4 = strchr(p3 + 1, ')');
			if (p4 == NULL) return 0;

			memcpy(ptr, (p3 + 1), (p4 - (p3 + 1)));
			ptr += (p4 - (p3 + 1));
			*ptr++ = CHR_NUL;
			num++;
		}
		else {
			p4 = p3;
		}

		if (*(p4 + 1) != CHR_NUL) return 0;

		return num;
	}

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
//				stream_out->println(F("ER"));

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
//		stream_out->println(F("OK"));

		return OK;
	}

} e230;


// -----------------------------------------------------------------------------


void setup()
{
#if WITH_LCD
	Wire.begin();

	lcd.begin();
	lcd.clear_display();
	lcd_row = 0;
	delay(1000);

	lcd_print20(__PROG__);
#endif

	// set stream_in hardware...
#if defined AltSoftSerial_h
	STREAM_IN.begin(300); // <AltSoftSerial.h> can't manage 7E1!
#else
	STREAM_IN.begin(300, SERIAL_7E1);
#endif
//	while (!STREAM_IN && millis()<3000) delay(1);

	// set stream_out hardware...
#if STREAM_OUT_IS_KEYBOARD
//DEBUG_LINE_PRINTF(F("<keyboard>"));
	Keyboard.begin();
//	while (!Keyboard) delay(1); // Sorry, unavailable!
//	delay(1000); // simple workaround for above sentence
	while (!Serial && millis()<3000) delay(1); // better workaround (Keyboard and Serial are on the same USB interface)
#else
#if defined _BRIDGE_CLIENT_H_
//DEBUG_LINE_PRINTF(F("<bridge>"));
	Bridge.begin();
	server.listenOnLocalhost();
	server.begin();
#else
//DEBUG_LINE_PRINTF(F("<serial>"));
	STREAM_OUT.begin(115200);
	while (!STREAM_OUT && millis()<3000) delay(1);
#endif
#endif

#if DEBUG
#if STREAM_OUT_ALSO_DEBUG
//DEBUG_LINE_PRINTF(F("<debug>"));
#else
//DEBUG_LINE_PRINTF(F("<debug>"));
	STREAM_DEBUG.begin(115200);
	while (!STREAM_DEBUG && millis()<3000) delay(1);
#endif
#endif

	// stabilize STREAM_IN and STREAM_OUT RX/TX lines after reset/power up...
	delay(1000);

//#if DEBUG
//debug_line_printf(__LINE__, __PROG__);
//#endif

//#if defined _BRIDGE_CLIENT_H_
//Serial.println(F("<server>"));
//	server.listenOnLocalhost();
//	server.begin();
//#endif

	e230.begin();

DEBUG_LINE_PRINTF(F("<setup>"));
}


void loop()
{
	e230.cycle();

	if (e230.started()) {
	}
	else {

#if defined _BRIDGE_CLIENT_H_

		// Get clients coming from server
		BridgeClient client = server.accept();

		// Is there a new client?
		if (client) {

			// Process request
			String s = client.readString();

			client.print(F(STR_BOM)); // BOM UTF-8 https://en.wikipedia.org/wiki/Byte_order_mark

			if (s == String("start" STR_CRLF)) {
				//
				if (e230.start())
					client.println(_OK_);
				else
					client.println(_ER_);
			}
			else
			if (s == String("print" STR_CRLF)) {
				//
				stream_out = & client;
				e230.stream_out_excel();
				stream_out = & Serial;
			}
			else {
				//
				client.println(_ER_);
			}

			// Close connection and free resources.
			client.stop();
		}

#else

		if (stream_out->available()) {

			// Process request
			String s = stream_out->readString();

			if (s == String("start" STR_CRLF)) {
				//
				if (e230.start())
					stream_out->println(_OK_);
				else
					stream_out->println(_ER_);
			}
			else
			if (s == String("print" STR_CRLF)) {
				//
				if (e230.stream_out_excel())
					stream_out->println(_OK_);
				else
					stream_out->println(_ER_);
			}
			else {
				//
				stream_out->println(_ER_);
			}
		}

#endif

	}
}

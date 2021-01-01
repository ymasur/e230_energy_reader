/*	e230_05.cpp
	-----------
	Function part
	YM: 24.12.2020 - func used by e230_05.hpp
*/
#ifndef E230_CPP
#define E230_CPP

#ifdef MAIN
  #define CLASS // intern...
#else
  #define CLASS extern
#endif

#include <Arduino.h>
#include "e230.h"
#include "e230_05.hpp"

// -----------------------------------------------------------------------------

/*	useful for compatibility 32 - 64 bits
*/
unsigned ms()
{
	return unsigned(millis());
}

/* Useful for transmission with 7-O-N on a 8-N
*/
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

/*
char * get_e230_buf()
{
	return p_e230->_buf;
}
*/
#endif // E230_CPP
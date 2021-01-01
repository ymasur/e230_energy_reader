/*
	e_calc.cpp
	----------
	YM 20.12.2020 
*/

#ifndef E_CALC_CPP
#define E_CALC_CPP

//#include "e230.h"
#include "e230_05.hpp"
#include "e_calc.hpp"


// The string value in ASCII; value is followed with chr as ")"
// exeample: "014605.516*kWh)" give: "014605.516"
// Return value: ASCII, without trailing chr
char * S_V::get_sval()
{
	static char svaln[16];	// for temp usage
	char * s = NULL;		// point into original buffer
	char * d = svaln;		// point to local buffer

	if (sval == NULL)		// Q: nothing to do?
	{
		svaln[0] = '\0';	// A: yes, empty value
	}
	else
	{ 
		s = sval; 	d = svaln;
		do {
			*d++ = *s++;	// copy chr until no numeric
		} while (*s != ')' && *s != '*' && *s != ' ' && *s != '\0' && d != svaln + sizeof(svaln) );
	}
	*d = '\0';				// be sure the string is terminated

	return svaln;
}


void get_all_values(char* p_buf)
{
	data.populate(p_buf);
	data.conv();
}

void data_print()
{
	// data.print();
}

#endif // E_CALC_CPP
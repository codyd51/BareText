#include "std.h"

void itoa(int i, char* b) {
	char const digit[] = "0123456789";
	char* p = b;
	if (i < 0) {
		*p++ = '-';
		i *= -1;
	}
	int shifter = i;
	do {
		//move to where representation ends
		++p;
		shifter = shifter/10;
	} while(shifter);

	*p = '\0';

	do {
		//move back, inserting digits as we go
		*(--p) = digit[i%10];
		i = i/10;
	} while (i);
}

#include <limits.h>
#include <stdarg.h>

#include <std.hpp>
#include <std/string.hpp>
#include <kernel/cpu.hpp>
#include <kernel/memory/alloc.hpp>


int strlen( char * ptr ){
	int len = 0;
	char * p = ptr;
	while( *p != '\0' ) {
		len++;
		p++;
	}
	return len;
}

int strcmp(const char* s1, const char* s2) {
	while(*s1 && (*s1==*s2))
		s1++,s2++;
	return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	for ( ; n > 0; s1++, s2++, --n)
			if (*s1 != *s2)
					return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
			else if (*s1 == '\0')
					return 0;
	return 0;
}

char tolower(char upper) {
	if (upper <= 'Z' && upper >= 'A')
		return upper - ('Z' - 'z');
	return upper;
}


char *strdup(const char *s1) {
	char* s2 = new char[strlen(s1)];
	memcpy(s2, s1, strlen(s1));
	return s2;
}


char *strcat(char *dest, const char *src) {
	char *rdest = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
	return rdest;
}

// taken from GLibC
char * strpbrk(const char *s, const char *accept) {
	while (*s != '\0') {
		const char *a = accept;

		while (*a != '\0')
			if (*a++ == *s)
				return (char *) s;
				++s;
	}
	return NULL;
}

/**
 * Copyright (c) 1988-1993 The Regents of the University of California.
 * All rights reserved.
*/
char * strstr(char *string, char *substring) {
    char *a;
	char *b;

    /* First scan quickly through the two strings looking for a
     * single-character match.  When it's found, then compare the
     * rest of the substring.
     */

    b = substring;
    if (*b == 0) {
		return string;
    }
    for ( ; *string != 0; string += 1) {
		if (*string != *b) {
			continue;
		}
		a = string;
		while (1) {
			if (*b == 0) {
				return string;
			}
			if (*a++ != *b++) {
				break;
			}
		}
		b = substring;
    }
    return nullptr;
}

// taken from GLibC
size_t strspn (const char *s, const char *accept) {
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p){
		for (a = accept; *a != '\0'; ++a)
			if (*p == *a)
				break;
			if (*a == '\0')
				return count;
			else
				++count;
	}

	return count;
}

// thanks to David Refaeli for this code
char * strtok(char *str, const char *delim) {

	static char *nxt; /* static variable used to advance the string to replace delimiters */
	static int size;  /* static variable used to count until the end of the string        */

	/* IMPORTANT: any advance to 'nxt' must be followed by a diminution of 'size', and vice verce */

	int i; /* counter of delimiter(s) in string */

	/* initialize the string when strtok2 is first calles and supplied with a valid string */
	if(str != NULL)
	{
		nxt = str;
		size = strlen(str);
	}

	/* if we havn't reached the end of the string, any other call to strtok2 with NULL will come here */
	else if(size > 0)
	{
		nxt++;      /* last run left nxt on a null terminator, so we have to advance it */
		size--;     /* any advancement must follow diminution of size                   */
		str = nxt;  /* string now points to the first char after the last delimiter     */
	}

	/* if we reached the end of string, return NULL pointer */
	else
	{
		str = NULL;
	}

	/* nxt is used to advance the string until a delimiter or a series of delimiters are encountered;
	* it then stops on the last delimiter which has turned to NULL terminator
	*/
	while(*nxt)
	{
		i = strspn(nxt, delim);
		while(i > 1)        /* turns delimiters to NULL terminator (except the last one) */
		{
			*nxt = '\0';
			nxt++;
			size--;
			i--;
		}                   /* in the last delimiter we have to do something a      */
		if(1 == i)          /* bit different we have to actually take nxt backwards */
		{                   /* one step, to break the while(*nxt) loop              */
			*nxt = '\0';
			if(size > 1)    /* if the delimiter is last char, don't do next lines   */
			{
				nxt--;
				size++;     /* nxt is diminished so size increases                    */
			}
		}
		nxt++;          /* if no delimiter is found, advance nxt                  */
		size--;         /* in case of the last delimiter in a series, we took nxt */
	}                   /* a step back, so now moving it a step forward means     */
						/* it rests on a NULL terminator                          */
	return str;
}

void strncpy( char * ptr_dest, char * ptr_src, int n ){
	for( int i=0; i < n; i++ )
		ptr_dest[i] = ptr_src[i];
}


/* Return the `ldiv_t' representation of NUMER over DENOM.  */
ldiv_t ldiv (unsigned long int numer, unsigned long int denom)
{
    ldiv_t result;

    result.quot = numer / denom;
    result.rem = numer % denom;

  /* The ANSI standard says that |QUOT| <= |NUMER / DENOM|, where
     NUMER / DENOM is to be computed in infinite precision.  In
     other words, we should always truncate the quotient towards
     zero, never -infinity.  Machine division and remainer may
     work either way when one or both of NUMER or DENOM is
     negative.  If only one is negative and QUOT has been
     truncated towards -infinity, REM will have the same sign as
     DENOM and the opposite sign of NUMER; if both are negative
     and QUOT has been truncated towards -infinity, REM will be
     positive (will have the opposite sign of NUMER).  These are
     considered `wrong'.  If both are NUM and DENOM are positive,
     RESULT will always be positive.  This all boils down to: if
     NUMER >= 0, but REM < 0, we got the wrong answer.  In that
     case, to get the right answer, add 1 to QUOT and subtract
     DENOM from REM.  */

    if (numer && result.rem < 0)
    {
        ++result.quot;
        result.rem -= denom;
    }

    return result;
}


#define BUFSIZE (sizeof(unsigned long long) * 8 + 1)

char *ltoa(unsigned long long N, char *str, int base)
{
      register int i = 2;
      unsigned long long uarg;
      char *tail, *head = str, buf[BUFSIZE] = {0};

      if (36 < base || 2 > base)
            base = 10;                    /* can only use 0-9, A-Z        */
      tail = &buf[BUFSIZE - 1];           /* last character position      */
      *tail-- = '\0';

      if (10 == base)
      {
            *head++ = '-';
            uarg    = -N;
      }
      else  uarg = N;

      if (uarg)
      {
            for (i = 1; uarg; ++i)
            {
                  register ldiv_t res;

                  res       = ldiv(uarg, base);
                  *tail-- = (char)(res.rem + ((9L < res.rem) ?
                                  ('A' - 10L) : '0'));
                  uarg    = res.quot;
            }
      }
      else  *tail-- = '0';

      memcpy(head, ++tail, i);
      return str;
}

int itoa(int value, char *sp, int radix) {
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign)
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp) {
        *sp++ = *--tp;
	}

	*sp = '\0';
    return len;
}

int putchar(int ic) {
	char c = (char) ic;
	//terminal_write(&c, sizeof(c));
	return ic;
}


int atoi(char* str) {
	int returnVal = 0;

	int multiplier = 1;

	if(str[0] == '-')
		multiplier = -1;

	for(int i = 0; i < strlen(str); i++) {
		returnVal = returnVal*10 + str[i] - '0';
	}

	return returnVal * multiplier;
}


static bool print(char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == -1)
			return false;
	return true;
}

int sprintf(char* buffer, const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	// set the buffer to zero before any writing
	int i = 0;
	while(buffer[i] != 0) { buffer[i] = 0; i++; }

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		buffer[written] = format[0];

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				buffer[written+amount] = format[amount];
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print((char*)format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		char* format_begun_at = (char*)(format++);

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			buffer[written] = c;
			written++;
		}
		else if (*format == 'x') {
			format++;

			char fmt_buffer[16] = {0};
			ltoa(va_arg(parameters, int), fmt_buffer, 16);

			// buffer[written] = '0';
			// buffer[written+1] = 'x';

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount] = fmt_buffer[amount];
				amount++;
			}

			written+=amount;
		}
		else if (*format == 'd' || *format == 'i') {
			format++;

			char fmt_buffer[16] = {0};
			itoa(va_arg(parameters, int), fmt_buffer, 10);

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount] = fmt_buffer[amount];
				amount++;
			}

			written+=amount;
		}
		else if (*format == 's') {
			format++;
			std::string str = va_arg(parameters, std::string);

			written += str.size();
		}
		else {
			format = format_begun_at;
			size_t len = strlen((char*)format);

			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}


int vsprintf(char* buffer, const char* format, va_list parameters) {
	// set the buffer to zero before any writing
	int i = 0;
	while(buffer[i] != 0) { buffer[i] = 0; i++; }

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		buffer[written] = format[0];

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				buffer[written+amount] = format[amount];
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print((char*)format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		char* format_begun_at = (char*)(format++);

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			buffer[written] = c;
			written++;
		}
		else if (*format == 'x') {
			format++;

			char fmt_buffer[16] = {0};

			ltoa(va_arg(parameters, int), fmt_buffer, 16);

			buffer[written] = '0';
			buffer[written+1] = 'x';

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount+2] = fmt_buffer[amount];
				amount++;
			}

			written+=amount+2;
		}
		else if (*format == 'd' || *format == 'i') {
			format++;

			char fmt_buffer[16] = {0};
			itoa(va_arg(parameters, int), fmt_buffer, 10);

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount] = fmt_buffer[amount];
				amount++;
			}

			written+=amount;

		}
		else if (*format == 's') {
			format++;
			char * str = va_arg(parameters, char*);

			size_t amount = 0;
			while (str[amount] != '\0') {
				buffer[written+amount] = str[amount];
				amount++;
			}

			written += amount;
		}
		else {
			format = format_begun_at;
			size_t len = strlen((char*)format);

			written += len;
			format += len;
		}
	}
	return written;
}


/* FIXME: It'd be nice to configure around these, but the include files are too
   painful.  These macros should at least be more portable than hardwired hex
   constants. */

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define	LONG_MAX	((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#endif

/*
 * Convert a string to a long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
long
strtol(const char *nptr, char **endptr, int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		//errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

template<class T_in, class T_out>
T_out copy(T_in first, T_in last, T_out d_first) {
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}
/* 
   Double to ASCII Conversion without sprintf.
   Roughly equivalent to: sprintf(s, "%.14g", n);
   Modified
*/

#ifndef DTOA_H
#define DTOA_H

#include <math.h>
#include <string.h>
// For printf
//#include <stdio.h>

const double DTOA_PRECISION = 0.000001; // single precision
const short int MAX_DTOA_STRLEN = 24; // depends on DTOA_PRECISION

/**
 * Double to ASCII
 */
unsigned int dtoa(char *s, double n) {
    // handle special cases
	if (!n) {
        strcpy(s, "0");
		return 1;
	} else if (isnan(n)) {
        strcpy(s, "nan");
		return 3;
    } else if (isinf(n)) {
        strcpy(s, "inf");
		return 3;
    } else {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
		n += DTOA_PRECISION * 0.5;
		
        // calculate magnitude
        m = log10(n);
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        // set up for scientific notation
        if (useExp) {
            m -= (m < 0);
            n /= pow(10, m);
            m1 = m;
            m = 0;
        }
        else if (m < 1.0) {
            m = 0;
        }
        
        // convert the number
        while (n > DTOA_PRECISION || m >= 0) {
            double weight = pow(10, m);
            if (weight > 0.0 && !isinf(weight)) {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (!m && n > 0.0)
                *(c++) = '.';
            m--;
        }
        if (useExp) {
            // convert the exponent
            int i, j;
            *(c++) = 'e';
            if (m1 > 0) {
                *(c++) = '+';
            } else {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0) {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--) {
                // swap without temporary
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
		return (c - s);
    }
}

#endif
/*
**  list.c
**  TCSS 333 - Winter 2015
**
**  Assignment 7 - 32 bit binary representation of float number
**  Alex Terikov (teraliv@uw.edu)
**  3/7/16.
*/

#include <stdio.h>

/* Union to represent 32 bit float number.
** This union will convert float number to
** 32 bits int
*/
typedef union {
    float fnum;
    unsigned int bits : 32;
} FloatBin;

// unsigned char represent a BYTE
typedef unsigned char BYTE;

// function prototypes
void printFloatBinary(FloatBin fb);
void printSEF(FloatBin fb);
void createFraction(FloatBin fb);

int main (void) {

    // user's input
    float fn;
    printf("Enter a float: ");
    scanf("%f", &fn);   // read float number from console
    printf("Your float was read as: %f\n", fn);

    FloatBin fb;
    fb.fnum = fn;

    printFloatBinary(fb);
    printSEF(fb);
    createFraction(fb);

    return 0;
}

/* A function to print 32 bit representation of float number.*/
void printFloatBinary(FloatBin fb) {
    int i;

    printf("Your float in 32 bits: ");
    // print bits
    for ( i = 0; i < 32; i++) {
        // bit pattern to print current most significant bit
        printf("%d", (fb.bits & 0x80000000) >> 31);
        fb.bits = fb.bits << 1; // shift one bit left
    }
    printf("\n");
}

/* A function to prin sign, exponent, and fraction 
** parts of float number separetely.
*/
void printSEF(FloatBin fb) {
    int i;

    // print SEF
    for (i = 0; i < 32; i++) {
        if (i == 0)   
            printf("Sign: ");
        else if (i == 1)
            printf("\nExponent: ");
        else if (i == 9)
            printf("\nFraction: ");

        printf("%d", (fb.bits & 0x80000000) >> 31);        
        fb.bits = fb.bits << 1;
    }
    printf("\n");
}

/* A function to print the process of creating fraction and exponent
** of float number and print the final answer.
*/
void createFraction(FloatBin fb) {
    BYTE    b;
    int     i, exp, bias, sign;
    double  num = 1.0, den = 2.0;
    float   fract = 1.0, result = 1.0;

    bias = ((fb.bits & 0x7F800000) >> 23) - 127; // exponent bias
    sign = (fb.bits & 0x80000000) >> 31; // sign
    fb.bits = fb.bits << 9;

    printf("\nCreating the fraction:\n");
    printf("fraction = 1.000000 (the implicit 1)\n");

    // fraction: 23 bits only
    for (i = 0; i < 23; i++) {
        b = (fb.bits & 0x80000000) >> 31; // current fraction bit
        fract = num / den;

        if (b == 1) {
            result += fract;
            printf("fraction = %f, after adding %f\n", result, fract);
        } 
        else
            printf("fraction = %f, after skipping %f\n", result, fract);  
        
        fb.bits = fb.bits << 1;
        den += den;
    }
    
    printf("\nApplying the exponent:\n");
    printf("unbiased exponent = %d\n", bias);
    if (bias > 0) {
        for (i = 0; i < bias; i++)
            printf("times 2 = %f\n", result *= 2);
    }
    else {
        for (i = 0; i > bias; i--)
            printf("divided by 2 = %f\n", result /= 2);
    }

    // make answer negative if sign is 1
    if (sign == 1)  
        result -= (result * 2);
    
    printf("\nFinal Answer: %f\n", result);
}

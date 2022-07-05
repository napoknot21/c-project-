//
// Created by Kevin on 05/07/2022.
//
#include <stdlib.h>

#ifndef C_PROJECT_LIB_H
#define C_PROJECT_LIB_H
/**
 * Indicate if the char given in argument is an operator
 * @param s The char which will be tested.
 * @return return true if the char is an operator, false otherwise.
 */
static int isAnOperator(char s);

/**
 * Indicate if the char a has a higher precedence than b.
 * @param a The first operator.
 * @param b The second operator.
 * @return true if a has a higher precedence than b, false otherwise.
 */
static int isHigher(char a, char b);

/**
 * Associate a priority with an operator.
 * @param a The operator.
 * @return The operator's priority
 */
static unsigned short priority(char a);

/**
 * Trim the string with the given regex. <br>
 * For example, all the space before the other character is ignored, and all the space after are deleted.
 * @param s The string which will be treated.
 * @return return the trimmed string
 */
static char *trim(const char *s, size_t len);

/**
 * Check is the given character is a sign symbol (+ or -) or is a number
 * @param c The character that will be tested.
 * @return true if its the case, false otherwise.
 */
static int isSignOrNumber(char c);

static char *intToString(int n);
#endif //C_PROJECT_LIB_H

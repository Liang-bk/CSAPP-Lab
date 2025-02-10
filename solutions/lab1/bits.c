/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * name: Liang-bk
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~((~((~x) & y)) & (~(x & (~y))));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return ((~0) << 31);
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // Tmax and -1
  // !(x + 1) when x == -1 --> 1, others will be 0
  return !(!(x + 1) + x + x + 2);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int odds = 0xAA;
  int tmp = odds;
  odds = tmp + (odds << 8);
  tmp = odds;
  odds = tmp + (odds << 16);
  return !((odds & x) ^ odds);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // 0x30 == 0011 0000
  // 0x39 == 0011 1001
  // should be 1 + 1 + 1
  // 1st uses 3 ops, 2nd uses 4 ops, 3rd uses 4 ops, finally 4 ops
  // 3 + 4 + 4 + 4 = 15 ops
  int first = !(x >> 8);
  int second = !(((x >> 4) & 0xF) ^ 0x3);
  int third = !(((x & 0xF) + 6) & 0xF0);
  return !((first + second + third) ^ 3);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int negX = !x;
  x = !negX;
  // x == 1 -> xy = y
  // x == 0 -> xy = 0
  return (((~negX) + 1) & z) + (((~x) + 1) & y);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // sign bit of x and y, compare it
  int signX = (x >> 31) & 1;
  int signY = (y >> 31) & 1;
  int case1 = signX & (~signY);
  // sign bit of x and y are same
  int sameSign = !(signX ^ signY);
  int case2 = sameSign & 
                      ((!(x ^ (1 << 31))) | 
                      (!((((~x + 1) + y) >> 31) & 1)));
  return case1 | case2;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int sign = (x >> 31) & 1;
  int negX = (~x) + 1;
  int negSign = (negX >> 31) & 1;
  return (~(sign | negSign)) + 2;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int sign = (x >> 31) & 1;
  int bits16 = 0;
  int bits8 = 0;
  int bits4 = 0;
  int bits2 = 0;
  int bits1 = 0;
  int bits0 = 0;
  // sign: 0 -> x, 1 -> ~x
  x = (((~(!sign)) + 1) & x) + (((~sign) + 1) & (~x));
  bits16 = (!!(x >> 16)) << 4;
  x = x >> bits16;
  bits8 = (!!(x >> 8)) << 3;
  x = x >> bits8;
  bits4 = (!!(x >> 4)) << 2;
  x = x >> bits4;
  bits2 = (!!(x >> 2)) << 1;
  x = x >> bits2;
  bits1 = (!!(x >> 1));
  x = x >> bits1;
  bits0 = (!!x);
  return bits0 + bits1 + bits2 + bits4 + bits8 + bits16 + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned end = (0x7F << 16) | (0xFF << 8) | 0xFF;
  unsigned s = (uf >> 31) & 1;
  unsigned exp = (uf >> 23) & 0xFF;
  unsigned frac = uf & end;
  unsigned NaN = !(exp ^ 0xFF);
  if (NaN) {
    return uf;
  }

  if (!exp) {
    frac = (frac << 1);
    return (s << 31) | (exp << 23) | frac;
  }
  // exp will never overflow
  if (exp) {
    exp = exp + 1;
    return (s << 31) | (exp << 23) | frac;
  }
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned end = (0x7F << 16) | (0xFF << 8) | 0xFF;
  unsigned s = (uf >> 31) & 1;
  unsigned exp = (uf >> 23) & 0xFF;
  unsigned frac = uf & end;
  unsigned NaN = !(exp ^ 0xFF);
  unsigned max = 0x9E << 23;
  unsigned uf_no_s = uf & 0x7FFFFFFF;
  if (NaN) {
    return 0x80000000u;
  }
  // < 1
  if (exp < 0x7F) {
    return 0;
  }
  // >= 2^31 or < -2^31
  if ((!s && uf_no_s >= max) || (s && uf_no_s > max)) {
    return 0x80000000u;
  }
  // >= 1
  frac = frac | (1 << 23);
  if (exp > 0x96) {
    frac = frac << (exp - 0x96);
  } else {
    frac = frac >> (0x96 - exp);
  }
  if (s) {
    frac = (~frac) + 1;
  }
  return frac;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  // -> 0
  // -> +INF
  unsigned pos_inf = 0xFF << 23;
  // < -149
  // -149 <= x < -126
  // -126 <= x <= 127 
  // 127 < x
  if (x < -149) {
    return 0;
  } else if (x < -126) {
    return 1 << (x + 149);
  } else if (x <= 127) {
    return (x + 127) << 23;
  } else {
    return pos_inf;
  }
}

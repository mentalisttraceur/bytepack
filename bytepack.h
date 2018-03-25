/*****************************************************************************\
 * Copyright (C) Alexander Kozhevnikov <mentalisttraceur@gmail.com> 2015, 2016
 * 
 * "bytepack"/"byteback" name idea contributed by Mike Serritella
 *     <author@mikeserritella.com> 2015-09-15
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
\*****************************************************************************/

#ifndef BYTEPACK_C_H
#define BYTEPACK_C_H

#include <limits.h> /* CHAR_BIT, UCHAR_MAX */


/* Declarations and explanations *********************************************/
/* When in doubt, assume this code does not error-check your arguments. */

#define bytepack_CONTINUE_BIT (unsigned char )((UCHAR_MAX >> 1) + 1)
/*\
The most significant bit of the unsigned char is the continue bit.
\*/

#define bytepack_SIGN_BIT (unsigned char )((UCHAR_MAX >> 2) + 1)
/*\
The second most significant bit of the unsigned char is the sign bit.
\*/

#define bytepack_NOT_CONTINUE_SIGN_BITS (unsigned char )(UCHAR_MAX >> 2)
/*\
All but the continue and sign bits of an unsigned char.
\*/

/* Macro definitions *********************************************************/

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 val has a value >= 0
 typeof(val) is an integral type
Mangles (these values are altered by this macro):
 val: approaches to 0 with each iteration.
Insertable Code (must be actual C code snippets):
 consumeByte_c:
  Code that receives each packed byte. This is where actual writing/counting,
  size/bounds checking, resume support, etc, is actually implemented.
\*/
#define bytepack_uGeneric_m(val, consumeByte_c) \
do \
{ \
 unsigned char bytepack_uGeneric_m_byte = val; \
 val >>= CHAR_BIT - 1; \
 if(val) \
 { \
  val -= 1; \
  bytepack_uGeneric_m_byte |= bytepack_CONTINUE_BIT; \
 } \
 consumeByte_c \
} \
while(val);

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 typeof(val) is an unsigned integral type
 OR typeof(val) is a signed integral type and the bytepack value (or the
  largest or smallest value representable by T, whichever has smallest
  magnitude) is within the range of values representable by typeof(val)
 T is an unsigned integral type
 OR T is a signed integral type or a type which is promoted to an integer
  during integer promotion, and the bytepack value is is within the range of
  values representable by T
Sets (these are the "return values" of this macro):
 val: The value of the bytepack
Insertable Code (must be actual C code snippets):
 produceByte_c:
  Code that provides each packed byte. This is where actual reading/counting,
  size/bounds checking, resume support, etc, is actually implemented.
\*/
#define byteback_uGeneric_m(val, T, produceByte_c) \
{ \
 unsigned char byteback_uGeneric_m_byte; \
 unsigned int byteback_uGeneric_m_shift = 0; \
 for(;;) \
 { \
  produceByte_c \
  val += (T )byteback_uGeneric_m_byte << byteback_uGeneric_m_shift; \
  if(!(byteback_uGeneric_m_byte & bytepack_CONTINUE_BIT)) \
  { \
   break; \
  } \
  byteback_uGeneric_m_shift += CHAR_BIT - 1; \
 } \
}

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 typeof(val) is an integral type
Mangles (these values are altered by this macro):
 val: approaches to 0 with each iteration.
Insertable Code (must be actual C code snippets):
 consumeByte_c:
  Code that receives each packed byte. This is where actual writing/counting,
  size/bounds checking, resume support, etc, is actually implemented.
\*/
#define bytepack_sGeneric_m(val, consumeByte_c) \
{ \
 unsigned char bytepack_sGeneric_m_byte; \
 if(val < 0) \
 { \
  val += 1; \
  val = -val; \
  bytepack_sGeneric_m_byte = val & bytepack_NOT_CONTINUE_SIGN_BITS; \
  bytepack_sGeneric_m_byte += 1; \
  val >>= CHAR_BIT - 2; \
  val += bytepack_sGeneric_m_byte >> (CHAR_BIT - 2); \
  bytepack_sGeneric_m_byte |= bytepack_SIGN_BIT; \
 } \
 else \
 { \
  bytepack_sGeneric_m_byte = val & bytepack_NOT_CONTINUE_SIGN_BITS; \
  val >>= CHAR_BIT - 2; \
 } \
 goto bytepack_sGeneric_m_loopSkipReInit; \
 do \
 { \
  bytepack_sGeneric_m_byte = val; \
  val >>= CHAR_BIT - 1; \
 bytepack_sGeneric_m_loopSkipReInit: \
  if(val) \
  { \
   val -= 1; \
   bytepack_sGeneric_m_byte |= bytepack_CONTINUE_BIT; \
  } \
  consumeByte_c \
 } \
 while(val); \
}

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 typeof(val) is an unsigned integral type
 OR typeof(val) is a signed integral type and the bytepack value (or the
  largest or smallest value representable by T, whichever has smallest
  magnitude) is within the range of values representable by typeof(val)
 T is an unsigned integral type, and the val value is representable by T
 OR T is a signed integral type or a type which is promoted to an integer
  during integer promotion, the bytepack value and the val value are within
  the range of values representable by T
Sets (these are the "return values" of this macro):
 val: The value of the bytepack
Insertable Code (must be actual C code snippets):
 produceByte_c:
  Code that provides each packed byte. This is where actual reading/counting,
  size/bounds checking, resume support, etc, is actually implemented.
\*/
#define byteback_sGeneric_m(val, T, produceByte_c) \
{ \
 T byteback_sGeneric_m_val = val; \
 unsigned char byteback_sGeneric_m_signBit; \
 do \
 { \
  unsigned char byteback_sGeneric_m_byte; \
  unsigned int byteback_sGeneric_m_shift; \
  produceByte_c \
  byteback_sGeneric_m_signBit = byteback_sGeneric_m_byte & bytepack_SIGN_BIT; \
  byteback_sGeneric_m_byte ^= byteback_sGeneric_m_signBit; \
  val = -byteback_sGeneric_m_byte; \
  if(!(byteback_sGeneric_m_byte & bytepack_CONTINUE_BIT)) \
  { \
   break; \
  } \
  val += bytepack_SIGN_BIT; \
  byteback_sGeneric_m_shift = CHAR_BIT - 2; \
  for(;;) \
  { \
   produceByte_c \
   val -= (T )byteback_sGeneric_m_byte << byteback_sGeneric_m_shift; \
   if(!(byteback_sGeneric_m_byte & bytepack_CONTINUE_BIT)) \
   { \
    break; \
   } \
   byteback_sGeneric_m_shift += CHAR_BIT - 1; \
  } \
 } \
 while(0); \
 if(!byteback_sGeneric_m_signBit) \
 { \
  val = -val; \
 } \
 val += byteback_sGeneric_m_val; \
}

#endif /* BYTEPACK_C_H */

/*****************************************************************************\
 * Copyright (C) Alexander Kozhevnikov <mentalisttraceur@gmail.com> 2016-04-08
 * 
 * "bytepack"/"byteback" name idea contributed by Mike Serritella
 *     <author@mikeserritella.com> 2015-09-15
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licence as published by
 * the Free Software Foundation, either version 3 of the licence or,
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for details.
 * 
 * You should've received a copy of the GNU General Public License
 * with this program. If not, see <http://www.gnu.org/licences/>,
 * or write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330 Boston MA 02111-1307 USA.
\*****************************************************************************/

#ifndef BYTEPACK_C_H
#define BYTEPACK_C_H

#include <limits.h> /* UCHAR_MAX */


/* Declarations and explanations *********************************************/
/* When in doubt, assume this code does not error-check your arguments. */

#define UCHAR_MASK_NOT_TOP_N_BITS_m(n) (unsigned char )(UCHAR_MAX >> (n))
/*\
All but the n most significant bits of an unsigned char.
\*/

#define UCHAR_NTH_BIT_m(n) (unsigned char )(1 << (n - 1))
/*\
The nth (1-indexed) bit of an unsigned char.
Triggers C undefined behavior if n < 1.
\*/

/* Macro definitions *********************************************************/

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 val has a value >= 0
 typeof(val) is an integral type
Mangles (these values are altered by this macro):
 val: approaches to 0 with each iteration.
Code Snippet Arguments (must be actual C code snippets):
 loopBodyEnd_c:
  Goes at the end of a body of the internal do-while loop. Intended for resume
  support, size/bounds checking, and writing/counting the byteback bytes.
\*/
#define bytepack_uGeneric_m(val, loopBodyEnd_c) \
do \
{ \
 unsigned char bytepack_uGeneric_m_byte = val; \
 val >>= CHAR_BIT - 1; \
 if(val) \
 { \
  val -= 1; \
  bytepack_uGeneric_m_byte |= UCHAR_NTH_BIT_m(CHAR_BIT); \
 } \
 loopBodyEnd_c \
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
  values presentable by T
Sets (these are the "return values" of this macro):
 val: The value of the bytepack
Insertable Code (must be actual C code snippets):
 loopBodyStart_c:
  Goes at the start of a body of the internal for loop. Intended for resume
  support, size/bounds checking, and reading/counting the byteback bytes.
\*/
#define byteback_uGeneric_m(val, T, loopBodyStart_c) \
for \
( \
 unsigned char byteback_uGeneric_m_continueBit = UCHAR_NTH_BIT_m(CHAR_BIT), \
  byteback_uGeneric_m_byte, byteback_uGeneric_m_shift = 0; \
 byteback_uGeneric_m_continueBit; \
 byteback_uGeneric_m_shift += CHAR_BIT - 1 \
) \
{ \
 loopBodyStart_c \
 byteback_uGeneric_m_continueBit &= byteback_uGeneric_m_byte; \
 val += (T )(byteback_uGeneric_m_byte << byteback_uGeneric_m_shift); \
}

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 typeof(val) is an integral type
Mangles (these values are altered by this macro):
 val: approaches to 0 with each iteration.
Code Snippet Arguments (must be actual C code snippets):
 loopBodyEnd_c:
  Goes at the end of a body of the internal do-while loop. Intended for resume
  support, size/bounds checking, and writing/counting the byteback bytes.
\*/
#define bytepack_sGeneric_m(val, loopBodyEnd_c) \
do \
{ \
 unsigned char bytepack_sGeneric_m_signBit; \
 if(val < 0) \
 { \
  bytepack_sGeneric_m_signBit = UCHAR_NTH_BIT_m(CHAR_BIT - 1); \
  val += 1; \
  val = -val; \
 } \
 else \
 { \
  bytepack_sGeneric_m_signBit = 0; \
 } \
 unsigned char bytepack_sGeneric_m_byte = val; \
 bytepack_sGeneric_m_byte &= UCHAR_MASK_NOT_TOP_N_BITS_m(2); \
 val >>= CHAR_BIT - 2; \
 if(bytepack_sGeneric_m_signBit) \
 { \
  bytepack_sGeneric_m_byte += 1; \
  if(bytepack_sGeneric_m_signBit & bytepack_sGeneric_m_byte) \
  { \
   val += 1; \
  } \
  else \
  { \
   bytepack_sGeneric_m_byte |= bytepack_sGeneric_m_signBit; \
  } \
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
   bytepack_sGeneric_m_byte |= UCHAR_NTH_BIT_m(CHAR_BIT); \
  } \
  loopBodyEnd_c \
 } \
 while(val); \
} \
while(0);

/*\
Assumes (violating these could be anything from harmless to catostrophic):
 typeof(val) is an unsigned integral type
 OR typeof(val) is a signed integral type and the bytepack value (or the
  largest or smallest value representable by T, whichever has smallest
  magnitude) is within the range of values representable by typeof(val)
 T is an unsigned integral type
 OR T is a signed integral type or a type which is promoted to an integer
  during integer promotion, and the bytepack value is is within the range of
  values presentable by T
Sets (these are the "return values" of this macro):
 val: The value of the bytepack
Insertable Code (must be actual C code snippets):
 loopBodyStart_c:
  Goes at the start of a body of the internal for loop. Intended for resume
  support, size/bounds checking, and reading/counting the byteback bytes.
\*/
#define byteback_sGeneric_m(val, T, loopBodyStart_c) \
do \
{ \
 unsigned char byteback_sGeneric_m_byte; \
 loopBodyStart_c \
 unsigned char byteback_sGeneric_m_shift = 0; \
 unsigned char byteback_sGeneric_m_continueBit \
 = byteback_sGeneric_m_byte & UCHAR_NTH_BIT_m(CHAR_BIT); \
 unsigned char byteback_sGeneric_m_signBit \
 = byteback_sGeneric_m_byte & UCHAR_NTH_BIT_m(CHAR_BIT - 1); \
 byteback_sGeneric_m_byte &= UCHAR_MASK_NOT_TOP_N_BITS_m(2); \
 if(byteback_sGeneric_m_continueBit) \
 { \
  byteback_sGeneric_m_byte |= UCHAR_NTH_BIT_m(CHAR_BIT - 1); \
 } \
 if(byteback_sGeneric_m_signBit) \
 { \
  val -= byteback_sGeneric_m_byte; \
 } \
 else \
 { \
  val += byteback_sGeneric_m_byte; \
 } \
 byteback_sGeneric_m_shift = CHAR_BIT - 2; \
 while(byteback_sGeneric_m_continueBit) \
 { \
  loopBodyStart_c \
  byteback_sGeneric_m_continueBit &= byteback_sGeneric_m_byte; \
  if(byteback_sGeneric_m_signBit) \
  { \
   val -= (T )(byteback_sGeneric_m_byte << byteback_sGeneric_m_shift); \
  } \
  else \
  { \
   val += (T )(byteback_sGeneric_m_byte << byteback_sGeneric_m_shift); \
  } \
  byteback_sGeneric_m_shift += CHAR_BIT - 1; \
 } \
} \
while(0);

#endif /* BYTEPACK_C_H */

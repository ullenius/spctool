/***************************************************************************************************
* File:       String Functions                                                                     *
* Platform:   x86                                                                                  *
* Programmer: Anti Resonance                                                                       *
*                                                                                                  *
* These are string functions I needed that aren't found in the standard C libraries.               *
*                                                                                                  *
*                                                                                                  *
* This program is free software; you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation; either version 2 of     *
* the License, or (at your option) any later version.                                              *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* See the GNU General Public License for more details.                                             *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program;       *
* if not, write to the Free Software Foundation, Inc.                                              *
* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                                        *
*                                                                                                  *
* ------------------------------------------------------------------------------------------------ *
* Revision History:                                                                                *
*                                                                                                  *
* 2004.09.04                                                                                       *
* + Changed CmpCopyL() to CmpCopy() because the length part was a hackish way to get a feature to  *
*   work in SNESamp that's no longer needed                                                        *
* + Changed CmpStrL() to CmpMem()                                                                  *
* + Added FindStrI()                                                                               *
*                                                                                                  *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/

#ifdef	__cplusplus
extern "C"
{
#endif

//**************************************************************************************************
// Find String End
//
// Returns a pointer to the NULL terminator of a string
//
// In:
//    pStr -> String to search
//
// Out:
//    Pointer to the end of the string

s8* StrEnd(const s8 *pStr);


//**************************************************************************************************
// Reverse Scan String for Character
//
// Scans a string for a character starting at the end of the string.  If the character is found,
// a pointer to the next character is returned.  Otherwise a pointer to the beginning of the
// string is returned.
//
// In:
//    str -> String to search in
//    c    = Character to search for
//
// Out:
//    Pointer to the character after the last occurrence of the character searched for

s8* ScanStrR(const s8* str, s8 c);


//**************************************************************************************************
// Find String in String, Case Insensitive
//
// Searches for a string inside of another string.  Both strings are not case sensitive.
//
// In:
//    pDest -> String to search in
//    pSrc  -> String to search for
//
// Out:
//    Pointer to the first occurrence of pSrc in pDest, NULL if the string was not found

s8* FindStrI(const s8 *pDest, const s8 *pSrc);


//**************************************************************************************************
// Compare Memory
//
// Compares two arrays
//
// In:
//    p1 -> First array
//    p2 -> Second array
//    len = Number of bytes to compare
//
// Out:
//    true, if bytes are the same (if 'len' is 0, function always returns true)

b8 CmpMem(const void *p1, const void *p2, u32 len);


//**************************************************************************************************
// Copy String
//
// Copies a string from one buffer to another
//
// In:
//    dest-> Buffer to copy string to
//    src -> String to copy
//
// Out:
//    Pointer to the end of the destination string after copy

s8* CopyStr(s8* dest, const s8* src);


//**************************************************************************************************
// Copy String with Length
//
// Copies a string from one buffer to another.  Characters from the source string are copied until
// a given number of characters are copied or NULL is found, which ever happens first.
//
// If the length is passed as a negative value, the destination string will padded with spaces, if
// necessary, to fill the length.
//
// In:
//    dest-> Buffer to copy string to
//    src -> String to copy
//    l    = Length of destination string (negative for padding)
//
// Out:
//    Pointer to the end of the destination string after copy

s8* CopyStrL(s8* dest, const s8* src, s32 l);


//**************************************************************************************************
// Copy String up to Character
//
// Copies a string from one buffer to another.  Characters from the source string are copied until
// the specified character or NULL is found.
//
// In:
//    dest-> Buffer to copy string to
//    src -> String to copy
//    c    = Character in src to stop copying on
//
// Out:
//    Pointer to the end of the destination string after copy

s8* CopyStrC(s8* dest, const s8* src, s8 c);


//**************************************************************************************************
// Compare and Copy String
//
// Compares two strings and copies the source string to the destination
//
// In:
//    dest -> Buffer to copy string to
//    src  -> String to copy
//
// Out:
//    true, if strings are equal

b8 CmpCopy(s8 *dest, const s8 *src);


//**************************************************************************************************
// Convert Integer to Hex
//
// Generates a given number of hex digits from a binary value.  The given number of digits are
// always created, regardless of the actual value of the integer.
//
// ex. Int2Hex(0x1AB, 2);
//     returns "AB"
//
// The pointer returned is for temporary use only.  Once Int2Hex is called, the resulting string
// should be copied into a permanent buffer.
//
// In:
//    i = Number to convert
//    d = Number of digits to create (0 to 8)
//
// Out:
//    -> string

s8*	Int2Hex(u32 i, u32 d);


//**************************************************************************************************
// Convert Hex to Integer
//
// Converts a string of hex digits into binary form.  Digits that aren't 0-9 or A-F are ignored,
// making it okay to pass complete strings containing characters like x, h, and $.
//
// In:
//    p -> String to convert to binary
//
// Out:
//   Binary value

u32 Hex2Int(s8 *p);


#ifdef	__cplusplus
}
#endif

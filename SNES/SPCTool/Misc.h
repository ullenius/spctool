/***************************************************************************************************
* File:       Miscellaneous functions for SPCTool                                                  *
* Platform:   Win32                                                                                *
* Programmer: Anti Resonance                                                                       *
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
* 0.1  xx.xx.xx                                                                                    *
*                                                           Copyright (C)2001 Alpha-II Productions *
***************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////
//External function headers

//**************************************************************************************************
// Bit Scan Forward
//
// Scans an integer, starting with bit 0, for the first bit that's set
//
// In:
//    i -> Integer
//
// Out:
//    bit position (0-31, -1 if no bit found)

s32 BitScanF(u32 i);


//**************************************************************************************************
// Scaled Log 2 of x
//
// Computes Y*log2(x)
//
// In:
//    scale = Amount to multiply intermediate result by
//    x     = Value to derrive exponent from
//
// Out:
//    Exponent

f80 YLog2(f32 scale, f32 x);


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
//    Pointer to the character after the last occurence of the character searched for

s8* ScanStrR(s8* str, s8 c);


//**************************************************************************************************
// String to Ticks
//
// Converts a string float of seconds into an integer of ticks.
// Time is clipped at 0 | 59.99
//
// In:
//    Str -> String
//
// Out:
//    Time in 1/64000ths second

u32 Str2Ticks(s8 *str);


//**************************************************************************************************
// Identify File
//
// Determines what kind of file is being opened.
//
// In:
//    FN -> Filename
//    FH -> File handle to opened file, if file was successfully identified
//
// Out:
//    0 - Unable to open file
//    1 - .SPC
//    2 - .SPC with extended info
//    3 - .ZST
//   -1 - Unknown file

s32 IDFile(s8 *fn, HANDLE &inFH);

/***************************************************************************************************
* File:       Japanese String Functions                                                            *
* Platform:   Any                                                                                  *
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
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#ifdef	__cplusplus
extern "C"
{
#endif

//**************************************************************************************************
// Katakana to Romaji
//
// Romanizes the Katakana characters (A1-DF of the JIS-X201 character set) in a string
//
// In:
//    pDest -> Output string
//    pSrc  -> Input string

void Romaji(u8 *pDest, u8 *pSrc);

#ifdef	__cplusplus
}
#endif

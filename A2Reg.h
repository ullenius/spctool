/***************************************************************************************************
* File:       Windows Registry Class                                                               *
* Platform:   Windows C++                                                                          *
* Programmer: Anti Resonance                                                                       *
*                                                                                                  *
* A class to simplify reading/writing values from/to the registry                                  *
*                                                                                                  *
* ------------------------------------------------------------------------------------------------ *
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
* 1.0  04.10.25                                                                                    *
*                                                           Copyright (C)2004 Alpha-II Productions *
***************************************************************************************************/

class A2Reg
{
private:
	HKEY	hkey;

public:
		A2Reg();
		~A2Reg();

	//**********************************************************************************************
	// Key Functions
	//
	// Open() opens a key for reading.
	// Create() opens a key for writing.  If the key doesn't exist, one is created.
	// Remove() removes a key from the registry.  The key must not be open or contain any sub-keys.
	// Close() closes the open key.
	//
	// In:
	//    key -> Name of key in HKEY_CURRENT_USER
	//
	// Out:
	//    true, if operation succeeded

	b8	__fastcall Open(s8 *key);
	b8	__fastcall Create(s8 *key);
	b8	__fastcall Remove(s8 *key);
	v0	__fastcall Close();

	
	//**********************************************************************************************
	// Read Registry Values
	//
	// Notes:
	//    No range checking is performed if 'min' == 'max'.
	//    When reading a floating-point number, if range checking is performed, denormalized values
	//     and NaNs will always return 'def'.
	//    When reading a string, 'size' is the size of the array including a byte for the NULL.
	//
	// In:
	//    val -> Name of registry value to read
	//    def  = Default value if entry does not exist in registry
	//    min  = Minimum value to return
	//    max  = Maximum value to return
	//    size = Size of storage buffer (in bytes)

	b8	__fastcall Get(s8 *val, b8 def);
	s32	__fastcall Get(s8 *val, s32 def, s32 min = 0, s32 max = 0);
	u32	__fastcall Get(s8 *val, u32 def, u32 min = 0, u32 max = 0);
	f32	__fastcall Get(s8 *val, f32 def, f32 min = 0.f, f32 max = 0.f);
	f64	__fastcall Get(s8 *val, f64 def, f64 min = 0.0, f64 max = 0.0);
	u32 __fastcall Get(s8 *val, s8 *str, u32 size, s8 *def = "");
	u32	__fastcall Get(s8 *val, void *pData, u32 size);

	
	//**********************************************************************************************
	// Write Registry Values
	//
	// In:
	//    val  -> Name of registry value to write
	//    data  = Value to write
	//
	//    str  -> NULL terminated ANSI string
	//
	//    pData-> Data to write
	//    size  = Size of data in bytes

	v0	__fastcall Set(s8 *val, b8 data);		//Type REG_DWORD
	v0	__fastcall Set(s8 *val, s32 data);		//   "  "
	v0	__fastcall Set(s8 *val, u32 data);		//   "  "
	v0	__fastcall Set(s8 *val, f32 data);		//   "  "
	v0	__fastcall Set(s8 *val, f64 data);		//Type REG_QWORD
	v0	__fastcall Set(s8 *val, s8 *str);		//Type REG_SZ
	v0	__fastcall Set(s8 *val, void *pData, u32 size);	//Type REG_BINARY


	//**********************************************************************************************
	// Delete a Registry Value
	//
	// In:
	//    val -> Name of registry value to delete
	//
	// Out:
	//    true, if deletion was successful

	b8	__fastcall Delete(s8 *val);
};
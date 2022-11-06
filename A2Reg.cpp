/***************************************************************************************************
* Windows Registry Class                                                                           *
*                                                           Copyright (C)2004 Alpha-II Productions *
***************************************************************************************************/

#include	<windows.h>
#include	"Types.h"
#include	"A2Reg.h"


//**************************************************************************************************

A2Reg::A2Reg()
{
	hkey = NULL;
}


A2Reg::~A2Reg()
{
	Close();
}


//**************************************************************************************************

b8 __fastcall A2Reg::Open(s8 *key)
{
	Close();
	return !RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_QUERY_VALUE, &hkey);
}


b8 __fastcall A2Reg::Create(s8 *key)
{
	Close();
	if (!RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_WRITE, &hkey)) return 1;
	return !RegCreateKeyEx(HKEY_CURRENT_USER, key, 0, NULL, 0, KEY_WRITE, NULL, &hkey, NULL);
}


b8 __fastcall A2Reg::Remove(s8 *key)
{
	return !RegDeleteKey(HKEY_CURRENT_USER, key);
}


void __fastcall A2Reg::Close()
{
	if (hkey)
	{
		RegCloseKey(hkey);
		hkey = NULL;
	}
}


//**************************************************************************************************

b8 __fastcall A2Reg::Get(s8 *val, b8 def)
{
	DWORD	data,size;

	data = 0;
	size = 4;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)&data, &size)) return def;

	return data != 0;
}


s32 __fastcall A2Reg::Get(s8 *val, s32 def, s32 min, s32 max)
{
	s32		data;
	DWORD	size;


	data = 0;
	size = 4;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)&data, &size) || size != 4) return def;

	if (min != max)
	{
		if (data < min) return min;
		if (data > max) return max;
	}

	return data;
}


u32 __fastcall A2Reg::Get(s8 *val, u32 def, u32 min, u32 max)
{
	u32		data;
	DWORD	size;
	

	size = 4;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)&data, &size) || size != 4) return def;

	if (min != max)
	{
		if (data < min) return min;
		if (data > max) return max;
	}

	return data;
}


f32 __fastcall A2Reg::Get(s8 *val, f32 def, f32 min, f32 max)
{
	f32		data;
	DWORD	size;


	size = 4;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)&data, &size) || size != 4) return def;

	if (*(u32*)&min == *(u32*)&max) return data;

	switch ((u8)(*(u32*)&data >> 23))
	{
	case(0):									//Check for denormalized numbers
	case(255):									// or NaNs
		if (*(u32*)&data & 0x7FFFFF) return def;
	}

	if (data < min) return min;
	if (data > max) return max;

	return data;
}


f64 __fastcall A2Reg::Get(s8 *val, f64 def, f64 min, f64 max)
{
	f64		data;
	DWORD	size;


	size = 8;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)&data, &size) || size != 8) return def;

	if (*(u64*)&min == *(u64*)&max) return data;

	switch ((((u32*)&data)[1] >> 20) & 0x7FF)
	{
	case(0):									//Check for denormalized numbers
	case(2047):									// or NaNs
		if (((u32*)&data)[0] | ((u32*)&data)[1] & 0xFFFFF) return def;
	}

	if (data < min) return min;
	if (data > max) return max;

	return data;
}


u32 __fastcall A2Reg::Get(s8 *val, s8 *str, u32 size, s8 *def)
{
	size++;
	if (RegQueryValueEx(hkey, val, NULL, NULL, (u8*)str, (DWORD*)&size))
	{
		strcpy(str, def);
		return 0;
	}

	return size;
}


u32 __fastcall A2Reg::Get(s8 *val, void *pData, u32 size)
{
	return RegQueryValueEx(hkey, val, NULL, NULL, (u8*)pData, (DWORD*)&size) ? 0 : size;
}


//**************************************************************************************************

void __fastcall A2Reg::Set(s8 *val, b8 data)
{
	u32	d = (data != 0);

	RegSetValueEx(hkey, val, 0, REG_DWORD, (u8*)&d, 4);
}


void __fastcall A2Reg::Set(s8 *val, s32 data)
{
	RegSetValueEx(hkey, val, 0, REG_DWORD, (u8*)&data, 4);
}


void __fastcall A2Reg::Set(s8 *val, u32 data)
{
	RegSetValueEx(hkey, val, 0, REG_DWORD, (u8*)&data, 4);
}


void __fastcall A2Reg::Set(s8 *val, f32 data)
{
	RegSetValueEx(hkey, val, 0, REG_DWORD, (u8*)&data, 4);
}


void __fastcall A2Reg::Set(s8 *val, f64 data)
{
	RegSetValueEx(hkey, val, 0, REG_QWORD, (u8*)&data, 8);
}


void __fastcall A2Reg::Set(s8 *val, s8 *str)
{
	RegSetValueEx(hkey, val, 0, REG_SZ, (u8*)str, strlen(str) + 1);
}


void __fastcall A2Reg::Set(s8 *val, void *pData, u32 size)
{
	RegSetValueEx(hkey, val, 0, REG_BINARY, (u8*)pData, size);
}


//**************************************************************************************************

b8 __fastcall A2Reg::Delete(s8 *val)
{
	return !RegDeleteValue(hkey, val);
}

/***************************************************************************************************
*                                                           Copyright (C)2001 Alpha-II Productions *
***************************************************************************************************/

#include	"Types.h"

#pragma	inline


//**************************************************************************************************
// Bit Scan Forward

#pragma	warn -rvl								//Turn off warning "functions have no return value"
s32 BitScanF(u32 i)
{
	asm
	{
		xor		edx,edx
		bsf		eax,[i]
		setnz	dl
		dec		edx
		or		eax,edx
	}
}


//**************************************************************************************************
// Scaled Log 2 of x

f80 YLog2(f32 scale, f32 x)
{
	asm
	{
		fld		[scale]
		fld		[x]
		fyl2x
	}
}


//**************************************************************************************************
// Reverse Scan String for Character

s8* ScanStrR(s8 *string, s8 c)
{
	asm
	{
		//Find end of string -------------------
		mov		edi,[string]
		xor		eax,eax
		mov		ecx,-1
		repne	scasb
		dec		edi								//EDI -> NULL terminator
		neg		ecx								//ECX = Length of string, including NULL
		dec		ecx

		//Search for character -----------------
		std										//Search backwards
		mov		al,[c]
		repne	scasb
		cld
		sete	al								//EAX = 1 if character found, 0 otherwise
		add		eax,edi							//Add pointer
		inc		eax								//Increase by one to fixup post decrement
	}
}
#pragma	warn +rvl


//**************************************************************************************************
// String to Ticks

u32 Str2Ticks(s8 *cstr)
{
	s8	str[8],*d;
	s32	s,c=0;
	u32	val;


	strcpy(str,cstr);
	s=atoi(str);								//Get integer value of number left of decimal
	d=strchr(str,'.');
	if (d++)									//If there's a decimal point, get fraction
	{
		if (*(d+1)<'0' || *(d+1)>'9')
			*(d+1)='0';							//Add another 0 if there's only one digit
		*(d+2)=0;								//Force string length to two characters
		c=atoi(d);
	}

	if (s<0)									//Time can't be negative
		val=0;
	else
	{
		val=(s*TICKS_SEC)+(c*TICKS_SEC/100);	//Clip time at max possible value (59.99 seconds)
		if (val>TICKS_MIN-(TICKS_SEC/100))
			val=TICKS_MIN-(TICKS_SEC/100);
	}

	return val;
}


//**************************************************************************************************
// Identify File

s32 IDFile(s8 *fn, HANDLE &inFH)
{
	HANDLE	fh;
	DWORD	l;
	s8		header[28],dif=0;
	s32		i;


	//Open file and read header ----------------
	inFH=INVALID_HANDLE_VALUE;
	fh=CreateFile(fn,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if (fh==INVALID_HANDLE_VALUE) return 0;
	ReadFile(fh,header,28,&l,NULL);
	SetFilePointer(fh,0,0,FILE_BEGIN);

	//Look for SPC header ----------------------
	for (i=0;i<28;i++)
		dif+=header[i]-spcID[i];
	if (!dif)
	{
		i=GetFileSize(fh,NULL);
		if (i<0x10200)							//Return invalid, if file is too small
		{
			CloseHandle(fh);
			return -1;
		}
		else
		{
			inFH=fh;
			if (i==0x10200)						//Return normal SPC
				return 1;
			SetFilePointer(fh,0x10200,0,FILE_BEGIN);	//Look for extended information
			ReadFile(fh,header,4,&l,NULL);
			SetFilePointer(fh,0,0,FILE_BEGIN);
			if (*(u32*)&header-*(u32*)&xid6==0)
				return 2;
			else
				return 1;
		}
	}

	//Look for ZST header ----------------------
	dif=0;
	for (i=0;i<21;i++)
		dif+=header[i]-zstID[i];
	if (!dif)
	{
		if (GetFileSize(fh,NULL)<266879)		//Return invalid, if file is too small
		{
			CloseHandle(fh);
			return -1;
		}
		inFH=fh;
		return 3;
	}

	CloseHandle(fh);
	return -1;
}

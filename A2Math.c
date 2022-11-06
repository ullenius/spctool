/***************************************************************************************************
* Math Functions                                                                                   *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/

//Turn off warning "functions have no return value"
#if defined	__BORLANDC__
#pragma	inline
#pragma	warn -rvl
#elif defined	_MSC_VER
#pragma warning(disable : 4035)
#endif

#include	"Types.h"

//**************************************************************************************************
// Bit Scan Forward

s32 BitScanF(u32 i)
{
#ifdef	__GNUC__
	s32	ret;


	asm("
		xorl	%%edx,%%edx
		bsfl	%1,%0
		setnz	%%dl
		decl	%%edx
		orl		%%edx,%0

	" : "=r" (ret) : "r" (i) : "edx");

	return ret;
#else
	_asm
	{
		xor		edx,edx
		bsf		eax,[i]
		setnz	dl
		dec		edx
		or		eax,edx
	}
#endif
}


//**************************************************************************************************
// Bit Scan Reversed

s32 BitScanR(u32 i)
{
#ifdef	__GNUC__
	s32	ret;


	asm("
		xorl	%%edx,%%edx
		bsrl	%1,%0
		setnz	%%dl
		decl	%%edx
		orl		%%edx,%0

	" : "=r" (ret) : "r" (i) : "edx");

	return ret;
#else
	_asm
	{
		xor		edx,edx
		bsr		eax,[i]
		setnz	dl
		dec		edx
		or		eax,edx
	}
#endif
}


//**************************************************************************************************
// Bit Count

s32 BitCount(u32 i)
{
#ifdef	__GNUC__
	s32	ret;


	asm("
		xorl	%%eax,%%eax
	1:
		adcb	0,%%al
		shrl	$1,%1
		jnz		1
		adcb	0,%%al

	" : "=&a" (ret) : "r" (i));

	return ret;
#else
	_asm
	{
		xor		eax,eax
		mov		ecx,[i]
	Bit:
		adc		al,0
		shr		ecx,1
		jnz		short Bit
		adc		al,0
	}
#endif
}


//**************************************************************************************************
// Round Float to Integer

s32 F2I(f64 f)
{
#ifdef	__GNUC__
	s32	ret;
	

	asm("
		fistl	%0
	" : "=m" (ret) : "f" (f));

	return ret;
#else
	_asm
	{
		fld		[f]
		fistp	dword ptr [esp-4]
		mov		eax,[esp-4]
	}
#endif
}


//**************************************************************************************************
// Logarithm

f64 Logx(f64 base, f64 value)
{
#ifdef	__GNUC__
	f80	ret;


	//*** This doesn't work. ***
	//*** GCC tries to pop the stack, but fyl2x already does that. ***
	asm("
		fyl2x
		fld1
		fdivrp	%%st,%%st(1)
		fld		%3
		fyl2x
	" : "=t" (ret) : "f" (1.0), "0" (base), "m" (value));

	return ret;
#else
	_asm
	{
		fld1
		fld		[base]
		fyl2x
		fld1
		fdivrp	st(1),st
		fld		[value]
		fyl2x
	}
#endif
}


//**************************************************************************************************
// Scaled Log 2 of x

f64 YLog2(f64 scale, f64 x)
{
#ifdef	__GNUC__
	f80	ret;


	//*** This doesn't work. ***
	//*** GCC tries to pop 'scale' off the stack, but fyl2x already does that. ***
	asm("
		fyl2x

	" : "=t" (ret) : "f" (scale), "0" (x));

	return ret;
#else
	_asm
	{
		fld		[scale]
		fld		[x]
		fyl2x
	}
#endif
}


//**************************************************************************************************
// Power of 2

f64 Pow2(f64 exp)
{
#ifdef	__GNUC__
	f80	ret;


	asm("
		f2xm1
		fld1
		faddp	%%st,%%st(1)

	" : "=t" (ret) : "0" (exp));

	return ret;
#else
	_asm
	{
		fld		[exp]
		f2xm1
		fld1
		faddp	st(1),st
	}
#endif
}

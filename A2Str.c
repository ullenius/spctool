/***************************************************************************************************
* String Functions                                                                                 *
*                                                           Copyright (C)2003 Alpha-II Productions *
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
// Find String End

s8* StrEnd(const s8 *pStr)
{
#ifdef	__GNUC__
	s8	*pRet;


	asm("
		xorl	%0,%0
		repne	scasb
		leal	-1(%1),%%eax

	" : "=a" (pRet) : "D" (pStr), "c" (-1));

	return pRet;
#else
	__asm
	{
		mov		edi,[pStr]
		xor		eax,eax
		mov		ecx,-1
		repne	scasb
		lea		eax,[edi-1]
	}
#endif
}


//**************************************************************************************************
// Reverse Scan String for Character

s8* ScanStrR(const s8 *pStr, s8 c)
{
#ifdef	__GNUC__
	s8	*pRet;


	asm("
		xorl	%0,%0
		movl	-1,%%ecx
		repne	scasb
		decl	%%edi
		negl	%%ecx
		decl	%%ecx

		std
		movb	%2,%%al
		repne	scasb
		cld
		sete	%%al
		addl	%%edi,%0
		incl	%0

	" : "=a" (pRet) : "D" (pStr), "m" (c) : "ecx");

	return pRet;
#else
	__asm
	{
		//Find end of string -------------------
		mov		edi,[pStr]
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
#endif
}


//**************************************************************************************************
// Find String in String, Case Insensitive

s8* FindStrI(const s8 *pDest, const s8 *pSrc)
{
#ifdef	__GNUC__
#else
	__asm
	{
		mov		esi,[pSrc]
		mov		edi,[pDest]

		mov		al,[esi]						//CL = first character to search for
		cmp		al,'a'
		setae	cl
		cmp		al,'z'
		setbe	dl
		and		cl,dl
		shl		cl,5
		not		cl
		and		cl,al
		jz		ErrorFSI						//If first character is NULL, return

	//Search pDest for the first character in pSrc
	NextFSI:
		mov		al,[edi]
		inc		edi
		cmp		al,'a'
		setae	dl
		cmp		al,'z'
		setbe	ch
		and		dl,ch
		shl		dl,5
		not		dl
		and		al,dl
		jz		short ErrorFSI

		cmp		al,cl
		jne		short NextFSI

	//Compare strings --------------------------
		lea		ebx,[edi-1]
		mov		esi,[pSrc]
		inc		esi

	CmpFSI:
		mov		al,[esi]
		inc		esi
		cmp		al,'a'
		setae	dl
		cmp		al,'z'
		setbe	ch
		and		dl,ch
		shl		dl,5
		not		dl
		and		dl,al
		jz		short DoneFSI

		mov		al,[edi]
		inc		edi
		cmp		al,'a'
		setae	ch
		cmp		al,'z'
		setbe	dh
		and		dh,ch
		shl		dh,5
		not		dh
		and		al,dh
		jz		short ErrorFSI

		cmp		al,dl
		je		short CmpFSI

		lea		edi,[1+ebx]
		jmp		NextFSI

	ErrorFSI:
		xor		ebx,ebx

	DoneFSI:
		mov		eax,ebx
	}
#endif
}


//**************************************************************************************************
// Compare Memory

b8 CmpMem(const void *p1, const void *p2, u32 len)
{
#ifdef	__GNUC__
	s32	temp;
	b8	ret;


	asm("
		xorl	%0,%0
		repe	cmpsd
		xchgl	%3,%4

		repe	cmpsb
		orl		%3,%4
		setz	al

	" : "=a" (ret) : "D" (p1), "S" (p1), "c" (len >> 2), "r" (len & 3));

	return ret;
#else
	__asm
	{
		xor		eax,eax
		mov		edi,[p1]
		mov		esi,[p2]
		mov		ecx,[len]
		shr		ecx,2
		repe	cmpsd
		mov		edx,ecx

		mov		ecx,[len]
		and		ecx,3
		repe	cmpsb
		or		edx,ecx
		setz	al
	}
#endif
}


//**************************************************************************************************
// Compare and Copy String

b8 CmpCopy(s8 *pDest, const s8 *pSrc)
{
#ifdef	__GNUC__
	b8	ret;

	asm("
		xorl	%0,%0

	1: 	movb	(%2),%%al
		cmpb	(%1),%%al
		jne		1f
		incl	%2
		incl	%1
		testb	%%al,%%al
		jnz		1b
		incl	%0
		jmp		0

	1: 	movl	(%2),%%al
		movl	%%al,(%1)
		incl	%2
		incl	%1
		testb	%%al,%%al
		jnz		1b

	0:
	" : "=a" (ret) : "D" (pDest), "S" (pSrc));
#else
	__asm
	{
		xor		eax,eax
		mov		esi,[pSrc]
		mov		edi,[pDest]

	NextCC:
		mov		al,[esi]
		cmp		al,[edi]
		jne		short CopyCC
		inc		esi
		inc		edi
		test	al,al
		jnz		short NextCC
		inc		eax
		jmp		short DoneCC

	CopyCC:
		mov		al,[esi]
		mov		[edi],al
		inc		esi
		inc		edi
		test	al,al
		jnz		short CopyCC

	DoneCC:
	}
#endif
}


//**************************************************************************************************
// Copy String

s8* CopyStr(s8 *pDest, const s8 *pSrc)
{
#ifdef	__GNUC__
	s8	*pRet;


	asm("
	1:	movb	(%2),%%al
		incl	%2
		movb	%%al,(%1)
		incl	%1
		testb	%%al,%%al
		jnz		1
		leal	-1(%1),%0

	" : "=a" (pRet) : "D" (pDest), "S" (pSrc));

	return pRet;
#else
	__asm
	{
		mov		esi,[pSrc]
		mov		edi,[pDest]
	NextCS:
		mov		al,[esi]
		inc		esi
		mov		[edi],al
		inc		edi
		test	al,al
		jnz		short NextCS
		lea		eax,[edi-1]
	}
#endif
}


//**************************************************************************************************
// Copy String with Length

s8* CopyStrL(s8 *pDest, const s8 *pSrc, s32 l)
{
#ifdef	__GNUC__
	s8	*pRet;

	asm("
		movl	%3,%%eax
		cdq
		xorl	%%edx,%%eax
		subl	%%edx,%%eax
		movl	%%eax,%%ecx

	1:	movb	(%2),%%al
		incl	%2
		movb	%%al,(%1)
		testb	%%al,%%al
		jz		2
		incl	%1
		decl	%%ecx
		jnz		1
		jmp		0

	2:	testb	$0x80,3+%3
		jns		0

		movb	' ',%%al
		rep		stosb

	0:
		movb	$0,(%1)

	" : "=D" (pRet) : "D" (pDest), "S" (pSrc), "m" (l) : "eax", "edx", "ecx");

	return pRet;
#else
	__asm
	{
		mov		esi,[pSrc]
		mov		edi,[pDest]
		mov		eax,[l]
		cdq
		xor		eax,edx
		sub		eax,edx
		mov		ecx,eax

	NextCSL:
		mov		al,[esi]
		inc		esi
		mov		[edi],al
		test	al,al
		jz		short DoneCSL
		inc		edi
		dec		ecx
		jnz		short NextCSL
		jmp		short QuitCSL

	DoneCSL:
		test	byte ptr [3+l],0x80
		jns		short QuitCSL

		mov		al,' '
		rep		stosb

	QuitCSL:
		mov		byte ptr [edi],0
		mov		eax,edi
	}
#endif
}


//**************************************************************************************************
// Copy String up to Character

s8* CopyStrC(s8 *pDest, const s8 *pSrc, s8 c)
{
#ifdef	__GNUC__
	s8	*pRet;


	asm("
		jmp		2
	1:	movb  	%%al,(%1)
		incl  	%1
	2:	movb	(%2),%%al
		incl	%2
		cmpb	%3,%%al
		je		0
		testb	%%al,%%al
		jnz		1

	0:	movb	$0,(%1)
		movl	%1,%0

	" : "=a" (pRet) : "D" (pDest), "S" (pSrc), "d" (c));

	return pRet;
#else
	__asm
	{
		mov		esi,[pSrc]
		mov		edi,[pDest]
		mov		dl,[c]
		jmp		short StartCSC

	NextCSC:
		mov		[edi],al
		inc		edi
	StartCSC:
		mov		al,[esi]
		inc		esi
		cmp		al,dl
		je		short DoneCSC
		test	al,al
		jnz		short NextCSC

	DoneCSC:
		mov		byte ptr [edi],0
		mov		eax,edi
	}
#endif
}


//**************************************************************************************************
// Convert Integer to Hex

s8*	Int2Hex(u32 i, u32 d)
{
	static s8	hex[16] = {"0123456789ABCDEF"};
	static s8	str[9];

	str[d] = 0;
	while(d--)
	{
		str[d] = hex[i & 0xF];
		i >>= 4;
	}

	return str;
}


//**************************************************************************************************
// Convert Hex to Integer

u32 Hex2Int(s8 *p)
{
#ifdef	__GNUC__
#else
	__asm
	{
		mov		esi,[p]
		xor		eax,eax
		xor		ebx,ebx

	NextHI:
		mov		al,[esi]
		inc		esi
		test	al,al
		jz		short DoneHI
		or		al,0x20
		sub		al,0x30
		cmp		al,9
		setbe	dl
		dec		dl
		and		dl,39
		sub		al,dl
		cmp		al,15
		seta	cl
		dec		cl
		and		al,cl
		and		cl,4
		shl		ebx,cl
		or		ebx,eax
		jmp		short NextHI

	DoneHI:
		mov		eax,ebx
	}
#endif
}

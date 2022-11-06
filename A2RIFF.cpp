#if defined __BORLANDC__
#pragma	inline
#pragma warning(disable : 4035)
#elif defined _MSC_VER
#pragma	warning(disable : 4095)
#else
#error Compiler not defined for in-line assembly
#endif

#include	"Types.h"


b8 __fastcall CmpChkID(void*, u32)
{
#if defined _MSC_VER
	_asm bswap	edx
	_asm xor	eax,eax
	_asm cmp	edx,[ecx]
#elif defined __BORLANDC__
	_asm cmp	edx,[eax]
	_asm mov	eax,0
#endif
	_asm sete	al
}


b8 __fastcall CmpChkType(void*, u32)
{
#if defined _MSC_VER
	_asm bswap	edx
	_asm xor	eax,eax
	_asm cmp	edx,[8+ecx]
#elif defined __BORLANDC__
	_asm cmp	edx,[8+eax]
	_asm mov	eax,0
#endif
	_asm sete	al
}


u16 __fastcall ByteSwap16(u16)
{
#if defined _MSC_VER
	_asm mov	ax,cx
#endif
	_asm ror	ax,8
}


void __fastcall ByteSwap16(u16*)
{
#if defined _MSC_VER
	_asm ror	word ptr [ecx],8
#elif defined __BORLANDC__
	_asm ror	word ptr [eax],8
#endif
}


void __fastcall ByteSwap16(u16*, u32)
{
	_asm
	{
		dec		edx
	NextBS16:
#if defined _MSC_VER
		ror		word ptr [edx*2+ecx],8
#elif defined __BORLANDC__
		ror		word ptr [edx*2+eax],8
#endif
		dec		edx
		jns		short NextBS16
	}
}


u32 __fastcall ByteSwap32(u32)
{
#if defined _MSC_VER
	_asm mov	eax,ecx
#endif
	_asm bswap	eax
}


void __fastcall ByteSwap32(u32*)
{
#if defined _MSC_VER
	_asm mov	eax,[ecx]
	_asm bswap	eax
	_asm mov	[ecx],eax
#elif defined __BORLANDC__
	_asm mov	ecx,[eax]
	_asm bswap	ecx
	_asm mov	[eax],ecx
#endif
}


void __fastcall ByteSwap32(u32*, u32)
{
#if defined __BORLANDC__
	_asm mov	ecx,eax
#endif
	_asm
	{
		dec		edx
	NextBS32:
		mov		eax,[edx*4+ecx]
		bswap	eax
		mov		[edx*4+ecx],eax
		dec		edx
		jns		short NextBS32
	}
}

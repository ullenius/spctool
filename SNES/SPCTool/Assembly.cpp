#pragma	inline
#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"Main.h"

#pragma warning(disable : 4035)

//**************************************************************************************************
// Calculate 16-bit Cyclic Redundancy Check
//
// In:
//    p  -> Array of bytes
//    len = Length of array
//
// Out:
//    16-bit CRC

u16 __fastcall CRC16(void *p, u32 len)
{
	_asm
	{
		mov		edi,eax
		xor		eax,eax

	Next16:
		xor		ah,[edi]
		inc		edi

		mov		cl,8
	Bit16:
		add		ax,ax
		jnc		short NoPoly16
		xor		ax,0x8005						//CRC-16 polynomial (x<<16 + x<<15 + x<<2 + x<<0)
	NoPoly16:

		dec		cl
		jnz		short Bit16
		dec		edx
		jnz		short Next16
	}
}


//**************************************************************************************************
// Read Time-Stamp Counter

u64 __fastcall ReadTimeStamp()
{
	_asm RdTSC
}


//**************************************************************************************************
void DivMod(u32 *q, u32 *r, u32 d)
{
	_asm
	{
		mov	ecx,[q]
		mov	ebx,[r]
		mov	eax,[ecx]
		xor	edx,edx
		div	[d]
		mov [ecx],eax
		mov	[ebx],edx
	}
}


//**************************************************************************************************
s8* CopyCase(s8 *pDest, const s8 *pSrc)
{
	_asm
	{
		mov		esi,[pSrc]
		mov		edi,[pDest]

		test	[Cfg.debug.ltrCase],-1
		setz	dl
		shl		dl,5

		mov		al,[esi]
	@@Next:
		or		al,dl
		mov		[edi],al
		add		esi,1
		add		edi,1
		mov		al,[esi]
		test	al,al
		jnz		short @@Next
		mov		[edi],al

		mov		eax,edi
	}
}


//**************************************************************************************************
// Scan Source
//
// In:
//    pSrc->idx = Index of source to scan
//
// Out:
//    pSrc->offset
//    pSrc->len
//    pSrc->loop
//    pSrc->crc
//    pSrc->flags (filters only)

void ScanSrc(Source *pSrc)
{
	u8	*ram;
	u8	dir;


	ram = Apu.ram;
	dir = Apu.dsp->dir;
	_asm
	{
		mov		edi,[pSrc]
		mov		esi,[ram]

		//Get loop length ----------------------
		movzx	ecx,byte ptr [10+edi]			//ecx = (Apu.dsp->dir << 8) + (pSrc->idx << 2)
		shl		ecx,2
		add		ch,[dir]

		movzx	eax,word ptr [esi+ecx+2]		//Get loop pointer from source table
		movzx	ecx,word ptr [esi+ecx]
		sub		eax,ecx							//Subtract base pointer
		mov		ebx,9
		xor		edx,edx
		div		ebx
		mov		[0+edi],ecx						//Store offset of source in Apu.ram
		mov		[6+edi],ax						//Store loop length (in blocks)

		//Scan source --------------------------
		add		esi,ecx
		xor		eax,eax
		xor		ecx,ecx
		xor		edx,edx
	NextSS:
		inc		edx
		mov		al,[esi]

		mov		cl,al
		mov		bl,1
		shr		cl,2
		and		cl,3
		shl		bl,cl
		or		ah,bl

		add		si,9							//Have we reached the end of RAM?
		jc		short DoneSS
		test	al,1							//Is this the end block?
		jz		short NextSS

	DoneSS:
		mov		[4+edi],dx						//Store source length (in blocks)

		xor		edx,edx							//Set loop length to -1 if loop flag isn't set
		test	al,2
		setnz	dl
		dec		edx
		or		[6+edi],dx

		and		byte ptr [12+edi],0xF0			//Store flags
		or		byte ptr [12+edi],ah
	}

	pSrc->crc = CRC16(&Apu.ram[pSrc->offset], pSrc->len * 9);
}

//**************************************************************************************************
// Scan Source Directory
//
// Scans the sound source directory in APU RAM and tries to determine which entries point to valid
// waveforms.
//
// In:
//    table  -> Array of Sources to store data
//    pDir   -> Source directory to scan (must reside in a block of memory aligned on a 64k boundary)
//    start   = First page that can contain source data
//    min     = Minimum size of source data (in blocks)
//    max     = Maximum size of source data (in blocks);
//    linear  = First block must use linear compression
//    silence = First block must contain silence
//    range   = Maximum range

void ScanSrcDir(Source table[256], u8 *pDir, u8 start,
				u32 min, u32 max, b8 linear, b8 silence, u8 range)
{
	u32	blkSize = 9;

	_asm
	{
	//==========================================
	// Scan Source Directory
		mov		al,[range]
		shl		al,4
		or		al,0xF
		mov		[range],al
		mov		edi,[table]
		mov		esi,[pDir]
		mov		ebx,[pDir]						//EBX -> APU RAM

		mov		ch,0
	Src:
		xor		eax,eax							//EAX = 0
		lea		edx,[eax-1]						//EDX = -1

		mov		[edi],edx						//Offset is -1 (invalid source)
		xor		dx,dx
		mov		[4+edi],edx						//Length is 0, Loop is -1 (no loop)
		mov		[8+edi],eax						//No checksum
		mov		[12+edi],eax					//Reset filter flags
		mov		dword ptr [16+edi],32000		//Default sample rate
		mov		dword ptr [20+edi],0x0080E0FF	//Set ADSR register, program number, and bank
		mov		[24+edi],eax					//Clear reserved fields
		mov		[28+edi],eax
		sub		al,ch							//Store index
		mov		[10+edi],al

		mov		bx,[esi]						//EBX -> sound source
		mov		ah,[range]
		mov		dh,[start]

	//Test for an initialization block ---------
		test	[linear],-1
		jz		short NoInit

		mov		al,[ebx]
		test	al,0xC							//Make sure filter is 0
		jnz		BadSrc

		test	[silence],-1
		jz		short NoInit

		mov		edx,[1+ebx]						//Make sure BRR data is all 0's
		or		edx,[5+ebx]
		jnz		BadSrc
	NoInit:

	//Search for end block =====================
	Blk:
		mov		al,[ebx]

		add		bx,9
		jc		BadSrc							//If we've reached the end of RAM, the source is bad

		cmp		bh,dh							//If block is in a direct page the source is bad
		jb		BadSrc

		cmp		al,ah							//If range is too high the source is bad
		ja		BadSrc

		mov		cl,al							//Save filter type
		mov		dl,1
		shr		cl,2
		and		cl,3
		shl		dl,cl
		or		[12+edi],dl

		test	al,1							//Is this the end block?
		jz		short Blk

	//Save length ------------------------------
		mov		cl,al							//Save block header
		movzx	eax,bx
		xor		edx,edx
		sub		ax,[esi]						//EAX = length (in bytes)
		div		[blkSize]

		cmp		eax,[min]
		jb		BadSrc
		cmp		eax,[max]
		ja		BadSrc

		mov		[4+edi],ax						//Store source length

	//Save loop point --------------------------
		test	cl,2							//Does source loop?
		jz		short NoLoop

		mov		ax,[2+esi]						//EAX = loop end

		cmp		ax,bx							//If loop is after source end, source is bad
		jae		BadSrc

		sub		ax,[esi]						//If loop is before source start, source is bad
		jc		BadSrc

		div		[blkSize]						//If loop isn't on a block boundary, source is bad
		test	edx,edx
		jnz		BadSrc

		mov		[6+edi],ax						//Store loop point
		mov		byte ptr [22+edi],0				//Set program type to tone
	NoLoop:

	//Calculate checksum -----------------------
		mov		edx,ebx
		mov		dx,[esi]						//EDX -> source data
		sub		bx,dx							//BX = length (in bytes)
		xor		eax,eax
		sub		bx,2
	CRC:
		add		ax,[edx]
		add		edx,2
		sub		bx,2
		jnc		short CRC

		test	bx,1							//Test for an extra byte
		setz	bh
		dec		bh
		shr		bx,8
		and		bx,[edx]
		add		ax,bx

		mov		[8+edi],ax						//Store checksum

	//Save offset ------------------------------
		mov		ax,[esi]
		mov		[edi],eax

	BadSrc:
		add		esi,4
		add		edi,32

		dec		ch
		jnz		Src
	}
}


//**************************************************************************************************
// Scan ROM for Sounds
//
// In:
//    table  -> Array of Sources to store data
//    pROM   -> ROM data to search
//    start   = Starting offset in ROM to search
//    end     = Ending offset in ROM to search
//    min     = Minimum size of source data (in blocks)
//    max     = Maximum size of source data (in blocks);
//    linear  = First block must use linear compression
//    silence = First block must contain silence
//    range   = Maximum range

u32 ScanROM(Source *table, u8 *pROM, u32 start, u32 end,
			u32 min, u32 max, b8 linear, b8 silence, u8 range)
{
	u32	num = 0;

	_asm
	{
	//==========================================
	// Search ROM

		mov		edi,[table]
		mov		eax,[pROM]
		add		[start],eax
		add		[end],eax

		mov		al,[range]
		shl		al,4
		or		al,0xF
		mov		[range],al

		mov		ch,0
	@@Ofs:
		movzx	esi,ch
		add		esi,[start]

	@@Src:
		xor		eax,eax							//EAX = 0

		mov		dword ptr [4+edi],0xFFFF0000	//Length is 0, Loop is -1 (no loop)
		mov		[8+edi],eax						//No checksum
		mov		[12+edi],eax					//Reset filter flags
		mov		dword ptr [16+edi],32000		//Default sample rate
		mov		dword ptr [20+edi],0x0080E0FF	//Set ADSR register, program number, and bank
		mov		[24+edi],eax					//Clear reserved fields
		mov		[28+edi],eax

		mov		ebx,esi
		sub		ebx,[pROM]
		mov		[edi],ebx						//Save offset
		xor		ebx,ebx

		mov		ah,[range]

	//Test for an initialization block ---------
		test	[linear],-1
		jz		@@Blk

		mov		al,[esi]
		test	al,0xC							//Make sure filter is 0
		jnz		short @@BadSrc

		test	[silence],-1
		jz		@@Blk

		mov		edx,[1+esi]						//Make sure BRR data is all 0's
		or		edx,[5+esi]
		jz		short @@Blk

	@@BadSrc:
		add		esi,9
		cmp		esi,[end]
		jb		short @@Src
		jmp		@@NextOfs

	//Search for end block ---------------------
	@@Blk:
		mov		al,[esi]

		add		esi,9
		cmp		esi,[end]
		jae		@@NextOfs				   		//If we've reached the end of RAM, the source is bad
		inc		ebx

		cmp		al,ah
		ja		@@Src					   		//If range is too high, the source is bad

		mov		cl,al							//Save filter type
		mov		dl,1
		shr		cl,2
		and		cl,3
		shl		dl,cl
		or		[12+edi],dl

		test	al,1
		jz		@@Blk

	//Save length ------------------------------
		cmp		ebx,[min]
		jb		@@Src
		cmp		ebx,[max]
		ja		@@Src

		mov		[4+edi],bx
		xor		ebx,ebx
		test	al,2
		setnz	bl
		setz	dl
		dec		ebx
		mov		word ptr [6+edi],bx
		ror		dl,1
		mov		[22+edi],dl

		mov		edx,[num]
		inc		edx
		mov		[num],edx
		add		edi,32

		cmp		edx,1000
		jb		@@Src
		jmp		@@Done

	@@NextOfs:
		inc		ch
		cmp		ch,9
		jb		@@Ofs

	@@Done:

	//Crappy bubble sort -----------------------
		mov		ecx,[num]
		dec		ecx
		jle		@@NoSort

	@@Wave:
		mov		ebx,ecx
		mov		edi,[table]

	@@Bubble:
		mov		eax,[edi]
		cmp		eax,[edi+32]
		jb		@@NoMove

		movq	mm0,[00+edi]
		movq	mm1,[08+edi]
		movq	mm2,[16+edi]
		movq	mm3,[24+edi]
		movq	mm4,[32+edi]
		movq	mm5,[40+edi]
		movq	mm6,[48+edi]
		movq	mm7,[56+edi]
		movq	[00+edi],mm4
		movq	[08+edi],mm5
		movq	[16+edi],mm6
		movq	[24+edi],mm7
		movq	[32+edi],mm0
		movq	[40+edi],mm1
		movq	[48+edi],mm2
		movq	[56+edi],mm3

	@@NoMove:
		add		edi,32
		dec		ebx
		jnz		@@Bubble

		dec		ecx
		jnz		@@Wave

	//Fill index -------------------------------
	@@NoSort:
		mov		edi,[table]
		mov		ecx,[num]
		dec		ecx
		shl		ecx,5
		lea		edi,[edi+ecx+10]
		shr		ecx,5
	@@SetIdx:
		mov		[edi],cx
		sub		edi,32
		dec		ecx
		jns		short @@SetIdx

		emms
	}

	return num;
}

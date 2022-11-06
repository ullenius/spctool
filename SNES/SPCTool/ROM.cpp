/***************************************************************************************************
* Form: ROM Creator                                                                                *
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#include	<vcl.h>
#include	"SPCTool.h"
#pragma hdrstop
#include	"A2Math.h"
#include	"SWC.h"
#include	"Progress.h"
#include	"ROM.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

#define		RETI

//**************************************************************************************************
// Structures

typedef union
{
	struct
	{
		//[0000-BFFF C000]
		s8		menu[24][32];	  				//Text for song select screen (not currently used)
		s8		info[63][24][32];				//ID666 information for each song (32x24 characters)

		//[C000-DFFF 2000]
		u16		songs;							//Number of songs
		u16		_r1[63];
		u8		dsp[63][128];					//DSP registers for each song

		//[E000-E7FF 800]
		u16		charset[128][8];				//Character maps for displaying text

		//[E800-EFFF 800]
		s8		_r2[32];
		s8		title[63][32];					//Index and title for each song

		//[F000-F7FF 800]
		u8		_r3[0x800];

		//[F800-FFBF 7C0]
		u8		code[0x7C0];					//65816 executable

		//[FFC0-FFFF 40]
		ROMHdr	hdr;							//Standard cart header

		u8		ram[63][0x10000];				//Remaining banks contain APU RAM states
	};

	s8	blk[512][8192];							//Blocks for uploading data to a copier
} ROM;

//	info[0] contains text for song select screen


typedef struct									//Information about song to be stored in ROM
{
	APUFile	*pAPU;								//-> to .spc file and ID666 tag
	u32		boot;								//Offset of bootstrap in APU RAM
	b8		echo;								//Bootstrap is stored in the echo region
	s8		title[33];							//Song title to be displayed on song select screen
} Song;


//**************************************************************************************************
// Data and Variables

static const u16	Charset[]={
		0x4438,0x0044,0x0044,0x4400,0x0044,0x0044,0x4438,0x0000,	//00 0  The first sixteen
		0x0400,0x0004,0x0004,0x0400,0x0004,0x0004,0x0400,0x0000,    //01 1  characters are for easy
		0x4438,0x0004,0x0004,0x4438,0x0040,0x0040,0x4438,0x0000,	//02 2  hex printing
		0x4438,0x0004,0x0004,0x4438,0x0004,0x0004,0x4438,0x0000,	//03 3
		0x4400,0x0044,0x0044,0x4438,0x0004,0x0004,0x0400,0x0000,	//04 4
		0x4438,0x0040,0x0040,0x4438,0x0004,0x0004,0x4438,0x0000,	//05 5
		0x4438,0x0040,0x0040,0x4438,0x0044,0x0044,0x4438,0x0000,	//06 6
		0x4438,0x0004,0x0004,0x0400,0x0004,0x0004,0x0400,0x0000,	//07 7
		0x4438,0x0044,0x0044,0x4438,0x0044,0x0044,0x4438,0x0000,	//08 8
		0x4438,0x0044,0x0044,0x4438,0x0004,0x0004,0x4438,0x0000,	//09 9
		0x4438,0x0044,0x0044,0x4438,0x0044,0x0044,0x4400,0x0000,	//0A A
		0x4000,0x0040,0x0040,0x4438,0x0044,0x0044,0x4438,0x0000,	//0B B
		0x4438,0x0040,0x0040,0x4000,0x0040,0x0040,0x4438,0x0000,	//0C C
		0x0400,0x0004,0x0004,0x4438,0x0044,0x0044,0x4438,0x0000,	//0D D
		0x4438,0x0040,0x0040,0x4438,0x0040,0x0040,0x4438,0x0000,	//0E E
		0x4438,0x0040,0x0040,0x4438,0x0040,0x0040,0x4000,0x0000,	//0F F
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//10
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//11
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//12
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//13
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//14
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//15
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//16
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//17
		0x0010,0x0038,0x007C,0x0010,0x0010,0x0010,0x0010,0x0000,	//18
		0x0010,0x0010,0x0010,0x0010,0x007C,0x0038,0x0010,0x0000,	//19
		0x0000,0x0008,0x000C,0x00FE,0x000C,0x0008,0x0000,0x0000,	//1A
		0x0000,0x0020,0x0060,0x00FE,0x0060,0x0020,0x0000,0x0000,	//1B
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//1C
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//1D
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//1E
		0x423C,0x0042,0x104A,0x0052,0x104A,0x0042,0x423C,0x0000,	//1F copyright
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//20 space
		0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,0x0010,0x0000,	//21 !
		0x0028,0x0028,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//22 "
		0x0028,0x0028,0x007C,0x0028,0x007C,0x0028,0x0028,0x0000,	//23 #
		0x0010,0x403C,0x0050,0x4438,0x0014,0x0478,0x0010,0x0000,	//24 $
		0x0064,0x0064,0x0008,0x0010,0x0020,0x004C,0x004C,0x0000,	//25 %
		0x5020,0x0050,0x5020,0x0834,0x0048,0x0448,0x4834,0x0000,	//26 &
		0x0010,0x2010,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,	//27 '
		0x1008,0x0010,0x0010,0x0010,0x0010,0x0010,0x1008,0x0000,	//28 (
		0x1020,0x0010,0x0010,0x0010,0x0010,0x0010,0x1020,0x0000,	//29 )
		0x0000,0x0010,0x2854,0x0038,0x2854,0x0010,0x0000,0x0000,	//2A *
		0x0000,0x0010,0x0010,0x007C,0x0010,0x0010,0x0000,0x0000,	//2B +
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0010,0x2010,0x0020,	//2C ,
		0x0000,0x0000,0x0000,0x007C,0x0000,0x0000,0x0000,0x0000,	//2D -
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0010,0x0000,	//2E .
		0x0008,0x1008,0x0810,0x0010,0x2010,0x1020,0x0020,0x0000,	//2F /
		0x4438,0x0044,0x004C,0x0054,0x0064,0x0044,0x4438,0x0000,	//30 0
		0x1020,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//31 1
		0x0478,0x0004,0x0004,0x4438,0x2040,0x0040,0x007C,0x0000,	//32 2
		0x0478,0x0804,0x0804,0x0438,0x0804,0x0804,0x0478,0x0000,	//33 3
		0x0044,0x0044,0x2044,0x403C,0x0004,0x0004,0x0004,0x0000,	//34 4
		0x007C,0x0040,0x0040,0x0478,0x0004,0x0004,0x0478,0x0000,	//35 5
		0x403C,0x0040,0x0040,0x0478,0x0044,0x0044,0x4438,0x0000,	//36 6
		0x007C,0x0804,0x1408,0x0810,0x0010,0x0010,0x0010,0x0000,	//37 7
		0x4438,0x0044,0x0044,0x4438,0x0044,0x0044,0x4438,0x0000,	//38 8
		0x4438,0x0044,0x0044,0x403C,0x0004,0x0804,0x0478,0x0000,	//39 9
		0x0000,0x0000,0x0010,0x0000,0x0000,0x0010,0x0000,0x0000,	//3A :
		0x0000,0x0000,0x0010,0x0000,0x0000,0x0010,0x0010,0x1020,	//3B ;
		0x0008,0x0810,0x1020,0x2040,0x1020,0x0810,0x0008,0x0000,	//3C <
		0x0000,0x0000,0x007C,0x0000,0x0000,0x007C,0x0000,0x0000,	//3D =
		0x0020,0x2010,0x1008,0x0804,0x1008,0x2010,0x0020,0x0000,	//3E >
		0x4438,0x0044,0x0804,0x1008,0x0010,0x0000,0x0010,0x0000,	//3F ?
		0x423C,0x0042,0x005A,0x005A,0x124C,0x0040,0x403C,0x0000,	//40 @
		0x4438,0x0044,0x0044,0x007C,0x0044,0x0044,0x0044,0x0000,	//41 A
		0x0478,0x0044,0x0044,0x0478,0x0044,0x0044,0x0478,0x0000,	//42 B
		0x403C,0x2040,0x0040,0x0040,0x0040,0x2040,0x403C,0x0000,	//43 C
		0x0478,0x0844,0x0044,0x0044,0x0044,0x0844,0x0478,0x0000,	//44 D
		0x007C,0x0040,0x0040,0x0078,0x0040,0x0040,0x007C,0x0000,	//45 E
		0x007C,0x0040,0x0040,0x0078,0x0040,0x0040,0x0040,0x0000,	//46 F
		0x403C,0x2040,0x0040,0x005C,0x0044,0x2044,0x403C,0x0000,	//47 G
		0x0044,0x0044,0x0044,0x007C,0x0044,0x0044,0x0044,0x0000,	//48 H
		0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//49 I
		0x0004,0x0004,0x0004,0x0004,0x0044,0x2844,0x4438,0x0000,	//4A J
		0x0044,0x0844,0x1048,0x0870,0x1048,0x0844,0x0044,0x0000,	//4B K
		0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x007C,0x0000,	//4C L
		0x2844,0x106C,0x2854,0x1044,0x0044,0x0044,0x0044,0x0000,	//4D M
		0x2044,0x1064,0x2854,0x104C,0x0844,0x0044,0x0044,0x0000,	//4E N
		0x4438,0x2844,0x0044,0x0044,0x0044,0x2844,0x4438,0x0000,	//4F O
		0x0478,0x0044,0x0044,0x0478,0x0040,0x0040,0x0040,0x0000,	//50 P
		0x4438,0x2844,0x0044,0x0044,0x0044,0x2448,0x4834,0x0000,	//51 Q
		0x0478,0x0044,0x0044,0x0478,0x0044,0x0044,0x0044,0x0000,	//52 R
		0x403C,0x0040,0x0040,0x4438,0x0004,0x0004,0x0478,0x0000,	//53 S
		0x007C,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//54 T
		0x0044,0x0044,0x0044,0x0044,0x0044,0x2844,0x4438,0x0000,	//55 U
		0x0044,0x0044,0x0044,0x0044,0x2844,0x1028,0x0010,0x0000,	//56 V
		0x0044,0x0044,0x0044,0x1044,0x2854,0x106C,0x2844,0x0000,	//57 W
		0x0044,0x2844,0x1028,0x2810,0x1028,0x2844,0x0044,0x0000,	//58 X
		0x0044,0x0044,0x4428,0x2810,0x0010,0x0010,0x0010,0x0000,	//59 Y
		0x007C,0x0804,0x1008,0x2010,0x4020,0x0040,0x007C,0x0000,	//5A Z
		0x0038,0x0020,0x0020,0x0020,0x0020,0x0020,0x0038,0x0000,	//5B [
		0x0020,0x1020,0x2010,0x0010,0x0810,0x1008,0x0008,0x0000,	//5C backslash
		0x0038,0x0008,0x0008,0x0008,0x0008,0x0008,0x0038,0x0000,	//5D ]
		0x0010,0x1028,0x2844,0x0000,0x0000,0x0000,0x0000,0x0000,	//5E ^
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00FE,	//5F _
		0x0020,0x1020,0x0010,0x0000,0x0000,0x0000,0x0000,0x0000,	//60 `
		0x0000,0x0000,0x4438,0x0044,0x0044,0x0844,0x403C,0x0000,	//61 a
		0x0040,0x0040,0x0478,0x0044,0x0044,0x0044,0x4438,0x0000,	//62 b
		0x0000,0x0000,0x403C,0x0040,0x0040,0x0040,0x403C,0x0000,	//63 c
		0x0004,0x0004,0x403C,0x0044,0x0044,0x0044,0x4438,0x0000,	//64 d
		0x0000,0x0000,0x4438,0x0844,0x0478,0x0040,0x403C,0x0000,	//65 e
		0x4038,0x2040,0x0070,0x0040,0x0040,0x0040,0x0040,0x0000,	//66 f
		0x0000,0x0000,0x4438,0x0044,0x0044,0x403C,0x0004,0x0478,	//67 g
		0x0040,0x0040,0x0040,0x0478,0x0044,0x0044,0x0044,0x0000,	//68 h
		0x0000,0x0000,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//69 i
		0x0000,0x0000,0x0004,0x0004,0x0004,0x0004,0x0044,0x4438,	//6A j
		0x0040,0x0040,0x0844,0x1448,0x0870,0x1448,0x0844,0x0000,	//6B k
		0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//6C l
		0x0000,0x0000,0x0478,0x0054,0x0054,0x0044,0x0044,0x0000,	//6D m
		0x0000,0x0000,0x0478,0x0044,0x0044,0x0044,0x0044,0x0000,	//6E n
		0x0000,0x0000,0x4438,0x0044,0x0044,0x0044,0x4438,0x0000,	//6F o
		0x0000,0x0000,0x4438,0x0044,0x0044,0x0478,0x0040,0x0040,	//70 p
		0x0000,0x0000,0x4438,0x0044,0x0044,0x403C,0x0004,0x0004,	//71 q
		0x0000,0x0000,0x4038,0x0040,0x0040,0x0040,0x0040,0x0000,	//72 r
		0x0000,0x0000,0x403C,0x0040,0x4438,0x0004,0x0478,0x0000,	//73 s
		0x0010,0x0010,0x0038,0x0010,0x0010,0x0010,0x0010,0x0000,	//74 t
		0x0000,0x0000,0x0044,0x0044,0x0044,0x0044,0x4438,0x0000,	//75 u
		0x0000,0x0000,0x0044,0x0044,0x2844,0x1028,0x0010,0x0000,	//76 v
		0x0000,0x0000,0x0044,0x1044,0x2854,0x106C,0x2844,0x0000,	//77 w
		0x0000,0x0000,0x0044,0x4428,0x2810,0x4428,0x0044,0x0000,	//78 x
		0x0000,0x0000,0x0044,0x0044,0x0044,0x403C,0x0004,0x0478,	//79 y
		0x0000,0x0000,0x007C,0x0408,0x2810,0x4020,0x007C,0x0000,	//7A z
		0x1008,0x0010,0x0010,0x0030,0x0010,0x0010,0x1008,0x0000,	//7B {
		0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x0000,	//7C |
		0x1020,0x0010,0x0010,0x0018,0x0010,0x0010,0x1020,0x0000,	//7D }
		0x4832,0x124C,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,	//7E ~
		0x00FF,0x00FF,0x00FF,0x00FF,0x00FF,0x00FF,0x00FF,0x00FF};	//7F block

#include	"ROMCode.h"					   		//65816 and SPC700 code (see ROMCode.s for assembly)

//==============================================
// Boot Strap
//
// Once the DSP and RAM have been restored, a small program has to reside in RAM that restores the
// final DSP, function, and SPC700 registers, and then resumes execution at the point the SPC was
// dumped.  The values in this code are specific to each SPC file.

u8	Bootstrap[] = {
		//Restore the first two bytes of DP0 ---
		0x8F,0x00,0x00,							//00	Mov	00h,??
		0x8F,0x00,0x01,							//03	Mov	01h,??

		//Restore function register 1 ----------
		0x8F,0x00,0xF1,							//06	Mov 0F1h,??			;Control

		//Restore final DSP registers ----------
		0x8F,0x6C,0xF2,							//09	Mov 0F2h,#6Ch		;FLG
		0x8F,0x00,0xF3,							//0C	Mov 0F3h,??
		0x8F,0x4C,0xF2,							//0F	Mov 0F2h,#4Ch		;KON
		0x8F,0x00,0xF3,							//12	Mov 0F3h,??
		0x8F,0x00,0xF2,							//15	Mov 0F2h,??			;DSP Address

		//Restore SPC700 registers -------------
		0xCD,0x00,								//18	Mov X,??
		0xBD,									//1A    Mov SP,X
		0xE8,0x00,								//1B	Mov A,??
		0x8D,0x00,								//1D	Mov Y,??
		0xCD,0x00,								//1F	Mov X,??
#ifdef	RETI
		0x7F};									//21	RetI
#else
		0x8E,									//21	Pop PSW
		0x5F,0x00,0x00};						//22	Jmp ??
#endif


static Song	Songs[63];							//Information for up to 63 songs stored in ROM
static u32	NumBanks;							//Number of 64KB banks in ROM file (song count + 1)
static u8	ROMSize;							//Size of padded ROM file (Log2 of size in KB)

static HANDLE	HIoDll;							//Handle to IO.DLL
static IOFunc	Io;								//Pointers to functions in IO.DLL

TFrmROM *FrmROM;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

static u32 CreateROM(ROM**);


//**************************************************************************************************
__fastcall TFrmROM::TFrmROM(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmROM::FormCreate(TObject *sender)
{
	HelpFile = Cfg.hlp;

	lvwFile->HelpContext	= HROM_List;
	chkPad->HelpContext		= HROM_Pad;
}


//**************************************************************************************************
void __fastcall TFrmROM::FormDestroy(TObject *sender)
{
	if (HIoDll)
	{
		FreeLibrary(HIoDll);
		HIoDll = NULL;
	}
}


//**************************************************************************************************
void __fastcall TFrmROM::FormShow(TObject *Sender)
{
	s32	i;
	b8	pad;


	lvwFile->Columns->Items[0]->Width = -1;
	lvwFile->Columns->Items[1]->Width = -1;
	lvwFile->Columns->Items[2]->Width = -2;

	pad = 1;
	i = BitScanR(NumBanks);
	if (i >= 2)
	{
		if (NumBanks & ~(1 << i)) i++;
		else pad = 0;
		i += 6;
	}
	else
		i = 8;

	chkPad->Caption = "Pad ROM to " + AnsiString(2 << (i-8)) + "Mb";
	chkPad->Enabled = pad;
	ROMSize = i;

	btnUpload->Visible = Cfg.out.snes > 1;
}


//**************************************************************************************************
// Verify the user entered a valid hex address

void __fastcall TFrmROM::lvwFileEdited(TObject *sender, TListItem *item, AnsiString &str)
{
	s8	*s;
	u32	i,n;


	if (str.Length() > 4) goto Error;			//Value must be a 16-bit address

	i = 0;
	s = str.c_str();
	while (*s)
	{
		n = (u8)(*s++) - 0x30;
		if (n > 9)
		{
			n = (n | 0x20) - 0x31;
			if (n > 5) goto Error;				//If character wasn't 0-9 or A-F
			n += 10;
		}
		i = (i<<4) | n;
	}

	if (i > 0xFFC0 - sizeof(Bootstrap)) goto Error;	//Bootstrap can't cross into IPL region
	if (i > 0x00F1 - sizeof(Bootstrap) &&
		i < 0x0100) goto Error;					//Bootstrap can overwrite function registers

	str = IntToHex((s32)i, 4);
	((Song*)item->Data)->boot = i;

	return;

Error:
	str = item->Caption;
}


//**************************************************************************************************
static void SendROM(ROM *pROM)
{
	u8	hdr[512];
	u32 size;
	u32	i;


	FrmProg->lblInfo->Caption = "";
	FrmProg->lblStatus->Caption = "Sending program data...";

	size = *(u32*)&pROM->_r3;

	FrmProg->BarCurrent->Reset(size);

	//Create header ----------------------------
	memset(hdr, 0, 512);
	*(u16*)&hdr[0] = size;

	hdr[2]	= 0x1C;
	hdr[8]	= 0xAA;
	hdr[9]	= 0xBB;
	hdr[10]	= 0x04;

	//Upload data ------------------------------
	SwcSetBank(0, 0);							//Write SWC header
	SwcWrite(0x400, hdr, 0x200);

	for (i=0; i<size; i++)						//Send ROM data
	{
		if (FrmProg->Cancel)
		{
			SwcSetBank(0, 0);
			return;
		}

		SwcWrite(0xC010, &hdr[3], 1);			//Specify which 32Mb bank to write to (always 0)
		SwcSetBank(i >> 2, i & 3);
		SwcWrite(0x8000, (u8*)pROM->blk[i], 0x2000);

		FrmProg->BarCurrent->Set(i);
	}

	SwcSetBank(0, 0);							//Reset current page

	SwcCall(SUB_SETSIZE, size, size >> 8, 0);
	SwcCall(SUB_EXEC_DRAM, hdr[2], 0, 0);

	FrmProg->ModalResult = mrOk;
}

void __fastcall TFrmROM::btnUploadClick(TObject *Sender)
{
	ROM		*pROM;
	u32		size;
	HANDLE	th;

	s16	__stdcall (*IsDriverInstalled)();


	if (!FrmProg) Application->CreateForm(__classid(TFrmProg), &FrmProg);

	//Load low-level I/O library ---------------
	if (!HIoDll)
	{
		HIoDll = LoadLibrary("io.dll");

		if (HIoDll)
		{
			(void*)Io.PortOut			= (void*)GetProcAddress(HIoDll, "PortOut");
			(void*)Io.PortIn			= (void*)GetProcAddress(HIoDll, "PortIn");
			(void*)Io.NotPortBit		= (void*)GetProcAddress(HIoDll, "NotPortBit");
			(void*)IsDriverInstalled	= (void*)GetProcAddress(HIoDll, "IsDriverInstalled");

			if (!IsDriverInstalled())
			{
				Application->MessageBox("Could not install IO driver.\n" \
										"Try disabling IO.SYS by opening a Command Prompt and entering \"net stop io.sys\"",
										"SPCTool", MB_OK | MB_ICONERROR);

				FreeLibrary(HIoDll);
				HIoDll = NULL;

				return;
			}
		}
		else
		{
			Application->MessageBox("Could not load IO.DLL.  This file needs to be in your " \
									"SPCTool directory in order to communicate with your copier.  " \
									"You can download IO.DLL from http://www.geekhideout.com",
									"SPCTool", MB_OK | MB_ICONERROR);

			return;
		}

		SwcInit(Cfg.out.port, &Io);
	}

	//Create ROM data --------------------------
	size = CreateROM(&pROM);
	*(u32*)&pROM->_r3 = size >> 13;

	//Progress thread --------------------------
	th = (HANDLE)CreateThread(NULL, 512, (LPTHREAD_START_ROUTINE)SendROM, pROM, 0, (LPDWORD)&size);

	FrmProg->ShowTotal = false;					//Don't display "total" progress bar
	FrmProg->AutoClose = true;					//Close form if user clicks Cancel
	FrmProg->Caption = "Uploading ROM...";
	if (FrmProg->ShowModal() == mrCancel)
	{	//Give thread half of a second to end, then assume it's stalled
		if (WaitForSingleObject(th, 500) == WAIT_TIMEOUT) TerminateThread(th, 0);
	}

	CloseHandle(th);							//Deallocate handle resources

	free(pROM);
}


//**************************************************************************************************
void __fastcall TFrmROM::btnSaveClick(TObject *sender)
{
	s32		fh;
	u32		size;
	ROM		*pROM;
	s8		str[512],*s;


	if (!dlgSave->Execute())
	{
		ModalResult = mrNone;
		return;
	}

	if (dlgSave->FileName.Length() > 507) return;

	//Create file ------------------------------
	strcpy(str, dlgSave->FileName.c_str());

	s = strrchr(str, '.');						//If filename doesn't have an extension, add one
	if (!s || strrchr(s, '\\'))
	{
		switch(dlgSave->FilterIndex)
		{
		case(1):
			strcat(str, ".swc");
			break;

		case(2):
			strcat(str, ".smc");
			break;

		case(3):
			strcat(str, ".mgd");
			break;
		}
	}

	fh = FileCreate(str);
	if (fh == -1)
	{
		MessageDlg(AnsiString("Could not save ") + ScanStrR(dlgSave->FileName.c_str(), '\\'),
				   mtError, TMsgDlgButtons() << mbOK, 0);
		return;
	}

	size = CreateROM(&pROM);

	if (dlgSave->FilterIndex == 1 || dlgSave->FilterIndex == 2)
	{
		memset(str, 0, 512);

		*(u16*)&str[0] = size >> 13;

		str[2]	= 0x1C; //Use reset vector, HiROM, no SRAM
		str[8]	= 0xAA;
		str[9]	= 0xBB;
		str[10]	= 0x04;	//Program data

		FileWrite(fh, str, 512);
	}

	FileWrite(fh, pROM, size);
	FileClose(fh);
	free(pROM);

	ModalResult = mrOk;
}


//**************************************************************************************************
u32 CreateROM(ROM **ppROM)
{
	s32		v,r,t;
	s32		d,m,y;
	ROM		*pROM;
	SPCFile	*pSPC;
	ID666	*pID6;
	s8		*s;
	u32		size,i;


	//Allocate memory --------------------------
	if (FrmROM->chkPad->Checked)
	{
		size = 1024 << ROMSize;					//Allocate kilobytes specified by ROM header
		pROM = (ROM*)malloc(size);
		memset(pROM->ram[NumBanks], 0, size - (NumBanks << 16));
	}
	else
	{
		size = NumBanks << 16;					//Allocate 64kB for each song
		pROM = (ROM*)malloc(size);
	}

	memset(pROM, 0, 0x10000);					//Erase first bank


	//Create ROM ===============================
	memcpy(pROM->charset, Charset, sizeof(Charset));
	memcpy(pROM->code, ROMCode, sizeof(ROMCode));

	pROM->hdr.rom = ROMSize;					//Save size of ROM data

	pROM->songs = --NumBanks;					//Store the number of songs

	for (i=0; i<NumBanks; i++)
	{
		memcpy(pROM->title[i], Songs[i].title, 32);

		//======================================
		// Copy ID666 Tag

		pID6 = &Songs[i].pAPU->id6;

		memset(pROM->info[i], ' ', 24*32);		//Erase information screen text

		t = 0;
		if (pID6->song[0])						//Does ID666 tag contain a song title?
		{
			strcpy(pROM->info[i][t++], "Song:");
			strncpy(pROM->info[i][t++], pID6->song, 32);
			t++;								//Advance to next line on screen
		}

		if (pID6->game[0])
		{
			strcpy(pROM->info[i][t++], "Game:");
			strncpy(pROM->info[i][t++], pID6->game, 32);

			r = 0;
			if (pID6->copy)
			{
				pROM->info[i][t][0] = 0x1F;
				sprintf(&pROM->info[i][t][1], "%i", pID6->copy);
				r = 6;
			}

			if (pID6->pub[0])
			{
				strncpy(&pROM->info[i][t][r], pID6->pub, 32-r);
				r = 1;
			}

			if (r) t++;
			t++;
		}

		if (pID6->ost[0])
		{
			strcpy(pROM->info[i][t++], "OST:");
			strncpy(pROM->info[i][t++], pID6->ost, 32);

			sprintf(pROM->info[i][t++], "Disc: %i   Track: %i%c", pID6->disc, pID6->track>>8, pID6->track&0xFF);
			t++;
		}

		if (pID6->artist[0])
		{
			strcpy(pROM->info[i][t++], "Artist:");
			strncpy(&pROM->info[i][t++][0], pID6->artist, 32);
			t++;
		}

		if (pID6->dumper[0])
		{
			strcpy(pROM->info[i][t], "Dumped by");
			strncpy(&pROM->info[i][t++][10], pID6->dumper, 22);

			r = 0;
			if (pID6->date.date)
			{
				r = sprintf(pROM->info[i][t], "on %i.%i.%i ", pID6->date.year, pID6->date.month, pID6->date.day) + 1;
			}

			if (pID6->emu && pID6->emu<7)
			{
				sprintf(&pROM->info[i][t][r], "with %s", EmuList[pID6->emu]);
				r = 1;
			}

			if (r) t++;
			t++;
		}

		if (pID6->comment[0])
		{
			s = pID6->comment;
			do
			{
				for (r=0; r<32; r++)
				{
					if (*s < 0x20)
					{
						while (*s && *s < 0x20) s++;

						if (!*s) t = 24;
						break;
					}
					pROM->info[i][t][r] = *s++;
				}
			} while (++t < 24);
		}

		//Change any NULL characters into spaces
		s = pROM->info[i][0];
		for (r=0; r<24*32; r++, s++)
			if (*s < 0x1F) *s = 0x20;


		//======================================
		// Copy DSP Registers

		pSPC = &Songs[i].pAPU->spc;

		memcpy(pROM->dsp[i], pSPC->dsp.reg, 128);

		pROM->dsp[i][0x4C] = 0;					//KON = 0
		pROM->dsp[i][0x5C] = 0;	  				//KOF = 0
		pROM->dsp[i][0x6C] = 0x60;				//FLG = Reset off, mute on, echo disabled


		//======================================
		// Copy RAM

		memcpy(pROM->ram[i], pSPC->ram, 0x10000);
		if (pSPC->ram[0xF1] & 0x80) memcpy(&pROM->ram[i][0xFFC0], pSPC->xram, 0x40);
		pROM->ram[i][0xFD]	= 0x00;
		pROM->ram[i][0xFE]	= 0x00;
		pROM->ram[i][0xFF]	= 0x00;

		//Erase echo region --------------------
		if (!(pSPC->dsp.flg & 0x20))
			memset(&pROM->ram[i][(u32)((u8)pSPC->dsp.esa) << 8], 0, (u32)(pSPC->dsp.edl & 0xF) << 11);

		//Store PC and PSW on the stack --------
#ifdef	RETI
		pROM->ram[i][0x100 + pSPC->hdr.reg.sp--] = pSPC->hdr.reg.pc[1];
		pROM->ram[i][0x100 + pSPC->hdr.reg.sp--] = pSPC->hdr.reg.pc[0];
#endif
		pROM->ram[i][0x100 + pSPC->hdr.reg.sp--] = pSPC->hdr.reg.psw;	//Store PSW on the stack

		//Set hardcoded values in bootstrap ----
		Bootstrap[0*3+1]	= pSPC->ram[0];	   	//Restore the two bytes eaten by the IPL
		Bootstrap[1*3+1]	= pSPC->ram[1];

		Bootstrap[2*3+1]	= pSPC->ram[0xF1];	//Control

		Bootstrap[4*3+1]	= pSPC->dsp.flg;	//FLG
		Bootstrap[6*3+1]  	= pSPC->dsp.kon;	//KON
		Bootstrap[7*3+1]	= pSPC->ram[0xF2];	//DSP Address

		Bootstrap[0x19]		= pSPC->hdr.reg.sp;
		Bootstrap[0x1C]	   	= pSPC->hdr.reg.a;
		Bootstrap[0x1E]		= pSPC->hdr.reg.y;
		Bootstrap[0x20]	 	= pSPC->hdr.reg.x;
#ifndef	RETI
		Bootstrap[0x23]		= pSPC->hdr.reg.pc[0];
		Bootstrap[0x24]		= pSPC->hdr.reg.pc[1];
#endif

		memcpy(&pROM->ram[i][Songs[i].boot], Bootstrap, sizeof(Bootstrap));

		*(u16*)pROM->ram[i] = Songs[i].boot;	//Store pointer to boot strap at beginning of RAM
	}

	*ppROM = pROM;

	return size;
}


//**************************************************************************************************
void __fastcall TFrmROM::Initialize()
{
	NumBanks = 1;
	lvwFile->Items->Clear();
}


//**************************************************************************************************
void __fastcall TFrmROM::AddFile(APUFile *pAPU)
{
	u8			*p;
	u32			count,i;
	DSPReg		*pDSP;
	TListItem	*pItem;
	s8			str[33],*s;


	Songs[NumBanks-1].pAPU = pAPU;

	//Search for a location to store the bootstrap
	Songs[NumBanks-1].echo = 0;

	p = &pAPU->spc.ram[0xFFC0];					//Start search at IPL region
	count = 0;

	for (i=0xFFC0; i>0x100; i--)				//Search from FFC0-0100
	{
		if (*--p == 0xFF)
			count++;
		else
			count = 0;
												//If we have enough consecutive FF's, call it good
		if (count == sizeof(Bootstrap))
			break;
	}

	if (i == 0x100)		  						//If not enough FF's were found, try the echo region
	{
		pDSP = &pAPU->spc.dsp;
		if (!(pDSP->flg & 0x20) && (pDSP->edl & 0xF))
		{
			p = &pAPU->spc.ram[(u32)pDSP->esa << 8];
			Songs[NumBanks-1].echo = 1;
		}
		else
			p = NULL;
	}

	if (p)
		Songs[NumBanks-1].boot = (u32)p - (u32)pAPU->spc.ram;
	else
		Songs[NumBanks-1].boot = 0;

	//Create a title ---------------------------
	s = ScanStrR(pAPU->id6.file, '\\');

	memset(str, ' ', 32);
	str[32] = 0;
	sprintf(str, "%2i", NumBanks);
	str[2] = ' ';

	if (pAPU->id6.song[0])
	{
		i = strlen(pAPU->id6.song);
		memcpy(&str[3], pAPU->id6.song, i <= 29 ? i : 29);
	}
	else
	{
		i = (u32)s - (u32)ScanStrR(s, '.');
		memcpy(&str[3], s, i <= 29 ? i : 29);
	}

	memcpy(Songs[NumBanks-1].title, str, 33);

	//Add file to list -------------------------
	pItem = lvwFile->Items->Add();
	pItem->Caption = IntToHex((s32)Songs[NumBanks-1].boot, 4) + (Songs[NumBanks-1].echo ? "(E)" : "");
	pItem->Data = &Songs[NumBanks-1];

	pItem->SubItems->Append(s);
	pItem->SubItems->Append(Songs[NumBanks-1].title);

	NumBanks++;
}

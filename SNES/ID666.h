/***************************************************************************************************
* Class:      ID666 Tag Manager                                                                    *
* Platform:   C++                                                                                  *
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
*  2004.11.12                                                                                      *
*  + Changed the date to a simple structure instead of using the A2Date class (I didn't realize    *
*    that class didn't even work, which is what I get for programming in the early hours of the    *
*    morning.)                                                                                     *
*  + Made a bunch of other changes that never got documented                                       *
*                                                                                                  *
*                                                      Copyright (C)2001-2005 Alpha-II Productions *
***************************************************************************************************/

#pragma	once

//**************************************************************************************************
// Defines and Enumerations

//#define	ID666_ROM	0						//Set to true to enable ROM support

//Field ID's
#define	XID6_SONG		0x01					//http://www.alpha-ii.com/Info/SPCFormat.txt
#define	XID6_GAME		0x02
#define	XID6_ARTIST		0x03
#define	XID6_DUMPER		0x04
#define	XID6_DATE		0x05
#define	XID6_EMU		0x06
#define	XID6_CMNTS		0x07
#define	XID6_INTRO		0x30
#define	XID6_LOOP		0x31
#define	XID6_END		0x32
#define	XID6_FADE		0x33
#define	XID6_MUTE		0x34
#define	XID6_LOOPX		0x35
#define	XID6_AMP		0x36
#define	XID6_OST		0x10
#define	XID6_DISC		0x11
#define	XID6_TRACK		0x12
#define	XID6_PUB		0x13
#define	XID6_COPY		0x14

//Data types
#define	XID6_TSIZE		0x00					//Tag item is stored in the chunk header 'size'
#define	XID6_TSTR		0x01					//NULL terminated ANSI string
#define	XID6_TWSTR		0x02					//NULL terminated unicode string
#define	XID6_TINT		0x04					//Integer data

//Timer stuff
#define	XID6_MAXTICKS	383999999				//Max ticks possible for any field (99:59.99 * 64k)
#define	XID6_TICKSMIN	3840000			  		//Number of ticks in a minute (60 * 64k)
#define	XID6_TICKSSEC	64000			  		//Number of ticks in a second
#define	XID6_TICKSMS	64				  		//Number of ticks in a millisecond
#define	XID6_MAXLOOP	9				  		//Max loop times

#define	XID6_EMULIST	7						//Number of items in emulator list

//File types
typedef enum
{
	ID6_UNK = -1,								//Unknown file type
	ID6_ERR,									//Error opening file
	ID6_SPC,									//Normal SPC
	ID6_EXT,									//SPC with extended ID666 tag
	ID6_ZST,									//ZSNES saved state
	ID6_ROM = 64,								//SNES ROM image
	ID6_SWC,									//  " " Super WildCard
	ID6_FIG,									//  " " Pro Fighter
	ID6_SF3,									//  " " Game Doctor SF III
	ID6_ROMH									//  " " dumper header unknown
} ID6Type;


//**************************************************************************************************
// Structures

typedef struct
{
	u8		pc[2];								//[25] Program Counter
	u8		a;									//[27] Accumulator
	u8		x;									//[28] X index
	u8		y;									//[29] Y index
	u8		psw;								//[2A] Program Status Word
	u8		sp;									//[2B] Stack Pointer
} SPCReg;

//SPC header structure
typedef struct
{
	s8		fTag[33];							//[00] File tag
	s8		tTerm[3];							//[21] Tag terminator (0x1A1A1A)
	s8		ver;								//[24] Version * 100 (i.e. 30 for v0.30)
	SPCReg	reg;								//[25] SPC Registers
	u16		__r1;								//[2C]
	s8		song[32];							//[2E] Song title
	s8		game[32];							//[4E] Game title
	s8		dumper[16];							//[6E] Name of dumper
	s8		comment[32];						//[7E] Comment
	s8		date[11];							//[9E] Date dumped
	s8		songLen[3];							//[A9] Song length (# of seconds to play before fading)
	s8		fadeLen[5];							//[AC] Fade length (milliseconds)
	s8		artist[32];							//[B1] Artist of song
	u8		chnDis;								//[D1] Channel Disabled?
	u8		emulator;							//[D2] Emulator dumped with
	u8		__r2[45];							//[D3]
} SPCHdr;

//Vector table for SNES ROM
typedef struct
{
	u16		_r[2];
	u16		cop;
	u16		brk;								//Software break
	u16		abort;
	u16		nmi;								//Non-maskable interrupt
	u16		reset;								//Reset (starting location)
	u16		irq;
} ROMVec;

//SNES ROM header
typedef struct
{
	s8		name[21];							//ROM name
	u8		makeup;								//ROM type
	u8		mtype;								//Memory types
	u8		rom;								//ROM size (log2 kB)
	u8		sram;								//SRAM size (log2 kB)
	u8		region;								//Region for distribution
	u8		maker;								//Developer/publisher
	u8		version;							//Version minor
	u16		icrc;								//Inverse checksum
	u16		crc;								//Checksum
	ROMVec	native;								//Vectors for native 16-bit 65816
	ROMVec	emu;								//Vectors for 6502 emulation mode
} ROMHdr;

//Field header
typedef struct
{
	u8		id;									//Field ID
	u8		type;								//Type of data
	u16		size;								//Size of data (in bytes)
	u32		data[];
} XID6Field;

typedef union
{
	struct
	{
		s8	day;
		s8	month;
		s16	year;
	};

	s32	date;
} ID6Date;


//**************************************************************************************************
// External Data

extern const s8	EmuList[7][12];


////////////////////////////////////////////////////////////////////////////////////////////////////

class ID666
{
private:
	//void				GetID666_(SPCHdr&);
        void                            GetID666(SPCHdr &spc);
	//b8					GetXID6_(XID6Field* pTag, s32 size);
        b8                                      GetXID6(XID6Field* pTag, s32 size);
	void				ToSPC(SPCHdr& spc, b8 bin);
	static s32			IsText(s8* str, u32 len);
	static XID6Field*	WriteSize(XID6Field* pChk, u8 id, u16 data);
	static XID6Field*	WriteInt(XID6Field* pChk, u8 id, u32 data);
	static XID6Field*	WriteStr(XID6Field* pChk, u8 id, const s8* str);
	b8					ToStr(s8** ppStr, const s8* pFmt, u32 len);


	//==========================================
	// Variables

	b8	bin;									//Tag was stored in binary format
	b8	ext;									//File contained an xid6 chunk

public:
				ID666();

	ID666*		operator = (const ID666&);		//Copy ID666 object

	//**********************************************************************************************
	// Load Tag from SPC File
	//
	// In:
	//    file-> Path and filename of a vaild SPC or ZST file
	//           or a buffer containg a file
	//    size = Size of buffer (0 if 'file' is a filename)
	//
	// Out:
	//    Type of file tag was loaded from

	ID6Type		LoadTag(const s8* file, u32 size = 0);


	//**********************************************************************************************
	// Save Tag to SPC File
	//
	// In:
	//    bin = Save ID666 tag in binary format
	//    ext = Save extended information
	//
	// Out:
	//    True, if tag was successfully saved

	b8			SaveTag(b8 bin, b8 ext);
//	b8			SaveTag(b8 bin, b8 ext, SPCReg* pReg, void* pRAM, void* pDSP, void* pXRAM);


	//**********************************************************************************************
	// Get Time
	//
	// Returns the length of the song in 64000ths of a second, if a length exists.  Otherwise the
	// default time is returned.
	//
	// GetTotal() returns the song length plus the fade.

	u32			GetSong();
	u32			GetFade();
	u32			GetTotal();


	//**********************************************************************************************
	// Set Date
	//
	// Takes YMD components and stores them in date
	//
	// In:
	//    year  = Year (1998 to 9999, values < 100 are assumed to be the two digit year starting with
	//                  1998.  i.e. 04 = 2004)
	//    month = Month (1 to 12)
	//    day   = Day (1 to 31)
	//
	// Out:
	//    true, if the date was successfully converted
	//    false, if the date contained invalid numbers, or was uninterpretable

	b8			SetDate(u32 year, u8 month, u8 day);


	//**********************************************************************************************
	// Create String from ID666
	//
	// Creates an information string based on a given format string
	//
	// In:
	//    str    -> String to stort output in
	//    format -> Format string
	//              %0 - Path      %2 - Song title  %7 - OST title  %A - Dumper
	//              %1 - Filename  %3 - Game title  %8 - OST disc   %B - Date dumped
	//                             %4 - Artist      %9 - OST track  %C - Emulator used
	//                             %5 - Publisher                   %D - Comments
	//                             %6 - Copyright
	//
	//              [IF | ELSE] If field contains a valid value, use the format IF.
	//                          Otherwise use the format ELSE.
	//
	//              ex. %9[%9 - ]%2[%2|(no song title)]
	//
	//              <nn Copy no more than nn characters from field
	//              =nn Print exactly nn characters.  If field is too short, pad with spaces
	//              >nn Print at least nn characters.  If field is too short, pad with spaces
	//
	//              ex. %3=16 - %2<32 by %4
	//
	// Out:
	//    True, if string contains items from the format string
	//    False, if string did not contain any items from the format string.  (Output string will
	//           contain the filename.)

	b8			ToStr(s8* str, const s8* format);


	//**********************************************************************************************

	inline b8	SameFile(const s8* fn) {return !strcmp(file, fn);}


	//**********************************************************************************************
	// Get ID666 Tag Information

	inline b8	IsBin() {return bin;}			//Tag was saved in binary format
	b8			IsExt();						//Tag contains extended information
	b8			HasTime();						//Tag contains song time


	//==========================================
	// Variables

	//Conversion options -----------------------
	static u32	defSong;						//Default song length in 1/64000ths second
	static u32	defFade;						//Default fade length in 1/64000ths second
	static b8	preferBin;						//Default to binary format, if type is indeterminable

	s8			file[256];					 	//Filename

	//Fields -----------------------------------
	s8			song[256];					 	//Song title
	s8			game[256];					 	//Game title
	s8			artist[256];				 	//Song artist
	s8			dumper[256];				 	//SPC dumper
	s8			comment[256];				 	//Comments
	s8			ost[256];					 	//Original soundtrack title
	s8			pub[256];					 	//Game publisher
	s8			datetxt[12];				 	//Date dumped (copied from ID666)
	ID6Date		date;						 	//Date dumped
	u32			copy;						 	//Game copyright date
	u32			track;						 	//OST track number
	u8			disc;						 	//OST disc number
	u8			mute;						 	//Muted voices
	s8			emu;						 	//Emulator used to dump
	u32			amp;						 	//Amplification level

	//Length in binary form for emulator -------
	u32			intro;							//Song in 1/64000ths second
	u32			loop;
	u32			loopx;
	s32			end;
	u32			fade;							//Fade in 1/64000ths second
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// External Functions

//**************************************************************************************************
// Does File Contain SPC Data?
//
// Determines what kind of file is being opened
//
// In:
//    fn -> Filename
//    fh -> File handle to opened file, if file was successfully identified
//
//    pFile-> Buffer containing file
//    size  = Size of buffer
//
// Out:
//    see SPCType enumeration

ID6Type	IsSPC(const s8* fn, FILE*& fh);
ID6Type	IsSPC(const s8* pFile, u32 size);


//**************************************************************************************************
// String to Ticks
//
// Converts a string float of seconds into an integer of ticks
// Time is clipped at 59.99
// The format can be ss.mmm or ss:ttttt, for seconds and milliseconds or seconds and ticks
//
// In:
//    Str -> String
//
// Out:
//    Time in 1/64000ths second

u32		Str2Ticks(const s8* str);

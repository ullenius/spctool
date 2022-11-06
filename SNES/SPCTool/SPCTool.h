/***************************************************************************************************
* File:       SPCTool                                                                              *
* Platform:   Win32 (BCB5)                                                                         *
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
* 0.2  xx.xx.xx                                                                                    *
*                                                           Copyright (C)2004 Alpha-II Productions *
***************************************************************************************************/


//**************************************************************************************************
// Auto Includes

#include	<stdio.h>
#include        <comctrls.hpp>
#include	"Types.h"
#include	"SNESAPU.h"
#include	"Unrar.h"
#include	"A2Math.h"	
#include	"A2Str.h"
#include	"ID666.h"
#include    "Help\Index.h"

//**************************************************************************************************
// Defines and Macros

//Source flags
#define	SF_FILTERS	0x0F						//[3-0] blocks use filters 3-0
#define	SF_CHANGED	0x10						//[4]   source has changed
#define	SF_INFO		0x20						//[5]   contains instrument information
#define	SF_PLAYED	0x40						//[6]   played
#define	SF_MUTE		0x80						//[7]

//File flags
#define	F_HIROM		0x01						//(ROM)
#define	F_MULTI		0x02						//(ROM)

#define	F_READONLY	0x10						//      File cannot be changed
#define	F_ARCHIVE	0x20						//      File is from a RAR archive
#define	F_TAG		0x40						//(SPC) Tag has been modified
#define	F_STATE		0x80						//      State has been modified
#define	F_SAVE		0xC0						//      File needs to be saved

#define	_IncPtr(p, i) *(u32*)(&p) += (i)

#define	_CountOf(x) (sizeof(x) / sizeof(x[0]))


//**************************************************************************************************
// Structures

typedef struct : public SAPUFunc
{
	u8		*ram;
	DSPReg	*dsp;
	Voice	*voice;
	APUProf	*profile;
} SAPUData;

typedef struct									//RIFF Chunk
{
	u32	id;
	u32	size;
	u32	type;
} Chunk;

typedef struct									//RIFF Subchunk
{
	u32	id;
	u32	size;
} SubChk;

typedef struct									//Sound Source
{
	u32	offset;									//Offset in APU RAM or ROM file (-1 if invalid src)
	u16	len;									//Length of source (in blocks)
	u16	loop;									//Position of loop from source start (-1 no loop)
	u16	crc;									//16-bit checksum
	u16	idx;									//Index in sound source directory
	u8	flags;                                  //see SF_???
	u8	_r1[3];

	//Instrument information -------------------
	u32	rate;									//Sample rate for middle-C playback
	u16	adsr;									//ADSR register (if !bit-7, high byte is GAIN reg)
	u8	prog;									//GM program number ([7] tone/drum  [6-0] program #)
	u8	bank;									//Program bank
	u32	_r2;

     	TListItem	*pItem;
} Source;

typedef struct									//[104D0] APU State (used by SNESAPU.DLL)
{
	s8	   		ram[65536];						//[10000]
	DSPReg		dsp;							//   [80]
	Voice		voice[8];						//  [400]

	SPCState	spcState;						//   [30]
	DSPState	dspState;						//   [20]
} APUState;

typedef struct									//[10200]
{
	SPCHdr		hdr;							//  [100]
	s8	   		ram[65536];						//[10000]
	DSPReg		dsp;							//   [80]
	s8			_r2[64];						//   [40]
	s8	   		xram[64];						//   [40]
} SPCFile;

typedef struct									//  [848]
{
	ID6Type	type;			//Type of file
	u8		flags;			//Flags for file modification
	u16		num;			//Number of items in src[]
	u8		*pData;			//Pointer to buffer containing sound source data (APU RAM or ROM file)
	u32		size;			//Size of buffer
	Source	*pTop;			//Sound source visible at the top of the list view
	ID666	id6;
	Source	src[];
} FileInfo;

typedef struct : public FileInfo				//[23078] Edit SPC/ZST
{
	Source			src[256];					// [2000] Table of sound sources
	u8				ord[256];					//  [100] Display order of sound sources
	Set<u8,0,255>	sel;						//   [20]
	APUState		state;						//[10510] Current execution
	SPCFile			spc;						//[10200] Original SPC file
} APUFile;

typedef struct : public FileInfo				// [89B0] Edit ROM
{
	Source			src[1000];					// [7D00] Possible sound sources
	u8				ord[1000];					//  [3E8]
	Set<u32,0,999>	sel;						//  [FA0]
	u8				rom[];						//        ROM data
} ROMFile;

//Configuration --------------------------------
typedef struct
{
	u32	rate;
	u8	bits;
	u8	mix;
	u8	inter;
	b8	analog;
	b8	filter;
	b8	surround;
	b8	reverse;
	b8	disEcho;
	b8	oldSmp;
	u32	stereo;
	s32	efbct;

	u32	Opts();
} CfgDSP;

typedef struct
{
	u8	aar;
	b8	reset;
	u32	amp;
	u32	min,max;
	u32	threshold;
} CfgMix;

typedef struct
{
	u32	ver;
	u32	lcid;

	s8	preset;

	u32	curFile;

	s8	ini[256];
	s8	hlp[256];
	s8	file[9][256];
	s8	col[16];

	s8	renFmt[64];

	struct
	{
		u32	ver;
		u32	min;
		u32	opt;
		u32	clk;
	} apu;

	CfgDSP	udsp;
	CfgMix	umix;

	CfgDSP	dsp;
	CfgMix	mix;

	struct
	{
		u32	min,max;
		b8	linear;
		b8	silence;
		u8	range;
		u8	noDP;
		b8	init;
	} src;

	struct
	{
		u32	song;
		u32	fade;
		u32	skip;
		b8	next;
		b8	fastSeek;
		b8	binary;
		b8	remain;
		b8	enable;
	} time;

	struct
	{
		u8	syntax;
		s8	ltrCase;

		b8	printPC;
		b8	printOp;
		b8	printCmnt;
		u8	cmntCol;

		s8	hexChar[16];
		s8	hex;
		s8	abs;
		s8	openBrace;
		s8	closeBrace;
	} debug;

	struct
	{
		s32	device;
		u8	size;
		u8	snes;
		u16	force;
		u16	port;
	} out;

	struct
	{
		u8	fps;
		b8	vu;
		b8	scope;
	} vis;

} Config;

//Color ----------------------------------------
typedef union
{
	struct
	{
		u8	r,g,b,a;
	};
	u32	rgba;
} RGBA;

typedef struct									//Colors for custom buttons
{												//Highlight, Light, Face, Shadow, Dark shadow
	TColor	sh,sl,sf,ss,sd;						//SPCTool's colors
	TColor	wh,wl,wf,ws,wd;						//Windows' colors
} BtnColors;

typedef struct
{
	HANDLE	__stdcall (*OpenArchive)(struct RAROpenArchiveData *ArchiveData);
	s32		__stdcall (*CloseArchive)(HANDLE hArcData);
	s32		__stdcall (*ReadHeader)(HANDLE hArcData, struct RARHeaderData *HeaderData);
	s32		__stdcall (*ProcessFile)(HANDLE hArcData, s32 Operation, s8 *DestPath, s8 *DestName);
	void	__stdcall (*SetCallback)(HANDLE hArcData, UNRARCALLBACK Callback, LONG UserData);
	s32		__stdcall (*GetDllVersion)();
} UnrarFunc;


//**************************************************************************************************
// External Data

extern const s8		NClk[32][12];
extern SAPUData		Apu;						//Pointers to APU data and functions
extern UnrarFunc	Rar;
extern Config		Cfg;						//Current configuration
extern BtnColors	CBtn;


//**************************************************************************************************
// Public Functions

void	__fastcall GetFileName(s8 *fn, FileInfo *pFile, s8 *ext);

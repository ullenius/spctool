/***************************************************************************************************
* Program:    Super Nintendo Entertainment System(tm) Audio Processing Unit Emulator DLL           *
* Platform:   Intel 80386 & MMX                                                                    *
* Programmer: Anti Resonance                                                                       *
*                                                                                                  *
* "SNES" and "Super Nintendo Entertainment System" are trademarks of Nintendo Co., Limited and its *
* subsidiary companies.                                                                            *
*                                                                                                  *
* This library is free software; you can redistribute it and/or modify it under the terms of the   *
* GNU Lesser General Public License as published by the Free Software Foundation; either version   *
* 2.1 of the License, or (at your option) any later version.                                       *
*                                                                                                  *
* This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* See the GNU Lesser General Public License for more details.                                      *
*                                                                                                  *
* You should have received a copy of the GNU Lesser General Public License along with this         *
* library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,        *
* Boston, MA  02111-1307  USA                                                                      *
*                                                                                                  *
* ------------------------------------------------------------------------------------------------ *
* Revision History:                                                                                *
*                                                                                                  *
*  2004.xx.xx SNESAmp v3.3                                                                         *
*  - Added a build check for SSE2 processors in EightToSixteen()                                   *
*  - Fixed the memcpy() in BitRateReduce() when the loop point was < 16                            *
*                                                                                                  *
*  2004.01.26                                                                                      *
*  + Added BitRateReduce, a better function for creating BBR blocks from raw samples               *
*                                                                                                  *
*  2003.02.10 SNESAmp v2.99                                                                        *
*  + Added version information to the DLL                                                          *
*  + Moved GetAPUData to SNESAPU.cpp, since it's only needed for SNESAPU.DLL                       *
*  + Moved memory allocation into SNESAPU.cpp, since memory allocation is system, and possibly     *
*    application, specific                                                                         *
*                                                                                                  *
*  2001.01.01 SNESAmp v2.0                                                                         *
*  + Made some changes to the internal interface with the DSP                                      *
*                                                                                                  *
*  2000.12.14 Super Jukebox v3.0                                                                   *
*                                                      Copyright (C)2001-2005 Alpha-II Productions *
***************************************************************************************************/

#define	SNESAPU_DLL	0x02280007					//Don't include local function/variable definitions

#include	"APU.h"
#include	"SPC700.h"
#include	"DSP.h"
//#include	"BRR.h"

//**************************************************************************************************
// Type Definitions

//See GetAPUData() for explanations of each
typedef enum
{
	SADATA_OPTIONS = -1,
	SADATA_RAM,
	SADATA_DSP,
	SADATA_VOICE,
	SADATA_PROFILE
} SADataType;

//Function pointers to SNESAPU
typedef struct
{
	void*	(__stdcall *GetAPUData)(SADataType dt);

	void	(__stdcall *ResetAPU)();
	void	(__stdcall *LoadSPCFile)(void* pSPC);
	void	(__stdcall *SaveAPU)(SPCState* pSPC, DSPState* pDSP);
	void	(__stdcall *RestoreAPU)(SPCState* pSPC, DSPState* pDSP);
	void	(__stdcall *SetAPUOpt)(u32 mix, u32 chn, u32 bits, u32 rate, u32 inter, u32 opts);
	void	(__stdcall *SetAPUSmpClk)(u32 speed);
	void*	(__stdcall *EmuAPU)(void* pBuf, u32 cycles, u32 samples);
	void	(__stdcall *SeekAPU)(u32 time, b8 fast);

	SPCDebug*	(__stdcall *SetSPCDbg)(SPCDebug* pTrace, u32 opts);
	void	(__stdcall *SetAPURAM)(u32 addr, u8 val);
	void	(__stdcall *SetAPUPort)(u32 port, u8 val);
	u8		(__stdcall *GetAPUPort)(u32 port);
	u32		(__stdcall *GetSPCTime)();
	s32		(__stdcall *EmuSPC)(u32 cyc);

	u32		(__stdcall *GetProcInfo)();
	DSPDebug*	(__stdcall *SetDSPDbg)(DSPDebug* pTrace, u32 opts);
	void	(__stdcall *SetDSPAmp)(u32 level);
	u32		(__stdcall *GetDSPAmp)();
	void	(__stdcall *SetDSPAAR)(u32 type, u32 thresh, u32 min, u32 max);
	u32		(__stdcall *SetDSPLength)(u32 song, u32 fade);
	void	(__stdcall *SetDSPPitch)(u32 base);
	void	(__stdcall *SetDSPStereo)(u32 sep);
	void	(__stdcall *SetDSPEFBCT)(s32 leak);
	b8		(__stdcall *SetDSPVoiceMute)(u32 voice, u32 state);
	b8		(__stdcall *SetDSPReg)(u8 reg, u8 val);
	void*	(__stdcall *EmuDSP)(void* pBuf, s32 size);
	void*	(__stdcall *PlaySrc)(void* pSrc, u32 loop, u32 rate);
	u32		(__stdcall *UnpackSrc)(s16* pBuf, void* pSrc, u32 num, u32 opts, s32* prev1, s32* prev2);
	void*	(__stdcall *PackSrc)(void* pBlk, void* pSmp, u32* pLen, u32* pLoop, s32 opt, s32* prev1, s32* prev2);
	void	(__stdcall *VMax2dB)(void* pList, b8 fp);
} SAPUFunc;


//**************************************************************************************************
//External Functions - See other header files for exported function descriptions

#define	import	__declspec(dllimport)

#ifdef	__cplusplus
extern "C" {
#endif


//**************************************************************************************************
// Get Pointer to APU Internal Data
//
// Returns pointers to internal data structures.  Access is provided solely for debugging purposes.
// All data should be considered read-only.
//
// SADATA_OPTIONS
//    Returns the build options specified in APU.Inc (the return value is a u32, not a void*)
//    bits 23-0  Options (see #define SA_??? in APU.h)
//    bits 31-24 SPC700 clock divisor (divide APU_CLK by the clock divisor to get the emulated
//               clock speed of the SPC700)
//
// SADATA_RAM
//    Returns a pointer to the 64KB of APU RAM - apuRAM[65536]
//
// SADATA_DSP
//    Returns a pointer to the 128 bytes of DSP registers - DSPReg (DSP.h)
//
// SADATA_VOICE
//    Returns a pointer to an array of 8 Voice structures - Voice[8] (DSP.h)
//
// SADATA_PROFILE
//    Returns a pointer to a profiling structure - APUProf (SPC700.h)
//    If profiling isn't enabled (see #define SA_PROFILE, APU.h) the pointer returned will be NULL.
//
// In:
//    dt = Pointer to retrieve (see SADataType)
//
// Out:
//    Pointer to requested data

import	void*	__stdcall GetAPUData(SADataType dt);


import	void	__stdcall BitRateReduce(void* pBlk, void* pSmp, u32* pLen, u32* pLoop, s32 opt);

import	void	__stdcall ResetAPU();
import	void	__stdcall LoadSPCFile(void* pSPC);
import	void	__stdcall SaveAPU(SPCState* pSPC, DSPState* pDSP);
import	void	__stdcall RestoreAPU(SPCState* pSPC, DSPState* pDSP);
import	void	__stdcall SetAPUOpt(u32 mix, u32 chn, u32 bits, u32 rate, u32 inter, u32 opts);
import	void	__stdcall SetAPUSmpClk(u32 speed);
import	void*	__stdcall EmuAPU(void* pBuf, u32 cycles, u32 samples);
import	void	__stdcall SeekAPU(u32 time, b8 fast);

import	SPCDebug*	__stdcall SetSPCDbg(SPCDebug* pTrace, u32 opts);
import	void	__stdcall SetAPURAM(u32 addr, u8 val);
import	void	__stdcall SetAPUPort(u32 port, u8 val);
import	u8		__stdcall GetAPUPort(u32 port);
import	u32		__stdcall GetSPCTime();
import	s32		__stdcall EmuSPC(u32 cyc);

import	u32		__stdcall GetProcInfo();
import	DSPDebug*	__stdcall SetDSPDbg(DSPDebug* pTrace, u32 opts);
import	void	__stdcall SetDSPAmp(u32 level);
import	u32		__stdcall GetDSPAmp();
import	void	__stdcall SetDSPAAR(u32 type, u32 thresh, u32 min, u32 max);
import	u32		__stdcall SetDSPLength(u32 song, u32 fade);
import	void	__stdcall SetDSPPitch(u32 base);
import	void	__stdcall SetDSPStereo(u32 sep);
import	void	__stdcall SetDSPEFBCT(s32 leak);
import	b8		__stdcall SetDSPVoiceMute(u32 voice, u32 state);
import	b8		__stdcall SetDSPReg(u8 reg, u8 val);
import	void*	__stdcall EmuDSP(void* pBuf, s32 size);
import	void*	__stdcall PlaySrc(void* pSrc, u32 loop, u32 rate);
import	u32		__stdcall UnpackSrc(s16* pBuf, void* pSrc, u32 num, u32 opts, s32* prev1, s32* prev2);
import	void*	__stdcall PackSrc(void* pBlk, void* pSmp, u32* pLen, u32* pLoop, s32 opt, s32* prev1, s32* prev2);
import	void	__stdcall VMax2dB(void* pList, b8 fp);

#ifdef	__cplusplus
}
#endif
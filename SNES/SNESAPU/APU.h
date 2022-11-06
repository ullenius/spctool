/***************************************************************************************************
* Program:    Super Nintendo Entertainment System(tm) Audio Processing Unit Emulator               *
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
*  2004.xx.xx SNESAPU 2.4                                                                          *
*  + Added NULL pointer support to SaveAPU and RestoreAPU                                          *
*  + Moved variables into the .bss                                                                 *
*  + The build options are now returned by InitAPU instead of being stored in a global variable    *
*  - Calculate the emulated sample rate at the top of EmuAPU so SetAPUSmpClk will be thread safe   *
*                                                                                                  *
*  2003.11.04 SNESAPU 2.0                                                                          *
*  + Updated EmuAPU to use SetEmuDSP when DSPINTEG is enabled                                      *
*  + Move fade out code into DSP.Asm                                                               *
*  + Removed amp setting from ResetAPU                                                             *
*                                                                                                  *
*  2003.07.12 SNESAPU 1.0a                                                                         *
*  - Fixed the call to SetDSPVol in SetFade that wasn't restoring the stack                        *
*                                                                                                  *
*  2003.06.20                                                                                      *
*  + Added LoadSPCFile                                                                             *
*  + Added ability to pass -1 to ResetAPU to keep current amp level                                *
*                                                                                                  *
*  2003.02.10                                                                                      *
*  + Rewrote in assembly                                                                           *
*  + Removed GetAPUData and memory allocation (they were specific to SNESAPU.DLL)                  *
*  - Sometimes EmuAPU would generate an extra sample                                               *
*                                                                                                  *
*  2002.06.20                                                                                      *
*  - Call SetFade after seeking                                                                    *
*                                                                                                  *
*  2001.04.12                                                                                      *
*  + Updated SeekAPU to incorporate new features in SPC700 and DSP emus                            *
*                                                                                                  *
*  2000.12.30 SNESAmp v2.0                                                                         *
*  + Added SetAPULength, SeekAPU, SaveAPU, RestoreAPU, HaltAPU                                     *
*  + General optimizations                                                                         *
*  - Simplified interface a bit                                                                    *
*                                                                                                  *
*  2000.04.04 SNESAmp v1.0                                                                         *
*                                                                                                  *
*  2000.03.17 SNESAmp v0.9                                                                         *
*                                                        Copyright (C)2003-05 Alpha-II Productions *
***************************************************************************************************/

//**************************************************************************************************
// Defines

//The build options correspond to the equates at the top of APU.Inc

//SPC700 clock speed divisor (bits 7-0) --------
#define	SA_CLKDIV	0xFF						//Mask for retieving the clock divisor

//APU build options (bits 15-8) ----------------
#define	SA_DEBUG	0x00000100					//Debugging ability enabled
#define	SA_DSPINTEG	0x00000200					//DSP emulation is integrated with the SPC700

//SPC700 build options (bits 23-16) ------------
#define	SA_HALFC	0x00010000					//Half-carry flag emulation enabled
#define	SA_IPLW		0x00080000					//IPL ROM region writeable
#define	SA_CNTBK	0x00020000					//Break SPC700/Update DSP if a counter is increased
#define	SA_DSPBK	0x00100000					//Break SPC700/Update DSP if 0F3h is read from
#define	SA_SPEED	0x00040000					//Hacks are enabled to skip cycles when a counter is
												// read or the processor is sleeping
#define SA_PROFILE	0x00200000					//Profiling counters enabled

//DSP build options (bits 31-24) ---------------
#define	SA_MAINPPM	0x01000000					//Peak program metering on main output (for AAR)
#define	SA_VOICEPPM	0x02000000					//Peak program metering on voices (for visualization)
#define	SA_STEREO	0x04000000					//Stereo controls enabled (seperation and EFBCT)


////////////////////////////////////////////////////////////////////////////////////////////////////
// External Functions

#ifndef	SNESAPU_DLL

#ifdef	__cplusplus
extern "C" {
#endif

//**************************************************************************************************
// Initialize Audio Processing Unit
//
// Initializes internal tables, mixer settings, memory, and registers.
// This only needs to be called once.
//
// see InitSPC() and InitDSP()
//
// Out:
//    APU build options (see the SA_??? #defines above, APU.Inc, and SNESAPU.h GetAPUData())

u32 InitAPU();


//**************************************************************************************************
// Reset Audio Processor
//
// Clears all memory and sets registers to their default values
//
// see ResetSPC() and ResetDSP()

void ResetAPU();


//**************************************************************************************************
// Load SPC File
//
// Restores the APU state from an .SPC file.  You do not need to call ResetAPU before loading a
// file.
//
// In:
//    pFile -> 66048 byte SPC file

void LoadSPCFile(void* pFile);


//**************************************************************************************************
// Save Audio Processor State
//
// Creates a saved state of the audio processor
//
// see SaveSPC() and SaveDSP()
//
// In:
//    pSPC -> SPCState structure (can be NULL)
//    pDSP -> DSPState structure (can be NULL)

void SaveAPU(SPCState* pSPC, DSPState* pDSP);


//**************************************************************************************************
// Restore Audio Processor State
//
// Restores the audio processor from a saved state
//
// see RestoreSPC() and RestoreDSP()
//
// In:
//    pSPC -> SPCState structure (can be NULL)
//    pDSP -> DSPState structure (can be NULL)

void RestoreAPU(SPCState* pSPC, DSPState* pDSP);


//**************************************************************************************************
// Set Audio Processor Options
//
// Configures the sound processor emulator.  Range checking is performed on all parameters.
//
// Notes:  -1 can be passed for any parameter you want to remain unchanged
//         see SetDSPOpt() in DSP.h for a more detailed explantion of the options
//
// In:
//    mix   = Mixing routine (default MIX_INT)
//    chn   = Number of channels (1 or 2, default 2)
//    bits  = Sample size (8, 16, 24, 32, or -32 [IEEE 754], default 16)
//    rate  = Sample rate (8000-192000, default 32000)
//    inter = Interpolation type (default INT_GAUSS)
//    opts  = See 'DSP options' in the Defines section of DSP.h

void SetAPUOpt(u32 mix, u32 chn, u32 bits, u32 rate, u32 inter, u32 opts);


//**************************************************************************************************
// Set Audio Processor Sample Clock
//
// Calculates the ratio of emulated clock cycles to sample output.  Used to speed up or slow down a
// song without affecting the pitch.  Range checking is performed.
//
// This function is safe to call while the APU is emulated in another thread
//
// In:
//    speed = Multiplier [16.16] (1/16x to 16x)

void SetAPUSmpClk(u32 speed);


//**************************************************************************************************
// Emulate Audio Processing Unit
//
// Emulates the APU for a specified amount of time.  DSP output is placed in a buffer to be handled
// by the main program.
//
// Calling EmuAPU instead of EmuSPC and EmuDSP will ensure the two processors are kept in sync with
// each other.
//
// If the cycle count is 0, the SPC700 will be emulated until the DSP has filled the output buffer
// with the specified number of samples.
//
// If the sample count is 0, the DSP will be produce output for the same amount of time as the
// SPC700 is emulated.
//
// If the cycle count and sample count aren't equal in terms of time, then one processor will be
// halted at the end of its count while the other processor is continued.
//
// In:
//    pBuf   -> Buffer to store output samples (can be NULL)
//    cycles  = Number of clock cycles to emulate SPC700 for (APU_CLK = 1 second)
//    samples = Number of samples for the DSP to generate
//
// Out:
//    -> End of buffer

void* EmuAPU(void* pBuf, u32 cycles, u32 samples);


//**************************************************************************************************
// Seek to Position
//
// Seeks forward in the song from the current position
//
// In:
//    time = 1/64000ths of a second to seek forward (must be >= 0)
//    fast = Use faster seeking method (may break some songs)

void SeekAPU(u32 time, b8 fast);


//**************************************************************************************************
// Shutdown Audio Processing Unit
//
// Currently does nothing

void ShutAPU();

#ifdef	__cplusplus
}
#endif

#endif	//SNESAPU_DLL
/***************************************************************************************************
* File:       Super WildCard Interface                                                             *
* Platform:   Win32                                                                                *
* Programmer: Anti Resonance                                                                       *
*                                                                                                  *
* The information here is based on my limited understanding of the SWC.  I make no claim that it   *
* is accurate or complete.                                                                         *
*                                                                                                  *
* ------------------------------------------------------------------------------------------------ *
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
* 2004.08.26                                                                                       *
* + Added SwcSetPage()                                                                             *
* + Added #defines for Super Magic Drive memory locations                                          *
* + Added #defines and structures for Front Fareast file headers                                   *
* - Improved the delay/timeout system (super fast systems should return less LPT connect errors)   *
*                                                                                                  *
* 2004.10.03                                                                                       *
* + Added a timeout to WriteByte()                                                                 *
* + Removed the message box from the exception thrower.  Exceptions must be caught and handled by  *
*   the caller.                                                                                    *
* + Added SwcClearOut(), don't know if it helps.                                                   *
*                                                                                                  *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/


// Memory Mapping
// ==============
//
// SWC banks map directly to cartridge ROM banks.  But unlike the SNES, ROM can only be accessed in
// 8kB pages located at 2000 and A000.  The current page being accessed can be changed by writing to
// the memory addresses defined in SWC_PAGE? or by using the "set memory page" command (5).
//
// Mode 0 - BIOS mode
//
// xx2000-xx3FFF  SRAM/Cart ROM [40-7D, C0-FF] (lower 32kB of HiROM banks)
// xx8000-xx9FFF  DRAM          [00-7D, 80-FF]
// xxA000-xxBFFF  SRAM/Cart ROM [00-7D, 80-FF] (LoROM banks or upper 32kB of HiROM banks)
// xxC000-xxDFFF  IO regs       [00-7D, 80-FF]
// xxE000-xxFFFF  SWC ROM       [0-1] (core firmware)
//
//
// Mode 1 - Play cartridge
// xx0000-xx7FFF  ROM mapping [40-7D, C0-FF] (HiROM)
// xx8000-xxFFFF  ROM mapping [00-7D, 80-FF]
//
//
// Mode 2 - Cartridge emulation
// xx0000-xx7FFF  DRAM [40-70, C0-E0] (mode 21 only)
// xx8000-xxFFFF  DRAM [00-70, 80-E0]
// 708000-70FFFF  SRAM (mode 1)
// 3x6000-3x7FFF  SRAM [0-3] (mode 2)
//
//
// Mode 3 - Cartridge emulation
// xx0000-xx7FFF  DRAM [40-6F, C0-DF] (mode 21 only)
// xx8000-xxFFFF  DRAM [00-6F, 80-DF]
// 708000-70FFFF  SRAM (mode 1)
// 3x6000-3x7FFF  SRAM [0-3] (mode 2)


//**************************************************************************************************
// Defines

// Except for the mode regs, registers are available only in system mode 0.
// Registers are not accessible from banks 7E and 7F because those banks map to SNES RAM.

//IO Registers ---------------------------------

//Floppy Drive
#define	SWC_FIN		0xC000						//R  Input
#define	SWC_FDOUT	0xC002						//W  Digital output
#define	SWC_FSTAT	0xC004						//R  Status
#define	SWC_FDATA	0xC005						//RW Data
#define	SWC_FDIN	0xC007						//R  Digtal input
#define	SWC_FCTRL	0xC007						//W  Control

//Parallel Port
#define	SWC_PIN		0xC008						//R  Reverse busy flag
#define	SWC_POUT	0xC008						//W  Mapping for data writes
												//   [0] DRAM mode 0 = 20(LoROM)  1 = 21(HiROM)
												//   [1] SRAM mode 0 = 1(LoROM)  1 = 2(HiROM)
#define	SWC_PBUSY1	0xC009						//R  [7] Busy flag (EP1810 core)
#define	SWC_PBUSY2	0xC000						//R  [5] Busy flag (FC9203 core)
#define	SWC_P32M	0xC010						//W  Select 32Mb bank

//Memory Mapping -------------------------------
#define	SWC_HEADER	0x0400						//Location of .SWC header

//Page Select
// (For internal use?  Not really necessary from the PC side
// since the page number gets set with the bank.)
#define	SWC_PAGE0	0xE000						//Memory page 0
#define	SWC_PAGE1	0xE001						//Memory page 1
#define	SWC_PAGE2	0xE002						//Memory page 2
#define	SWC_PAGE3	0xE003						//Memory page 3

//System Mode Select
#define	SWC_MODE0	0xE004						//BIOS mode (power-on default)
#define	SWC_MODE1	0xE005						//Play cartridge
#define	SWC_MODE2	0xE006						//Cartridge emulation 1
#define	SWC_MODE3	0xE007						//Cartridge emulation 2

//Other registers
#define	SWC_DRAM1	0xE008						//44256 DRAM type
#define	SWC_DRAM2	0xE009						//441000 DRAM type
#define	SWC_MAP1	0xE00C						//Memory mapping
												// (Mode0) Map cartridge ROM to 2000/A000
												// (Mode2) Disable cart mapping at banks 20-5F
												// (Mode3) Disable cart mapping at banks A0-DF
#define	SWC_MAP2	0xE00D						//Memory mapping
												// (Mode0) Map cartridge SRAM to 2000/A000
												// (Mode2) Enable cart mapping at banks 20-5F
												// (Mode3) Enable cart mapping at banks A0-DF
//Super Magic Drive
#define	SMD_PAGE	0x2000						//Select cart ROM/SMD DRAM page
#define	SMD_MODE	0x2001						//Memory and mode control

#define	SMD_HEADER	0xDC00						//Location of .SMD header
#define	SMD_ROMSIZE	0xDFF0						//Cart size (in 128k units)
#define	SMD_DRAM	0xDFF1						//SMD's DRAM size (in 128k units)

//Sub-routines ---------------------------------
#define	SUB_INIT		0						//Initialize
#define	SUB_EXEC_DRAM	1						//Execute program in DRAM
#define	SUB_EXEC_CART	2						//Execute program in cartridge
#define	SUB_RESTORE		3						//Restore state after rts data has been transferred
#define	SUB_PCX			4						//Display a PCX
#define	SUB_SETSIZE		5						//Set size of program in DRAM


//Front Fareast file header mode flags ---------
#define	FF_MULTIFILE	0x40					//A split file exists after this file
#define	FF_SWC_SRAM		0x20					//Enable mode2
#define	FF_SWC_HIROM	0x10					//Enable mode21 (HIROM)
#define	FF_SWC_SRAM256	0x00					//256Kb of SRAM
#define	FF_SWC_SRAM64	0x04					//64Kb of SRAM
#define	FF_SWC_SRAM16	0x08					//16Kb of SRAM
#define	FF_SWC_SRAM00	0x0C					//No SRAM


#define	SWC_EXC_READ	0xE0000001				//Exception thrown when the SWC could not be read
												// from or written to

//**************************************************************************************************
// Structures

typedef struct
{
	void	(__stdcall *PortOut)(u16 port, u8 data);	//Sent byte to port
	u8		(__stdcall *PortIn)(u16 port);				//Read byte from port
	void	(__stdcall *NotPortBit)(u16 port, u8 bit);	//Compliment bit in port (bit = 0-7)
} IOFunc;

typedef enum
{
	HDR_SWC_PROG=4,								//SNES program data
	HDR_SWC_BBD,								//SNES battery back-up data
	HDR_SMD_PROG,								//Genesis program data
	HDR_SMD_SRAM,								//Genesis SRAM data
	HDR_SWC_RTS									//SWC real-time save data
} FFType;

//Front Fareast back-up file header
typedef struct
{
	u8	blocks[2];								//Number of 16KB blocks
	u8	mode;									//see #define FF_???
	u8	__r1[5];								//reserved
	u8	fileID1,fileID2;						//0xAA and 0xBB
	u8	fileType;								//see FFType
	u8	__r2[501];								//reserved
} FFHdr;


//**************************************************************************************************
// Initialize Super WildCard Interface
//
// Sets the LPT port to be used for communication and loads io.dll, a library used for parallel port
// access in Windows.  This function can be called multiple times to change the LPT port in use or
// to get the initialization status.
//
// In:
//    port        = LPT port (1, 2, 3, 0x378, 0x278, or 0x3BC)
//    hMainWindow = Handle to application window (needed for displaying errors in message boxes)
//    pFunc      -> User defined functions for LPT I/O (if NULL, SwcInit will try to load IO.DLL)
//
// Out:
//    true, if initialization was successful

b8 SwcInit(u32 port, IOFunc *pFunc = NULL);


//**************************************************************************************************
// Specify LPT Port
//
// Allows you to change the LPT port without reinitializing
//
// In:
//    port = LPT port (1, 2, 3, 0x378, 0x278, or 0x3BC)
//
// Out:
//    true, if port was valid

b8 SwcSetPort(u32 port);


//**************************************************************************************************
// Shutdown Super WildCard Interface
//
// Frees the IO library

void SwcShut();


//**************************************************************************************************
// Write a Byte
//
// In:
//    addr = Address in bank to write byte to
//    val  = Byte value to write

void SwcWrite(u32 addr, u8 val);


//**************************************************************************************************
// Write a Block of Data
//
// In:
//    addr  = Address in bank to write block to
//    pData-> Buffer of data to write
//    len   = Length of block

void SwcWrite(u32 addr, u8 *pData, u32 len);


//**************************************************************************************************
// Read a Byte
//
// In:
//    addr = Address in bank to read byte from
//
// Out:
//    value of byte read (0 - 255) or -1 if an error occurred

u32 SwcRead(u32 addr);


//**************************************************************************************************
// Read a Block of Data
//
// In:
//    addr  = Address in bank to read block from
//    pData-> Buffer to store data in
//    len   = Length of block
//
// Out:
//    true, if block was successfully read

b8 SwcRead(u32 addr, u8 *pData, u32 len);


//**************************************************************************************************
// Select Memory Bank
//
// Selects a bank and an 8 kilobyte page to read from/write to
//
// In:
//    bank = Bank to access (0-FFh)
//    page = 8kB page within bank (0-3)

void SwcSetBank(u8 bank, u8 page);
void SwcSetPage(u8 page);						//Alternate setting method (page = 0-255)


//**************************************************************************************************
// Call Subroutine
//
// In:
//    sub      = Subroutine (see SUB_??? defines)
//    p1,p2,p3 = Parameters

void SwcCall(u8 sub, u8 p1, u8 p2, u8 p3);


//**************************************************************************************************
// Clear Output Pins
//
// Pulls the data and control pins low

void SwcClearOut();


//**************************************************************************************************
// Get SWC's RAM Size
//
// Out:
//    The number of banks of RAM in the SWC (0 if there was an error)

u32 SwcGetRAMSize();


//**************************************************************************************************
// Get Cartridge ROM Size
//
// Returns the number of banks in a cartridge and if the cart has address line 15 connected
//
// In:
//    banks-> Variable to store number of banks in
//    a15  -> Variable to store A15 (HiROM) status in
//
// Out:
//    true, if a cart was detected in the SWC

b8 SwcGetCartSize(u8 *banks, u8 *a15);

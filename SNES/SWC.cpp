/***************************************************************************************************
* Super WildCard Interface                                                                         *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/

#if defined __BORLANDC__
#pragma inline
#endif

#include	"windows.h"
#include	"Types.h"
#include	"SWC.h"


//**************************************************************************************************
// Defines

#define	CMD_UPLOAD		0						//ROM upload
#define	CMD_DOWNLOAD	1						//ROM download
#define	CMD_VUPLOAD		2						//VRAM upload
#define	CMD_VDOWNLOAD	3						//VRAM download
#define	CMD_JUMP		4						//Force program jump
#define	CMD_SETBANK		5						//Select memory bank
#define	CMD_SUBR		6	   					//Call sub-routine (see SUB_??? defines in .h)


//**************************************************************************************************
// Variables

static HINSTANCE	HIoDLL = NULL;				//Handle to IO.DLL
static IOFunc		Io = {NULL, NULL, NULL};	//Functions for IO.DLL
static u32			Port;						//Current base hardware port for LPT
static u64			Freq;

static u8			Test[512];					//Buffer used for detecting ROM size


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Functions

// Parallel port garbage (could be useful)
//
//	p+0 Data (out)    D7-0 pins 9-2
//	p+1 Status (in)   S7-3 pins 11, 10, 12, 13, 15
//	p+2 Control (out) C3-0 pins 17, 16, 14, 1
//
//	!S7 BUSY		Printer is busy
//	 S6 -ACK		Printer received character
//	 S5 PE			Paper tray is empty
//	 S4 SELECT		Printer is on-line
//	 S3 -ERROR		An error has occurred
//
//	!C3 -SELECT		Printer is selected
//	 C2 -INIT		Initialize printer
//	!C1 -AUTO FD	Feed paper
//	!C0 -STROBE
//
//	! indicates that the state the pin is pulled to is opposite of the bit in the register
//	- indicates that the function is performed when the pin is pulled low
//
//	By default, Control is initialized to 0xC, which makes pin 17 low and pins 16, 14, and 1 high.
//	 (i.e. SELECT = ON, INIT = OFF, AUTO = OFF, STROBE = OFF)


//**************************************************************************************************
// Wait for a Specified Number of Milliseconds
//
// If the input is positive, the function waits for the specified number of milliseconds before
// returning.
//
// If the input is negative, the function stores the number of milliseconds as a positive value then
// returns immediately.
//
// If the input is 0, the function returns true if the stored amount of time has passed.
//
// Note:
//    If the system has no performance counter, the length of the wait is indeterminate and faster
//     processors will have shorter waits.
//
// In:
//    ms = Milliseconds to wait for
//
// Out:
//    ms == 0, true if the specified amount of time has passed
//    ms != 0, always true

static b8 Wait(s32 ms = 0)
{
	static LARGE_INTEGER	base,delta;
	static u32				wait;


	if (!ms)
	{
		if (Freq)
		{
			QueryPerformanceCounter(&delta);
			return (u32)((delta.QuadPart - base.QuadPart) / Freq) >= wait;
		}
		else
			return --wait;
	}
	else
	if (ms < 0)
	{
		if (Freq)
		{
			QueryPerformanceCounter(&base);
			wait = (u32)(0 - ms);
		}
		else
			wait = 0x200000;
	}
	else
	{
		Sleep(ms);
	}

	return 1;
}

//**************************************************************************************************
// Write a Byte
//
// Sends one byte to the SWC.  Because the BUSY pin is high by default, there's no way to check if
// data was successfully sent to the SWC.

static void WriteByte(u8 data)
{
	Wait(-500);
	while (!(Io.PortIn(Port+1) & 0x80))			//Wait for BUSY pin to be high (ready for data)
	{
		if (Wait())
		{										//Raise an exception to abort operation
			RaiseException(SWC_EXC_READ, EXCEPTION_NONCONTINUABLE, 0, 0);
			return;
		}
	}

	Io.PortOut(Port, data);						//Write 8-bits of data
	Io.NotPortBit(Port+2, 0);					//Reverse STROBE bit (data has been written)

	Io.PortIn(Port+1);							//Insert a bit of a delay
}


//**************************************************************************************************
// Read a Byte
//
// Read one byte from the SWC.  If the SWC doesn't give a ready signal after so many reads, an error
// message is displayed and an exception (SWC_EXC_READ) is raised so the caller can abort operation.

static u8 ReadByte()
{
	u8	data;


	//Read low nybble --------------------------
	Wait(-500);
	while (Io.PortIn(Port+1) & 0x80)			//Wait for BUSY pin to be low (ready for data)
	{
		if (Wait()) goto Error;
	}

	data = (Io.PortIn(Port+1) & 0x78) >> 3;
	Io.NotPortBit(Port+2, 0);					//Reverse STROBE bit (data has been read)

	//Read high nybble -------------------------
	Wait(-500);
	while (Io.PortIn(Port+1) & 0x80)
	{
		if (Wait()) goto Error;
	}

	data |= (Io.PortIn(Port+1) & 0x78) << 1;
	Io.NotPortBit(Port+2, 0);

	return data;

Error:											//If the SWC isn't responding, raise an exception
	RaiseException(SWC_EXC_READ, EXCEPTION_NONCONTINUABLE, 0, 0);
	return 0;
}


//**************************************************************************************************
// Send Command
//
// Sends a command (see CMD_??? defines) to the SWC
//
// In:
//   cmd  = Command
//   addr = Address
//   len  = Data length

static void SendCmd(u8 cmd, u16 addr, u16 len)
{
	u8	chk;


	chk = 0x81 ^ cmd ^ (u8)addr ^ (addr >> 8) ^ (u8)len ^ (len >> 8);

	WriteByte(0xD5);
	WriteByte(0xAA);
	WriteByte(0x96);
	WriteByte(cmd);
	WriteByte((u8)addr);
	WriteByte(addr >> 8);
	WriteByte((u8)len);
	WriteByte(len >> 8);
	WriteByte(chk);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Exported Functions

//**************************************************************************************************
// Initialize Super WildCard Interface

b8 SwcInit(u32 p, IOFunc *pFunc)
{
	s16	(__stdcall *IsDriverInstalled)();


	if (!SwcSetPort(p)) return 0;

	//Load low-level I/O library ---------------
	if (pFunc)
	{
		Io.PortOut	 	= pFunc->PortOut;
		Io.PortIn	 	= pFunc->PortIn;
		Io.NotPortBit	= pFunc->NotPortBit;
	}
	else
	if (!HIoDLL)
	{
		HIoDLL = LoadLibrary("io.dll");

		if (HIoDLL)
		{
			*(void**)&Io.PortOut	   	= (void*)GetProcAddress(HIoDLL, "PortOut");
			*(void**)&Io.PortIn		   	= (void*)GetProcAddress(HIoDLL, "PortIn");
			*(void**)&Io.NotPortBit	   	= (void*)GetProcAddress(HIoDLL, "NotPortBit");
			*(void**)&IsDriverInstalled	= (void*)GetProcAddress(HIoDLL, "IsDriverInstalled");

			if (!IsDriverInstalled())
			{
				MessageBox(GetForegroundWindow(),
							"Could not install IO driver.\n" \
							"Try disabling IO.SYS by opening a Command Prompt and entering \"net stop io.sys\"",
							"SWC Interface", MB_OK | MB_ICONERROR);

				FreeLibrary(HIoDLL);
				HIoDLL = NULL;

				return 0;
			}
		}
		else
		{
			MessageBox(GetForegroundWindow(),
						"Could not load IO.DLL\n" \
						"This file is used to communicate with your copier.\n" \
						"You can download IO.DLL from http://www.geekhideout.com",
						"SWC Interface", MB_OK | MB_ICONERROR);

			return 0;
		}
	}

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&Freq)) Freq /= 1000;
	else Freq = 0;

	return 1;
}


//**************************************************************************************************
// Specify LPT Port

b8 SwcSetPort(u32 p)
{
	//Select IO address ------------------------
	switch(p)
	{
	case(1):
	case(0x378):
		Port = 0x378;
		break;

	case(2):
	case(0x278):
		Port = 0x278;
		break;

	case(3):
	case(0x3BC):
		Port = 0x3BC;
		break;

	default:
		return 0;
	}

	return 1;
}


//**************************************************************************************************
// Shutdown Super WildCard Interface

void SwcShut()
{
	if (Io.PortOut)
	{
		Io.PortOut(Port, 0);					//Return ports to their defualt state
		Io.PortOut(Port+2, 0xC);
	}

	if (HIoDLL)
	{
		FreeLibrary(HIoDLL);
		HIoDLL = NULL;
	}
}


//**************************************************************************************************
// Write a Byte

void SwcWrite(u32 addr, u8 val)
{
	SendCmd(CMD_UPLOAD, addr, 1);
	WriteByte(val);
	WriteByte(0x81 ^ val);
}


//**************************************************************************************************
// Write a Block of Data

void SwcWrite(u32 addr, u8 *pData, u32 len)
{
	u8	chk;


	SendCmd(CMD_UPLOAD, addr, len);				//Notify SWC we're going to send data

	chk = 0x81;
	while (len--)
	{
		WriteByte(*pData);
		chk ^= *pData++;
	}

	WriteByte(chk);								//Write data checksum
}


//**************************************************************************************************
// Read a Byte

u32 SwcRead(u32 addr)
{
	u32	r;


	SendCmd(CMD_DOWNLOAD, addr, 1);
	Wait(1);

	r = ReadByte();

	if (ReadByte() != (u8)(0x81 ^ r)) r = -1;	//If checksum doesn't match, return error
	Wait(1);

	return r;
}


//**************************************************************************************************
// Read a Block of Data

b8 SwcRead(u32 addr, u8 *pData, u32 len)
{
	u8	chk;


	SendCmd(CMD_DOWNLOAD, addr, len);			//Notify SWC we want to read some data
	Wait(1);

	chk = 0x81;
	while (len--) chk ^= *pData++ = ReadByte();

	chk = (ReadByte() == chk);					//If checksums match, transfer was good
	Wait(1);

	return chk;
}


//**************************************************************************************************
// Select Memory Bank

void SwcSetBank(u8 bank, u8 page)
{
	SendCmd(CMD_SETBANK, (bank << 2) | page, 0);
}

void SwcSetPage(u8 page)
{
	SendCmd(CMD_SETBANK, page, 0);
}


//**************************************************************************************************
// Call Subroutine
//
// An alternative to calling SwcCmd(CMD_SUBR,?,?);

void SwcCall(u8 r, u8 p1, u8 p2, u8 p3)
{
	u8	chk;


	chk = 0x81 ^ CMD_SUBR ^ r ^ p1 ^ p2 ^ p3;

	WriteByte(0xD5);
	WriteByte(0xAA);
	WriteByte(0x96);
	WriteByte(CMD_SUBR);
	WriteByte(r);
	WriteByte(p1);
	WriteByte(p2);
	WriteByte(p3);
	WriteByte(chk);
}


//**************************************************************************************************
// Clear Output Pins

void SwcClearOut()
{
	Wait(1);
	Io.PortOut(Port, 0);
	Io.PortOut(Port+2, (Io.PortIn(Port+2) & 1) | 0xB);	//Preserve the state of the STROBE bit
}


//**************************************************************************************************
// Get SWC's RAM Size

// Returns true if a bank is writable
// b = Bank to test

static b8 Check(int b)
{
	u8	c;


	SwcSetBank(b,0);

	c = SwcRead(0x8000);
	SwcWrite(0x8000, ~c);
	if (SwcRead(0x8000) == c) return 0;

	SwcWrite(0x8000, c);

	return 1;
}

u32 SwcGetRAMSize()
{
	if (Check(0x76))  return  0x76;
	if (Check(0x56))  return  0x56;
	if (Check(0x36))  return  0x36;

	return 0x16;
}


//**************************************************************************************************
// Get Cartridge ROM Size

// Returns true if banks are the same
// b1,b2 = Starting banks
// i     = Number of banks to comapare

static b8 Same(u32 b1, u32 b2, u32 i)
{
	b1 <<= 2;
	b2 <<= 2;
	i <<= 2;
	do if (Test[--i+b1] != Test[i+b2]) return 0;
	while (i);

	return 1;
}

// Returns true if four consecutive banks all contain the same byte value
// b = Starting bank to test

static b8 Repeat(u32 b)
{
	u32 i;

	b <<= 2;
	for (i=b+15; i>b; i--)
		if (Test[b] != Test[i]) return 0;

	return 1;
}

// Returns true if all bytes from a bank contain the same given value
// b = Bank to test
// i = Byte value to test for

static b8 Contains(u32 b, u32 i)
{
	i |= (i << 8) | (i << 16) | (i << 24);
	if (*(u32*)&Test[b << 2] != i) return 0;

	return 1;
}

b8 SwcGetCartSize(u8 *banks, u8 *a15)
{
	u32	i,j;
	u8	*p,b;


	//Grab the byte at 00A0 in every page of LoROM
	//'test' will then be filled with four bytes from each bank
	__try
	{
		SwcWrite(SWC_MAP1, 0);					//Map ROM data

		p = Test;
		for (i=0x80; i<0x100; i++)
		{
			for (j=0; j<4; j++)
			{
				SwcSetBank(i,j);
				*p++ = SwcRead(0xA0A0);
			}
		}
	}
	__except (GetExceptionCode() == SWC_EXC_READ) {return 0;}

	if (Repeat(0x00)) return 0;					//If banks 00-03 repeat
	if (Contains(0x04, 0x84)) return 0;			//If bank 4 contains 84h

	b = 8;
	if (Same(0x00, 0x08, 8)) goto Done;			//If banks 00-07 match 08-0F

	b = 16;
	if (Same(0x00, 0x10, 8)) goto Done;			//If banks 00-07 match 10-17

	b = 24;
	if (Same(0x10, 0x18, 8)) goto Done;			//If banks 10-17 match 18-1F

	b = 32;
	if (Same(0x00, 0x20, 4)) goto Done;			//If banks 00-03 match 20-23
	if (Repeat(0x20)) goto Done;				//If banks 20-23 are the same
	if (Same(0x20, 0x24, 4)) goto Done;			//If banks 20-23 match 24-27
	if (Contains(0x20, 0xA0)) goto Done;		//If bank 20 contains A0h

	b = 40;
	if (Same(0x20, 0x28, 8)) goto Done;			//If banks 20-27 match 28-2F

	b = 48;
	if (Repeat(0x30)) goto Done;				//If banks 30-33 are the same
	if (Contains(0x30, 0xB0)) goto Done;		//If bank 30 contains B0h
	if (Same(0x20, 0x30, 8)) goto Done;			//If banks 20-27 match 30-37

	b = 64;
	if (Same(0x00, 0x40, 4)) goto Done;			//If banks 00-03 match 40-43
	if (Contains(0x40, 0xC0)) goto Done;		//If bank 40 contains C0h

	b = 72;
	if (Same(0x40, 0x48, 4)) goto Done;			//If banks 40-43 match 48-4B

	b = 80;
	if (Same(0x40, 0x50, 4)) goto Done;			//If banks 40-43 match 50-53
	if (Contains(0x50, 0xD0)) goto Done;		//If bank 50 contains D0h

	b = 96;
	if (Same(0x40, 0x60, 4)) goto Done;			//If banks 40-43 match 60-63
	if (Contains(0x60, 0xE0)) goto Done;		//If bank 60 contains E0h

	b = 112;
	if (Same(0x60, 0x70, 4))goto Done;			//If banks 60-63 match 70-73

	b = 128;
	if (Same(0x7C, 0x7C, 4)) goto Done;			//If banks 7C-7F match 7C-7F

Done:
	if (banks) *banks = b;

	//Check for HiROM cart ---------------------
	// If four bytes from 8000-FFFF match corresponding bytes from 0000-7FFF
	// the cartridge is LoROM
	if (a15)
	{
		__try
		{
			for (i=0; i<4; i++)
			{
				SwcSetBank(0x40,i);
				Test[i] = SwcRead(0x20A0);
				Test[4+i] = SwcRead(0xA0A0);
			}

			*a15 = *(u32*)&Test[0] != *(u32*)&Test[4];
		}
		__except (GetExceptionCode() == SWC_EXC_READ) {return 0;}
	}

	return 1;
}

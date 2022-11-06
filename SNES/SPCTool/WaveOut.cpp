/***************************************************************************************************
* Wave Output for the Windows Driver Model Audio Architecture                                      *
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
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#pragma	inline

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"WaveOut.h"
#include	"windows.h"
#include	"mmsystem.h"
#define		_MSC_VER	1100
#include	"ks.h"
#include	"ksmedia.h"


//**************************************************************************************************
// Local Functions

static u8*	MakeScopeS32(void *p);
static u8*	MakeScopeS16(void *p);


//**************************************************************************************************
// Global Variables

u32	visBuffer;
u32	visFrame;

//----------------------------------------------
//User Defined Callback Function
//
//In: void* -> Sample buffer (NULL if no buffers are active)
//    u32    = Size of buffer in bytes
//    u32    = Buffer being filled (0 or 1)

u32	(*WavCallback)(void*,u32,u32);
u8*	(*WavMakeScope)(void*);


//**************************************************************************************************
// Local Variables

static HWAVEOUT				hWav;				//Device handle
static WAVEOUTCAPS			cap;				//Device capabilities
static WAVEFORMATEXTENSIBLE	fmt;				//Wave format structure

static u8		open;						 	//Output device is open
static b8		paused;							//Output is paused
static b8		sleep;							//Callback should sleep
static u8		closing;						//Output device is closing

static s32 		bufSize;						//Size of buffer
static void		*pBuf;							//Double buffer used for output

static WAVEHDR 	hdr[2];							//Wave headers
static u32	   	actCnt;							//Wave headers that are actively playing

static MMTIME	mmTime;							//Used to retrieve current position in output
static s32		samples;						//Sample frames written since a reset
static u32		baseTime;						//Current time at reset

#pragma	pack(push, 8)
static u8	scope[96000 * 2 / 50];
#pragma	pack(pop)


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

//**************************************************************************************************
// Wave Out Callback
//
// Event handler called by Windows for dealing with an open wave device
//
// In:
//    hwo        = Handle to output device
//    uMsg       = Message being sent
//    dwInstance = User defined data (not used in here)
//    pWHdr     -> Current wave header (if applicable)
//    dwParam2   = not used

static void CALLBACK WaveOutDone(HWAVEOUT hwo, u32 uMsg, u32 dwInstance, WAVEHDR *pWHdr, u32 dwParam2)
{
	if (uMsg!=WOM_DONE || sleep) return;		//If a wave block isn't through playing or the
												// callback should sleep for a bit
	actCnt -= dwInstance;						//Decrease number of active buffers
	if (actCnt)
	{
		visBuffer = (pWHdr->lpData == pBuf);  	//Set current playback buffer
		visFrame = 0;						  	//Reset frame
	}

	pWHdr->dwBufferLength = WavCallback(pWHdr->lpData, bufSize, (pWHdr->lpData != pBuf));
	if (pWHdr->dwBufferLength)					//Was anything written?
	{
		waveOutWrite(hWav, pWHdr, sizeof(WAVEHDR));
		samples += pWHdr->dwBufferLength / fmt.Format.nBlockAlign;
		actCnt++;
	}
	else
	if (!actCnt) WavCallback(NULL, 0, 0);		//If nothing is playing, notify callback
}


//**************************************************************************************************
void WavInit()
{
	memset(&mmTime, 0, sizeof(MMTIME));			//Erase time structure
	mmTime.wType = TIME_SAMPLES;

	pBuf = NULL;

	sleep = 0;
	paused = 0;									//Output isn't paused
	open = 0;									//Device isn't open
	closing = 0;
}


//**************************************************************************************************
void WavReset()
{
	u32	error;


	WavStop();

	//Build format structure -------------------
	fmt.Format.wFormatTag		= WAVE_FORMAT_PCM;
	fmt.Format.nChannels		= 2;
	fmt.Format.nSamplesPerSec	= Cfg.dsp.rate;
	fmt.Format.nAvgBytesPerSec	= 2 * (abs(Cfg.dsp.bits) >> 3) * Cfg.dsp.rate;
	fmt.Format.nBlockAlign		= 2 * (abs(Cfg.dsp.bits) >> 3);
	fmt.Format.wBitsPerSample	= abs(Cfg.dsp.bits);
	fmt.Format.cbSize			= 0;

	error = waveOutOpen(&hWav, Cfg.out.device, (tWAVEFORMATEX*)&fmt, 0, 0, CALLBACK_NULL);
//	error = 32;

	if (!error)									//If device opened, close it
	{
		waveOutClose(hWav);
		hWav = 0;
	}
	else
	if (error == WAVERR_BADFORMAT)				//Try extensible format for WDM
	{
		fmt.Format.wFormatTag			= WAVE_FORMAT_EXTENSIBLE;
		fmt.Format.cbSize				= 22;
		fmt.Samples.wValidBitsPerSample	= abs(Cfg.dsp.bits);
		fmt.dwChannelMask				= 0;	//Use Format.nChannels
		fmt.SubFormat					= KSDATAFORMAT_SUBTYPE_PCM;
	}

	bufSize = (abs(Cfg.dsp.bits) * Cfg.dsp.rate * 2 * Cfg.out.size) / (8 * 1000); //8-bits * 1 second
	pBuf = realloc(pBuf, (bufSize * 2) + 1024);	//Add 1k for any buffer overflow

	switch (abs(Cfg.dsp.bits))
	{
	case(32):
		WavMakeScope = MakeScopeS32;
		break;

	case(16):
		WavMakeScope = MakeScopeS16;
		break;
	}
}


//**************************************************************************************************
void WavQuit()
{
	WavStop();									//Close output device, incase someone forgot to
	free(pBuf);									//Free up allocated memory
}


//**************************************************************************************************
b8 WavStart()
{
	s8	str[256];
	s32	error;


	if (open)
	{
		sleep = 1;
		waveOutReset(hWav);
		actCnt = 0;
		sleep = 0;
		goto Restart;
	}

	//Open audio device ------------------------
	error = waveOutOpen(&hWav, Cfg.out.device, (tWAVEFORMATEX*)&fmt,
						(u32)WaveOutDone, 1, CALLBACK_FUNCTION|WAVE_ALLOWSYNC|Cfg.out.force);
	switch (error)
	{
	case(MMSYSERR_NOERROR):
		//Initialize wave blocks ---------------
		memset(&hdr[0], 0, sizeof(WAVEHDR));
		hdr[0].dwBufferLength = bufSize;
		hdr[0].lpData = (s8*)pBuf;
		waveOutPrepareHeader(hWav, &hdr[0], sizeof(WAVEHDR));
		hdr[0].dwUser = 0;

		memset(&hdr[1], 0, sizeof(WAVEHDR));
		hdr[1].dwBufferLength = bufSize;
		hdr[1].lpData = (s8*)(((u32)pBuf + bufSize + 512) & ~31);
		waveOutPrepareHeader(hWav, &hdr[1], sizeof(WAVEHDR));
		hdr[1].dwUser = 1;

		memset(&mmTime, 0, sizeof(MMTIME));
		mmTime.wType = TIME_SAMPLES;
		samples = 0;
		baseTime = 0;

Restart:
		paused = 0;								//Output isn't paused
		open = 1;								//Device is open

		WaveOutDone(hWav, WOM_DONE, 0, &hdr[0], 0);
		WaveOutDone(hWav, WOM_DONE, 0, &hdr[1], 0);
		return 1;

	case(MMSYSERR_BADDEVICEID):
		sprintf(str,"Invalid output device ID [%i].  Check your configuration.", Cfg.out.device);
		MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	case(MMSYSERR_ALLOCATED):
		sprintf(str,"%s is already in use", cap.szPname);
		MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	case(MMSYSERR_NODRIVER):
		sprintf(str,"No driver is loaded for %s",cap.szPname);
		MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	case(MMSYSERR_NOMEM):
		MessageDlg("Unable to allocate memory for output device", mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	case(MMSYSERR_INVALFLAG):
		MessageDlg("Invalid flag parameter.  (Try disabling ""Force direct output"")", mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	case(WAVERR_BADFORMAT):
		MessageDlg("The extensible format is not supported by your driver.  Get the WDM drivers for your soundcard.", mtError, TMsgDlgButtons() << mbOK, 0);
		break;

	default:
		sprintf(str,"Error [%i] trying to open output device %s",error,cap.szPname);
		MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
	}
// Taken from mmsystem.h
//  1 MMSYSERR_ERROR		unspecified error
//  2 MMSYSERR_BADDEVICEID	device ID out of range
//  3 MMSYSERR_NOTENABLED	driver failed enable
//  4 MMSYSERR_ALLOCATED	device already allocated
//  5 MMSYSERR_INVALHANDLE	device handle is invalid
//  6 MMSYSERR_NODRIVER		no device driver present
//  7 MMSYSERR_NOMEM		memory allocation error
//  8 MMSYSERR_NOTSUPPORTED	function isn't supported
//  9 MMSYSERR_BADERRNUM	error value out of range
// 10 MMSYSERR_INVALFLAG	invalid flag passed
// 11 MMSYSERR_INVALPARAM	invalid parameter passed
// 12 MMSYSERR_HANDLEBUSY	handle being used simultaneously on another thread (eg callback)
// 32 WAVERR_BADFORMAT		unsupported wave format
// 33 WAVERR_STILLPLAYING	still something playing
// 34 WAVERR_UNPREPARED		header not prepared
// 35 WAVERR_SYNC			device is synchronous

	return 0;
}


//**************************************************************************************************
void WavStop()
{
	s32	i;


	if (!(open || closing)) return;				//If device is already closed

	open = 0;

	if (closing)								//If device is being closed by another thread
	{
		while (closing) Sleep(0);				//Wait for device to be closed
		return;
	}

	//Close output handle ----------------------
	closing = 1;
	sleep = 1;

	waveOutReset(hWav);							//Tell Windows to reset the output device
	waveOutUnprepareHeader(hWav, &hdr[0], sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWav, &hdr[1], sizeof(WAVEHDR));
	waveOutClose(hWav);							//Close output device

	actCnt = 0;
	hWav = 0;

	sleep = 0;
	closing = 0;
	paused = 0;
}


//**************************************************************************************************
// Set Pause State

b8 WavPause(b8 state)
{
	b8	ret;


	ret = paused;

	if (open && paused != state)
	{
		if (state)
		{
			waveOutPause(hWav);
			paused = 1;
		}
		else
		{
			waveOutRestart(hWav);
			paused = 0;
		}
	}

	return ret;
}


//**************************************************************************************************
// Flush Output Device

void WavFlush()
{
	if (!open) return;

	samples	= 0;

	sleep = 1;
	waveOutReset(hWav);							//Reset output device
	actCnt = 0;
	visFrame = -1;
	sleep = 0;
	paused = 0;
}


//**************************************************************************************************
// Restart Ouput Device After a Flush

void WavRestart(u32 time)
{
	if (!open) return;

	baseTime = time;
	WaveOutDone(hWav, WOM_DONE, 0, &hdr[0], 0);
	WaveOutDone(hWav, WOM_DONE, 0, &hdr[1], 0);
}


//**************************************************************************************************
// Get Time Output by Device

u32 WavPos()
{
	if (!open) return 0;

	waveOutGetPosition(hWav, &mmTime, sizeof(MMTIME));

	return mmTime.u.sample;
}

s32 WavTime()
{
	if (!open) return 0;

	waveOutGetPosition(hWav, &mmTime, sizeof(MMTIME));

	return MulDiv(64000, mmTime.u.sample, fmt.Format.nSamplesPerSec) + baseTime;
}


//**************************************************************************************************
// Check if Output Device is Open

b8 IsWavOpen()
{
	return open;
}


//**************************************************************************************************
u8* MakeScopeS32(void *p)
{
	u32	xd,xs;
	u32	rate = Cfg.dsp.rate;

	_asm
	{
		mov		eax,65536
		mul		[rate]
		mov		ecx,416 * 50
		div		ecx
		mov		[xd],eax
		mov		[xs],0

		mov		esi,[p]
		mov		edi,offset scope
		mov		ecx,416
	NextS32:
		mov		eax,[esi]
		mov		edx,[4+esi]
		sar		eax,1
		sar		edx,1
		add		eax,edx
		sar		eax,26
		neg		eax
		add		eax,31
		mov		[edi],al
		inc		edi

		mov		ebx,[xs]
		add		ebx,[xd]
		mov		word ptr [xs],bx
		shr		ebx,16
		lea		esi,[ebx*8+esi]

		dec		ecx
		jnz		short NextS32
	}

	return scope;
}


u8* MakeScopeS16(void *p)
{
	u32	xd,xs;
	u32	rate = Cfg.dsp.rate;

	_asm
	{
		mov		eax,65536
		mul		[rate]
		mov		ecx,416 * 50
		div		ecx
		mov		[xd],eax
		mov		[xs],0

		mov		esi,[p]
		mov		edi,offset scope
		mov		ecx,416
	NextS16:
		movsx	eax,word ptr [esi]
		movsx	edx,word ptr [2+esi]
		add		eax,edx
		sar		eax,11
		neg		eax
		add		eax,31
		mov		[edi],al
		inc		edi

		mov		ebx,[xs]
		add		ebx,[xd]
		mov		word ptr [xs],bx
		shr		ebx,16
		lea		esi,[ebx*4+esi]

		dec		ecx
		jnz		short NextS16
	}

	return scope;
}


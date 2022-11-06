/***************************************************************************************************
* Form: File Renamer                                                                               *
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#include	<vcl.h>
#include	"SPCTool.h"
#pragma hdrstop
#include	<math.h>
#include	"A2Math.h"
#include	"A2RIFF.h"	
#include	"Progress.h"
#include	"MIDI.h"

#pragma	package(smart_init)
#pragma	resource "*.dfm"
#pragma	inline


//**************************************************************************************************
// Structures

typedef struct
{
	SubChk	chk;
	u16		type;
	u16		tracks;
	u16		ppqn;
} MThd;

typedef struct
{
	SubChk	chk;
	s8		data[];
} MTrk;


//**************************************************************************************************
// Data and Variables

static DSPDebug	*OldDSPDbg;
static RegLog	*PLog;
static s8		LastReg;
static u8		EchoVol;
static f32		VolQuant;

TFrmMIDI	*FrmMIDI;


static void LogMIDI();

//**************************************************************************************************
__fastcall TFrmMIDI::TFrmMIDI(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmMIDI::FormShow(TObject *Sender)
{
	rdoT0->Enabled = Apu.ram[0xF1] & 0x1;
	rdoT1->Enabled = Apu.ram[0xF1] & 0x2;
	rdoT2->Enabled = Apu.ram[0xF1] & 0x4;
	
	trkTempoChange(NULL);
}


//**************************************************************************************************
void __fastcall TFrmMIDI::trkTempoChange(TObject *Sender)
{
	u32	mult;
	u32	t0,t1,t2;


	mult = trkTempo->Position;
	if (!mult) mult = 1;
	t0 = Apu.ram[0xFA];
	if (!t0) t0 = 0x100;
	t1 = Apu.ram[0xFB];
	if (!t1) t1 = 0x100;
	t2 = Apu.ram[0xFC];
	if (!t2) t2 = 0x100;

	rdoT0->Caption = (60 * 8000) / (mult * t0);
	rdoT1->Caption = (60 * 8000) / (mult * t1);
	rdoT2->Caption = (60 * 64000) / (mult * t2);
	rdoUser->Caption = mult;
}


//**************************************************************************************************
void __fastcall TFrmMIDI::btnOKClick(TObject *Sender)
{
	void	*pBase,*pFile;
	HANDLE	th;
	u32		tID;
	s32		fh;
	s8		str[256];


	if (!FrmProg) Application->CreateForm(__classid(TFrmProg), &FrmProg);

	pBase = malloc(4 << 20);
	pFile = malloc(4 << 20);
	PLog = (RegLog*)pBase;
	LastReg = 0xFF;

	th = (HANDLE)CreateThread(NULL, 512, (LPTHREAD_START_ROUTINE)LogMIDI, NULL, 0, (LPDWORD)&tID);
	FrmProg->ShowTotal = true;
	FrmProg->AutoClose = false;
	FrmProg->Caption = "Logging DSP...";
	FrmProg->ShowModal();
	CloseHandle(th);

	strcpy(str, pCurAPU->id6.file);
	strcpy(ScanStrR(str, '.'), "mid");
	fh = FileCreate(str);
	FileWrite(fh, pFile, CreateMIDI((s8*)pFile, (s8*)pBase));
	FileClose(fh);

	free(pFile);
	free(pBase);

	Close();
}


//**************************************************************************************************
// Log DSP Register Writes
//
// DSP debugging vector that records writes to the registers to later be converted into MIDI data

static void LogDSP(volatile u8 *reg, volatile u8 val)
{
	DSPVoice	*pVoice;
	u8			v,i;


//	_CallDSPDebug(OldDSPDbg, reg, val);			//Call previous vector

	//Check for writes to the global registers
	switch((u8)reg)
	{
	case(offsetof(DSPReg, kon)):
	case(offsetof(DSPReg, kof)):
		if (!val) return;

		PLog->time = Apu.GetSPCTime();
		PLog->reg = (u8)reg;
		PLog->flags = val;

		LastReg = (u8)reg;

		if ((u8)reg == offsetof(DSPReg, kon))
		{
			v = val;
			i = 0;
			pVoice = Apu.dsp->voice;
			while (v)
			{
				if (v & 1)
				{
					PLog->voice[i].volL = abs(pVoice->volL);
					PLog->voice[i].volL = abs(pVoice->volR);
					PLog->voice[i].pitch = pVoice->pitch;
					PLog->voice[i].srcn = pVoice->srcn;
				}

				v >>= 1;
				pVoice++;
				i++;
			}
		}
		_IncPtr(PLog, 6 + (5 * i));
		return;

	case(offsetof(DSPReg, eon)):
		if (*reg == val) return;

		PLog->time = Apu.GetSPCTime();
		PLog->reg = (u8)reg;
		PLog->flags = val;
		_IncPtr(PLog, 6);

		LastReg = (u8)reg;
		return;

	default:
		if ((u8)reg & 0xF >= offsetof(DSPVoice, envx)) return;	//All other global regs return
	}

	if (*reg == val) return;

	//Check for writes to the voice registers --
	if (Apu.voice[(u8)reg >> 4].mFlg & 0x8) return;	//Is the current voice keyed on?

	switch((u8)reg & 0xF)
	{
	case(offsetof(DSPVoice, volL)):
	case(offsetof(DSPVoice, volR)):
		i = ((u8)reg & 0xF0) | offsetof(DSPVoice, volL);

		if (i != LastReg && (i+1) != LastReg)
		{
			PLog->time = Apu.GetSPCTime();
			PLog->reg = i;
			_IncPtr(PLog, 7);
		}
		LastReg = (u8)reg;

		((u8*)&PLog)[-2] = abs(Apu.dsp->voice[i >> 4].volL);
		((u8*)&PLog)[-1] = abs(Apu.dsp->voice[i >> 4].volR);
		return;

	case(offsetof(DSPVoice, pitch)):
	case(offsetof(DSPVoice, pitch)+1):
		i = ((u8)reg & 0xF0) | offsetof(DSPVoice, pitch);

		if (i != LastReg && (i+1) != LastReg)
		{
			PLog->time = Apu.GetSPCTime();
			PLog->reg = i;
			_IncPtr(PLog, 7);
		}
		LastReg = (u8)reg;

		((u16*)&PLog)[-1] = Apu.dsp->voice[i >> 4].pitch;
		return;
	}
}


//**************************************************************************************************
// Thread to convert log data into MIDI

static void LogMIDI()
{
	APUFile	*p;
	u32		i,total;
	s32		fh;

	u32		min,max,last;
	u64		avg;
	u8		v;
	s8		str[64];


	p = FrmMIDI->pCurAPU;

	OldDSPDbg = Apu.SetDSPDbg(LogDSP, 0);

	//Set DSP to SNES compatibilty -------------
	Apu.SetAPUOpt(MIX_INT, 2, 16, 32000, INT_GAUSS, 0);
	Apu.SetAPUSmpClk(0x10000);
	Apu.SetDSPPitch(32000);
	Apu.SetDSPStereo(0x8000);

	//Reset APU --------------------------------
	Apu.LoadSPCFile(&p->spc);
	Apu.SetDSPAmp(0x10000);

	Apu.SetDSPLength(p->id6.GetSong(), p->id6.GetFade());
	total = p->id6.GetTotal();

	FrmProg->BarCurrent->Reset(total);

	//Log DSP ----------------------------------
	EchoVol = ((f64)(abs(Apu.dsp->evolL) + abs(Apu.dsp->evolR)) /
			   (f64)(abs(Apu.dsp->mvolL) + abs(Apu.dsp->mvolR))) * 127.0;

	LogDSP(&Apu.dsp->kon, Apu.dsp->kon);
	do
	{
		Apu.EmuAPU(NULL, APU_CLK, 0);
		FrmProg->BarCurrent->Set(Apu.GetSPCTime());
	} while(Apu.GetSPCTime() < total);

	*(u32*)PLog = -1;

	//Restore DSP options ----------------------
	Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());
	Apu.SetDSPStereo(Cfg.dsp.stereo);

	Apu.SetDSPDbg(OldDSPDbg, 0);

	FrmProg->ModalResult = mrOk;				//Close progress dialog
}


//**************************************************************************************************
// Convert a 28-bit integer into a variable length quantity
//
// 28-bit delta and length values are saved as a variable length
//
// In:
//    pVar -> Location to store var len
//    val   = 28-bit value to split up
//
// Out:
//    pVar + length of variable

#pragma warning(disable : 4035)
s8* __fastcall Int2Var(s8 *pVar, u32 val)
{
	_asm
	{
		mov		ecx,eax
		xor		eax,eax
		mov		ebx,1
	NextVar:
		mov		al,dl
		and		al,0x7F
		or		al,bh
		shr		edx,7
		jz		short DoneVar
		shl		eax,8
		mov		bh,0x80
		inc		bl
		jmp		short NextVar

	DoneVar:
		mov		bh,0
		mov		[ecx],eax
		lea		eax,[ebx+ecx]
	}
}
#pragma warning(default : 4035)


//**************************************************************************************************
// Convert left and right volume into single volume and pan
//
// How DSP left and right volumes are converted into MIDI volume and pan:
//         _____________
//        |    2      2
//        |  L      R
//  V = _ | ---  + ---
//       \| 128    128
//
//         2
//       R
//  P = ---
//       V
//
//  volume = sqrt(V) * 127
//  pan = P * 127
//
// According to the DLS specification, the final left and right volumes will never be greater than
// -3dB with a center pan (because of the attenuation caused by the panning algorithm).  However,
// in the SNES it is possible to write 128,128 to the left and right volumes.  Doing so will create
// a vol/pan of 180/64, which is not possible using the 7-bit numbers in MIDI.
//
// In:
//    pVol -> Variable to store volume (0-127)
//    pPan -> Variable to store pan (0-127)
//    left  = Left volume (0-128)
//    right = Right volume (0-128)

void __fastcall LR2VP(s8 *pVol, s8 *pPan, u8 left, u8 right)
{
	f32	v,p,l,r;


	if (left == right)							//Center pan
	{
		*pPan = 64;
		v = left / 128.0;
		v = (v * v);
		*pVol = sqrt(sqrt(v + v)) * VolQuant;
	}
	else
	if (!(left && right))						//Hard pan left or right
	{
		if (left)
		{
			*pPan = 0;
			*pVol = F2I(sqrt(left / 128.0) * VolQuant);
		}
		else
		{
			*pPan = 127;
			*pVol = F2I(sqrt(right / 128.0) * VolQuant);
		}
	}
	else
	{
		l = left / 128.0;
		r = right / 128.0;

		v = sqrt((l * l) + (r * r));
		p = r / v;

		*pPan = (p * p) * 127.0;
		*pVol = sqrt(v) * VolQuant;
	}
}


//**************************************************************************************************

#define	_SaveDelta(time) \
			pMidi = Int2Var(pMidi, (time - lastTime) / tpp); \
			lastTime += ((time - lastTime) / tpp) * tpp;

u32 __fastcall TFrmMIDI::CreateMIDI(s8 *pMidi, s8 *pData)
{
	MThd	*pHdr;
	MTrk	*pTrk;
	u8		c,m,v,i,rs;							//Channel, mask, voice, index, running status
	u8		srcn,prog,note,vol,pan;
	u8		drum;
	u8		notes[8];
	s16		volLR;
	s16		volume[8];
	u32		lastTime,tpp,ppqn;
	RegLog	*pLog;


	//Clocks -----------------------------------
	ppqn = rdoUser->Checked ? 96 : trkTempo->Position;		//Pulses Per Quarter Note

	if (rdoT0->Checked) tpp = pCurAPU->spc.ram[0xFA] << 3;	//64kHz ticks per pulse
	else
	if (rdoT1->Checked) tpp = pCurAPU->spc.ram[0xFB] << 3;
	else
	if (rdoT2->Checked) tpp = pCurAPU->spc.ram[0xFC];
	else
	if (rdoUser->Checked) tpp = (60 * 64000) / (96 * trkTempo->Position);

	//MIDI file header -------------------------
	pHdr = (MThd*)pMidi;
	pMidi += 14;

	pHdr->chk.id	= 'MThd';
	pHdr->chk.size	= ByteSwap32(6);
	pHdr->type		= ByteSwap16(1);
	pHdr->tracks	= ByteSwap16(9);
	pHdr->ppqn		= ByteSwap16(ppqn);

	//First track header -----------------------
	pTrk = (MTrk*)pMidi;
	pMidi += 8;

	pTrk->chk.id = 'MTrk';

	pMidi[0] = 0x00;							//First event is a tempo meta event
	pMidi[1] = 0xFF;							//Convert quarter note length from 64kHz ticks to
	pMidi[2] = 0x51;							// microseconds
	*(u32*)&pMidi[3] = ByteSwap32(MulDiv(ppqn * tpp, 1000000, 64000));
	pMidi[3] = 0x03;
	pMidi += 7;


	//==========================================
	// Search for Maximum Volume

	vol = 0;
	VolQuant = 127.0f;

	pLog = (RegLog*)pData;
	while (pLog->time != -1)
	{
		switch(pLog->reg)
		{
		case(offsetof(DSPReg, kon)):
			m = pLog->flags;
			i = v = 0;

			do
			{
				if (m & 1)
				{
					if (!(pCurAPU->src[pLog->voice[v].srcn].prog & 0x80))
					{
						note |= (1 << i);

						LR2VP(&c, &pan, pLog->voice[v].volL, pLog->voice[v].volR);
						if (c > vol) vol = c;					
					}

					v++;
				}

				i++;
				m >>= 1;
			} while (m);

			_IncPtr(pLog, 6 + (BitCount(pLog->flags) * 5));
			continue;

		case(offsetof(DSPReg, kof)):
			note &= ~pLog->flags;

		case(offsetof(DSPReg, eon)):
			_IncPtr(pLog, 6);
			continue;
		}

		switch(pLog->reg & 0xF)
		{
		case(offsetof(DSPVoice, volL)):
			if (note & (1 << (pLog->reg & 0xF)))
			{
				LR2VP(&v, &pan, pLog->volL, pLog->volR);
				if (v > vol) vol = v;
			}

		case(offsetof(DSPVoice, pitch)):
			_IncPtr(pLog, 7);
			continue;
		}
	}

	VolQuant = floor(126.0 * 127.0 / vol);

	//==========================================
	// Create MIDI Tracks

	for (c=0; c<8; c++)
	{
		m = 1 << c;

		rs = 0;
		prog = 0;
		note = 60;
		drum = 0;
		volLR = 0;
		lastTime = 0;

		pLog = (RegLog*)pData;
		while (pLog->time != -1)
		{
			switch(pLog->reg)
			{
			case(offsetof(DSPReg, kon)):
				if (pLog->flags & m)
				{
					v = BitCount(pLog->flags & (m-1));
					srcn = pLog->voice[v].srcn;

					if (pCurAPU->src[srcn].prog & 0x80)
					{
						drum |= m;
					}
					else
					{
						drum &= ~m;

						if (prog != pCurAPU->src[srcn].prog)
						{
							_SaveDelta(pLog->time);

							prog = pCurAPU->src[srcn].prog;

							if (rs != 0xC0)
							{
								*pMidi++ = 0xC0 | c;
								rs = 0xC0;
							}
							*pMidi++ = prog;
						}

						if (volLR != *(s16*)&pLog->voice[v].volL)
						{
							_SaveDelta(pLog->time);

							volLR = *(s16*)&pLog->voice[v].volL;

							LR2VP(&vol, &pan, pLog->voice[v].volL, pLog->voice[v].volR);

							if (rs != 0xB0)
							{
								*pMidi++ = 0xB0 | c;
								rs = 0xB0;
							}
							pMidi[0] = 0x0B;
							pMidi[1] = vol;
							pMidi[2] = 0;
							pMidi[3] = 0x0A;
							pMidi[4] = pan;
							pMidi += 5;
						}

						_SaveDelta(pLog->time);

						note = 60 + F2I(Logx(1.05946309436,
											(f32)((pLog->voice[v].pitch * 32000) >> 12) /
											(s32)pCurAPU->src[pLog->voice[v].srcn].rate));

						if (rs != 0x90)
						{
							*pMidi++ = 0x90 | c;
							rs = 0x90;
						}
						pMidi[0] = note;
						pMidi[1] = 0x40;
						pMidi += 2;
					}
				}

				_IncPtr(pLog, 6 + (BitCount(pLog->flags) * 5));
				continue;

			case(offsetof(DSPReg, kof)):
				if (pLog->flags & ~drum & m)
				{
					_SaveDelta(pLog->time);

					if (rs != 0x90)
					{
						*pMidi++ = 0x90 | c;
						rs = 0x90;
					}
					pMidi[0] = note;
					pMidi[1] = 0;
					pMidi += 2;
				}

				_IncPtr(pLog, 6);
				continue;

			case(offsetof(DSPReg, eon)):
				if (~drum & m)
				{
					_SaveDelta(pLog->time);

					if (rs != 0xB0)
					{
						*pMidi++ = 0xB0 | c;
						rs = 0xB0;
					}

					pMidi[0] = 0x5B;
					pMidi[1] = (pLog->flags & m) ? EchoVol : 0;
					pMidi += 2;
				}

				_IncPtr(pLog, 6);
				continue;
			}

			switch(pLog->reg & 0xF)
			{
			case(offsetof(DSPVoice, volL)):
				if (!(drum & m) && volLR != *(s16*)&pLog->voice[v].volL)
				{
					_SaveDelta(pLog->time);

					volLR = *(s16*)&pLog->voice[v].volL;

					LR2VP(&vol, &pan, pLog->voice[v].volL, pLog->voice[v].volR);

					if (rs != 0xB0)
					{
						*pMidi++ = 0xB0 | c;
						rs = 0xB0;
					}
					pMidi[0] = 0x0B;
					pMidi[1] = vol;
					pMidi[2] = 0;
					pMidi[3] = 0x0A;
					pMidi[4] = pan;
					pMidi += 5;
				}

				_IncPtr(pLog, 7);
				continue;

			case(offsetof(DSPVoice, pitch)):
				if (~drum & m)
				{
				}

				_IncPtr(pLog, 7);
				continue;
			}
		}

		//End track ----------------------------
		*pMidi++ = 0x00;						//End of track meta event
		*pMidi++ = 0xFF;
		*pMidi++ = 0x2F;
		*pMidi++ = 0x00;
												//Track length
		pTrk->chk.size = ByteSwap32((u32)pMidi - (u32)pTrk - 8);

		//Prepare next track -------------------
		pTrk = (MTrk*)pMidi;
		pTrk->chk.id = 'MTrk';
		pMidi += 8;
	}


	//==========================================
	// Create drum track

	rs = 0;
	memset(notes, 0, 8);
	drum = 0;
	lastTime = 0;

	pLog = (RegLog*)pData;
	while (pLog->time != -1)
	{
		switch(pLog->reg)
		{
		case(offsetof(DSPReg, kon)):
			m = pLog->flags;
			v = i = 0;

			do
			{
				if (m & 1)
				{
					srcn = pLog->voice[v].srcn;

					if (pCurAPU->src[srcn].prog & 0x80)
					{
						drum |= 1 << i;

						_SaveDelta(pLog->time);

						notes[i] = pCurAPU->src[srcn].prog & 0x7F;
						LR2VP(&vol, &pan, pLog->voice[v].volL, pLog->voice[v].volR);

						if (rs != 0x90)
						{
							*pMidi++ = 0x99;
							rs = 0x90;
						}
						pMidi[0] = notes[i];
						pMidi[1] = 0x40;//vol;
						pMidi += 2;

					}
					else
					{
						drum &= ~(1 << i);
					}

					v++;
				}

				i++;
				m >>= 1;
			} while(m);

			_IncPtr(pLog, 6 + (BitCount(pLog->flags) * 5));
			continue;

		case(offsetof(DSPReg, kof)):
			m = pLog->flags & drum;
			i = 0;

			while (m)
			{
				if (m & 1)
				{
					_SaveDelta(pLog->time);

					if (rs != 0x90)
					{
						*pMidi++ = 0x99;
						rs = 0x90;
					}
					pMidi[0] = notes[i];
					pMidi[1] = 0;
					pMidi += 2;
				}

				i++;
				m >>= 1;
			}

			_IncPtr(pLog, 6);
			continue;

		case(offsetof(DSPReg, eon)):
			_IncPtr(pLog, 6);
			continue;
		}

		switch(pLog->reg & 0xF)
		{
		case(offsetof(DSPVoice, volL)):
		case(offsetof(DSPVoice, pitch)):
			_IncPtr(pLog, 7);
			continue;
		}
	}

	//End track --------------------------------
	*pMidi++ = 0x00;							//End of track meta event
	*pMidi++ = 0xFF;
	*pMidi++ = 0x2F;
	*pMidi++ = 0x00;
												//Track length
	pTrk->chk.size = ByteSwap32((u32)pMidi - (u32)pTrk - 8);

	return ((u32)pMidi - (u32)pHdr);
}

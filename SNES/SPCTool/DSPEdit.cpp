/***************************************************************************************************
* Form: DSP Register Editor                                                                        *
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#pragma inline

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"A2Math.h"
#include	"Debug.h"
#include	"DSPEdit.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


//**************************************************************************************************
// Data and Variables

static const s32	envRate[32] = {				//Table used for generating envelopes
		   0,2048, 1536,1280,1024, 768, 640, 512,
		 384, 320,  256, 192, 160, 128,  96,  80,
		  64,  48,   40,  32,  24,  20,  16,  12,
		  10,   8,    6,   5,   4,   3,   2,   1};

static const u32	sust0[] = {256,383,469,532,584,626,663,695};
static const u32	sust1[] = {256,383,469,532,583,626,663,691};
static const u32	dec0[] = {439,312,226,163,111,69,32,0};
static const u32	dec1[] = {435,308,222,159,108,65,28,0};

static u32	level;								//Current sustain level set in ADSR[1]
static s32	attack,decay,sustain,release;		//Current ADSR length in 32kHz samples
static u16	modified[8];						//Flags for regs that have been modified by the DSP
static u16	override[8];						//Flags for regs that have been modified by the user
static u8	noUpdate;							//Update DSP register with value from control
static b8	keyHandled;

static DSPDebug		*oldDebug;

static TTrackBar*	trkFIR[8];
static TLabel*		lblFIR[8];
static TLabel*		lblVol[8];

TFrmDSP *FrmDSP;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

void __fastcall HotKey(WORD key, TShiftState shift);


//**************************************************************************************************
// DSP Debugging Function
//
// Called every time the DSP data register is written to.  Used to determine if a register's
// corresponding control in the editor should be updated.  Also used to prevent the DSP from
// processing a register that the user has overridden.

static void DebugDSP(volatile u8 *reg, volatile u8 val)
{
	u32	rh,rl;

	
	if (oldDebug)
	{
		_CallDSPDebug(oldDebug, reg, val);
	}

	if ((u8)reg > 0x7F) return;					//If register is invalid, return

	rh = (u8)reg >> 4;							//Split up the register index into its two nybbles
	rl = (u8)reg & 0xF;

	if (override[rh] & (1 << rl))				//Check if the reg has been modified by the user
	{
		*(u8*)(&reg) = 0x80;					//Return an invalid register for the DSP to process
		return;
	}

	if (*reg != val) modified[rh] |= (1 << rl);	//Mark register as being modified
}


//**************************************************************************************************
// Write a Value to a DSP Register

static void WriteDSP(u32 reg, u32 val)
{
	override[reg >> 4] |= 1 << (reg & 0xF);		//Flag that the reg has been modified by the user
	Apu.SetDSPReg(reg, val);
}


//**************************************************************************************************
__fastcall TFrmDSP::TFrmDSP(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormCreate(TObject *sender)
{
	HelpFile = Cfg.hlp;
	
	txtVolL->HelpContext	= HDSP_VOLL;
	txtVolR->HelpContext	= HDSP_VOLR;
	txtPitch->HelpContext	= HDSP_P;
	txtSrc->HelpContext		= HDSP_SRCN;
	txtEnvX->HelpContext	= HDSP_ENVX;
	txtOutX->HelpContext	= HDSP_OUTX;

	chkADSR->HelpContext	= HDSP_ADSR;
	trkAR->HelpContext		= HDSP_Attack;
	trkDR->HelpContext		= HDSP_Decay;
	trkSL->HelpContext		= HDSP_Level;
	trkSR->HelpContext		= HDSP_Sust;

	rdoDirect->HelpContext	= HDSP_Direct;
	rdoInc->HelpContext		= HDSP_Inc;
	rdoBent->HelpContext	= HDSP_Bent;
	rdoDec->HelpContext		= HDSP_Dec;
	rdoExp->HelpContext		= HDSP_Exp;
	trkGain->HelpContext	= HDSP_GAIN;

	chkRES->HelpContext		= HDSP_RES;
	chkMUTE->HelpContext	= HDSP_MUTE;
	chkECEN->HelpContext	= HDSP_ECEN;
	trkNCK->HelpContext		= HDSP_NCK;

	btnKON0->HelpContext	= HDSP_KON;
	btnKON1->HelpContext	= HDSP_KON;
	btnKON2->HelpContext	= HDSP_KON;
	btnKON3->HelpContext	= HDSP_KON;
	btnKON4->HelpContext	= HDSP_KON;
	btnKON5->HelpContext	= HDSP_KON;
	btnKON6->HelpContext	= HDSP_KON;
	btnKON7->HelpContext	= HDSP_KON;
	btnKOF0->HelpContext	= HDSP_KOF;
	btnKOF1->HelpContext	= HDSP_KOF;
	btnKOF2->HelpContext	= HDSP_KOF;
	btnKOF3->HelpContext	= HDSP_KOF;
	btnKOF4->HelpContext	= HDSP_KOF;
	btnKOF5->HelpContext	= HDSP_KOF;
	btnKOF6->HelpContext	= HDSP_KOF;
	btnKOF7->HelpContext	= HDSP_KOF;

	btnENDX->HelpContext	= HDSP_ENDX;
	chkEON0->HelpContext	= HDSP_EON;
	chkEON1->HelpContext	= HDSP_EON;
	chkEON2->HelpContext	= HDSP_EON;
	chkEON3->HelpContext	= HDSP_EON;
	chkEON4->HelpContext	= HDSP_EON;
	chkEON5->HelpContext	= HDSP_EON;
	chkEON6->HelpContext	= HDSP_EON;
	chkEON7->HelpContext	= HDSP_EON;
	chkPMON0->HelpContext	= HDSP_PMON;
	chkPMON1->HelpContext	= HDSP_PMON;
	chkPMON2->HelpContext	= HDSP_PMON;
	chkPMON3->HelpContext	= HDSP_PMON;
	chkPMON4->HelpContext	= HDSP_PMON;
	chkPMON5->HelpContext	= HDSP_PMON;
	chkPMON6->HelpContext	= HDSP_PMON;
	chkPMON7->HelpContext	= HDSP_PMON;
	chkNON0->HelpContext	= HDSP_NON;
	chkNON1->HelpContext	= HDSP_NON;
	chkNON2->HelpContext	= HDSP_NON;
	chkNON3->HelpContext	= HDSP_NON;
	chkNON4->HelpContext	= HDSP_NON;
	chkNON5->HelpContext	= HDSP_NON;
	chkNON6->HelpContext	= HDSP_NON;
	chkNON7->HelpContext	= HDSP_NON;
	chkENDX0->HelpContext	= HDSP_ENDX;
	chkENDX1->HelpContext	= HDSP_ENDX;
	chkENDX2->HelpContext	= HDSP_ENDX;
	chkENDX3->HelpContext	= HDSP_ENDX;
	chkENDX4->HelpContext	= HDSP_ENDX;
	chkENDX5->HelpContext	= HDSP_ENDX;
	chkENDX6->HelpContext	= HDSP_ENDX;
	chkENDX7->HelpContext	= HDSP_ENDX;

	trkMVolL->HelpContext	= HDSP_MVOLL;
	trkMVolR->HelpContext	= HDSP_MVOLR;
	trkEVolL->HelpContext	= HDSP_EVOLL;
	trkEVolR->HelpContext	= HDSP_EVOLR;

	txtDIR->HelpContext		= HDSP_DIR;
	txtESA->HelpContext		= HDSP_ESA;

	trkEDL->HelpContext		= HDSP_EDL;
	trkEFB->HelpContext		= HDSP_EFB;

	trkFIR0->HelpContext	= HDSP_C;
	trkFIR1->HelpContext	= HDSP_C;
	trkFIR2->HelpContext	= HDSP_C;
	trkFIR3->HelpContext	= HDSP_C;
	trkFIR4->HelpContext	= HDSP_C;
	trkFIR5->HelpContext	= HDSP_C;
	trkFIR6->HelpContext	= HDSP_C;
	trkFIR7->HelpContext	= HDSP_C;

	btnRelease->HelpContext	= HDSP_Rel;

	level = 0;									//Default level to 1/8th
	release = 25;								//Default release to shortest possible time

	trkFIR[0] = trkFIR0;
	trkFIR[1] = trkFIR1;
	trkFIR[2] = trkFIR2;
	trkFIR[3] = trkFIR3;
	trkFIR[4] = trkFIR4;
	trkFIR[5] = trkFIR5;
	trkFIR[6] = trkFIR6;
	trkFIR[7] = trkFIR7;

	lblFIR[0] = lblFIR0;
	lblFIR[1] = lblFIR1;
	lblFIR[2] = lblFIR2;
	lblFIR[3] = lblFIR3;
	lblFIR[4] = lblFIR4;
	lblFIR[5] = lblFIR5;
	lblFIR[6] = lblFIR6;
	lblFIR[7] = lblFIR7;

	lblVol[0] = lblMVolL;
	lblVol[1] = lblMVolR;
	lblVol[2] = lblEVolL;
	lblVol[3] = lblEVolR;

	noUpdate = 0;
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormShow(TObject *sender)
{
	oldDebug = Apu.SetDSPDbg(DebugDSP, 0);
	Reset();
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormClose(TObject *sender, TCloseAction &Action)
{
	Apu.SetDSPDbg(oldDebug, 0);					//Restore debug vector
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormKeyDown(TObject *sender, WORD &key, TShiftState shift)
{
	if (keyHandled) return;

	switch(key)
	{
	case(VK_F9):
	case(VK_F10):
	case(VK_F11):
	case(VK_F12):
		HotKey(key, shift);
		keyHandled = 1;
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormKeyPress(TObject *sender, char &key)
{
	if (keyHandled) key = 0;
}


//**************************************************************************************************
void __fastcall TFrmDSP::FormKeyUp(TObject *sender, WORD &key, TShiftState shift)
{
	if (keyHandled)
	{
		key = 0;
		keyHandled = 0;
	}
}


//**************************************************************************************************
// Release all DSP registers from the user's control

void __fastcall TFrmDSP::btnReleaseClick(TObject *sender)
{
	override[0] = 0;
	override[1] = 0;
	override[2] = 0;
	override[3] = 0;
	override[4] = 0;
	override[5] = 0;
	override[6] = 0;
	override[7] = 0;
}


//**************************************************************************************************
// Update the voice controls to reflect the voice selected

void __fastcall TFrmDSP::tabVoiceChange(TObject *sender)
{
	DSPVoice	*p;
	u8			*a;


	noUpdate |= 2;

	//Update voice controls --------------------
	p = &Apu.dsp->voice[tabVoice->TabIndex];

	txtVolL->Text = (s32)p->volL;
	txtVolR->Text = (s32)p->volR;
	txtPitch->Text = (s32)p->pitch;
	txtSrc->Text = (s32)p->srcn;
	txtEnvX->Text = (s32)p->envx;
	txtOutX->Text = (s32)p->outx;

	//Update envelope controls -----------------
	a = &Apu.dsp->voice[tabVoice->TabIndex].adsr1;

	chkADSR->Checked = a[0] & 0x80;

	trkAR->Position = a[0] & 0xF;
	trkDR->Position = (a[0] >> 4) & 7;
	trkSL->Position = a[1] >> 5;
	trkSR->Position = a[1] & 0x1F;

	if (a[2] & 0x80)
	{
		rdoDirect->Checked = true;
		trkGain->Position = a[2];
	}
	else
	{
		switch(a[2] & 0x60)
		{
		case(0x00):
			rdoDec->Checked = true;
			break;

		case(0x20):
			rdoExp->Checked = true;
			break;

		case(0x40):
			rdoInc->Checked = true;
			break;

		case(0x60):
			rdoBent->Checked = true;
			break;
		}
		trkGain->Position = a[2] & 0x1F;
	}

	noUpdate &= ~2;
}


//**************************************************************************************************
void __fastcall TFrmDSP::txtVolKeyDown(TObject *sender, WORD &key, TShiftState shift)
{
	if ((key >= '0' && key <= '9') ||
		key == '-' ||
		key == 189 ||
		key == VK_SUBTRACT ||
		key == VK_BACK) return;

	key = 0;
	keyHandled = 1;
}


//**************************************************************************************************
void __fastcall TFrmDSP::txtVolChange(TObject *sender)
{
	s32	i;


	if (noUpdate) return;

	i = ((TEdit*)sender)->Text.ToIntDef(200);

	if (i != 200 && i >= -128 && i <= 127)
		WriteDSP(((TEdit*)sender)->Tag + (tabVoice->TabIndex << 4), i);
}


//**************************************************************************************************
void __fastcall TFrmDSP::txtPitchKeyDown(TObject *sender, WORD &key, TShiftState shift)
{
	if (key < '0' || key > '9')
	{
		key = 0;
		keyHandled = 1;
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::txtPitchChange(TObject *sender)
{
	s32	i,r;


	if (noUpdate) return;

	i = txtPitch->Text.ToIntDef(-1);

	if (i != -1 && i >= 0 && i <= 16383)
	{
		r = txtPitch->Tag + (tabVoice->TabIndex << 4);
		WriteDSP(r, i & 0xFF);
		WriteDSP(r + 1, i >> 8);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::txtSrcChange(TObject *sender)
{
	s32	i;


	if (noUpdate) return;

	i = txtSrc->Text.ToIntDef(-1);

	if (i != -1 && i >= 0 && i <= 255)
		WriteDSP(txtSrc->Tag + (tabVoice->TabIndex << 4), i);
}


//**************************************************************************************************
// En/Disable controls based on the envelope mode

void __fastcall TFrmDSP::chkADSRClick(TObject *sender)
{
	s32	r;


	if (!noUpdate)
	{
		r = chkADSR->Checked ? 0x80 : 0;
		r |= Apu.dsp->voice[tabVoice->TabIndex].adsr1 & ~0x80;
		WriteDSP(chkADSR->Tag | (tabVoice->TabIndex << 4), r);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkARChange(TObject *sender)
{
	s8	str[32];
	f64	f;
	s32	r,i;


	r = trkAR->Position;
	if (r < 15)
	{
		//Number of changes multiplied by the number of samples between each change
		//Attack goes from 0 to 2016 at the rate of 32 (63 changes)
		attack = 63 * envRate[(r << 1) + 1];
		f = attack / 32.0;

		if (f < 1000)							//If the time is < 1 second, print milliseconds
			sprintf(str, "%.0fms", f);
		else
			sprintf(str, "%.1f", f / 1000.0);

		lblAR->Caption = str;
	}
	else
	{
		attack = 0;								//Special case for an attack time of 0
		lblAR->Caption = "0ms";
	}

	if (!noUpdate)
	{
		i = tabVoice->TabIndex;
		r |= Apu.dsp->voice[i].adsr1 & ~0xF;
		WriteDSP(trkAR->Tag | (i << 4), r);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkDRChange(TObject *sender)
{
	s8	str[16];
	f64	f;
	s32	r,i;


	r = trkDR->Position;

	//Number of changes to go from 1.0 to SL at the rate of 1/256th
//	decay = (s32)Logx(256.0 / 255.0, 1.0 / level) * envRate[(r << 1) + 16];
	if (attack)
		decay = dec1[level] * envRate[(r << 1) + 16];
	else
		decay = dec0[level] * envRate[(r << 1) + 16];

	f = decay / 32.0;
	sprintf(str, "%.0fms", f);
	lblDR->Caption = str;

	if (!noUpdate)
	{
		i = tabVoice->TabIndex;
		r = (r << 4) | (Apu.dsp->voice[i].adsr1 & ~0x70);
		WriteDSP(trkDR->Tag | (i << 4), r);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkSLChange(TObject *sender)
{
	s8	str[32];


	level = trkSL->Position;
	switch(level)
	{
	case(0):
		lblSL->Caption = "1/8th";
		break;

	case(7):
		lblSL->Caption = "1";
		break;

	default:
		lblSL->Caption = AnsiString(level + 1) + "/8ths";
	}

	trkDRChange(NULL);
	trkSRChange(NULL);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkSRChange(TObject *sender)
{
	s8	str[16];
	f64	f;
	s32	r,i;


	r = trkSR->Position;
	if (r)
	{
//		sustain = (s32)Logx(256.0 / 255.0, level / 0.1) * envRate[r];
		if (attack)
			sustain = sust1[level] * envRate[r];
		else
			sustain = sust0[level] * envRate[r];

		f = sustain / 32.0;

		if (f < 1000) sprintf(str, "%.0fms", f);
		else
		if (f < 10000) sprintf(str, "%.1f", f / 1000.0);
		else
			sprintf(str, "%.0f", f / 1000.0);

		lblSR->Caption = str;
	}
	else
		lblSR->Caption = "inf";

	if (!noUpdate)
	{
		i = tabVoice->TabIndex;
		r |= trkSL->Position << 5;
		WriteDSP(trkSR->Tag | (i << 4), r);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::grpGainClick(TObject *sender)
{
	if (rdoDirect->Checked)
	{
		trkGain->Max = 127;						//128 possible values
		trkGain->Frequency = 2;
	}
	else
	{
		trkGain->Max = 31;						//32 possible values
		trkGain->Frequency = 1;
	}

	trkGainChange(NULL);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkGainChange(TObject *sender)
{
	f64	f,r;
	s8	str[16],*p;
	s32	v,i;


	v = trkGain->Position;

	if (rdoDirect->Checked)
		lblGain->Caption = v;
	else
	if (trkGain->Position)
	{
		i = Apu.voice[tabVoice->TabIndex].eVal;	//Get the real envelope height

		if (rdoInc->Checked)	//Increase linear
		{
			i = (2078 - i) >> 5; 				//i = number of changes to reach 1
			r = (i * envRate[v]) / 32.0;
			f = (64 * envRate[v]) / 32.0;
			v |= 0xC0;
		}
		else
		if (rdoBent->Checked)	//Increase bent line
		{
			if (i >= (1536))					//Is envelope already in "bent" mode?
				i = (2054 - i) >> 3;
			else
				i = ((1567 - i) >> 5) + 64;
			r = (i * envRate[v]) / 32.0;
			f = (112 * envRate[v]) / 32.0;
			v |= 0xE0;
		}
		else
		if (rdoDec->Checked)	//Decrease linear
		{
			i = (i + 31) >> 5;
			r = (i * envRate[v]) / 32.0;
			f = (64 * envRate[v]) / 32.0;
			v |= 0x80;
		}
		else					//Decrease exponential
		{
//			r = ((s32)Logx(256.0 / 255.0, ((f64)i / 2047.0) / 0.1) * envRate[v]) / 32.0;
//			f = ((s32)Logx(256.0 / 255.0, 1.0 / 0.1) * envRate[v]) / 32.0;

			i = 2047;
			r = 0.0;
			while (i)
			{
				r += 1.0;
				i += (-i) >> 8;
			}
			r *= envRate[v] / 32.0;

			f = sust0[7] * envRate[v] / 32.0;
			v |= 0xA0;
		}

		//Print full time ----------------------
		if (f < 1000) sprintf(str, "%.0fms", f);
		else
		if (f < 10000) sprintf(str, "%.1f", f / 1000.0);
		else
			sprintf(str, "%.0f", f / 1000.0);
		p = str + strlen(str);
		
		//Print actual time based on ENVX ------
		if (r < 1000) sprintf(p, " (%.0fms)", r);
		else
		if (r < 10000) sprintf(p, " (%.1f)", r / 1000.0);
		else
			sprintf(p, " (%.0f)", r / 1000.0);

		lblGain->Caption = str;
	}
	else
		lblGain->Caption = "inf";

	if (!noUpdate) WriteDSP(trkGain->Tag | (tabVoice->TabIndex << 4), v);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkNCKChange(TObject *sender)
{
	u32	i;


	lblNCK->Caption = AnsiString(NClk[trkNCK->Position]);
	if (!noUpdate)
	{
		i = Apu.dsp->flg & ~0x1F;
		i |= trkNCK->Position;
		WriteDSP(trkNCK->Tag, i);
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::btnKeyMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (btn == mbLeft && shift.Contains(ssLeft))
		Apu.SetDSPReg(((TButton*)sender)->Tag & 0xFF, 1 << (((TButton*)sender)->Tag >> 8));
}


//**************************************************************************************************
void __fastcall TFrmDSP::chkSwitchClick(TObject *sender)
{
	u32	reg,val,bit;


	if (noUpdate) return;

	reg = ((TComponent*)sender)->Tag & 0xFF;		//Get register index
	val = Apu.dsp->reg[reg];						//Get current register value
	bit = 1 << (((TComponent*)sender)->Tag >> 8);	//Create a bit mask
	val &= ~bit;									//Clear the bit
	if (((TCheckBox*)sender)->Checked) val |= bit;	//If the checkbox is checked, set the bit
	WriteDSP(reg, val);
}


//**************************************************************************************************
void __fastcall TFrmDSP::btnENDXClick(TObject *sender)
{
	chkENDX0->Checked = false;
	chkENDX1->Checked = false;
	chkENDX2->Checked = false;
	chkENDX3->Checked = false;
	chkENDX4->Checked = false;
	chkENDX5->Checked = false;
	chkENDX6->Checked = false;
	chkENDX7->Checked = false;

	if (!noUpdate) WriteDSP(btnENDX->Tag, 0);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkVolChange(TObject *sender)
{
	u32	reg;
	s32	val;

	reg = ((TTrackBar*)sender)->Tag;
	val = 0 - ((TTrackBar*)sender)->Position;
	lblVol[reg >> 4]->Caption = val;
	if (!noUpdate) WriteDSP(reg, (u8)val);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkEDLChange(TObject *sender)
{
	lblEDL->Caption = AnsiString(trkEDL->Position << 4) + "ms";
	if (!noUpdate) WriteDSP(trkEDL->Tag, trkEDL->Position);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkEFBChange(TObject *sender)
{
	lblEFB->Caption = AnsiString(trkEFB->Position * 100 / 128) + "%";
	if (!noUpdate) WriteDSP(trkEFB->Tag, trkEFB->Position);
}


//**************************************************************************************************
void __fastcall TFrmDSP::trkFIRChange(TObject *sender)
{
	u32	reg;
	s32	val;

	reg = ((TTrackBar*)sender)->Tag;
	val = 0 - ((TTrackBar*)sender)->Position;
	lblFIR[reg >> 4]->Caption = val;
	if (!noUpdate) WriteDSP(reg, (u8)val);
}


//**************************************************************************************************
void __fastcall TFrmDSP::UpdateReg(u8 reg)
{
	u32	rh,rl;


	rh = reg >> 4;
	rl = reg & 0xF;

	if (rl > 9)									//Is this a global register?
	{
		switch(rl)
		{
		case(0xC):								//Volume registers
			switch(rh)
			{
			case(0):	//MVOL(L)
				FrmDSP->trkMVolL->Position = 0 - Apu.dsp->mvolL;
				break;

			case(1):	//MVOL(R)
				FrmDSP->trkMVolR->Position = 0 - Apu.dsp->mvolR;
				break;

			case(2):	//EVOL(L)
				FrmDSP->trkEVolL->Position = 0 - Apu.dsp->evolL;
				break;

			case(3):	//EVOL(R)
				FrmDSP->trkEVolR->Position = 0 - Apu.dsp->evolR;
				break;

			case(6):	//FLG
				FrmDSP->chkRES->Checked  = Apu.dsp->flg & 0x80;
				FrmDSP->chkMUTE->Checked = Apu.dsp->flg & 0x40;
				FrmDSP->chkECEN->Checked = Apu.dsp->flg & 0x20;
				FrmDSP->trkNCK->Position = Apu.dsp->flg & 0x1F;
				break;

			case(7):	//ENDX
				FrmDSP->btnENDXClick(NULL);
				break;
			}
			break;

		case(0xD):								//Switches
			switch(rh)
			{
			case(0):
				FrmDSP->trkEFB->Position = Apu.dsp->efb;
				break;

			case(2):
				FrmDSP->chkPMON0->Checked = Apu.dsp->pmon & 0x01;
				FrmDSP->chkPMON1->Checked = Apu.dsp->pmon & 0x02;
				FrmDSP->chkPMON2->Checked = Apu.dsp->pmon & 0x04;
				FrmDSP->chkPMON3->Checked = Apu.dsp->pmon & 0x08;
				FrmDSP->chkPMON4->Checked = Apu.dsp->pmon & 0x10;
				FrmDSP->chkPMON5->Checked = Apu.dsp->pmon & 0x20;
				FrmDSP->chkPMON6->Checked = Apu.dsp->pmon & 0x40;
				FrmDSP->chkPMON7->Checked = Apu.dsp->pmon & 0x80;
				break;

			case(3):
				FrmDSP->chkNON0->Checked = Apu.dsp->non & 0x01;
				FrmDSP->chkNON1->Checked = Apu.dsp->non & 0x02;
				FrmDSP->chkNON2->Checked = Apu.dsp->non & 0x04;
				FrmDSP->chkNON3->Checked = Apu.dsp->non & 0x08;
				FrmDSP->chkNON4->Checked = Apu.dsp->non & 0x10;
				FrmDSP->chkNON5->Checked = Apu.dsp->non & 0x20;
				FrmDSP->chkNON6->Checked = Apu.dsp->non & 0x40;
				FrmDSP->chkNON7->Checked = Apu.dsp->non & 0x80;
				break;

			case(4):
				FrmDSP->chkEON0->Checked = Apu.dsp->eon & 0x01;
				FrmDSP->chkEON1->Checked = Apu.dsp->eon & 0x02;
				FrmDSP->chkEON2->Checked = Apu.dsp->eon & 0x04;
				FrmDSP->chkEON3->Checked = Apu.dsp->eon & 0x08;
				FrmDSP->chkEON4->Checked = Apu.dsp->eon & 0x10;
				FrmDSP->chkEON5->Checked = Apu.dsp->eon & 0x20;
				FrmDSP->chkEON6->Checked = Apu.dsp->eon & 0x40;
				FrmDSP->chkEON7->Checked = Apu.dsp->eon & 0x80;
				break;

			case(5):
				FrmDSP->txtDIR->Text = IntToHex((s32)Apu.dsp->dir << 8, 4);
				break;

			case(6):
				FrmDSP->txtESA->Text = IntToHex((s32)Apu.dsp->esa << 8, 4);
				break;

			case(7):
				FrmDSP->trkEDL->Position = Apu.dsp->edl & 0xF;
				break;
			}
			break;

		case(0xF):
			trkFIR[rh]->Position = 0 - Apu.dsp->reg[reg];
			break;
		}

	}
	else
	if (rh == FrmDSP->tabVoice->TabIndex)
	{
		switch(rl)
		{
		case(0):
			FrmDSP->txtVolL->Text = (s32)(s8)Apu.dsp->reg[reg];
			break;

		case(1):
			FrmDSP->txtVolR->Text = (s32)(s8)Apu.dsp->reg[reg];
			break;

		case(2):
		case(3):
			FrmDSP->txtPitch->Text = (u32)Apu.dsp->voice[rh].pitch;
			break;

		case(4):
			FrmDSP->txtSrc->Text = Apu.dsp->reg[reg];
			break;

		case(5):
			FrmDSP->chkADSR->Checked = Apu.dsp->reg[reg] & 0x80;
			FrmDSP->trkAR->Position = Apu.dsp->reg[reg] & 0xF;
			FrmDSP->trkDR->Position = (Apu.dsp->reg[reg] >> 4) & 7;
			break;

		case(6):
			FrmDSP->trkSL->Position = Apu.dsp->reg[reg] >> 5;
			FrmDSP->trkSR->Position = Apu.dsp->reg[reg] & 0x1F;
			break;

		case(7):
			FrmDSP->rdoDirect->Checked = !(Apu.dsp->reg[reg] & 0x80);
			if (FrmDSP->rdoDirect->Checked)
				FrmDSP->trkGain->Position = Apu.dsp->reg[reg];
			else
				FrmDSP->trkGain->Position = Apu.dsp->reg[reg] & 0x1F;
			break;

		case(8):
			FrmDSP->txtEnvX->Text = Apu.dsp->reg[reg];
			break;

		case(9):
			FrmDSP->txtOutX->Text = (s32)(s8)Apu.dsp->reg[reg];
			break;
		}
	}
}


//**************************************************************************************************
void __fastcall TFrmDSP::Reset()
{
	noUpdate |= 2;								//Place editor in "no update" mode

	modified[0] = 0;
	modified[1] = 0;
	modified[2] = 0;
	modified[3] = 0;
	modified[4] = 0;
	modified[5] = 0;
	modified[6] = 0;
	modified[7] = 0;

	btnReleaseClick(NULL);						//Reset all modified flags

	chkRES->Checked		= Apu.dsp->flg & 0x80;
	chkMUTE->Checked	= Apu.dsp->flg & 0x40;
	chkECEN->Checked	= Apu.dsp->flg & 0x20;
	trkNCK->Position	= Apu.dsp->flg & 0x1F;

	txtDIR->Text		= IntToHex((s32)Apu.dsp->dir << 8, 4);
	txtESA->Text		= IntToHex((s32)Apu.dsp->esa << 8, 4);

	trkMVolL->Position	= 0 - Apu.dsp->mvolL;
	trkMVolR->Position	= 0 - Apu.dsp->mvolR;
	trkEVolL->Position	= 0 - Apu.dsp->evolL;
	trkEVolR->Position	= 0 - Apu.dsp->evolR;

	trkEDL->Position	= Apu.dsp->edl & 0xF;
	trkEFB->Position	= Apu.dsp->efb;

	trkFIR0->Position	= 0 - Apu.dsp->fir[0].c;
	trkFIR1->Position	= 0 - Apu.dsp->fir[1].c;
	trkFIR2->Position	= 0 - Apu.dsp->fir[2].c;
	trkFIR3->Position	= 0 - Apu.dsp->fir[3].c;
	trkFIR4->Position	= 0 - Apu.dsp->fir[4].c;
	trkFIR5->Position	= 0 - Apu.dsp->fir[5].c;
	trkFIR6->Position	= 0 - Apu.dsp->fir[6].c;
	trkFIR7->Position	= 0 - Apu.dsp->fir[7].c;

	chkEON0->Checked	= Apu.dsp->eon & 0x01;
	chkEON1->Checked	= Apu.dsp->eon & 0x02;
	chkEON2->Checked	= Apu.dsp->eon & 0x04;
	chkEON3->Checked	= Apu.dsp->eon & 0x08;
	chkEON4->Checked	= Apu.dsp->eon & 0x10;
	chkEON5->Checked	= Apu.dsp->eon & 0x20;
	chkEON6->Checked	= Apu.dsp->eon & 0x40;
	chkEON7->Checked	= Apu.dsp->eon & 0x80;

	chkPMON0->Checked	= Apu.dsp->pmon & 0x01;
	chkPMON1->Checked	= Apu.dsp->pmon & 0x02;
	chkPMON2->Checked	= Apu.dsp->pmon & 0x04;
	chkPMON3->Checked	= Apu.dsp->pmon & 0x08;
	chkPMON4->Checked	= Apu.dsp->pmon & 0x10;
	chkPMON5->Checked	= Apu.dsp->pmon & 0x20;
	chkPMON6->Checked	= Apu.dsp->pmon & 0x40;
	chkPMON7->Checked	= Apu.dsp->pmon & 0x80;

	chkNON0->Checked	= Apu.dsp->non & 0x01;
	chkNON1->Checked	= Apu.dsp->non & 0x02;
	chkNON2->Checked	= Apu.dsp->non & 0x04;
	chkNON3->Checked	= Apu.dsp->non & 0x08;
	chkNON4->Checked	= Apu.dsp->non & 0x10;
	chkNON5->Checked	= Apu.dsp->non & 0x20;
	chkNON6->Checked	= Apu.dsp->non & 0x40;
	chkNON7->Checked	= Apu.dsp->non & 0x80;

	Update();									//Reset read-only registers

	tabVoiceChange(NULL);						//Put this last because it clears 'noUpdate'
}


//**************************************************************************************************
void __fastcall TFrmDSP::Update()
{
	u32			v;
	u16			i,*p;
	u8			r;


	noUpdate |= 1;

	//Update read-only registers ---------------
	txtEnvX->Text = (s32)Apu.dsp->voice[tabVoice->TabIndex].envx;
	txtOutX->Text = (s32)Apu.dsp->voice[tabVoice->TabIndex].outx;

	r = Apu.dsp->endx;
	chkENDX0->Checked = r & 0x01;
	chkENDX1->Checked = r & 0x02;
	chkENDX2->Checked = r & 0x04;
	chkENDX3->Checked = r & 0x08;
	chkENDX4->Checked = r & 0x10;
	chkENDX5->Checked = r & 0x20;
	chkENDX6->Checked = r & 0x40;
	chkENDX7->Checked = r & 0x80;

	//Update read/write registers --------------
	r = -1;
	p = modified;
	for (v=0; v<8; v++)
	{
		for (i=1; i; i<<=1)
		{
			r++;
			if (*p & i) UpdateReg(r);
		}

		*p++ = 0;
	}

	noUpdate &= ~1;
}


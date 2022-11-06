/***************************************************************************************************
* Configuration Dialog                                                                             *
*                                                           Copyright (C)2003 Alpha-II Productions *
***************************************************************************************************/

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"Config.h"
#include	"WaveOut.h"
#include	"math.h"
#include	"dstring.h"
#include	"mmsystem.h"
#include	"A2Math.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


//**************************************************************************************************

static const u32	rateList[]		= {8000,11025,16000,22050,32000,44100,48000,64000,88200,96000};
static const u32	pitchList[]		= {32000,32458,32768};
static const s8		interList[][9]	= {"none","linear","cubic","gaussian","sinc"};
static const s8		attenList[][9]	= {"off","on","increase"};
static const s8		syntaxList[][8]	= {"isas","spctool"};
static const s8		caseList[][6]	= {"lower","mixed","upper"};
static const u16	portList[]		= {0x378, 0x278, 0x3BC};


TFrmConfig	*FrmConfig;

s8*	__fastcall PrintDP(s8 *pStr, u8 a);

//**************************************************************************************************
__fastcall TFrmConfig::TFrmConfig(TComponent* owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmConfig::Reinit()
{
	HelpFile = Application->HelpFile;
}


//**************************************************************************************************
void __fastcall TFrmConfig::FormCreate(TObject *sender)
{
	u32			i,n;
	WAVEOUTCAPS	cap;


	HelpFile = Application->HelpFile;

	n = waveOutGetNumDevs();
	for (i=0; i<n; i++)
	{
		waveOutGetDevCaps(i, &cap, sizeof(WAVEOUTCAPS));
		cboDev->Items->Add(cap.szPname);
	}
}


//**************************************************************************************************
void __fastcall TFrmConfig::FormShow(TObject *sender)
{
	u32	i;

	//Copy user defined settings ---------------
	memcpy(&udsp, &Cfg.udsp, sizeof(udsp));
	memcpy(&umix, &Cfg.umix, sizeof(umix));

	//Set preset to initialize DSP and Mixing --
	switch(Cfg.preset)
	{
	case(0): grpPreset->ItemIndex = 2; break;
	case(-1): grpPreset->ItemIndex = 0; break;
	case(-2): grpPreset->ItemIndex = 1; break;
	}
	cboRate->Text		= udsp.rate;

	//Time -------------------------------------
	txtSongMin->Text	= Cfg.time.song / XID6_TICKSMIN;
	txtSongSec->Text	= FloatToStrF((f64)(Cfg.time.song % XID6_TICKSMIN) / XID6_TICKSSEC, ffFixed, 5, 2);
	txtFade->Text		= FloatToStrF((f64)Cfg.time.fade / XID6_TICKSSEC, ffFixed, 5, 2);
	trkFF->Position		= Cfg.time.skip;
	chkNext->Checked	= Cfg.time.next;
	chkFast->Checked	= Cfg.time.fastSeek;
	chkBin->Checked		= Cfg.time.binary;

	//Source -----------------------------------
	txtMinSize->Tag		= Cfg.src.min << 4;
	txtMinSize->Text	= Cfg.src.min << 4;
	txtMaxSize->Tag		= Cfg.src.max << 4;
	txtMaxSize->Text	= Cfg.src.max << 4;
	chkLinear->Checked	= Cfg.src.linear;
	chkSilence->Checked	= Cfg.src.silence;
	chkRange->Checked	= Cfg.src.range <= 12;
	chkDP->Checked		= Cfg.src.noDP != 0;
	chkInit->Checked	= Cfg.src.init;

	//Debug ------------------------------------
	grpSyn->ItemIndex	= Cfg.debug.syntax;
	grpCase->ItemIndex	= Cfg.debug.ltrCase;
	switch(Cfg.debug.hex)
	{
	case('h'):	grpHex->ItemIndex = 1; break;
	case('x'):	grpHex->ItemIndex = 2; break;
	default:	grpHex->ItemIndex = 0;
	}
	chkPC->Checked		= Cfg.debug.printPC;
	chkOpcode->Checked	= Cfg.debug.printOp;
	chkCmnt->Checked	= Cfg.debug.printCmnt;

	//Devices ----------------------------------
	cboDev->ItemIndex	= Cfg.out.device + 1;
	trkBuf->Position	= (Cfg.out.size - 40) / 20;
	chkForce->Checked	= Cfg.out.force;

	cboHW->ItemIndex	= Cfg.out.snes;
	switch(Cfg.out.port)
	{
	case(0x378): cboLPT->ItemIndex = 0; break;
	case(0x278): cboLPT->ItemIndex = 1; break;
	case(0x3BC): cboLPT->ItemIndex = 2; break;
	}
	cboHWChange(NULL);

	//Other ------------------------------------
	grpVis->ItemIndex	= Cfg.vis.vu ? ((Cfg.vis.fps >= 50) ? 2 : 1) : 0;

	btnApply->Enabled = false;
}


//**************************************************************************************************
void __fastcall TFrmConfig::itmWhatClick(TObject *sender)
{
	Application->HelpCommand(HELP_CONTEXTPOPUP,((TWinControl*)popHelp->PopupComponent)->HelpContext);
}


//**************************************************************************************************
void __fastcall TFrmConfig::btnApplyClick(TObject *sender)
{
	SaveConfig();
	btnApply->Enabled = false;
}


//**************************************************************************************************
void __fastcall TFrmConfig::btnOKClick(TObject *sender)
{
	if (btnApply->Enabled) SaveConfig();
	Close();
}


//**************************************************************************************************
void __fastcall TFrmConfig::btnCancelClick(TObject *sender)
{
	Apu.SetDSPStereo(Cfg.dsp.stereo);
	Apu.SetDSPEFBCT(Cfg.dsp.efbct);
	Close();
}


//**************************************************************************************************
void __fastcall TFrmConfig::btnHelpClick(TObject *sender)
{
	Application->HelpCommand(HELP_CONTEXT,9999);
}


//**************************************************************************************************
void __fastcall TFrmConfig::btnResetClick(TObject *sender)
{
	b8	reset;


	WritePrivateProfileString("Device", NULL, NULL, Cfg.ini);
	WritePrivateProfileString("Debug", NULL, NULL, Cfg.ini);
	WritePrivateProfileString("Mixer", NULL, NULL, Cfg.ini);
	WritePrivateProfileString("Time", NULL, NULL, Cfg.ini);
	WritePrivateProfileString("Source", NULL, NULL, Cfg.ini);
	WritePrivateProfileString("DSP", NULL, NULL, Cfg.ini);

	LoadConfig();
	pgcCfg->ActivePageIndex = 0;
	FormShow(NULL);

	reset = IsWavOpen();
	if (reset) WavReset();

	Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());
	Apu.SetDSPAAR(Cfg.mix.aar, Cfg.mix.threshold, Cfg.mix.min, Cfg.mix.max);
	Apu.SetDSPStereo(Cfg.dsp.stereo);
	Apu.SetDSPEFBCT(Cfg.dsp.efbct);

	if (reset) WavStart();
}


//**************************************************************************************************
void __fastcall TFrmConfig::ItemChange(TObject *sender)
{
	if (!btnApply->Enabled) btnApply->Enabled = true;
}


//**************************************************************************************************
void __fastcall TFrmConfig::grpPresetClick(TObject *sender)
{
	b8	state;


	//En/Disable controls based on preset selected
	state = (grpPreset->ItemIndex == 2);

	lblBits->Enabled		= state;
	cboBits->Enabled		= state;
	lblInter->Enabled		= state;
	cboInter->Enabled		= state;
	chkMix->Enabled			= state;
	chkLow->Enabled			= state;
	chkSurr->Enabled		= state;
	chkReverse->Enabled		= state;
	chkADPCM->Enabled		= state;
	chkNoEcho->Enabled		= state;
	lblTStereo->Enabled		= state;
	lblStereo->Enabled		= state;
	trkStereo->Enabled		= state;
	lblTEcho->Enabled		= state;
	lblEcho->Enabled		= state;
	trkEcho->Enabled		= state;

	grpAAR->Enabled			= state;
	lblTAmp->Enabled		= state;
	lblAmp->Enabled			= state;
	trkAmp->Enabled			= state;
	txtMinAmp->Enabled		= state;
	txtMaxAmp->Enabled		= state;
	lblTThresh->Enabled		= state;
	lblThresh->Enabled		= state;
	trkThresh->Enabled		= state;
	chkReset->Enabled		= state;

	//Save current settings if preset was "Custom"
	if (!state && grpPreset->Tag == 2)
	{
		udsp.rate	   	= cboRate->Text.ToInt();
		udsp.bits	   	= cboBits->ItemIndex ? 32 : 16;
		udsp.inter		= cboInter->ItemIndex;
		udsp.mix		= chkMix->Checked ? MIX_FLOAT : MIX_INT;
		udsp.analog   	= chkLow->Checked;
		udsp.filter		= chkFilter->Checked;
		udsp.surround  	= chkSurr->Checked;
		udsp.reverse   	= chkReverse->Checked;
		udsp.disEcho	= chkNoEcho->Checked;
		udsp.oldSmp  	= chkADPCM->Checked;
		udsp.stereo		= (trkStereo->Position << 16) / 100;
		udsp.efbct	   	= (trkEcho->Position << 15) / 100;

		umix.aar	   	= (s32)grpAAR->ItemIndex;
		umix.amp	   	= trkAmp->Tag;
		umix.min		= txtMinAmp->Tag;
		umix.max		= txtMaxAmp->Tag;
		umix.threshold	= trkThresh->Tag;
		umix.reset		= chkReset->Checked;
	}

	//Set controls -----------------------------
	switch(grpPreset->ItemIndex)
	{
	case(0):
		cboBits->ItemIndex		= 0;
		cboInter->ItemIndex		= 2;
		chkMix->Checked			= 1;
		chkLow->Checked			= false;
		chkSurr->Checked		= false;
		chkReverse->Checked		= false;
		chkADPCM->Checked		= false;
		chkNoEcho->Checked		= false;
		trkStereo->Position		= 50;
		trkEcho->Position		= -50;

		grpAAR->ItemIndex		= 2;
		trkAmp->SelStart		= 0;
		trkAmp->SelEnd			= 180;
		trkAmp->Position		= 60;
		trkAmp->Tag				= 131072;
		txtMinAmp->Text			= "0.0";
		txtMaxAmp->Text			= "18.0";
		txtMinAmp->Tag			= 65536;
		txtMaxAmp->Tag			= 524288;
		trkThresh->Position		= 2;
		trkThresh->Tag			= 33148;
		chkReset->Checked		= true;

		grpPreset->ItemIndex	= 0;
		grpPreset->Tag			= 0;
		break;

	case(1):
		cboBits->ItemIndex		= 0;
		cboInter->ItemIndex		= 3;
		chkMix->Checked			= 0;
		chkLow->Checked			= true;
		chkSurr->Checked		= false;
		chkReverse->Checked		= false;
		chkADPCM->Checked		= false;
		chkNoEcho->Checked		= false;
		trkStereo->Position		= 50;
		trkEcho->Position		= 100;

		grpAAR->ItemIndex		= 0;
		trkAmp->SelStart		= 0;
		trkAmp->SelEnd			= 180;
		trkAmp->Position		= 0;
		trkAmp->Tag				= 65536;
		txtMinAmp->Text			= "0.0";
		txtMaxAmp->Text			= "18.0";
		txtMinAmp->Tag			= 65536;
		txtMaxAmp->Tag			= 524288;
		trkThresh->Position		= 0;
		trkThresh->Tag			= 32768;
		chkReset->Checked		= true;

		grpPreset->ItemIndex	= 1;
		grpPreset->Tag			= 1;
		break;

	case(2):
		//Set DSP emulation options ------------
		switch(udsp.bits)
		{
		case(16):	cboBits->ItemIndex = 0; break;
		case(32):	cboBits->ItemIndex = 1; break;
		}
		cboInter->ItemIndex		= udsp.inter;
		chkMix->Checked			= udsp.mix == MIX_FLOAT;
		chkLow->Checked			= udsp.analog;
		chkMix->Enabled			= !udsp.analog;
		chkLow->Enabled			= udsp.mix != MIX_FLOAT;
		chkFilter->Checked		= udsp.filter;
		chkSurr->Checked		= udsp.surround;
		chkReverse->Checked		= udsp.reverse;
		chkADPCM->Checked		= udsp.oldSmp;
		chkNoEcho->Checked		= udsp.disEcho;
		trkStereo->Position		= (udsp.stereo * 100) >> 16;
		trkEcho->Position		= (udsp.efbct * 100) >> 15;

		//Set mixing options -------------------
		grpAAR->ItemIndex		= umix.aar;
		trkAmp->SelStart		= F2I(YLog2(60.0, (s32)umix.min / 65536.0));
		trkAmp->SelEnd			= F2I(YLog2(60.0, (s32)umix.max / 65536.0));
		trkAmp->Position		= F2I(YLog2(60.0, (s32)umix.amp / 65536.0));
		trkAmp->Tag				= umix.amp;
		txtMinAmp->Text			= FloatToStrF(trkAmp->SelStart / 10.0, ffFixed, 4, 1);
		txtMaxAmp->Text			= FloatToStrF(trkAmp->SelEnd / 10.0, ffFixed, 4, 1);
		txtMinAmp->Tag			= umix.min;
		txtMaxAmp->Tag			= umix.max;
		trkThresh->Position		= F2I(YLog2(120.0, (s32)umix.threshold / 32768.0));
		trkThresh->Tag			= umix.threshold;
		chkReset->Checked		= umix.reset;

		grpPreset->Tag			= 2;
		break;
	}

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::cboRateExit(TObject *sender)
{
	s32	i = cboRate->Text.ToIntDef(48000);

	_Clamp(i, 8000, 96000);
	cboRate->Text = i;
}


//**************************************************************************************************
void __fastcall TFrmConfig::chkMixClick(TObject *sender)
{
	if (grpPreset->ItemIndex != 2) return;

	chkLow->Enabled = !chkMix->Checked;
	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::chkLowClick(TObject *sender)
{
	if (grpPreset->ItemIndex != 2) return;

	chkMix->Enabled = !chkLow->Checked;
	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::trkStereoChange(TObject *sender)
{
	lblStereo->Caption = trkStereo->Position;
	Apu.SetDSPStereo((trkStereo->Position << 16) / 100);

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::trkEchoChange(TObject *sender)
{
	AnsiString	str;
	s32			i;


	i = trkEcho->Position;
	if (i < 0) str = "-";
	i = 100 - abs(i);
	str += i;
	str += "%";
	lblEcho->Caption = str;

	Apu.SetDSPEFBCT((trkEcho->Position << 15) / 100);

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::trkFFChange(TObject *sender)
{
	lblFF->Caption = AnsiString(trkFF->Position) + "x";
	ItemChange(sender);
}


//**************************************************************************************************
// Disable the threshold slider if AAR is turned off

void __fastcall TFrmConfig::grpAARClick(TObject *sender)
{
	b8	state;


	if (grpPreset->ItemIndex < 2) return;

	state = (grpAAR->ItemIndex != 0);
	lblTThresh->Enabled = state;
	lblThresh->Enabled = state;
	trkThresh->Enabled = state;

	ItemChange(sender);
}


//**************************************************************************************************
// Limits the level to a value between the min and max values specified by the user

void __fastcall TFrmConfig::trkAmpChange(TObject *sender)
{
	s32	i = trkAmp->Position;


	if (i < trkAmp->SelStart)
	{
		i = trkAmp->SelStart;
		trkAmp->Position = i;
	}
	else
	if (i > trkAmp->SelEnd)
	{
		i = trkAmp->SelEnd;
		trkAmp->Position = i;
	}

	lblAmp->Caption = FormatFloat("0.0'dB'", trkAmp->Position / 10.0);
	trkAmp->Tag = F2I(pow(2.0, trkAmp->Position / 60.0) * 65536.0);

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::trkThreshChange(TObject *sender)
{
	lblThresh->Caption = FormatFloat("0.00'dB'", (f64)trkThresh->Position / 20.0);
	trkThresh->Tag = F2I(pow(2.0, trkThresh->Position / 120.0) * 32768.0);

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::txtMinAmpExit(TObject *sender)
{
	f32	min,max;
	s32	i;


	try
	{
		min = txtMinAmp->Text.ToDouble();
		max = txtMaxAmp->Text.ToDouble();
	}
	catch(...)
	{
		min = 0.0f;
	}

	_Clamp(min, -6.0f, max);

	i = min * 10.0f;
	trkAmp->SelStart = i;
	txtMinAmp->Text = FloatToStrF(min, ffFixed, 4, 1);
	txtMinAmp->Tag = F2I(pow(2.0, txtMinAmp->Text.ToDouble() / 6.0) * 65536.0);

	if (trkAmp->Position < i) trkAmp->Position = i;
}


//**************************************************************************************************
void __fastcall TFrmConfig::txtMaxAmpExit(TObject *sender)
{
	f32	min,max;
	s32	i;


	try
	{
		min = txtMinAmp->Text.ToDouble();
		max = txtMaxAmp->Text.ToDouble();
	}
	catch(...)
	{
		max = 18.0f;
	}

	_Clamp(max, min, 18.0f);

	i = max * 10.0f;
	trkAmp->SelEnd = i;
	txtMaxAmp->Text = FloatToStrF(max, ffFixed, 4, 1);
	txtMaxAmp->Tag = F2I(pow(2.0, txtMaxAmp->Text.ToDouble() / 6.0) * 65536.0 + 0.5);

	if (trkAmp->Position > i) trkAmp->Position = i;
}


//**************************************************************************************************
void __fastcall TFrmConfig::txtSongMinExit(TObject *sender)
{
	s32	i;

	i = txtSongMin->Text.ToIntDef(0);
	_Clamp(i, 0, 15);
	txtSongMin->Text = i;
}


//**************************************************************************************************
// Check Time
//
//    Verifies the time entered is in 00.00 form

void __fastcall TFrmConfig::CheckTime(TObject *sender)
{
	TEdit	*txt = static_cast<TEdit*>(sender);

	txt->Text = FloatToStrF((f64)Str2Ticks(txt->Text.c_str()) / XID6_TICKSSEC, ffFixed, 5, 2);
}


//**************************************************************************************************
void __fastcall TFrmConfig::txtMinSizeExit(TObject *sender)
{
	s32	i;

	i = txtMinSize->Text.ToIntDef(32) & ~0xF;
	_Clamp(i, 16, txtMaxSize->Tag);
	txtMinSize->Tag = i;
	txtMinSize->Text = i;
}


//**************************************************************************************************
void __fastcall TFrmConfig::txtMaxSizeExit(TObject *sender)
{
	s32	i;

	i = txtMaxSize->Text.ToIntDef(114688) & ~0xF;
	_Clamp(i, txtMinSize->Tag, 131072);
	txtMaxSize->Tag = i;
	txtMaxSize->Text = i;
}


//**************************************************************************************************
void __fastcall TFrmConfig::PrintHex(AnsiString &line, s32 b)
{
	u32	i;
	s8	str[5],c;


	switch (grpHex->ItemIndex)
	{
	case(0):
		line += "$";
		break;

	case(1):
		if (b > 0x9FFF || (b > 0x9F && b < 0x100)) line += "0";
		break;

	case(2):
		line += "0x";
		break;
	}

	for (i=0;i<4;i++)
	{
		c = ((b >> (i << 2)) & 0xF) | 0x30;
		if (c > '9')
		{
			c += '@' - '9';
			if (grpCase->ItemIndex == 0) c |= 0x20;
		}
		str[3-i] = c;
	}
	str[4] = 0;

	if (b > 0xFF)
		line += str;
	else
		line += (str+2);

	if (grpHex->ItemIndex == 1)
		line += "h";
}


//**************************************************************************************************
/*
Differences between SPCTool's and Intelligent System's assembler

					SPCTool  	ISAS
Immediate value		00			#00
					$00			#$00
					00h			#00h

Direct page index	[00]	 	00
					[00+X]		00+X
					[00+Y]		00+Y

Absolute address	[\00]	   	!00
					[\00+X]		!00+X
					[\00+Y]		!00+Y

Indirect pointer	[X]			(X)

Indexed indirect	[[00+X]]   	[00+X]
					[[00]+Y]   	[00]+Y
					[[\00+X]]	[!00+X]

Auto increment		[X+]	  	(X)+

Bit reversal		~[00],0		/00.0

SPCTool only requires an absolute override "\" when the address is less than 100h.
*/


void __fastcall TFrmConfig::lstDisasmChange(TObject *sender)
{
	AnsiString	line1,line2,line3,line4;
	u32			cmnt = 19;


	if (chkPC->Checked)
	{
		line1 = "0536 ";
		line2 = "0539 ";
		if (grpCase->ItemIndex)
		{
			line3 = "053B ";
			line4 = "053D ";
		}
		else
		{
			line3 = "053b ";
			line4 = "053d ";
		}
		cmnt += 5;
	}

	if (chkOpcode->Checked)
	{
		if (grpCase->ItemIndex)
		{
			line1 += "8F 40 F2  ";
			line2 += "77 18     ";
			line3 += "D0 E7     ";
			line4 += "6A 15 AB  ";
		}
		else
		{
			line1 += "8f 40 f2  ";
			line2 += "77 18     ";
			line3 += "d0 e7     ";
			line4 += "6a 15 bb  ";
		}
		cmnt += 10;
	}

	switch(grpCase->ItemIndex)
	{
	case(0):
		line1 += "mov  ";
		line2 += "cmp  ";
		line3 += "bne  ";
		line4 += "and1 ";
		break;
	case(1):
		line1 += "Mov  ";
		line2 += "Cmp  ";
		line3 += "BNE  ";
		line4 += "And1 ";
		break;
	case(2):
		line1 += "MOV  ";
		line2 += "CMP  ";
		line3 += "BNE  ";
		line4 += "AND1 ";
		break;
	}

	if (grpSyn->ItemIndex)
	{
		line1 += "[";
		PrintHex(line1,0xF2);
		line1 += "],";
		PrintHex(line1,0x40);

		if (grpCase->ItemIndex)
		{
			line2 += "A,[[";
			PrintHex(line2,0x18);
			line2 += "]+Y]";
		}
		else
		{
			line2 += "a,[[";
			PrintHex(line2,0x18);
			line2 += "]+y]";
		}

		line3 += "0524";

		if (grpCase->ItemIndex)
			line4 += "C,~[";
		else
			line4 += "c,~[";

		PrintHex(line4, 0x1B15);
		line4 += "],5";
	}
	else
	{
		PrintHex(line1,0xF2);
		line1 += ",#";
		PrintHex(line1,0x40);

		if (grpCase->ItemIndex)
		{
			line2 += "A,[";
			PrintHex(line2,0x18);
			line2 += "]+Y";
		}
		else
		{
			line2 += "a,[";
			PrintHex(line2,0x18);
			line2 += "]+y";
		}

		line3+="0524";

		if (grpCase->ItemIndex)
			line4 += "C,/";
		else
			line4 += "c,/";

		PrintHex(line4, 0x1B15);
		line4 += ".5";
	}

	if (chkCmnt->Checked)
	{
		line1 += AnsiString::StringOfChar(' ', cmnt - line1.Length()) + ";F2->02";
		line2 += AnsiString::StringOfChar(' ', cmnt - line2.Length()) + ";18->1080->13";
		line3 += AnsiString::StringOfChar(' ', cmnt - line3.Length()) + ";-25";
		line4 += AnsiString::StringOfChar(' ', cmnt - line4.Length()) + ";1B15.5->1";
	}

	lstDisasm->Clear();
	lstDisasm->Items->Append(line1);
	lstDisasm->Items->Append(line2);
	lstDisasm->Items->Append(line3);
	lstDisasm->Items->Append(line4);

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::chkLinearClick(TObject *sender)
{
	if (chkLinear->Checked) chkSilence->Enabled = true;
	else
	{
		chkSilence->Checked = false;
		chkSilence->Enabled = false;
	}

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::trkBufChange(TObject *sender)
{
	lblBufSize->Caption = AnsiString((trkBuf->Position * 20) + 40) + "ms";
	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::cboHWChange(TObject *sender)
{
	b8	state;


	state = (cboHW->ItemIndex != 0);
	lblLPT->Enabled = state;
	cboLPT->Enabled = state;

	ItemChange(sender);
}


//**************************************************************************************************
void __fastcall TFrmConfig::chk50HzClick(TObject *sender)
{
	ItemChange(sender);
}


//**************************************************************************************************
// Copy Preset

static void CopyPreset(Config &cfg)
{
	switch(cfg.preset)
	{
	case(-1):
		cfg.dsp.rate		= cfg.udsp.rate;
		cfg.dsp.bits		= 16;
		cfg.dsp.mix			= MIX_FLOAT;
		cfg.dsp.inter		= 2;
		cfg.dsp.analog		= 0;
		cfg.dsp.filter		= 0;
		cfg.dsp.surround	= 0;
		cfg.dsp.reverse		= 0;
		cfg.dsp.disEcho		= 0;
		cfg.dsp.oldSmp		= 0;
		cfg.dsp.stereo		= 32768;
		cfg.dsp.efbct		= -16384;

		cfg.mix.aar			= 2;
		cfg.mix.reset		= 1;
		cfg.mix.amp			= 131072;
		cfg.mix.min			= 65536;
		cfg.mix.max			= 524288;
		cfg.mix.threshold	= 33148;
		break;

	case(-2):
		cfg.dsp.rate		= cfg.udsp.rate;
		cfg.dsp.bits		= 16;
		cfg.dsp.mix			= MIX_INT;
		cfg.dsp.inter		= 3;
		cfg.dsp.analog		= 1;
		cfg.dsp.filter		= 0;
		cfg.dsp.surround	= 0;
		cfg.dsp.reverse		= 0;
		cfg.dsp.oldSmp		= 0;
		cfg.dsp.disEcho		= 0;
		cfg.dsp.stereo		= 32768;
		cfg.dsp.efbct		= 32768;

		cfg.mix.aar			= 0;
		cfg.mix.reset		= 1;
		cfg.mix.amp			= 65536;
		cfg.mix.min			= 65536;
		cfg.mix.max			= 524288;
		cfg.mix.threshold	= 32768;
		break;

	case(0):
		memcpy(&cfg.dsp, &cfg.udsp, sizeof(cfg.dsp));
		memcpy(&cfg.mix, &cfg.umix, sizeof(cfg.mix));
		break;
	}
}


//**************************************************************************************************
// Load Configuration

void LoadConfig()
{
	s8	temp[16];
	s32	i;


	//SPCTool version --------------------------
	Cfg.ver				= GetPrivateProfileInt("SPCTool", "version", 0, Cfg.ini);
	Cfg.preset			= GetPrivateProfileInt("SPCTool", "preset", -1, Cfg.ini);
	if (Cfg.preset != -1 &&
		Cfg.preset != -2 &&
		Cfg.preset != 0) Cfg.preset = -1;


	//Recent file list -------------------------
	Cfg.curFile			= 0;
	for (i='8'; i>='0'; i--)
	{
		GetPrivateProfileString("Files", (s8*)&i, "", Cfg.file[Cfg.curFile], 256, Cfg.ini);
		if (Cfg.file[Cfg.curFile][0]) Cfg.curFile++;
	}
	Cfg.curFile--;

	//File list columns ------------------------
	GetPrivateProfileString("Files", "columns", "SGL", Cfg.col, 16, Cfg.ini);

	//File renamer -----------------------------
	GetPrivateProfileString("Rename", "format", "%N - %S", Cfg.renFmt, 64, Cfg.ini);

	//DSP emulation ----------------------------
	Cfg.udsp.rate		= GetPrivateProfileInt("DSP", "rate", 48000, Cfg.ini);
	Cfg.udsp.bits		= GetPrivateProfileInt("DSP", "bits", 16, Cfg.ini);
	Cfg.udsp.mix		= GetPrivateProfileInt("DSP", "quality", MIX_FLOAT, Cfg.ini);
						  GetPrivateProfileString("DSP", "inter", "cubic", temp, 16, Cfg.ini);
	Cfg.udsp.analog		= GetPrivateProfileInt("DSP", "hardware", 0, Cfg.ini) != 0;
	Cfg.udsp.filter		= GetPrivateProfileInt("DSP", "filter", 0, Cfg.ini) != 0;
	Cfg.udsp.surround	= GetPrivateProfileInt("DSP", "phase", 0, Cfg.ini) != 0;
	Cfg.udsp.reverse	= GetPrivateProfileInt("DSP", "reverse", 0, Cfg.ini) != 0;
	Cfg.udsp.oldSmp		= GetPrivateProfileInt("DSP", "oldadpcm", 0, Cfg.ini) != 0;
	Cfg.udsp.disEcho	= GetPrivateProfileInt("DSP", "noecho", 0, Cfg.ini) != 0;
	Cfg.udsp.stereo		= GetPrivateProfileInt("DSP", "separation", 32768, Cfg.ini);
	Cfg.udsp.efbct		= GetPrivateProfileInt("DSP", "efbct", -16384, Cfg.ini);

	_Clamp(Cfg.udsp.rate, 8000, 96000);
	if (Cfg.udsp.stereo > 65536) Cfg.udsp.stereo = 65536;
	_Clamp(Cfg.udsp.efbct, -32768, 32768);

	if (Cfg.udsp.bits != 16 && Cfg.udsp.bits != 32) Cfg.udsp.bits = 16;

	Cfg.udsp.inter = 0;
	while ((Cfg.udsp.inter < 4) && (stricmp(temp, interList[Cfg.udsp.inter]))) Cfg.udsp.inter++;

	//Mixer settings ---------------------------
						  GetPrivateProfileString("Mixer", "attenuation", "increase", temp, 16, Cfg.ini);
	Cfg.umix.amp		= GetPrivateProfileInt("Mixer", "amp", 131072, Cfg.ini);
	Cfg.umix.min		= GetPrivateProfileInt("Mixer", "min", 65536, Cfg.ini);
	Cfg.umix.max		= GetPrivateProfileInt("Mixer", "max", 524288, Cfg.ini);
	Cfg.umix.threshold	= GetPrivateProfileInt("Mixer", "threshold", 4013, Cfg.ini) + 32768;
	Cfg.umix.reset		= GetPrivateProfileInt("Mixer", "reset", 0, Cfg.ini) != 0;

	if (Cfg.umix.min > (1<<19)) Cfg.umix.min = 1<<19;
	_Clamp(Cfg.umix.max, Cfg.umix.min, 1<<19);
	_Clamp(Cfg.umix.amp, Cfg.umix.min, Cfg.umix.max);
	_Clamp(Cfg.umix.threshold, 16384, 65536);

	Cfg.umix.aar = 0;
	while ((Cfg.umix.aar < 2) && (stricmp(temp, attenList[Cfg.umix.aar]))) Cfg.umix.aar++;

	//Select preset settings -------------------
	CopyPreset(Cfg);

	//Time -------------------------------------
	Cfg.time.song		= GetPrivateProfileInt("Time", "song", 10880000, Cfg.ini);
	Cfg.time.fade		= GetPrivateProfileInt("Time", "fade", 640000, Cfg.ini);
	Cfg.time.skip		= GetPrivateProfileInt("Time", "frameskip", 4, Cfg.ini);
	Cfg.time.next		= GetPrivateProfileInt("Time", "advance", 0, Cfg.ini) != 0;
	Cfg.time.fastSeek	= GetPrivateProfileInt("Time", "fastseek", 0, Cfg.ini) != 0;
	Cfg.time.binary		= GetPrivateProfileInt("Time", "binary", 0, Cfg.ini) != 0;
	Cfg.time.remain		= GetPrivateProfileInt("Time", "remaining", 1, Cfg.ini) != 0;
	Cfg.time.enable		= GetPrivateProfileInt("Time", "enabled", 1, Cfg.ini) != 0;

	_Clamp(Cfg.time.song, 1, XID6_MAXTICKS);
	_Clamp(Cfg.time.fade, 1, XID6_TICKSMIN - 1);
    _Clamp(Cfg.time.fastSeek, 2, 16);

	ID666::defSong = Cfg.time.song;
	ID666::defFade = Cfg.time.fade;
	ID666::preferBin = Cfg.time.binary;

	//Sound source detection -------------------
	Cfg.src.min			= GetPrivateProfileInt("Source", "min", 2, Cfg.ini);
	Cfg.src.max			= GetPrivateProfileInt("Source", "max", 7168, Cfg.ini);
	Cfg.src.range		= GetPrivateProfileInt("Source", "range", 15, Cfg.ini);
	Cfg.src.linear		= GetPrivateProfileInt("Source", "linear", 1, Cfg.ini) != 0;
	Cfg.src.silence		= GetPrivateProfileInt("Source", "silence", 0, Cfg.ini) != 0;
	Cfg.src.noDP		= GetPrivateProfileInt("Source", "page", 0, Cfg.ini) != 0;

	_Clamp(Cfg.src.min, 1, 4096);
	_Clamp(Cfg.src.max, 2, 524288);
	if (Cfg.src.min > Cfg.src.max) Cfg.src.min = Cfg.src.max;

	_Clamp(Cfg.src.range, 12, 15);

	//Debugger ---------------------------------
						  GetPrivateProfileString("Debug", "syntax", "spctool", temp, 16, Cfg.ini);
	Cfg.debug.printPC	= GetPrivateProfileInt("Debug", "pc", 1, Cfg.ini) != 0;
	Cfg.debug.printOp	= GetPrivateProfileInt("Debug", "opcodes", 1, Cfg.ini) != 0;
	Cfg.debug.printCmnt	= GetPrivateProfileInt("Debug", "comments", 1, Cfg.ini) != 0;
						  GetPrivateProfileString("Debug", "hex", "h", &Cfg.debug.hex, 2, Cfg.ini);

	Cfg.debug.syntax = 0;
	while ((Cfg.debug.syntax < 1) && (stricmp(temp, syntaxList[Cfg.debug.syntax]))) Cfg.debug.syntax++;

						  GetPrivateProfileString("Debug", "case", "mixed", temp, 15, Cfg.ini);
	Cfg.debug.ltrCase = 0;
	while ((Cfg.debug.ltrCase < 2) && (stricmp(temp, caseList[Cfg.debug.ltrCase]))) Cfg.debug.ltrCase++;

	if (Cfg.debug.syntax)
	{
		Cfg.debug.abs = '\\';
		Cfg.debug.openBrace = '[';
		Cfg.debug.closeBrace = ']';
	}
	else
	{
		Cfg.debug.abs = '!';
		Cfg.debug.openBrace = '(';
		Cfg.debug.closeBrace = ')';
	}

	Cfg.debug.cmntCol = 24 + (Cfg.debug.printPC ? 5 : 0) + (Cfg.debug.printOp ? 10 : 0);

	if (Cfg.debug.ltrCase)
		memcpy(Cfg.debug.hexChar,"0123456789ABCDEF",16);
	else
		memcpy(Cfg.debug.hexChar,"0123456789abcdef",16);

	//Devices ----------------------------------
	Cfg.out.device		= GetPrivateProfileInt("Device", "output", -1, Cfg.ini);
	Cfg.out.size		= GetPrivateProfileInt("Device", "buffer", 60, Cfg.ini);
	Cfg.out.force		= GetPrivateProfileInt("Device", "force", 0, Cfg.ini) != 0;
	Cfg.out.snes		= GetPrivateProfileInt("Device", "snes", 0, Cfg.ini);
	Cfg.out.port		= GetPrivateProfileInt("Device", "lpt", 1, Cfg.ini) - 1;

	_Clamp(Cfg.out.size, 40, 140);
	if (Cfg.out.force) Cfg.out.force = WAVE_FORMAT_DIRECT;
	if (Cfg.out.snes > 2) Cfg.out.snes = 0;
	if (Cfg.out.port > 2) Cfg.out.port = 0;
	Cfg.out.port = portList[Cfg.out.port];

	//Visualization ----------------------------
	Cfg.vis.fps			= GetPrivateProfileInt("Visualize", "fps", 50, Cfg.ini);
	Cfg.vis.vu			= GetPrivateProfileInt("Visualize", "vu", 1, Cfg.ini) != 0;
	Cfg.vis.scope		= GetPrivateProfileInt("Visualize", "scope", 1, Cfg.ini) != 0;

	if (Cfg.vis.fps >= 50) Cfg.vis.fps = 50;
	else Cfg.vis.fps = 25;

	//Select preset settings -------------------
	CopyPreset(Cfg);
}


//**************************************************************************************************
// Save Configuration

#define	_WriteCfgI(v,a,k) \
		if (newcfg.v != Cfg.v) \
		{ \
			Cfg.v = newcfg.v; \
			sprintf(str, "%i", Cfg.v); \
			WritePrivateProfileString(a, k, str, Cfg.ini); \
			changed = 1; \
		}

#define	_WriteCfgS(v,a,k,s) \
		if (newcfg.v != Cfg.v) \
		{ \
			Cfg.v = newcfg.v; \
			WritePrivateProfileString(a, k, s, Cfg.ini); \
			changed = 1; \
		}

void __fastcall TFrmConfig::SaveConfig()
{
	s8		str[16];
	Config	newcfg;
	b8		changed,reset;
	u32		opt;


	switch(grpPreset->ItemIndex)
	{
	case(0): newcfg.preset = -1; break;
	case(1): newcfg.preset = -2; break;
	case(2): newcfg.preset = 0; break;
	}

	//Get new configuration --------------------
	if (!newcfg.preset)
	{
		newcfg.udsp.rate		= cboRate->Text.ToIntDef(48000);
		newcfg.udsp.bits		= cboBits->ItemIndex ? 32 : 16;
		newcfg.udsp.mix			= chkMix->Checked ? MIX_FLOAT : MIX_INT;
		newcfg.udsp.inter		= cboInter->ItemIndex;
		newcfg.udsp.analog		= chkLow->Checked;
		newcfg.udsp.filter		= chkFilter->Checked;
		newcfg.udsp.surround	= chkSurr->Checked;
		newcfg.udsp.reverse		= +chkReverse->Checked;
		newcfg.udsp.disEcho		= chkNoEcho->Checked;
		newcfg.udsp.oldSmp		= chkADPCM->Checked;
		newcfg.udsp.stereo		= (trkStereo->Position << 16) / 100;
		newcfg.udsp.efbct		= (trkEcho->Position << 15) / 100;

		newcfg.umix.aar			= grpAAR->ItemIndex;
		newcfg.umix.amp			= trkAmp->Tag;
		newcfg.umix.min			= 65536.0 * pow(2.0, trkAmp->SelStart / 60.0);
		newcfg.umix.max			= 65536.0 * pow(2.0, trkAmp->SelEnd / 60.0);
		newcfg.umix.threshold	= trkThresh->Tag - 32768;
		newcfg.umix.reset		= chkReset->Checked;
	}
	else
	{
		memcpy(&newcfg.udsp, &udsp, sizeof(udsp));
		memcpy(&newcfg.umix, &umix, sizeof(umix));
	}

	newcfg.time.song		= txtSongMin->Text.ToIntDef(0) * XID6_TICKSMIN;
	try
	{
		newcfg.time.song 	+= txtSongSec->Text.ToDouble() * XID6_TICKSSEC;
		newcfg.time.fade	= txtFade->Text.ToDouble() * XID6_TICKSSEC;
	}
	catch(...) {}
	newcfg.time.skip		= trkFF->Position;
	newcfg.time.next		= chkNext->Checked;
	newcfg.time.fastSeek	= chkFast->Checked;
	newcfg.time.binary		= chkBin->Checked;

	newcfg.src.min			= txtMinSize->Tag >> 4;
	newcfg.src.max			= txtMaxSize->Tag >> 4;
	newcfg.src.linear		= chkLinear->Checked;
	newcfg.src.silence		= chkSilence->Checked;
	newcfg.src.range		= chkRange->Checked ? 12 : 15;
	newcfg.src.noDP			= chkDP->Checked ? 2 : 0;

	newcfg.debug.syntax		= grpSyn->ItemIndex;
	newcfg.debug.ltrCase	= grpCase->ItemIndex;
	switch(grpHex->ItemIndex)
	{
	case(1):
		newcfg.debug.hex	= 'h';
		break;

	case(2):
		newcfg.debug.hex	= 'x';
		break;

	default:
		newcfg.debug.hex	= '$';
	}
	newcfg.debug.printPC	= chkPC->Checked;
	newcfg.debug.printOp	= chkOpcode->Checked;
	newcfg.debug.printCmnt	= chkCmnt->Checked;

	newcfg.out.device		= cboDev->ItemIndex	- 1;
	newcfg.out.size			= trkBuf->Position * 20 + 40;
	newcfg.out.force		= chkForce->Checked ? WAVE_FORMAT_DIRECT : 0;
	newcfg.out.snes			= cboHW->ItemIndex;
	newcfg.out.port			= cboLPT->ItemIndex + 1;

	newcfg.vis.vu			= grpVis->ItemIndex != 0;
	newcfg.vis.fps			= (grpVis->ItemIndex == 2) ? 50 : 25;

	//Write configuration out to ini file ------
	changed = 0;
	_WriteCfgI(preset,			"SPCTool", "preset");
	opt = reset = changed; changed = 0;
	
	_WriteCfgI(udsp.rate,		"DSP", "rate");
	_WriteCfgI(udsp.bits,		"DSP", "bits");
	if (!newcfg.preset)
	{
		opt |= changed;
		reset |= changed;
	}
	changed = 0;
	_WriteCfgI(udsp.mix,		"DSP", "quality");
	_WriteCfgS(udsp.inter,		"DSP", "inter", interList[Cfg.udsp.inter]);
	_WriteCfgI(udsp.analog,		"DSP", "hardware");
	_WriteCfgI(udsp.filter,		"DSP", "filter");
	_WriteCfgI(udsp.surround,	"DSP", "phase");
	_WriteCfgI(udsp.reverse,	"DSP", "reverse");
	_WriteCfgI(udsp.oldSmp,		"DSP", "oldsmp");
	_WriteCfgI(udsp.disEcho,	"DSP", "noecho");
	opt |= changed && !newcfg.preset; changed = 0;
	_WriteCfgI(udsp.stereo,		"DSP", "separation");
	_WriteCfgI(udsp.efbct,		"DSP", "efbct");

	if (newcfg.umix.aar != Cfg.umix.aar)
	{
		Cfg.umix.aar = newcfg.umix.aar;
		WritePrivateProfileString("Mixer", "attenuation", attenList[Cfg.umix.aar], Cfg.ini);
	}
	_WriteCfgI(umix.amp,		"Mixer", "amp");
	_WriteCfgI(umix.min,		"Mixer", "min");
	_WriteCfgI(umix.max,		"Mixer", "max");
	_WriteCfgI(umix.threshold,	"Mixer", "threshold");
	_WriteCfgI(umix.reset,		"Mixer", "reset");
	Cfg.umix.threshold += 32768;

	_WriteCfgI(time.song,		"Time", "song");
	_WriteCfgI(time.fade,		"Time", "fade");
	_WriteCfgI(time.skip,		"Time", "frameskip");
	_WriteCfgI(time.next,		"Time", "advance");
	_WriteCfgI(time.fastSeek,	"Time", "fastseek");
	_WriteCfgI(time.binary,		"Time", "binary");

	_WriteCfgI(src.min,			"Source", "min");
	_WriteCfgI(src.max,			"Source", "max");
	_WriteCfgI(src.range,		"Source", "range");
	_WriteCfgI(src.linear,		"Source", "linear");
	_WriteCfgI(src.silence,		"Source", "silence");
	_WriteCfgI(src.noDP,		"Source", "page");

	_WriteCfgS(debug.syntax,	"Debug", "syntax", syntaxList[Cfg.debug.syntax]);
	_WriteCfgI(debug.printPC,	"Debug", "pc");
	_WriteCfgI(debug.printOp,	"Debug", "opcodes");
	_WriteCfgI(debug.printCmnt,	"Debug", "comments");
	_WriteCfgS(debug.ltrCase,	"Debug", "case", caseList[Cfg.debug.ltrCase]);

	changed = 0;
	_WriteCfgI(out.device,		"Device", "output");
	_WriteCfgI(out.size,		"Device", "buffer");
	if (newcfg.out.force != (Cfg.out.force))
	{
		Cfg.out.force |= newcfg.out.force;
		WritePrivateProfileString("Device", "force", Cfg.out.force ? "1" : "0", Cfg.ini);
	}
	_WriteCfgI(out.snes,		"Device", "snes");
	_WriteCfgI(out.port,		"Device", "lpt");
	Cfg.out.port = portList[Cfg.out.port - 1];
	reset |= changed;

	_WriteCfgI(vis.fps,			"Visualize", "fps");
	_WriteCfgI(vis.vu,			"Visualize", "vu");

	//Change playback settings -----------------
	CopyPreset(Cfg);

	ID666::defSong = Cfg.time.song;
	ID666::defFade = Cfg.time.fade;
	ID666::preferBin = Cfg.time.binary;

	if (reset)
	{
		reset = IsWavOpen();
		WavReset();
	}

	if (opt)
	{
		Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());
	}

	Apu.SetDSPAAR(Cfg.mix.aar, Cfg.mix.threshold, Cfg.mix.min, Cfg.mix.max);
	Apu.SetDSPStereo(Cfg.dsp.stereo);
	Apu.SetDSPEFBCT(Cfg.dsp.efbct);

	if (reset) WavStart();
}

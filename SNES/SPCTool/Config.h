/***************************************************************************************************
* Form: Configuration Dialog                                                                       *
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
*                                                           Copyright (C)2001 Alpha-II Productions *
***************************************************************************************************/

#ifndef ConfigH
#define ConfigH

//**************************************************************************************************
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>

//**************************************************************************************************
class TFrmConfig:public TForm
{
__published:	// IDE-managed Components
	TPageControl *pgcCfg;
	TTabSheet *tabDSP;
	TTabSheet *tabTime;
	TTabSheet *tabMixing;
	TTabSheet *tabDebug;
	TComboBox *cboRate;
	TButton *btnHelp;
	TButton *btnOK;
	TButton *btnCancel;
	TComboBox *cboBits;
	TComboBox *cboInter;
	TLabel *lblRate;
	TLabel *lblBits;
	TLabel *lblInter;
	TLabel *lblTEcho;
	TTrackBar *trkEcho;
	TEdit *txtSongMin;
	TEdit *txtSongSec;
	TEdit *txtFade;
	TLabel *lblSep;
	TLabel *lblSong;
	TLabel *lblFade;
	TCheckBox *chkFast;
	TRadioGroup *grpAAR;
	TTrackBar *trkAmp;
	TTrackBar *trkThresh;
	TLabel *lblTAmp;
	TLabel *lblTThresh;
	TLabel *lblAmp;
	TLabel *lblThresh;
	TLabel *lblDebug;
	TBevel *bvlDebug;
	TBevel *bvlMixing;
	TLabel *lblMixing;
	TBevel *bvlTime;
	TLabel *lblTime;
	TTabSheet *tabDev;
	TLabel *lblEcho;
	TCheckBox *chkBin;
	TEdit *txtMinAmp;
	TEdit *txtMaxAmp;
	TButton *btnApply;
	TCheckBox *chkReset;
	TLabel *lblTStereo;
	TLabel *lblStereo;
	TTrackBar *trkStereo;
	TPopupMenu *popHelp;
	TMenuItem *itmWhat;
	TListBox *lstDisasm;
	TCheckBox *chkPC;
	TCheckBox *chkOpcode;
	TRadioGroup *grpCase;
	TCheckBox *chkCmnt;
	TRadioGroup *grpHex;
	TTabSheet *tabSrc;
	TCheckBox *chkLinear;
	TCheckBox *chkSilence;
	TCheckBox *chkRange;
	TBevel *bvlWave;
	TLabel *lblSrc;
	TEdit *txtMinSize;
	TEdit *txtMaxSize;
	TLabel *lblMin;
	TLabel *lblMax;
	TRadioGroup *grpSyn;
	TCheckBox *chkDP;
	TLabel *lblOut;
	TBevel *bvlOut;
	TComboBox *cboDev;
	TLabel *lblDev;
	TTrackBar *trkBuf;
	TLabel *lblBuf;
	TLabel *lblBufSize;
	TCheckBox *chkSurr;
	TCheckBox *chkReverse;
	TCheckBox *chkADPCM;
	TCheckBox *chkNoEcho;
	TCheckBox *chkForce;
	TLabel *lblFFT;
	TLabel *lblFF;
	TTrackBar *trkFF;
	TCheckBox *chkNext;
	TLabel *lblHW;
	TBevel *bvlHW;
	TLabel *lblLPT;
	TComboBox *cboLPT;
	TTabSheet *TabOther;
	TLabel *lblCfg;
	TBevel *bvlReset;
	TLabel *lblReset;
	TButton *btnReset;
	TLabel *lblVis;
	TBevel *bvlVis;
	TLabel *lblHWT;
	TComboBox *cboHW;
	TRadioGroup *grpPreset;
	TCheckBox *chkInit;
	TCheckBox *chkLow;
	TCheckBox *chkMix;
	TRadioGroup *grpVis;
	TCheckBox *chkFilter;
	TLabel *lblTMin;
	TLabel *lblTMax;
	void __fastcall btnCancelClick(TObject *sender);
	void __fastcall btnOKClick(TObject *sender);
	void __fastcall trkEchoChange(TObject *sender);
	void __fastcall trkThreshChange(TObject *sender);
	void __fastcall trkAmpChange(TObject *sender);
	void __fastcall FormShow(TObject *sender);
	void __fastcall cboRateExit(TObject *sender);
	void __fastcall txtSongMinExit(TObject *sender);
	void __fastcall CheckTime(TObject *sender);
	void __fastcall txtMinAmpExit(TObject *sender);
	void __fastcall txtMaxAmpExit(TObject *sender);
	void __fastcall btnApplyClick(TObject *sender);
	void __fastcall trkStereoChange(TObject *sender);
	void __fastcall FormCreate(TObject *sender);
	void __fastcall btnHelpClick(TObject *sender);
	void __fastcall itmWhatClick(TObject *sender);
	void __fastcall ItemChange(TObject *sender);
	void __fastcall lstDisasmChange(TObject *sender);
	void __fastcall chkLinearClick(TObject *sender);
	void __fastcall trkBufChange(TObject *sender);
	void __fastcall btnResetClick(TObject *sender);
	void __fastcall trkFFChange(TObject *sender);
	void __fastcall chk50HzClick(TObject *sender);
	void __fastcall txtMinSizeExit(TObject *sender);
	void __fastcall txtMaxSizeExit(TObject *sender);
	void __fastcall grpAARClick(TObject *sender);
	void __fastcall cboHWChange(TObject *sender);
	void __fastcall grpPresetClick(TObject *Sender);
	void __fastcall chkMixClick(TObject *Sender);
	void __fastcall chkLowClick(TObject *Sender);

private:	// User declarations
	CfgDSP	udsp;
	CfgMix	umix;

	void	__fastcall SaveConfig();
	void	__fastcall PrintHex(AnsiString &line, s32 b);

public:		// User declarations
	__fastcall TFrmConfig(TComponent* owner);

	//***********************************************************************************************
	// Reinitialize Form
	//
	// Desc:
	//    Initializes data affected by changing the language resource module

	void	__fastcall Reinit();
};

void	LoadConfig();

extern PACKAGE TFrmConfig *FrmConfig;
#endif


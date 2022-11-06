/***************************************************************************************************
* Form: DSP Register Editor                                                                        *
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

#ifndef DSPRegH
#define DSPRegH

#include	<Classes.hpp>
#include	<Controls.hpp>
#include	<StdCtrls.hpp>
#include	<Forms.hpp>
#include	<ComCtrls.hpp>
#include	<ExtCtrls.hpp>

class TFrmDSP : public TForm
{
__published:	// IDE-managed Components

	TTabControl	*tabVoice;
	TPanel		*pnlVoice;
	TLabel		*lblVolT;
	TLabel		*lblPitch;
	TLabel		*lblSrc;
	TLabel		*lblEnvX;
	TLabel		*lblOutX;
	TEdit		*txtVolL;
	TEdit		*txtVolR;
	TEdit		*txtPitch;
	TEdit		*txtSrc;
	TEdit		*txtEnvX;
	TEdit		*txtOutX;

	TCheckBox	*chkADSR;
	TLabel		*lblART;
	TLabel		*lblDRT;
	TLabel		*lblSLT;
	TLabel		*lblSRT;
	TTrackBar	*trkAR;
	TTrackBar	*trkDR;
	TTrackBar	*trkSL;
	TTrackBar	*trkSR;
	TLabel		*lblAR;
	TLabel		*lblSL;
	TLabel		*lblSR;
	TLabel		*lblDR;

	TGroupBox		*grpGain;
	TRadioButton	*rdoDirect;
	TRadioButton	*rdoInc;
	TRadioButton	*rdoBent;
	TRadioButton	*rdoDec;
	TRadioButton	*rdoExp;
	TLabel			*lblGain;
	TTrackBar		*trkGain;

	TGroupBox	*grpFlg;
	TCheckBox	*chkRES;
	TCheckBox	*chkMUTE;
	TCheckBox	*chkECEN;
	TTrackBar	*trkNCK;
	TLabel		*lblNCK;

	TLabel		*lblKON;
	TLabel		*lblKOF;
	TButton		*btnKON0;
	TButton		*btnKON1;
	TButton		*btnKON2;
	TButton		*btnKON3;
	TButton		*btnKON4;
	TButton		*btnKON5;
	TButton		*btnKON6;
	TButton		*btnKON7;
	TButton		*btnKOF0;
	TButton		*btnKOF1;
	TButton		*btnKOF2;
	TButton		*btnKOF3;
	TButton		*btnKOF4;
	TButton		*btnKOF5;
	TButton		*btnKOF6;
	TButton		*btnKOF7;

	TLabel		*lblEON;
	TLabel		*lblPMON;
	TLabel		*lblNON;
	TButton		*btnENDX;
	TCheckBox	*chkEON0;
	TCheckBox	*chkEON1;
	TCheckBox	*chkEON2;
	TCheckBox	*chkEON3;
	TCheckBox	*chkEON4;
	TCheckBox	*chkEON5;
	TCheckBox	*chkEON6;
	TCheckBox	*chkEON7;
	TCheckBox	*chkPMON0;
	TCheckBox	*chkPMON1;
	TCheckBox	*chkPMON2;
	TCheckBox	*chkPMON3;
	TCheckBox	*chkPMON4;
	TCheckBox	*chkPMON5;
	TCheckBox	*chkPMON6;
	TCheckBox	*chkPMON7;
	TCheckBox	*chkNON0;
	TCheckBox	*chkNON1;
	TCheckBox	*chkNON2;
	TCheckBox	*chkNON3;
	TCheckBox	*chkNON4;
	TCheckBox	*chkNON5;
	TCheckBox	*chkNON6;
	TCheckBox	*chkNON7;
	TCheckBox	*chkENDX0;
	TCheckBox	*chkENDX1;
	TCheckBox	*chkENDX2;
	TCheckBox	*chkENDX3;
	TCheckBox	*chkENDX4;
	TCheckBox	*chkENDX5;
	TCheckBox	*chkENDX6;
	TCheckBox	*chkENDX7;

	TBevel		*bvlSep2;

	TLabel		*lblMVol;
	TLabel		*lblEVol;
	TLabel		*lblMVolL;
	TLabel		*lblMVolR;
	TLabel		*lblEVolL;
	TLabel		*lblEVolR;
	TTrackBar	*trkMVolL;
	TTrackBar	*trkMVolR;
	TTrackBar	*trkEVolL;
	TTrackBar	*trkEVolR;

	TLabel		*lblDIR;
	TEdit		*txtDIR;
	TLabel		*lblESA;
	TEdit		*txtESA;

	TLabel		*lblEDLT;
	TLabel		*lblEFBT;
	TTrackBar	*trkEDL;
	TTrackBar	*trkEFB;
	TLabel		*lblEFB;
	TLabel		*lblEDL;

	TLabel		*lblFIRT;
	TLabel		*lblFIR0;
	TLabel		*lblFIR1;
	TLabel		*lblFIR2;
	TLabel		*lblFIR3;
	TLabel		*lblFIR4;
	TLabel		*lblFIR5;
	TLabel		*lblFIR6;
	TLabel		*lblFIR7;
	TTrackBar	*trkFIR0;
	TTrackBar	*trkFIR1;
	TTrackBar	*trkFIR2;
	TTrackBar	*trkFIR3;
	TTrackBar	*trkFIR4;
	TTrackBar	*trkFIR5;
	TTrackBar	*trkFIR6;
	TTrackBar	*trkFIR7;

	TButton		*btnRelease;
	TBevel		*bvlSpc;
	TLabel *lblV0;
	TLabel *lblV1;
	TLabel *lblV2;
	TLabel *lblV3;
	TLabel *lblV4;
	TLabel *lblV5;
	TLabel *lblV6;
	TLabel *lblV7;

	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall FormShow(TObject *sender);
	void	__fastcall FormClose(TObject *sender, TCloseAction &action);
	void	__fastcall FormKeyDown(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall FormKeyPress(TObject *sender, char &key);
	void	__fastcall FormKeyUp(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall btnReleaseClick(TObject *sender);
	void	__fastcall tabVoiceChange(TObject *sender);
	void	__fastcall txtVolKeyDown(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall txtVolChange(TObject *sender);
	void	__fastcall txtPitchKeyDown(TObject *sender, WORD &jey, TShiftState shift);
	void	__fastcall txtPitchChange(TObject *sender);
	void	__fastcall txtSrcChange(TObject *sender);
	void	__fastcall chkADSRClick(TObject *sender);
	void	__fastcall trkARChange(TObject *sender);
	void	__fastcall trkDRChange(TObject *sender);
	void	__fastcall trkSLChange(TObject *sender);
	void	__fastcall trkSRChange(TObject *sender);
	void	__fastcall grpGainClick(TObject *sender);
	void	__fastcall trkGainChange(TObject *sender);
	void	__fastcall trkNCKChange(TObject *sender);
	void	__fastcall btnKeyMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y);
	void	__fastcall chkSwitchClick(TObject *sender);
	void	__fastcall btnENDXClick(TObject *sender);
	void	__fastcall trkVolChange(TObject *sender);
	void	__fastcall trkEDLChange(TObject *sender);
	void	__fastcall trkEFBChange(TObject *sender);
	void	__fastcall trkFIRChange(TObject *sender);

private:
	void	__fastcall UpdateReg(u8 reg);

public:
			__fastcall TFrmDSP(TComponent* Owner);

	//**********************************************************************************************
	// Reset DSP Editor
	//
	// Resets all controls to match the current state of the DSP

	void	__fastcall Reset();

	//**********************************************************************************************
	// Update DSP Editor
	//
	// Notifies the editor to update any controls for registers that have changed

	void	__fastcall Update();
};

extern PACKAGE TFrmDSP *FrmDSP;
#endif


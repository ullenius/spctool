/***************************************************************************************************
* Form: Sound Source Options Editor                                                                *
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

#ifndef	GMListH
#define	GMListH

#include	<Classes.hpp>
#include	<Controls.hpp>
#include	<StdCtrls.hpp>
#include	<Forms.hpp>
#include	<ExtCtrls.hpp>

class TFrmSSI : public TForm
{
__published:	// IDE-managed Components
	TLabel		*lblRate;
	TEdit		*txtRate;
	TRadioGroup	*grpType;
	TListBox	*lstGroup;
	TListBox	*lstInstr;

	TBevel		*bvlSep;
	TLabel		*lblOfs;
	TLabel		*lblLoop;
	TEdit		*txtOfs;
	TEdit		*txtLoop;

	TButton		*btnOK;
	TButton		*btnCancel;

	void __fastcall btnOKClick(TObject *sender);
	void __fastcall lstGroupClick(TObject *sender);
	void __fastcall FormShow(TObject *sender);
	void __fastcall grpTypeClick(TObject *sender);
	void __fastcall txtRateExit(TObject *sender);
	void __fastcall txtOfsExit(TObject *Sender);
	void __fastcall txtLoopExit(TObject *Sender);

private:

public:
	u32	prog;
	__fastcall TFrmSSI(TComponent* Owner);
};

extern const s8	ProgLst[16][8][18];
extern const s8	DrumLst[11][12][20];
extern PACKAGE TFrmSSI *FrmSSI;
#endif


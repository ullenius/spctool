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

#ifndef RenameH
#define RenameH

#include	<Classes.hpp>
#include	<Controls.hpp>
#include	<StdCtrls.hpp>
#include	<Forms.hpp>
#include <ComCtrls.hpp>

class TFrmRen : public TForm
{
__published:
	TButton		*btnClear;
	TButton		*btnRen;
	TButton		*btnCancel;
	TListView *lvwFile;
	TGroupBox *grpFormat;
	TLabel *lblSong;
	TLabel *lblGame;
	TLabel *lblArtist;
	TLabel *lblOST;
	TLabel *lblDisc;
	TLabel *lblTrack;
	TEdit *txtFormat;
	TButton *btnPre;

	void	__fastcall FormCreate(TObject *Sender);
	void	__fastcall FormHide(TObject *Sender);
	void	__fastcall btnClearClick(TObject *Sender);
	void	__fastcall btnPreClick(TObject *Sender);
	void	__fastcall btnRenClick(TObject *sender);
	void __fastcall FormShow(TObject *Sender);

private:
	void	__fastcall CreateParams(TCreateParams &params);
	void	__fastcall WMDropFiles(TMessage &msg);

#pragma	warn -8027								//Turn off warning produced by BEGIN_MESSAGE_MAP
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_DROPFILES, TMessage, WMDropFiles)
END_MESSAGE_MAP(TForm)
#pragma	warn .8027

	void	__fastcall Rename(b8 rename);

public:
	__fastcall TFrmRen(TComponent* Owner);

	//**********************************************************************************************
	// Add File
	//
	// Adds a file to be renamed
	//
	// In:
	//    pID6 -> ID666 object of file to be renamed
	//    local = True, if ID666 object should be deleted after closing rename dialog

	void	__fastcall AddFile(ID666 *pID6, b8 local);
};

extern PACKAGE TFrmRen *FrmRen;
#endif


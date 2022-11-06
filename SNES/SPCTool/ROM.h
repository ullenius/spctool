/***************************************************************************************************
* Form: ROM Creator                                                                                *
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


#ifndef	ROMH
#define	ROMH

#include	<Classes.hpp>
#include	<Controls.hpp>
#include	<StdCtrls.hpp>
#include	<Forms.hpp>
#include	<ComCtrls.hpp>
#include	<Dialogs.hpp>


class TFrmROM : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog	*dlgSave;
	TListView	*lvwFile;
	TCheckBox	*chkPad;
	TButton		*btnUpload;
	TButton		*btnSave;
	TButton		*btnCancel;

	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall FormShow(TObject *sender);
	void	__fastcall FormDestroy(TObject *sender);
	void	__fastcall lvwFileEdited(TObject *sender, TListItem *item, AnsiString &s);
	void	__fastcall btnUploadClick(TObject *sender);
	void	__fastcall btnSaveClick(TObject *sender);

private:

public:
			__fastcall TFrmROM(TComponent* owner);
	void	__fastcall Initialize();
	void	__fastcall AddFile(APUFile *pAPU);
};

extern PACKAGE TFrmROM *FrmROM;
#endif


/***************************************************************************************************
* Form: File Renamer                                                                               *
*                                                           Copyright (C)2002 Alpha-II Productions *
***************************************************************************************************/

#include	<vcl.h>
#include	"SPCTool.h"
#pragma hdrstop
#include	"Rename.h"

#pragma	package(smart_init)
#pragma	resource "*.dfm"

//**************************************************************************************************
// Data and Variables

typedef struct
{
	ID666	*pID6;								//-> ID666 tag
	b8		renamed;							//File has been renamed
	b8		local;								//Filename is local to dialog (delete pID6 on close)
} InTag;

static u32		inCount,inSize;					//Number of files, max number of files
static InTag	*inTags;						//-> array of tags
static s8		format[64],renFmt[64];			//Renaming format strings

TFrmRen *FrmRen;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

//**************************************************************************************************
__fastcall TFrmRen::TFrmRen(TComponent* owner) : TForm(owner)
{
}


//**************************************************************************************************
// Create Window Style Parameters

void __fastcall TFrmRen::CreateParams(TCreateParams &params)
{
	TForm::CreateParams(params);
	params.ExStyle |= WS_EX_ACCEPTFILES;		//This dialog accepts files being dropped onto it
}


//**************************************************************************************************
void __fastcall TFrmRen::FormCreate(TObject *Sender)
{
	HelpFile = Cfg.hlp;

	lvwFile->HelpContext	= HRen_List;
	txtFormat->HelpContext	= HRen_Format;
	btnPre->HelpContext		= HRen_Prev;
	btnClear->HelpContext	= HRen_Clear;
	
	if ((Cfg.lcid & 0x3FF) == LANG_JAPANESE)	//If default language is Japanese
	{
		lvwFile->Font->Charset = SHIFTJIS_CHARSET;
		lvwFile->Font->Name = "‚l‚r ‚oƒSƒVƒbƒN";
		lvwFile->Font->Size = 10;

		txtFormat->Font->Charset = SHIFTJIS_CHARSET;
		txtFormat->Font->Name = "‚l‚r ‚oƒSƒVƒbƒN";
		txtFormat->Font->Size = 10;
	}

	inCount = 0;
	inSize = 0;
	inTags = NULL;
}


//**************************************************************************************************
void __fastcall TFrmRen::FormShow(TObject *Sender)
{
	TListItem	*pItem;


	if (!inCount)
	{
		pItem = lvwFile->Items->Add();
		pItem->Caption = "Drag and drop files to be renamed";
	}

	lvwFile->Columns->Items[0]->Width = -1;
	lvwFile->Columns->Items[1]->Width = -2;
}


//**************************************************************************************************
// Free any allocated memory on exit

void __fastcall TFrmRen::FormHide(TObject *Sender)
{
	lvwFile->Items->Clear();
	lvwFile->Columns->Items[1]->Width = -2;

	if (inTags)
	{
		while (inCount--)
		{
			if (inTags[inCount].local) delete inTags[inCount].pID6;
		}

		free(inTags);
	}

	inCount = 0;
	inSize = 0;
	inTags = NULL;
}


//**************************************************************************************************
// Window's Message:  Drop Files - Sent when the user drops files on the dialog

void __fastcall TFrmRen::WMDropFiles(TMessage &msg)
{
	s8		str[256];
	HDROP	hdrop;
	ID666	*pID6;
	ID6Type	type;
	u32		i;


	pID6 = new ID666;

	hdrop = (HDROP)msg.WParam;
	i = DragQueryFile(hdrop, -1, NULL, 0);		//Get the number of files dropped
	while (i--)
	{
		DragQueryFile(hdrop, i, str, 255);		//Get a path and filename of a file
		type = pID6->LoadTag(str);				//Try to load the tag from the file
		if (type == ID6_SPC || type == ID6_EXT)
		{
			AddFile(pID6, 1);
			pID6 = new ID666;					//Create a new object for the next file
		}
	}

	delete pID6;								//Delete the last ID666 object that didn't get used
}


//**************************************************************************************************
void __fastcall TFrmRen::btnPreClick(TObject *Sender)
{
	Rename(0);
}


//**************************************************************************************************
void __fastcall TFrmRen::btnClearClick(TObject *Sender)
{
	lvwFile->Items->Clear();
	lvwFile->Columns->Items[1]->Width = -2;

	if (inTags)
	{
		while (inCount--)
		{
			if (inTags[inCount].local) delete inTags[inCount].pID6;
		}
	}

	inCount = 0;
}


//**************************************************************************************************
void __fastcall TFrmRen::btnRenClick(TObject *sender)
{
	u32	i;


	if (!CmpCopy(renFmt, txtFormat->Text.c_str()))
	{
		for (i=0; i<inCount; i++)				//If format string changed, rename ALL files
			inTags[i].renamed = 0;
	}

	Rename(1);
}


//**************************************************************************************************
void __fastcall TFrmRen::Rename(b8 rename)
{
	s8		out[1536],							//Output filename and path
			str[272];							//Message displayed in listbox
	s8		*s,*n,*f,*t,*fn;					//Various string pointers
	InTag	*pTag;
	ID666	*pID6;
	u32		i,j;
	u8		cpy;								//Flags denoting the failure of creating a new name


	lvwFile->Columns->Items[1]->Width = -1;
	strcpy(format, txtFormat->Text.c_str());

	for (i=0; i<inCount; i++)
	{
		pTag = (InTag*)lvwFile->Items->Item[i]->Data;
		pID6 = pTag->pID6;

		fn = ScanStrR(pID6->file, '\\');

		if (pTag->renamed)						//If file's already been renamed, just add to list
		{
			lvwFile->Items->Item[i]->SubItems->Text = fn;
			continue;
		}

		CopyStr(out,pID6->file);
		s = n = ScanStrR(out,'\\');

		cpy = 1;
		f = format;
		while (*f)
		{
			if (*f == '%')						//Is current character a format delimiter?
			{
				f += 2;							//Move pointer to first char after format specifier
				switch(f[-1] | 32)
				{
				case('s'):						//Song str
					if (pID6->song[0])
					{
						s = CopyStr(s,pID6->song);
						cpy &= ~1;				//If a field was copied, clear the first flag of cpy
					}
					else
						cpy |= 2;				//If nothing was copied, mark a missing field
					break;

				case('g'):						//Game str
					if (pID6->game[0])
					{
						s = CopyStr(s,pID6->game);
						cpy &= ~1;
					}
					else
						cpy |= 2;
					break;

				case('a'):						//Artist
					if (pID6->artist[0])
					{
						s = CopyStr(s,pID6->artist);
						cpy &= ~1;
					}
					else
						cpy |= 2;
					break;

				case('t'):						//OST title
					if (pID6->ost[0])
					{
						s = CopyStr(s,pID6->ost);
						cpy &= ~1;
					}
					else
						cpy |= 2;
					break;

				case('d'):						//OST disc
					if (pID6->disc)
					{
						s += sprintf(s,"%i",pID6->disc);
						cpy &= ~1;
					}
					else
					{
						t = ScanStrR(fn, '-');
						t -= 2;
						if (t >= fn && (*t >= '0' && *t <= '9'))
						{
							*s++ = *t;
							cpy &= ~1;
						}
						else
							cpy |= 2;
					}
					break;

				case('n'):						//OST track
					if (pID6->track)
					{
						if (pID6->track & 0xFF)
							s += sprintf(s,"%02i%c", pID6->track >> 8, pID6->track & 0xFF);
						else
							s += sprintf(s,"%02i", pID6->track >> 8);
						cpy &= ~1;
					}
					else
					{
						t = ScanStrR(fn, '-');
						if (t >= fn && (*t >= '0' && *t <= '9'))
						{
							if (strchr(t, ' '))
								s = CopyStrC(s, t, ' ');
							else
								s = CopyStrC(s, t, '.');
							cpy &= ~1;
						}
						else
							cpy |= 2;
					}
					break;

				case('%'):						//Percent sign
					*s++ = '%';
					break;

				default:						//Unknown format specifier
					f--;
					*s++ = '%';
				}
			}
			else
			{
				//Copy characters upto next delimiter
				j = CopyStrC(s, f, '%') - s;
				s += j;
				f += j;
			}
		}

		switch(cpy)
		{
		case(0):	//New filename was created successfully.  Try to rename.
			CopyStr(s,".spc");					//Append .spc extension

			if (strlen(out) > 255)
			{
				strcpy(str,"Path is more than 255 characters");
				break;
			}

			s = n;
			while (*n)	//Replace invalid characters with an underscore
			{
				if (*n < ' ' || strchr("\\/:*?\"<>|",*n)) *n = '_';
				n++;
			}

			if (rename)
			{
				if (MoveFile(pID6->file, out))	//Try to rename file
				{
					strcpy(pID6->file, out);	//If successful, copy new name to tag
					pTag->renamed = 1;
					lvwFile->Items->Item[i]->SubItems->Text = s;
				}
				else
					lvwFile->Items->Item[i]->SubItems->Text = AnsiString("[Error] ") + s;
			}
			else
				lvwFile->Items->Item[i]->SubItems->Text = s;

			break;

		case(1):	//Format string didn't contain any valid fields
			ShowMessage("The format string must contain at least one field identifier, such as %S.");
			return;

		case(2):	//Some items weren't in the ID666 tag
			lvwFile->Items->Item[i]->SubItems->Text = "[Error] ID666 tag is missing some fields";
			break;

		case(3):	//No items were in the ID666 tag
			lvwFile->Items->Item[i]->SubItems->Text = "[Error] ID666 tag does not contain any information";
			break;
		}
	} while (i--);
}


//**************************************************************************************************
// Add File

void __fastcall TFrmRen::AddFile(ID666 *pID6, b8 local)
{
	TListItem	*pItem;


	//Allocate (more) space for files ----------
	if (inCount == inSize)
	{
		if (!inCount) lvwFile->Items->Clear();

		inSize += 256;
		inTags = (InTag*)realloc(inTags, inSize * sizeof(InTag));
	}

	//Add tag to list --------------------------
	pItem = lvwFile->Items->Add();
	pItem->Caption = ScanStrR(pID6->file, '\\');
	pItem->Data = (void*)&inTags[inCount];

	inTags[inCount].pID6 = pID6;
	inTags[inCount].local = local;
	inTags[inCount].renamed = 0;
	inCount++;
}


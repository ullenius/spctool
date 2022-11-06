//**************************************************************************************************

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	<math.h>
#include	"Assembly.h"
#include	"Tag.h"
#include	"WaveOut.h"
#include	"A2Math.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


////////////////////////////////////////////////////////////////////////////////////////////////////

class ID6Time
{
private:
	s32		min,sec;

	void	Print();

public:
	TEdit	*txtMin,*txtSec;
	u32		ticks;
	b8		noFrac;
	b8		neg;

	static b8	printTicks;						//True, to print ticks instead of milliseconds

	void	SetTime(u32 ticks);
	void	VerifyMin();
	void	VerifySec();
};


b8	ID6Time::printTicks;

static ID6Time	Intro,Loop,End,Fade;
static ID6Time	*Times[4];


//**************************************************************************************************

static ID6Field		Field[EN_TOTAL];
static TCheckBox*	ChkMute[8];					//Pointers to mute checkboxes
static RECT			MuteFocus;					//Dimensions of focus rectangle around mute checks
static b8			PBState;					//State to switch buttons to as the mouse is dragged
static b8			KeyProcessed,DateChanged;
static b8			CheckExt;					//False, truncate fields when chkExt is unchecked

static AnsiString	OldDate;					//Old date text, used as an undo

static struct									//Contains the binary values of fields that are
{												// stored as binary in the tag.  Used to compare
//TODO
//	A2Date	date;								// with the original tag on exit to see if the
	u32	  	copy;								// fields were changed.
	u32	  	track;
	u32		mute;
	u32		loopx;
	s32	  	amp;
	u8	  	disc;
	s8	  	emu;
	b8		ext;								//Tag contains extended information
} Edit;

static TPoint	TriUp[4];						//A triangle pointing up
static TPoint	TriDn[4];						//A triangle pointing down

static TStringList	*DiffList[10];				//Pointers to string lists contaning the different
												// items in a field when editing multiple tags
TFrmTag *FrmTag;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

void __fastcall HotKey(WORD key, TShiftState shift);
void __fastcall SeekSong(u32 time);
void __fastcall SetAPULength();


//**************************************************************************************************
__fastcall TFrmTag::TFrmTag(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmTag::FormCreate(TObject *sender)
{
	u32	i;


	HelpFile = Cfg.hlp;

	//Context help ID's for each field ---------
	txtSong->HelpContext		= HID6_Song;
	txtGame->HelpContext		= HID6_Game;
	txtArtist->HelpContext		= HID6_Artist;
	txtPub->HelpContext			= HID6_Pub;
	txtCopy->HelpContext		= HID6_Copy;
	txtOST->HelpContext			= HID6_OST;
	txtDisc->HelpContext		= HID6_Disc;
	txtTrack->HelpContext		= HID6_Track;
	mmoCmnt->HelpContext		= HID6_Cmnt;
	txtDump->HelpContext		= HID6_Dump;
	txtDate->HelpContext		= HID6_Date;
	cboEmu->HelpContext			= HID6_Emu;
	chkMute1->HelpContext		= HID6_Mute;
	chkMute2->HelpContext		= HID6_Mute;
	chkMute3->HelpContext		= HID6_Mute;
	chkMute4->HelpContext		= HID6_Mute;
	chkMute5->HelpContext		= HID6_Mute;
	chkMute6->HelpContext		= HID6_Mute;
	chkMute7->HelpContext		= HID6_Mute;
	chkMute8->HelpContext		= HID6_Mute;
	txtAmp->HelpContext			= HID6_Amp;
	txtIntroMin->HelpContext	= HID6_Intro;
	txtIntroSec->HelpContext	= HID6_Intro;
	txtLoopMin->HelpContext		= HID6_Loop;
	txtLoopSec->HelpContext		= HID6_Loop;
	txtLoopNum->HelpContext		= HID6_LoopX;
	txtEndMin->HelpContext		= HID6_End;
	txtEndSec->HelpContext		= HID6_End;
	txtFadeSec->HelpContext		= HID6_Fade;
	txtTimeMin->HelpContext		= HID6_Time;
	txtTimeSec->HelpContext		= HID6_Time;
	chkExt->HelpContext			= HID6_Ext;
	chkBin->HelpContext			= HID6_Bin;
	btnID6->HelpContext			= HID6_ID6;

	//String lists -----------------------------
	strSong = new TStringList;
	strSong->Sorted				= true;
	strSong->Duplicates			= dupIgnore;
	pbSongD->Tag				= (s32)txtSong;
	DiffList[txtSong->Tag - 1]	= strSong;

	strGame = new TStringList;
	strGame->Sorted				= true;
	strGame->Duplicates			= dupIgnore;
	pbGameD->Tag				= (s32)txtGame;
	DiffList[txtGame->Tag - 1]	= strGame;

	strArtist = new TStringList;
	strArtist->Sorted			= true;
	strArtist->Duplicates		= dupIgnore;
	pbArtistD->Tag				= (s32)txtArtist;
	DiffList[txtArtist->Tag - 1] = strArtist;

	strPub = new TStringList;
	strPub->Sorted				= true;
	strPub->Duplicates			= dupIgnore;
	pbPubD->Tag					= (s32)txtPub;
	DiffList[txtPub->Tag - 1]	= strPub;

	strOST = new TStringList;
	strOST->Sorted				= true;
	strOST->Duplicates			= dupIgnore;
	pbOSTD->Tag					= (s32)txtOST;
	DiffList[txtOST->Tag - 1]	= strOST;

	strDumper = new TStringList;
	strDumper->Sorted			= true;
	strDumper->Duplicates		= dupIgnore;
	pbDumperD->Tag				= (s32)txtDump;
	DiffList[txtDump->Tag - 1]	= strDumper;

	//Time objects -----------------------------
	Intro.txtMin	= txtIntroMin;
	Intro.txtSec	= txtIntroSec;
	Intro.noFrac	= 0;
	Intro.neg		= 0;
	Loop.txtMin		= txtLoopMin;
	Loop.txtSec		= txtLoopSec;
	Loop.noFrac		= 0;
	Loop.neg		= 0;
	End.txtMin		= txtEndMin;
	End.txtSec		= txtEndSec;
	End.noFrac		= 0;
	End.neg			= 1;
	Fade.txtMin		= NULL;
	Fade.txtSec		= txtFadeSec;
	Fade.noFrac		= 0;
	Fade.neg		= 0;

	Times[0]		= &Intro;
	Times[1]		= &Loop;
	Times[2]		= &End;
	Times[3]		= &Fade;

	//Field information ------------------------
	Field[EN_SONG].pb			= pbSong;
	Field[EN_SONG].control		= txtSong;
	Field[EN_SONG].label		= lblISong;
	Field[EN_SONG].type			= FT_TEXT;
	Field[EN_SONG].update		= 1;
	pbSong->Tag					= (s32)&Field[EN_SONG];

	Field[EN_GAME].pb			= pbGame;
	Field[EN_GAME].control		= txtGame;
	Field[EN_GAME].label		= lblIGame;
	Field[EN_GAME].type			= FT_TEXT;
	Field[EN_GAME].update		= 1;
	pbGame->Tag					= (s32)&Field[EN_GAME];

	Field[EN_ARTIST].pb			= pbArtist;
	Field[EN_ARTIST].control	= txtArtist;
	Field[EN_ARTIST].label		= lblIArtist;
	Field[EN_ARTIST].type		= FT_TEXT;
	Field[EN_ARTIST].update		= 1;
	pbArtist->Tag				= (s32)&Field[EN_ARTIST];

	Field[EN_PUB].pb			= pbPub;
	Field[EN_PUB].control		= txtPub;
	Field[EN_PUB].label			= NULL;
	Field[EN_PUB].type			= FT_TEXT;
	Field[EN_PUB].update		= 1;
	pbPub->Tag					= (s32)&Field[EN_PUB];

	Field[EN_COPY].pb			= pbCopy;
	Field[EN_COPY].control		= txtCopy;
	Field[EN_COPY].label		= NULL;
	Field[EN_COPY].type			= FT_TEXT;
	Field[EN_COPY].update		= 1;
	pbCopy->Tag					= (s32)&Field[EN_COPY];

	Field[EN_OST].pb			= pbOST;
	Field[EN_OST].control		= txtOST;
	Field[EN_OST].label			= NULL;
	Field[EN_OST].type			= FT_TEXT;
	Field[EN_OST].update		= 1;
	pbOST->Tag					= (s32)&Field[EN_OST];

	Field[EN_DISC].pb			= pbDisc;
	Field[EN_DISC].control		= txtDisc;
	Field[EN_DISC].label		= NULL;
	Field[EN_DISC].type			= FT_TEXT;
	Field[EN_DISC].update		= 1;
	pbDisc->Tag					= (s32)&Field[EN_DISC];

	Field[EN_TRACK].pb			= pbTrack;
	Field[EN_TRACK].control		= txtTrack;
	Field[EN_TRACK].label		= NULL;
	Field[EN_TRACK].type		= FT_TEXT;
	Field[EN_TRACK].update		= 1;
	pbTrack->Tag				= (s32)&Field[EN_TRACK];

	Field[EN_CMNT].pb			= pbCmnt;
	Field[EN_CMNT].control		= mmoCmnt;
	Field[EN_CMNT].label		= lblICmnt;
	Field[EN_CMNT].type			= FT_MEMO;
	Field[EN_CMNT].update		= 1;
	pbCmnt->Tag					= (s32)&Field[EN_CMNT];

	Field[EN_DUMPER].pb			= pbDumper;
	Field[EN_DUMPER].control	= txtDump;
	Field[EN_DUMPER].label		= lblIDumper;
	Field[EN_DUMPER].type		= FT_TEXT;
	Field[EN_DUMPER].update		= 1;
	pbDumper->Tag				= (s32)&Field[EN_DUMPER];

	Field[EN_DATE].pb			= pbDate;
	Field[EN_DATE].control		= txtDate;
	Field[EN_DATE].label		= lblIDate;
	Field[EN_DATE].type			= FT_TEXT;
	Field[EN_DATE].update		= 1;
	pbDate->Tag					= (s32)&Field[EN_DATE];

	Field[EN_EMU].pb			= pbEmu;
	Field[EN_EMU].control		= cboEmu;
	Field[EN_EMU].label			= lblIEmu;
	Field[EN_EMU].type			= FT_COMBO;
	Field[EN_EMU].update		= 1;
	pbEmu->Tag					= (s32)&Field[EN_EMU];

	Field[EN_MUTE].pb			= pbMute;
	Field[EN_MUTE].control		= txtMuteFocus;
	Field[EN_MUTE].type			= FT_MUTE;
	Field[EN_MUTE].update		= 1;
	pbMute->Tag					= (s32)&Field[EN_MUTE];

	Field[EN_AMP].pb			= pbAmp;
	Field[EN_AMP].control		= txtAmp;
	Field[EN_AMP].label			= NULL;
	Field[EN_AMP].type			= FT_TEXT;
	Field[EN_AMP].update		= 1;
	pbAmp->Tag					= (s32)&Field[EN_AMP];

	Field[EN_INTRO].pb			= pbIntro;
	Field[EN_INTRO].control		= txtIntroMin;
	Field[EN_INTRO].label		= lblISong2;
	Field[EN_INTRO].type		= FT_INTRO;
	Field[EN_INTRO].update		= 1;
	pbIntro->Tag				= (s32)&Field[EN_INTRO];

	Field[EN_LOOP].pb			= pbLoop;
	Field[EN_LOOP].control		= txtLoopMin;
	Field[EN_LOOP].label		= lblISong2;
	Field[EN_LOOP].type			= FT_LOOP;
	Field[EN_LOOP].update		= 1;
	pbLoop->Tag					= (s32)&Field[EN_LOOP];

	Field[EN_END].pb			= pbEnd;
	Field[EN_END].control		= txtEndMin;
	Field[EN_END].label			= lblISong2;
	Field[EN_END].type			= FT_END;
	Field[EN_END].update		= 1;
	pbEnd->Tag					= (s32)&Field[EN_END];

	Field[EN_FADE].pb			= pbFade;
	Field[EN_FADE].control		= txtFadeSec;
	Field[EN_FADE].label		= lblIFade;
	Field[EN_FADE].type			= FT_FADE;
	Field[EN_FADE].update		= 1;
	pbFade->Tag					= (s32)&Field[EN_FADE];

	//Mute -------------------------------------
	MuteFocus.left		= lblMute1->Left - 1;
	MuteFocus.right		= lblMute8->Left + lblMute8->Width + 3;
	MuteFocus.top		= chkMute1->Top - 2;
	MuteFocus.bottom	= chkMute1->Top + chkMute1->Height + 2;

	ChkMute[0]	= chkMute1;
	ChkMute[1]	= chkMute2;
	ChkMute[2]	= chkMute3;
	ChkMute[3]	= chkMute4;
	ChkMute[4]	= chkMute5;
	ChkMute[5]	= chkMute6;
	ChkMute[6]	= chkMute7;
	ChkMute[7]	= chkMute8;


	for (i=0; i<XID6_EMULIST; i++)
		cboEmu->Items->Append(EmuList[i]);

	TriUp[0].x = 6;
	TriUp[0].y = 11;
	TriUp[1].x = 6;
	TriUp[1].y = 5;
	TriUp[2].x = 9;
	TriUp[2].y = 8;
	TriUp[3].x = 6;
	TriUp[3].y = 11;

	TriDn[0].x = 7;
	TriDn[0].y = 12;
	TriDn[1].x = 7;
	TriDn[1].y = 6;
	TriDn[2].x = 10;
	TriDn[2].y = 9;
	TriDn[3].x = 7;
	TriDn[3].y = 12;

	PBState = 0;
	KeyProcessed = 0;
	CheckExt = 0;

	if ((Cfg.lcid & 0x3FF) == LANG_JAPANESE)	//If default language is Japanese
	{
		Font->Charset = SHIFTJIS_CHARSET;
		Font->Name = "‚l‚r ‚oƒSƒVƒbƒN";
		Font->Size = 10;
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::FormDestroy(TObject *Sender)
{
	delete strDumper;
	delete strOST;
	delete strPub;
	delete strArtist;
	delete strGame;
	delete strSong;
}


//**************************************************************************************************
#define	_InvalStr(s) \
		memset(s, 255, 255); \
		s[255] = 0;

void __fastcall TFrmTag::FormShow(TObject *sender)
{
	s8	str[32];
	u32	i;


	if (pID6)
	{
		Edit.ext = id6.IsExt();
		chkBin->Checked = id6.IsBin();
	}
	else
	{
		Edit.ext = (_Test(update, EN_SONG)		&& strlen(id6.song) > 32) ||
				   (_Test(update, EN_GAME)		&& strlen(id6.game) > 32) ||
				   (_Test(update, EN_ARTIST)	&& strlen(id6.artist) > 32) ||
				   (_Test(update, EN_DUMPER)	&& strlen(id6.dumper) > 32) ||
				   (_Test(update, EN_CMNT)		&& strlen(id6.comment) > 32) ||
				   (_Test(update, EN_INTRO)		&& (id6.intro % XID6_TICKSSEC)) ||
				   (_Test(update, EN_OST)		&& id6.ost[0] != 0) ||
				   (_Test(update, EN_PUB)		&& id6.pub[0] != 0) ||
				   (_Test(update, EN_COPY)		&& id6.copy != 0) ||
				   (_Test(update, EN_DISC)		&& id6.disc != 0) ||
				   (_Test(update, EN_TRACK)		&& id6.track != 0) ||
				   (_Test(update, EN_MUTE)		&& id6.mute != 0) ||
				   (_Test(update, EN_AMP)		&& id6.amp != 0) ||
				   (_Test(update, EN_LOOP)		&& id6.loop != 0) ||
				   (_Test(update, EN_END)		&& id6.end != 0);
	}

	CheckExt = 1;
	chkExt->Checked = Edit.ext;
	CheckExt = 0;

	txtSong->Width		= (pbSongD->Left - txtSong->Left) + ((strSong->Count > 1) ? 0 : pbSongD->Width);
	txtGame->Width		= (pbGameD->Left - txtGame->Left) + ((strGame->Count > 1) ? 0 : pbGameD->Width);
	txtArtist->Width	= (pbArtistD->Left - txtArtist->Left) + ((strArtist->Count > 1) ? 0 : pbArtistD->Width);
	txtPub->Width		= (pbPubD->Left - txtPub->Left) + ((strPub->Count > 1) ? 0 : pbPubD->Width);
	txtOST->Width		= (pbOSTD->Left - txtOST->Left) + ((strOST->Count > 1) ? 0 : pbOSTD->Width);
	txtDump->Width		= (pbDumperD->Left - txtDump->Left) + ((strDumper->Count > 1) ? 0 : pbDumperD->Width);

	if (_Test(update, EN_SONG)) txtSong->Text = id6.song;
	else
	{
		txtSong->Text = "";
		_InvalStr(id6.song);
	}

	if (_Test(update, EN_GAME)) txtGame->Text = id6.game;
	else
	{
		txtGame->Text = "";
		_InvalStr(id6.game);
	}

	if (_Test(update, EN_ARTIST)) txtArtist->Text = id6.artist;
	else
	{
		txtArtist->Text = "";
		_InvalStr(id6.artist);
	}

	if (_Test(update, EN_PUB)) txtPub->Text = id6.pub;
	else
	{
		txtPub->Text = "";
		_InvalStr(id6.pub);
	}

	if (_Test(update, EN_COPY))
	{
		if (id6.copy)
			txtCopy->Text = id6.copy;
		else
			txtCopy->Text = "";
		Edit.copy = id6.copy;
	}
	else
	{
		txtCopy->Text = "";
		Edit.copy = 0;
		id6.copy = -1;
	}

	if (_Test(update, EN_OST)) txtOST->Text = id6.ost;
	else
	{
		txtOST->Text = "";
		_InvalStr(id6.ost);
	}

	if (_Test(update, EN_DISC))
	{
		if (id6.disc)
			txtDisc->Text = id6.disc;
		else
			txtDisc->Text = "";
		Edit.disc = id6.disc;
	}
	else
	{
		txtDisc->Text = "";
		Edit.disc = 0;
		id6.disc = -1;
	}

	if (_Test(update, EN_TRACK))
	{
		if (id6.track)
		{
			txtTrack->Text = id6.track >> 8;
			if (id6.track & 0xFF)
				txtTrack->Text += AnsiString((s8)id6.track);
		}
		else
			txtTrack->Text = "";
		Edit.track = id6.track;
	}
	else
	{
		txtTrack->Text = "";
		Edit.track = 0;
		id6.track = -1;
	}

	if (_Test(update, EN_CMNT)) mmoCmnt->Text = id6.comment;
	else
	{
		mmoCmnt->Text = "";
		_InvalStr(id6.comment);
	}

	if (_Test(update, EN_DUMPER)) txtDump->Text = id6.dumper;
	else
	{
		txtDump->Text = "";
		_InvalStr(id6.dumper);
	}

	if (_Test(update, EN_DATE))
	{
		if (id6.date.date)
		{
//TODO
//			StrDate(str,id6.date);
//			txtDate->Text = str;
		}
		else
			txtDate->Text = id6.datetxt;

//		Edit.date = id6.date;
	}
	else
	{
		txtDate->Text = "";
//		Edit.date.Invalidate();
//		id6.date.SetDate(1,1,1);
	}

	if (_Test(update, EN_EMU))
	{
		if (id6.emu <= 6)
			cboEmu->ItemIndex = id6.emu;
		else
			cboEmu->ItemIndex = 0;

		Edit.emu = id6.emu;
	}
	else
	{
		cboEmu->Text = "";
		Edit.emu = 0;
		id6.emu = -1;
	}

	if (_Test(update, EN_MUTE))
	{
		for (i=0; i<8; i++)
			ChkMute[i]->Checked = (id6.mute >> i) & 1;
		Edit.mute = id6.mute;
	}
	else
	{
		for (i=0; i<8; i++)
			ChkMute[i]->Checked = false;
		Edit.mute = -1;
		id6.mute = 0;
	}

	if (_Test(update, EN_AMP))
	{
		if (id6.amp)
			txtAmp->Text = FloatToStrF(YLog2(6.0, (s32)id6.amp / 65536.0), ffFixed, 4, 1);
		else
			txtAmp->Text = "";
		Edit.amp = id6.amp;
	}
	else
	{
		txtAmp->Text = "";
		Edit.amp = 0;
		id6.amp = -1;
	}

	if (id6.IsExt())
	{
		if (_Test(update, EN_INTRO))
			Intro.SetTime(id6.intro);
		else
		{
			Intro.SetTime(0);
			id6.intro = -1;
		}

		if (_Test(update, EN_LOOP))
		{
			Loop.SetTime(id6.loop);
			txtLoopNum->Text = id6.loopx;
			Edit.loopx = id6.loopx;
		}
		else
		{
			txtLoopNum->Text = "1";
			Loop.SetTime(0);
			Edit.loopx = 1;
			id6.loop = -1;
			id6.loopx = 0;
		}

		if (_Test(update, EN_END))
			End.SetTime(id6.end);
		else
		{
			End.SetTime(0);
			id6.end = -1;
		}
	}
	else
	{
		if (_Test(update, EN_INTRO))
			End.SetTime(id6.intro);
		else
		{
			End.SetTime(0);
			id6.intro = -1;
		}

		Intro.SetTime(0);
		Loop.SetTime(0);
		txtLoopNum->Text = "1";
		Edit.loopx = 1;
		id6.loop = -1;
		id6.loopx = 0;
		id6.end = -1;
	}

	if (_Test(update, EN_FADE))
		Fade.SetTime(id6.fade);
	else
	{
		Fade.SetTime(0);
		id6.fade = -1;
	}

	PrintTotal();

	for (i=0; i<EN_TOTAL; i++)
	{
		Field[i].update = (update >> i) & 1;
		if (Field[i].label)
		{
			if (Field[i].update) ID6Change(Field[i].control);
			else Field[i].label->Caption = "";
		}
		pbChkState(&Field[i]);
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::FormClose(TObject *sender, TCloseAction &Action)
{
	u32	i;

	i = popDiff->Items->Count;
	while(i) delete popDiff->Items->Items[--i];
}


//**************************************************************************************************
void __fastcall TFrmTag::TxtPaint(TEdit *txt, TPaintBox *pb)
{
	TRect	r;


	r.top		= txt->Top - 2;
	r.left		= txt->Left - 2;
	r.bottom	= r.top + txt->Height + 4;
	r.right		= pb->Left + pb->Width + 2;

	Canvas->Pen->Color = CBtn.ws;
	Canvas->PenPos = TPoint(r.left, r.bottom - 1);
	Canvas->LineTo(r.left, r.top);
	Canvas->LineTo(r.right - 1, r.top);

	Canvas->Pen->Color = CBtn.wh;
	Canvas->LineTo(r.right - 1, r.bottom - 1);
	Canvas->LineTo(r.left - 1, r.bottom - 1);

	Canvas->Pen->Color = CBtn.wd;
	Canvas->PenPos = TPoint(r.left + 1, r.bottom - 2);
	Canvas->LineTo(r.left + 1, r.top + 1);
	Canvas->LineTo(r.right - 2, r.top + 1);

	Canvas->Pen->Color = CBtn.wl;
	Canvas->LineTo(r.right - 2, r.bottom - 2);
	Canvas->LineTo(r.left, r.bottom - 2);
}

void __fastcall TFrmTag::FormPaint(TObject *sender)
{
	TxtPaint(txtSong, pbSongD);
	TxtPaint(txtGame, pbGameD);
	TxtPaint(txtArtist, pbArtistD);
	TxtPaint(txtPub, pbPubD);
	TxtPaint(txtOST, pbOSTD);
	TxtPaint(txtDump, pbDumperD);

	if (ActiveControl->Tag - 1 == EN_MUTE) chkMuteFocus(1);
}


//**************************************************************************************************
void __fastcall TFrmTag::ShowBtn(b8 enable)
{
	btnIntro->Visible	= enable;
	btnLoop->Visible	= enable;
	btnEnd->Visible		= enable;
	btnFade->Visible	= enable;
	btnTime->Visible	= enable;
}


//**************************************************************************************************
void __fastcall TFrmTag::btnOKClick(TObject *sender)
{
	u32	i,mute;


	update = 0;

	//Copy items common to both types ----------
	if (Field[EN_SONG].update && !CmpCopy(id6.song, txtSong->Text.c_str()))
		_Set(update, EN_SONG);

	if (Field[EN_GAME].update && !CmpCopy(id6.game, txtGame->Text.c_str()))
		_Set(update, EN_GAME);

	if (Field[EN_ARTIST].update && !CmpCopy(id6.artist, txtArtist->Text.c_str()))
		_Set(update, EN_ARTIST);

	if (Field[EN_CMNT].update && !CmpCopy(id6.comment, mmoCmnt->Text.c_str()))
		_Set(update, EN_CMNT);

	if (Field[EN_DUMPER].update && !CmpCopy(id6.dumper, txtDump->Text.c_str()))
		_Set(update, EN_DUMPER);

//TODO
//	if (Field[EN_DATE].update && id6.date != Edit.date)
//	{
//		id6.date = Edit.date;
//		_Set(update, EN_DATE);
//	}

	if (Field[EN_EMU].update && id6.emu != Edit.emu)
	{
		id6.emu = Edit.emu;
		_Set(update, EN_EMU);
	}

	if (chkExt->Checked)
	{
		//Copy extended items ------------------
		if (Field[EN_PUB].update && !CmpCopy(id6.pub, txtPub->Text.c_str()))
			_Set(update, EN_PUB);

		if (Field[EN_COPY].update && id6.copy != Edit.copy)
		{
			id6.copy = Edit.copy;
			_Set(update, EN_COPY);
		}

		if (Field[EN_OST].update && !CmpCopy(id6.ost, txtOST->Text.c_str()))
			_Set(update, EN_OST);

		if (Field[EN_DISC].update && id6.disc != Edit.disc)
		{
			id6.disc = Edit.disc;
			_Set(update, EN_DISC);
		}

		if (Field[EN_TRACK].update && id6.track != Edit.track)
		{
			id6.track = Edit.track;
			_Set(update, EN_TRACK);
		}

		mute = 0;
		for (i=0; i<8; i++)
			if (ChkMute[i]->Checked) _Set(mute, i);
		if (Edit.mute != mute)
		{
			id6.mute = mute;
			_Set(update, EN_MUTE);
		}

		if (Field[EN_AMP].update && id6.amp != Edit.amp)
		{
			id6.amp = Edit.amp;
			_Set(update, EN_AMP);
		}

		if (Field[EN_INTRO].update && id6.intro != Intro.ticks)
		{
			id6.intro = Intro.ticks;
			_Set(update, EN_INTRO);
		}

		if (Field[EN_LOOP].update && (id6.loop != Loop.ticks || id6.loopx != Edit.loopx))
		{
			id6.loop = Loop.ticks;
			id6.loopx = Edit.loopx;
			_Set(update, EN_LOOP);
		}

		if (Field[EN_END].update && id6.end != (s32)End.ticks)
		{
			id6.end = End.ticks;
			_Set(update, EN_END);
		}
	}
	else
	{
		if (Field[EN_END].update && id6.intro != End.ticks)
		{
			id6.intro = End.ticks;
			_Set(update, EN_INTRO);
		}

		//Reset all extended items -------------
		if (Edit.ext)
		{
			id6.pub[0]	= 0;
			id6.copy	= 0;
			id6.ost[0]	= 0;
			id6.disc	= 0;
			id6.track	= 0;
			id6.mute	= 0;
			id6.amp		= 0;
			id6.loop	= 0;
			id6.loopx	= 0;
			id6.end		= 0;

			update |= _Flag(EN_PUB) |		   	//Flag extended items as being updated with nothing
					  _Flag(EN_COPY) |
					  _Flag(EN_OST) |
					  _Flag(EN_DISC) |
					  _Flag(EN_TRACK) |
					  _Flag(EN_MUTE) |
					  _Flag(EN_AMP) |
					  _Flag(EN_LOOP) |
					  _Flag(EN_END);
		}
	}

	if (Field[EN_FADE].update && id6.fade != Fade.ticks)
	{
		id6.fade = Fade.ticks;
		_Set(update, EN_FADE);
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::btnID6Click(TObject *sender)
{
	if (Height < bvlID6->Top)
	{
		ClientHeight = lblIFade->Top + lblIFade->Height + 3;
		btnID6->Caption = "ID666 <<";
	}
	else
	{
		ClientHeight = bvlID6->Top - 2;
		btnID6->Caption = "ID666 >>";
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::FormKeyDown(TObject *sender, WORD &key, TShiftState shift)
{
	if (KeyProcessed) return;

	switch(key)
	{
	case(VK_SPACE):
		if (shift.Contains(ssCtrl) && ActiveControl->Tag)
		{
			if (ActiveControl->Tag <= EN_TOTAL)
			{
				Field[ActiveControl->Tag-1].update ^= 1;
				pbChkState(&Field[ActiveControl->Tag-1]);
			}
			key = 0;
			KeyProcessed = 1;
		}
		break;

	case(VK_F5):
		if (!pID6) break;
		if (shift.Contains(ssShift)) HotKey(VK_F9, shift);
		else
		{
			shift << ssLeft;
			if (chkExt->Checked)
				btnIntroClick(NULL);
			else
				btnEndClick(NULL);
		}
		break;

	case(VK_F6):
		if (!pID6) break;
		if (shift.Contains(ssShift))
		{
			if (chkExt->Checked)
				SeekSong(Intro.ticks - (shift.Contains(ssCtrl) * XID6_TICKSSEC));
			else
				HotKey(VK_F9, shift);
		}
		else
		{
			shift << ssLeft;
			if (chkExt->Checked)
				btnLoopClick(NULL);
			else
				btnEndClick(NULL);
		}
		break;

	case(VK_F7):
		if (!pID6) break;
		if (shift.Contains(ssShift))
		{
			if (chkExt->Checked)
				SeekSong(Intro.ticks + (Loop.ticks * Edit.loopx) - (shift.Contains(ssCtrl) * XID6_TICKSSEC));
			else
				HotKey(VK_F9, shift);
		}
		else
		{
			shift << ssLeft;
			btnEndClick(NULL);
		}
		break;

	case(VK_F8):
		if (!pID6) break;
		if (shift.Contains(ssShift))
			SeekSong(Intro.ticks + (Loop.ticks * Edit.loopx) + End.ticks - (shift.Contains(ssCtrl) * XID6_TICKSSEC));
		else
		{
			shift << ssLeft;
			btnFadeClick(NULL);
		}
		break;

	case(VK_F9):
	case(VK_F10):
	case(VK_F11):
	case(VK_F12):
		HotKey(key, shift);
		KeyProcessed = 1;
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::FormKeyPress(TObject *sender, char &key)
{
	if (KeyProcessed) key = 0;
}


//**************************************************************************************************
void __fastcall TFrmTag::FormKeyUp(TObject *sender, WORD &key, TShiftState shift)
{
	if (KeyProcessed)
	{
		key = 0;
		KeyProcessed = 0;
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::txtMuteFocusKeyDown(TObject *sender, WORD &key, TShiftState shift)
{
	u32	i;


	if (key >= '1' && key <= '8')
		ChkMute[key - '1']->Checked ^= 1;
	else
	if (key == '0')
		for(i=0; i<8; i++)
			ChkMute[i]->Checked = false;

	key = 0;
	KeyProcessed = 1;
}


//**************************************************************************************************
void __fastcall TFrmTag::txtMuteFocusEnter(TObject *sender)
{
	chkMuteFocus(1);
}


//**************************************************************************************************
void __fastcall TFrmTag::txtMuteFocusExit(TObject *sender)
{
	chkMuteFocus(0);
}


//**************************************************************************************************
void __fastcall TFrmTag::chkMuteFocus(b8 draw)
{
	HDC		dc;
	HBRUSH	wincolor;


	dc = GetDC(Handle);							//Get a device context to draw on
	wincolor = GetSysColorBrush(COLOR_BTNFACE);	//Get the system default color

	FrameRect(dc, &MuteFocus, wincolor);		//Draw a gray box
	if (draw) DrawFocusRect(dc, &MuteFocus);	//Then put a focus box on top of it
	ReleaseDC(Handle, dc);						//Release the DC for another program
}


//**************************************************************************************************
void __fastcall TFrmTag::chkExtClick(TObject *sender)
{
	u32	checked,i;
	s8	str[33];


	checked = chkExt->Checked;

	if (!(checked || CheckExt))
	{
		if (txtSong->Text.Length() > 32 ||
			txtGame->Text.Length() > 32 ||
			txtArtist->Text.Length() > 32 ||
			txtDump->Text.Length() > 16 ||
			mmoCmnt->Text.Length() > 32)
		{
			if (Application->MessageBox("By disabling extended ID666 support, you will truncate some information.",
										"SPCTool ID666 Tag Editor", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
			{
				CheckExt = 1;
				chkExt->Checked = true;
				CheckExt = 0;
				return;
			}
		}
	}

	//En/Disable extended fields ---------------
	pbPub->Enabled			= checked;
	lblPub->Enabled			= checked;
	txtPub->Enabled			= checked;

	pbCopy->Enabled			= checked;
	lblCopy->Enabled		= checked;
	txtCopy->Enabled		= checked;

	pbOST->Enabled			= checked;
	lblOST->Enabled			= checked;
	txtOST->Enabled			= checked;

	pbDisc->Enabled			= checked;
	lblDisc->Enabled		= checked;
	txtDisc->Enabled		= checked;

	pbTrack->Enabled		= checked;
	lblTrack->Enabled		= checked;
	txtTrack->Enabled		= checked;

	pbMute->Enabled			= checked;
	lblMute->Enabled		= checked;
	lblMute1->Enabled		= checked;
	lblMute8->Enabled		= checked;
	txtMuteFocus->Enabled	= checked;

	pbAmp->Enabled			= checked;
	lblAmp->Enabled			= checked;
	txtAmp->Enabled			= checked;

	pbIntro->Enabled		= checked;
	btnIntro->Enabled		= checked;
	lblIntro->Enabled		= checked;
	lblIntroSep->Enabled	= checked;
	txtIntroMin->Enabled	= checked;
	txtIntroSec->Enabled	= checked;

	pbLoop->Enabled			= checked;
	btnLoop->Enabled		= checked;
	lblLoop->Enabled		= checked;
	lblLoopSep->Enabled		= checked;
	lblLoopX->Enabled		= checked;
	txtLoopMin->Enabled		= checked;
	txtLoopSec->Enabled		= checked;
	txtLoopNum->Enabled		= checked;
	End.noFrac				= !checked;
	End.neg					= checked;

	//Set title of time field ------------------
	lblEnd->Caption = checked ? "End" : "Song";
	btnEnd->Caption = checked ? "End" : "Song";

	if (checked)
	{
		//Set max length of edit fields --------
		txtSong->MaxLength		= 255;
		txtGame->MaxLength		= 255;
		txtArtist->MaxLength	= 255;
		txtDump->MaxLength		= 255;
		mmoCmnt->MaxLength		= 255;

		btnEnd->HelpContext		= HID6_EndB;
		txtEndMin->HelpContext	= HID6_End;
		txtEndSec->HelpContext	= HID6_End;

		Intro.SetTime(id6.intro != -1 ? id6.intro : 0);
		Loop.SetTime(id6.loop != -1 ? id6.loop : 0);
		End.SetTime(id6.end != -1 ? id6.end : 0);
	}
	else
	{
		//Set max length of edit fields --------
		txtSong->MaxLength		= 32;
		txtGame->MaxLength		= 32;
		txtArtist->MaxLength	= 32;
		txtDump->MaxLength		= 16;
		mmoCmnt->MaxLength		= 32;

		btnEnd->HelpContext		= HID6_LenB;
		txtEndMin->HelpContext	= HID6_Len;
		txtEndSec->HelpContext	= HID6_Len;

		//Truncate text strings ----------------
		if (!CheckExt)
		{
			str[32] = 0;
			strncpy(str, txtSong->Text.c_str(), 32);
			txtSong->Text = str;
			strncpy(str, txtGame->Text.c_str(), 32);
			txtGame->Text = str;
			strncpy(str, txtArtist->Text.c_str(), 32);
			txtArtist->Text = str;
			strncpy(str, mmoCmnt->Text.c_str(), 32);
			mmoCmnt->Text = str;

			str[16] = 0;
			strncpy(str,txtDump->Text.c_str(), 16);
			txtDump->Text = str;
		}

		Intro.SetTime(0);
		Loop.SetTime(0);
		End.SetTime(id6.intro != -1 ? id6.intro : 0);
	}

	pbChkState((ID6Field*)pbPub->Tag);
	pbChkState((ID6Field*)pbCopy->Tag);
	pbChkState((ID6Field*)pbOST->Tag);
	pbChkState((ID6Field*)pbDisc->Tag);
	pbChkState((ID6Field*)pbTrack->Tag);
	pbChkState((ID6Field*)pbMute->Tag);
	pbChkState((ID6Field*)pbAmp->Tag);
	pbChkState((ID6Field*)pbIntro->Tag);
	pbChkState((ID6Field*)pbLoop->Tag);
}


//**************************************************************************************************
void __fastcall TFrmTag::ID6Change(TObject *sender)
{
	TLabel	*lbl;
	u32		i,j;
	s8		str[33],*t;


	i = ((TControl*)sender)->Tag - 1;
	lbl = Field[i].label;

	switch(Field[i].type)
	{
	case(FT_TEXT):
		str[lbl->Tag] = 0;
		strncpy(str, ((TEdit*)sender)->Text.c_str(), lbl->Tag);
		break;

	case(FT_MEMO):
		str[32] = 0;
		strncpy(str, ((TMemo*)sender)->Lines->Text.c_str(), 32);

		t = str;
		while((u8)*t >= 0x20) {t++;}
		*t = 0;
		break;

	case(FT_COMBO):
		str[lbl->Tag] = 0;
		strncpy(str, ((TComboBox*)sender)->Text.c_str(), lbl->Tag);
		break;

	case(FT_INTRO):
	case(FT_LOOP):
	case(FT_END):
		i = Intro.ticks + End.ticks;

		if (Loop.ticks)
		{
			j = (999 * XID6_TICKSSEC) / Loop.ticks;
			if (j > Edit.loopx) j = Edit.loopx;
			i += Loop.ticks * j;
		}

		i /= XID6_TICKSSEC;
		if (i > 999) i = 999;

		sprintf(str, "%i:%i", i / 60, i % 60);
		break;

	case(FT_FADE):
        sprintf(str, "%.3f", (f64)Fade.ticks / XID6_TICKSSEC);
		break;
	}

	lbl->Caption = str;
}


//**************************************************************************************************
void __fastcall TFrmTag::pbChkPaint(TObject *sender)
{
	TPaintBox	*pb;
	RECT		r;


	pb = (TPaintBox*)sender;

	r.left = 0;
	r.top = 0;
	r.right = 13;
	r.bottom = 13;

	if (!((ID6Field*)pb->Tag)->update)
		DrawFrameControl(pb->Canvas->Handle, &r, DFC_BUTTON, DFCS_BUTTONPUSH);
	else
		DrawFrameControl(pb->Canvas->Handle, &r, DFC_BUTTON, DFCS_PUSHED);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbChkState(ID6Field *pChk)
{
	u32		i;
	TColor	c;


	c = pChk->control->Enabled ? (pChk->update ? clWindow : clBtnFace) : clBtnFace;
	switch(pChk->type)
	{
	case(FT_TEXT):
		((TEdit*)pChk->control)->Color = c;
		break;

	case(FT_MEMO):
		((TMemo*)pChk->control)->Color = c;
		break;

	case(FT_COMBO):
		((TComboBox*)pChk->control)->Color = c;
		break;

	case(FT_MUTE):
		if (c == clWindow)
			for (i=0; i<8; i++)
				ChkMute[i]->Enabled = true;
		else
			for (i=0; i<8; i++)
				ChkMute[i]->Enabled = false;
		break;

	case(FT_INTRO):
		txtIntroMin->Color = c;
		txtIntroSec->Color = c;
		break;

	case(FT_LOOP):
		txtLoopMin->Color = c;
		txtLoopSec->Color = c;
		txtLoopNum->Color = c;
		break;

	case(FT_END):
		txtEndMin->Color = c;
		txtEndSec->Color = c;
		break;

	case(FT_FADE):
		txtFadeSec->Color = c;
		break;
	}

	pbChkPaint(pChk->pb);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbChkMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	ID6Field	*pChk;


	pChk = (ID6Field*)((TPaintBox*)sender)->Tag;
	pChk->update ^= 1;
	PBState = pChk->update;
	pbChkState(pChk);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbChkMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	RECT		r;
	u32			i;


	if (!shift.Contains(ssLeft)) return;

	x += ((TPaintBox*)sender)->Left;
	y += ((TPaintBox*)sender)->Top;

	for (i=0; i<EN_TOTAL; i++)
	{
		r = Field[i].pb->BoundsRect;

		if (Field[i].pb->Enabled && Field[i].update != PBState &&
			x >= r.left && x < r.right &&
			y >= r.top && y < r.bottom)
		{
			Field[i].update = PBState;
			pbChkState(&Field[i]);
			return;
		}
	}
}


//**************************************************************************************************
//TODO
/*
b8 __fastcall TFrmTag::StrDate(s8 *str, A2Date &date)
{
	TDateTime	*tDate;
	s32			y,m,d;


	if (!date) return 0;

	date.GetDate(y,m,d);
	tDate = new TDateTime(y,m,d);
	strcpy(str, tDate->DateString().c_str());
	delete tDate;

	return 1;
}
*/

//**************************************************************************************************
void __fastcall PrintTime(s8 *str, u32 ticks)
{
	s8	*t;
	u32	i;


	t = str;

	if ((s32)ticks < 0)
	{
		strcpy(str, "0.00");
		return;
	}

	DivMod(&ticks, &i, XID6_TICKSMIN * 60);
	if (ticks)
	{
		sprintf(t, "%i:", ticks);
		t += 2;
	}

	DivMod(&i, &ticks, XID6_TICKSMIN);
	if (t != str)
	{
		sprintf(t, "%02i:", i);
		t += 3;
	}
	else
	if (i)
	{
		sprintf(t, "%i:", i);
		t += strlen(t);
	}

	DivMod(&ticks, &i, XID6_TICKSSEC);
	if (t != str)
	{
		sprintf(t, "%02i.", ticks);
		t += 3;
	}
	else
	{
		sprintf(t, "%i.", ticks);
		t += strlen(t);
	}

	sprintf(t, "%02i", i / (XID6_TICKSMS * 10));
}


//**************************************************************************************************
void __fastcall TFrmTag::txtCopyExit(TObject *sender)
{
	s32	i;


	i = txtCopy->Text.ToIntDef(-2000);
	if (i < 100)
	{
		i += 1900;
		if (i < 1989) i += 100;
	}
	if (i < 1989) i = 0;

	if (i)
		txtCopy->Text = i;
	else
		txtCopy->Text = "";

	Edit.copy = i;
}


//**************************************************************************************************
void __fastcall TFrmTag::txtDiscExit(TObject *sender)
{
	u32	i;


	i = txtDisc->Text.ToIntDef(0);

	if (i<1 || i>9) txtDisc->Text = "";

	Edit.disc = i;
}


//**************************************************************************************************
void __fastcall TFrmTag::txtTrackExit(TObject *sender)
{
	s8 	str[4];
	u32	l,t;
	s8 	a=0;


	strcpy(str, txtTrack->Text.c_str());
	l = strlen(str);

	if (l--)									//Does string have a length?
	{
		if (str[l]<'0' || str[l]>'9')			//Is last character non-numeric?
		{
			a = str[l];							//Get last character
			a |= 0x20;							//Change to lowercase
			if (a<'a' || a>'z') a = 0;			//If not alpha, invalidate
			str[l] = 0;							//Remove last character from string
		}
	}
	t = atoi(str);								//Convert string to int

	if (t<=0 || t>99)							//If track is invalid, erase field
	{
		txtTrack->Text = "";
		Edit.track = 0;
	}
	else
	{
		txtTrack->Text.printf(str, "%i%c", t, a);
		Edit.track = (t << 8) | a;
	}
}


//**************************************************************************************************
void __fastcall TFrmTag::txtDateEnter(TObject *sender)
{
	s32	d,m,y;

	DateChanged = false;
	OldDate = txtDate->Text;
//TODO
//	if ((s32)Edit.date)
//	{
//		Edit.date.GetDate(y,m,d);
//		txtDate->Text.printf("%i/%i/%i", m, d, y % 100);
//	}
}


//**************************************************************************************************
void __fastcall TFrmTag::txtDateKeyPress(TObject *sender, char &key)
{
	if ((key>='0' && key<='9') || key=='.' || key=='/' || key==VK_BACK || key==VK_DELETE)
		DateChanged = true;						//Flag date field as having changed
	else
		key = 0;								//Otherwise invalidate keypress
}


//**************************************************************************************************
void __fastcall TFrmTag::txtDateExit(TObject *sender)
{
//	A2Date	*date;
	s32	  	d,m,y;
	s8	  	str[32],*s;


	d = txtDate->Text.Length();

	if (DateChanged || d != OldDate.Length())	//Was any text entered?
	{
		if (d)
		{
			strcpy(str,txtDate->Text.c_str());
			txtDate->Text = "";					//Erase date field incase date is invalid

			s = strchr(str,'/');				//Search for US style date separator
			if (s)
			{
				m = atoi(str);
				d = atoi(++s);
				s = strchr(s,'/');
				if (!s) return;
				y = atoi(s + 1);
			}
			else
			{
				s = strchr(str,'.');			//Search for European separator
				if (!s) return;
				d = atoi(str);
				m = atoi(++s);
				s = strchr(s,'.');
				if (!s) return;
				y = atoi(s + 1);
			}
//TODO
//			date = new A2Date;
//			if (id6.FixDate(*date,y,m,d))		//Verify date
//			{
//				Edit.date = *date;
//				StrDate(str,*date);
//				txtDate->Text = str;
//			}
//			delete date;
		}
		else
		{
			txtDate->Text = "";
//			Edit.date.Invalidate();
			id6.datetxt[0] = 0;
		}
	}
	else
		txtDate->Text = OldDate;				//If nothing changed, restore old text

	ID6Change(sender);
}


//**************************************************************************************************
void __fastcall TFrmTag::txtAmpExit(TObject *sender)
{
	f32	f;


	try
	{
		f = txtAmp->Text.ToDouble();
	}
	catch(...)
	{
		txtAmp->Text = "";
		Edit.amp = 0;
		return;
	}

	_Clamp(f, -6.0, 18.0);

	txtAmp->Text = FloatToStrF(f, ffFixed, 4, 1);

	Edit.amp = pow(2.0, f / 6.0) * 65536.0;
}


//**************************************************************************************************
void __fastcall TFrmTag::btnIntroClick(TObject *sender)
{
	s32	i;


	i = WavTime();
	if (i > XID6_MAXTICKS) i = XID6_MAXTICKS;

	Intro.SetTime(i);
	PrintTotal();
}


//**************************************************************************************************
void __fastcall TFrmTag::btnLoopClick(TObject *sender)
{
	s32	i;


	i = WavTime() - Intro.ticks;
	_Clamp(i, 0, XID6_MAXTICKS);

	Loop.SetTime(i);
	PrintTotal();
}


//**************************************************************************************************
void __fastcall TFrmTag::btnEndClick(TObject *sender)
{
	s32	i;


	if (chkExt->Checked)
	{
		i = WavTime() - (Intro.ticks + (Loop.ticks * Edit.loopx));
		_Clamp(i, 0, XID6_MAXTICKS);

		End.SetTime(i);
	}
	else
	{
		i = WavTime();
		_Clamp(i, 0, XID6_MAXTICKS);
		i -= i % XID6_TICKSSEC;

		End.SetTime(i);
	}

	PrintTotal();
}


//**************************************************************************************************
void __fastcall TFrmTag::btnFadeClick(TObject *sender)
{
	s32	i;


	i = WavTime() - (Intro.ticks + (Loop.ticks * Edit.loopx) + End.ticks);
	_Clamp(i, 0, XID6_TICKSMIN - 1);

	Fade.SetTime(i);
	PrintTotal();
}


//**************************************************************************************************
void __fastcall TFrmTag::txtMinExit(TObject *sender)
{
	Times[((TEdit*)sender)->Tag - EN_INTRO - 1]->VerifyMin();
	PrintTotal();
	ID6Change(sender);
}


//**************************************************************************************************
void __fastcall TFrmTag::txtSecExit(TObject *sender)
{
	Times[((TEdit*)sender)->Tag - EN_INTRO - 1]->VerifySec();
	PrintTotal();
	ID6Change(sender);
}


//**************************************************************************************************
void __fastcall TFrmTag::txtLoopNumExit(TObject *sender)
{
	Edit.loopx = txtLoopNum->Text.ToIntDef(0);
	if (!Edit.loopx)
	{
		txtLoopNum->Text = "1";
		Edit.loopx = 1;
	}

	PrintTotal();
	ID6Change(sender);
}


//**************************************************************************************************
void __fastcall TFrmTag::btnTimeClick(TObject *sender)
{
	pID6->intro	= Intro.ticks;
	pID6->loop	= Loop.ticks;
	pID6->loopx	= Edit.loopx;
	pID6->end	= End.ticks;
	pID6->fade	= Fade.ticks;

	SetAPULength();
}


//**************************************************************************************************
void __fastcall TFrmTag::PrintTotal()
{
	s32	i;


	i = Intro.ticks + (Loop.ticks * Edit.loopx) + End.ticks + Fade.ticks;
	txtTimeMin->Text = i / XID6_TICKSMIN;
	txtTimeSec->Text = FloatToStrF((f64)(i % XID6_TICKSMIN) / XID6_TICKSSEC, ffFixed, 5, 3);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbDiffPaint(TObject *sender)
{
	TPaintBox	*pb;
	TCanvas		*pCanvas;


	pb = (TPaintBox*)sender;
	pCanvas = pb->Canvas;

	pCanvas->Pen->Color = CBtn.wl;
	pCanvas->PenPos = TPoint(0, pb->Height - 1);
	pCanvas->LineTo(0, 0);
	pCanvas->LineTo(pb->Width - 1, 0);

	pCanvas->Pen->Color = CBtn.wd;
	pCanvas->LineTo(pb->Width - 1, pb->Height - 1);
	pCanvas->LineTo(0 - 1, pb->Height - 1);

	pCanvas->Pen->Color = CBtn.wh;
	pCanvas->PenPos = TPoint(0 + 1, pb->Height - 2);
	pCanvas->LineTo(0 + 1, 0 + 1);
	pCanvas->LineTo(pb->Width - 2, 0 + 1);

	pCanvas->Pen->Color = CBtn.ws;
	pCanvas->LineTo(pb->Width - 2, pb->Height - 2);
	pCanvas->LineTo(0, pb->Height - 2);

	pCanvas->Brush->Color = CBtn.wf;
	pCanvas->FillRect(TRect(2, 2, pb->Width - 2, pb->Height - 2));

	pCanvas->Pen->Color = clBlack;
	pCanvas->Brush->Color = clBlack;
	pCanvas->Polygon(TriUp, 3);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbDiffMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	TPaintBox	*pb;
	TCanvas		*pCanvas;


	pb = (TPaintBox*)sender;
	pCanvas = pb->Canvas;

	pCanvas->Brush->Color = CBtn.ws;
	pCanvas->FrameRect(TRect(0, 0, pb->Width, pb->Height));

	pCanvas->Brush->Color = CBtn.wf;
	pCanvas->FillRect(TRect(1, 1, pb->Width - 1, pb->Height - 1));

	pCanvas->Pen->Color = clBlack;
	pCanvas->Brush->Color = clBlack;
	pCanvas->Polygon(TriDn, 3);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbDiffMouseUp(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	pbDiffPaint(sender);
}


//**************************************************************************************************
void __fastcall TFrmTag::pbDiffClick(TObject *sender)
{
	TPaintBox	*pb;
	TEdit		*txt;
	TStringList	*sl;
	TPoint		point;
	s32			i;


	i = popDiff->Items->Count;
	while(i) delete popDiff->Items->Items[--i];

	popDiff->Items->Clear();

	pb = (TPaintBox*)sender;
	txt = (TEdit*)pb->Tag;
	sl = DiffList[txt->Tag - 1];

	for (i=0; i<sl->Count; i++)
	{
		TMenuItem	*itm = new TMenuItem(popDiff);

		itm->Caption = sl->Strings[i];
		itm->Tag = (s32)txt;
		itm->OnClick = itmClick;

		popDiff->Items->Add(itm);
	}

	point = pb->ClientOrigin;
	popDiff->Popup(point.x, point.y);
}


//**************************************************************************************************
void __fastcall TFrmTag::itmClick(TObject *sender)
{
	((TEdit*)((TMenuItem*)sender)->Tag)->Text = ((TMenuItem*)sender)->Caption;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Time Class

//**************************************************************************************************
void ID6Time::SetTime(u32 time)
{
	ticks = time;
	min = abs(time);
	DivMod((u32*)&min, (u32*)&sec, XID6_TICKSMIN);

	Print();
}


//**************************************************************************************************
void ID6Time::VerifyMin()
{
	b8	n;


	min = txtMin->Text.ToIntDef(0);

	if (neg)
	{
		n = txtMin->Text[1] == '-';
		_Clamp(min, -59, 59);
		if (n)
		{
			min = -min;
			txtMin->Text = "-" + AnsiString(min);
		}
		else
			txtMin->Text = min;
	}
	else
	{
		n = 0;
		_Clamp(min, 0, 59);
		txtMin->Text = min;
	}

	ticks = (min * XID6_TICKSMIN) + sec;
	if (n) ticks = 0 - ticks;
}


//**************************************************************************************************
void ID6Time::VerifySec()
{
	sec = Str2Ticks(txtSec->Text.c_str());

	if (noFrac) sec -= sec % XID6_TICKSSEC;

	if (printTicks)
		txtSec->Text = AnsiString(sec / XID6_TICKSSEC) + ":" + AnsiString(sec % XID6_TICKSSEC);
	else
		txtSec->Text = FloatToStrF((f64)sec / XID6_TICKSSEC, ffFixed, 5, 3);

	if ((s32)ticks < 0)
		ticks = -((min * XID6_TICKSMIN) + sec);
	else
		ticks = (min * XID6_TICKSMIN) + sec;
}


//**************************************************************************************************
void ID6Time::Print()
{
	if (txtMin)
	{
		if ((s32)ticks < 0)
			txtMin->Text = "-" + AnsiString(min);
		else
			txtMin->Text = min;
	}

	if (printTicks)
		txtSec->Text = AnsiString(sec / XID6_TICKSSEC) + ":" + AnsiString(sec % XID6_TICKSSEC);
	else
		txtSec->Text = FloatToStrF((f64)sec / XID6_TICKSSEC, ffFixed, 5, 3);
}

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	<dir.h>
#include	"mmsystem.h"
#include	"Main.h"
#include	"Assembly.h"
#include	"About.h"
#include	"Config.h"
#include	"Debug.h"
#include	"DSPEdit.h"
#include	"SSI.h"
#include	"WAV.h"
#include	"MIDI.h"
#include	"Profile.h"
#include	"Progress.h"
#include	"Rename.h"
#include	"ROM.h"
#include	"Tag.h"
#include	"WaveOut.h"
#include	"A2Math.h"
#include	"A2Nihon.h"
#include	"math.h"

#pragma package(smart_init)
#pragma resource "*.dfm"


//**************************************************************************************************
// Defines and Structures

#define	BTN_STATE	0x01						//Button is pressed down
#define	BTN_TOGGLE	0x02						//Button is a toggle switch

typedef struct
{
	s8	*pData;									//-> data buffer
	s32	alloc;									//Size of buffer
	s32	size;									//Size of data in buffer (can be less than 'alloc')
} RARFile;

//Sound Source Files ---------------------------
#pragma pack(push, 2)
typedef struct _SrcInfo							//Sound Source Information
{
	u32	offset;									//Offset in APU RAM or ROM file
	u16	len;									//Length of source (in blocks)
	u16	loop;									//Position of loop (from offset)
	u16	crc;									//16-bit checksum
	u16	_r1[3];

	u32	rate;									//Sample rate for middle-C playback
	u16	adsr;									//ADSR register (if !bit-7, high byte is GAIN reg)
	u8	prog;									//GM program number ([7] tone/drum  [6-0] program #)
	u8	bank;									//Program bank
	struct _SrcInfo	*pPrev,*pNext;
} SourceInfo;

typedef struct									//Source Block
{
	struct
	{
		u8	end:1;								//End block
		u8	loop:1;								//Sound loops
		u8	filter:2;							//ADPCM filter to use for this block
		u8	range:4;							//Range of BRR data
	} rf;

	s8	data[8];								//16 nybbles of bit-rate reduced data
} BRRBlock;

typedef struct									//Source List
{
	SubChk		schk;							//'info'
	SourceInfo	info[1];						//Can be more than one if file is a sound font
	SubChk		data;							//'data'
	BRRBlock	blocks[];
} BRRList;

typedef struct									//Sound Source File (.brr)
{
	Chunk 	riff;	 	  						//'RIFF' 'snes'
	Chunk	list;		   						//'LIST' 'srcd'
	BRRList	src;
} BRRFile;
#pragma	pack(pop)

//Visualization --------------------------------
typedef struct
{
	s32	volL,volR;
	u8	mode;
	u8	env;
	u8	src;
	u8	sw;
	u32	rate;
} VisVoice;

typedef struct									//One frame of visualization
{
	VisVoice	voice[8];
	s32			mvolL,mvolR;					//Values of main and echo volumes
	s32			evolL,evolR;
	s32			peak[18];						//Peak output for each voice and main

	u32			time;							//t64Cnt at time of visualization frame capture
	void		*pBuf;							//-> PCM data in output stream
} VisFrame;

typedef struct
{
	TLabel	*volL;
	TLabel	*volR;
	TLabel	*mode;
	TLabel	*env;
	TLabel	*src;
	TLabel	*rate;
	TLabel	*sw;
} VisLabel;


//**************************************************************************************************
// Global Data

#include	"Buttons.h"
#include	"Gaussian.h"

static const TColor	CBlkFilter[4] = {(TColor)0xFFFFFF, (TColor)0xFFC000, (TColor)0xFF0000, (TColor)0xFF00C0};
static const s8		NoteLst[12][4]={"C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-"};

static Graphics::TBitmap	*BmpCBtn;

static	s32			TagWidth, TagCol1, TagCol2;

//File list ------------------------------------
static TListItem	*PCurItem;					//Current item selected (NULL if more than one sel)
static TMenuItem	*ItmFile[9];				//-> menu items for recent files
static b8			FileDeselect;

//Source directory -----------------------------
static Source		*PCurSrc;					//Current sound source selected (NULL if > 1)
static u32			NumSrc;
static s32			Octave;

static SourceInfo	SrcInfo[1024];
static SourceInfo	*PInfoStart,*PInfoFree;

static struct
{
	u32	offset;
	u16	len;
	u16	loop;
	u8	*pScope;
} DWSrc;

//Visualization --------------------------------
static struct
{
	VisFrame		frame[2][8];				//Information for each frame (2 buffers, 160/20 ms)
	VisLabel		label[8];					//Labels in emulation panel

	VisFrame		*pFrame;					//Current frame[] being used for visualization
	u32				second;						//Counts the number of frames until one second

	MMRESULT		tid;						//Timer ID

	u32				rate;						//Playback rate for sound source
	u32				lastPos;

	struct
	{
		HDC			hdc;						//Offscreen device context containing meter bitmap
		HBITMAP		hbm, hbmOld;				//Handle to bitmap containing meter bar
		s32			peak[2][18];
		u32			frame;						//Current peak[] being used for visualization
	} meters;

	struct
	{
		u32			loop,end,fade;
		u32			marker;						//Position of marker on time bar
	} timeBar;
} Vis;

static b8			Scope;						//Enable oscilloscope for DSP output

//Playback -------------------------------------
static EmuState		State;				  		//Current emulation state
static b8			SPCPaused;			  		//True, if only SPC700 should be paused
static b8			UseTimer;				  	//Enable song length timer
static b8			Remaining;				  	//Time displayed is remaining
static b8			Seeking;			  		//Song is seeking
static u32			SeekPos;			  		//Position to seek to
static TLabel*		LblVoice[8];		  		//-> voice numbers (used for muting)
static u32			Length;

static union
{
	FileInfo		*PCurFile;
	APUFile			*PCurAPU;
	ROMFile			*PCurROM;
};

TFrmMain			*FrmMain;


//**************************************************************************************************
// Private Functions

static void CALLBACK TimeSPC(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static void CALLBACK TimeSrc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static u32	PlaySPC(void *pOut, u32 len, u32 buf);
static u32	PlaySrc2(void *pOut, u32 len, u32 buf);
static void	__fastcall VoiceMute(u32 voice, u32 state);
static void __fastcall DrawWave(Source *pSrc);
static void __fastcall SetTimeBarPos(u32 time, u32 total);
static void __fastcall DrawScope(u8 *p);
static void	DebugDSP(volatile u8 *reg, volatile u8 val);

void __fastcall SetAPULength();


//**************************************************************************************************
__fastcall TFrmMain::TFrmMain(TComponent* owner) : TForm(owner)
{
}


//**************************************************************************************************
// Create Window Style Parameters

void __fastcall TFrmMain::CreateParams(TCreateParams &params)
{
	TForm::CreateParams(params);
	params.ExStyle |= WS_EX_ACCEPTFILES;
}


//**************************************************************************************************
void __fastcall TFrmMain::FormCreate(TObject *sender)
{
	s8			str[256],*s;
	s32			i;
	TListColumn	*pCol;
	RECT		r;
	HPEN		hp,hpOld;
	HBRUSH		hbBlack;
	RGBA		h,f;
	struct
	{
		u16		ver;
		u16		num;
		RGBA	pal[10];
	} logPal;									//Stucture for creating palette handles


	if (Screen->Width <= Width || Screen->Height <= Height) WindowState = wsMaximized;

	TagWidth = pnlTag->Width;
	TagCol1 = lblGameT->Left;
	TagCol2 = lblCmntT->Left;

	lblGame->Tag = lblGame->Left - TagCol1;
	lblCopy->Tag = lblCopy->Left - TagCol1;
	lblPub->Tag = lblPub->Left - TagCol1;
	lblDumperT->Tag = lblDumperT->Left - TagCol1;
	lblDumper->Tag = lblDumper->Left - TagCol1;

	//Location of help file --------------------
	strcpy(str, Application->ExeName.c_str());
	strcpy(ScanStrR(str, '\\'), "Manual.hlp");
	Application->HelpFile = str;

	//Use Shift-JIS character set on Japanese systems
	str[0] = '0';
	str[1] = 'x';
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTLANGUAGE, &str[2], 5);
	Cfg.lcid = StrToIntDef(str, 0);

	if ((Cfg.lcid & 0x3FF) == LANG_JAPANESE)	//If default language is Japanese
	{
		pnlTag->Font->Charset = SHIFTJIS_CHARSET;
		pnlTag->Font->Name = "‚l‚r ‚oƒSƒVƒbƒN";
		pnlTag->Font->Size = 10;

		lvwFile->Font->Charset = SHIFTJIS_CHARSET;
		lvwFile->Font->Name = "‚l‚r ‚oƒSƒVƒbƒN";
		lvwFile->Font->Size = 10;
	}


	//==========================================
	// Files

	// Open dialog -----------------------------
	// The dialog has to be manually created so we can add a template to it

	dlgOpen = new TOpenDlgEx(NULL);
	dlgOpen->Filter =
		"SNES-SPC700 Sound File (*.spc)|*.sp?|" \
		"ZSNES Save State File (*.zst)|*.zst|" \
		"RAR File (*.rar,*.rsn,*.exe)|*.rar;*.rsn;*.exe|" \
		"SNES ROM image (*.smc,*.sfc,*.swc,*.fig)|*.smc;*.sfc;*.swc;*.fig|" \
		"All SNES files|*.sp?;*.zst;*.rar;*.rsn;*.exe;*.smc;*.sfc;*.swc;*.fig|"
		"All files (*.*)|*.*";
	dlgOpen->FilterIndex = 5;
	dlgOpen->Options.Clear();
	dlgOpen->Options << ofHideReadOnly << ofAllowMultiSelect << ofPathMustExist << ofFileMustExist;

	//Initialize pointers to recent files ------
	ItmFile[0] = itmFile1;
	ItmFile[1] = itmFile2;
	ItmFile[2] = itmFile3;
	ItmFile[3] = itmFile4;
	ItmFile[4] = itmFile5;
	ItmFile[5] = itmFile6;
	ItmFile[6] = itmFile7;
	ItmFile[7] = itmFile8;
	ItmFile[8] = itmFile9;

	//Initialize dialog with most recent directory
	if (Cfg.curFile != -1)
	{
		strcpy(str, Cfg.file[Cfg.curFile]);
		s = strrchr(str, '\\');
		if (s) *s = 0;
		dlgOpen->InitialDir = str;
		dlgOpen->FileName = "";

		RecentFiles();
	}

	//File list columns ------------------------
	s = Cfg.col;
	while (*s)
	{
		for (i=0; i<popFile->Items->Count; i++)
		{
			if (popFile->Items->Items[i]->Tag == *s)
			{
				popFile->Items->Items[i]->Checked = true;
				break;
			}
		}

		pCol = lvwFile->Columns->Add();

		switch(*s++)
		{
		case('S'):
			pCol->Caption = "Song";
			pCol->Tag = 'S';
			break;

		case('G'):
			pCol->Caption = "Game";
			pCol->Tag = 'G';
			break;

		case('A'):
			pCol->Caption = "Artist";
			pCol->Tag = 'A';
			break;

		case('P'):
			pCol->Caption = "Publisher";
			pCol->Tag = 'P';
			break;

		case('O'):
			pCol->Caption = "OST";
			pCol->Tag = 'O';
			break;

		case('T'):
			pCol->Caption = "Track";
			pCol->Tag = 'T';
			break;

		case('U'):
			pCol->Caption = "Dumper";
			pCol->Tag = 'U';
			break;

		case('D'):
			pCol->Caption = "Date";
			pCol->Tag = 'D';
			break;

		case('E'):
			pCol->Caption = "Emulator";
			pCol->Tag = 'E';
			break;

		case('C'):
			pCol->Caption = "Comments";
			pCol->Tag = 'C';
			break;

		case('L'):
			pCol->Caption = "Length";
			pCol->Tag = 'L';
			pCol->Alignment = taRightJustify;
			break;
		}

		pCol->Width = 0;
	}

	PCurItem = NULL;


	//==========================================
	// Sound Sources

	//Resize source list -----------------------
	lvwSrc->Column[0]->Width = Canvas->TextWidth("255") + 8;
	lvwSrc->Column[1]->Width = Canvas->TextWidth("1234567") + 12;
	lvwSrc->Column[2]->Width = Canvas->TextWidth("12345") + 12;
	lvwSrc->Column[3]->Width = Canvas->TextWidth("12345") + 12;
	lvwSrc->Column[4]->Width = Canvas->TextWidth("Filters") + 12;

	i = lvwSrc->Column[0]->Width +				//Width of columns
		lvwSrc->Column[1]->Width +
		lvwSrc->Column[2]->Width +
		lvwSrc->Column[3]->Width +
		lvwSrc->Column[4]->Width +
		GetSystemMetrics(SM_CXVSCROLL) + 4 + 8;	//Width of scrollbar + list view border

	pgcSrc->Width = i;

	i = pnlSrc->Left - i - 4;					//Locate pgcSrc four pixels away from the source
	pgcSrc->Left = i;							// information box

	lvwFile->Width = i - 3;						//Assign the remaining width of the window to files

	//Source information -----------------------
	PInfoStart = NULL;
	PInfoFree = SrcInfo;

	for (i=0; i<1024; i++)
	{
		SrcInfo[i].pPrev = &SrcInfo[i-1];
		SrcInfo[i].pNext = &SrcInfo[1+i];
	}

	SrcInfo[0].pPrev = NULL;
	SrcInfo[1023].pNext = NULL;


	//==========================================
	// Custom Buttons

	//Create palette for DIB -------------------
	h.rgba = GetSysColor(COLOR_3DHILIGHT);		//Get system colors
	f.rgba = GetSysColor(COLOR_3DFACE);

	logPal.ver = 0x300;							//Create a palette for buttons
	logPal.num = 10;
	for (i=0; i<10; i++)
	{
		switch (PalCBtn[i].rgba)				//Replace disabled colors with system colors
		{
		case(0xFFFFFF):
			logPal.pal[i] = h;
			break;

		case(0xE0E0E0):
			logPal.pal[i].r = (f.r + h.r) >> 1;
			logPal.pal[i].g = (f.g + h.g) >> 1;
			logPal.pal[i].b = (f.b + h.b) >> 1;
			logPal.pal[i].a = 0;

		case(0xC0C0C0):
			logPal.pal[i] = f;
			break;

		default:
			logPal.pal[i] = PalCBtn[i];
		}
	}

	//Create DIB containing button icons -------
	BmpCBtn = new Graphics::TBitmap;
	BmpCBtn->Monochrome = false;
	BmpCBtn->PixelFormat = pf8bit;
	BmpCBtn->Width = CBTN_SIZE * 2;
	BmpCBtn->Height = sizeof(PelCBtn) / (CBTN_SIZE * 2);
	BmpCBtn->Palette = CreatePalette((LOGPALETTE*)&logPal);
	for (i=0; i<BmpCBtn->Height; i++)
	{
		memcpy(BmpCBtn->ScanLine[i], PelCBtn[i], CBTN_SIZE * 2);
	}

	//Get colors for drawing button edges ------
	CBtn.wh = (TColor)GetSysColor(COLOR_3DHILIGHT);	//Windows' colors for disabled buttons
	CBtn.wl = (TColor)GetSysColor(COLOR_3DLIGHT);
	CBtn.wf = (TColor)GetSysColor(COLOR_3DFACE);
	CBtn.ws = (TColor)GetSysColor(COLOR_3DSHADOW);
	CBtn.wd = (TColor)GetSysColor(COLOR_3DDKSHADOW);

	CBtn.sh = (TColor)logPal.pal[ColCBtn[0]].rgba;	//Purple colors for enabled CBtn
	CBtn.sl = (TColor)logPal.pal[ColCBtn[1]].rgba;
	CBtn.sf = (TColor)logPal.pal[ColCBtn[2]].rgba;
	CBtn.ss = (TColor)logPal.pal[ColCBtn[3]].rgba;
	CBtn.sd = (TColor)logPal.pal[ColCBtn[4]].rgba;

	//Initialize icon indices ------------------
	pbReset->Tag	= CBTN_BEGIN << 8;
	pbRun->Tag		= CBTN_PLAY << 8;
	pbHalt->Tag		= CBTN_PAUSE << 8;
	pbAbort->Tag	= CBTN_STOP << 8;
	pbFF->Tag		= CBTN_SEEKF << 8;
	pbUpload->Tag	= CBTN_UP << 8;
	pbOpen->Tag		= CBTN_EJECT << 8;
	pbSave->Tag		= CBTN_SAVE << 8;
	pbWAV->Tag		= CBTN_RECORD << 8;
	pbMIDI->Tag		= CBTN_MIDI << 8;
	pbTimer->Tag	= (CBTN_WATCH << 8) | BTN_TOGGLE;

	pbPlay->Tag		= CBTN_PLAY << 8;
	pbStop->Tag		= CBTN_STOP << 8;
	pbImport->Tag	= CBTN_EJECT << 8;
	pbExport->Tag	= CBTN_SAVE << 8;
	pbZoom->Tag		= (CBTN_ZOOM << 8) | BTN_TOGGLE;


	//==========================================
	// Visualization

	//Pointers to visualization labels ---------
	Vis.label[0].volL	= lblVolL0;
	Vis.label[1].volL	= lblVolL1;
	Vis.label[2].volL	= lblVolL2;
	Vis.label[3].volL	= lblVolL3;
	Vis.label[4].volL	= lblVolL4;
	Vis.label[5].volL	= lblVolL5;
	Vis.label[6].volL	= lblVolL6;
	Vis.label[7].volL	= lblVolL7;

	Vis.label[0].volR	= lblVolR0;
	Vis.label[1].volR	= lblVolR1;
	Vis.label[2].volR	= lblVolR2;
	Vis.label[3].volR	= lblVolR3;
	Vis.label[4].volR	= lblVolR4;
	Vis.label[5].volR	= lblVolR5;
	Vis.label[6].volR	= lblVolR6;
	Vis.label[7].volR	= lblVolR7;

	Vis.label[0].mode	= lblMode0;
	Vis.label[1].mode	= lblMode1;
	Vis.label[2].mode	= lblMode2;
	Vis.label[3].mode	= lblMode3;
	Vis.label[4].mode	= lblMode4;
	Vis.label[5].mode	= lblMode5;
	Vis.label[6].mode	= lblMode6;
	Vis.label[7].mode	= lblMode7;

	Vis.label[0].env	= lblEnv0;
	Vis.label[1].env	= lblEnv1;
	Vis.label[2].env	= lblEnv2;
	Vis.label[3].env	= lblEnv3;
	Vis.label[4].env	= lblEnv4;
	Vis.label[5].env	= lblEnv5;
	Vis.label[6].env	= lblEnv6;
	Vis.label[7].env	= lblEnv7;

	Vis.label[0].src	= lblSrc0;
	Vis.label[1].src	= lblSrc1;
	Vis.label[2].src	= lblSrc2;
	Vis.label[3].src	= lblSrc3;
	Vis.label[4].src	= lblSrc4;
	Vis.label[5].src	= lblSrc5;
	Vis.label[6].src	= lblSrc6;
	Vis.label[7].src	= lblSrc7;

	Vis.label[0].rate	= lblRate0;
	Vis.label[1].rate	= lblRate1;
	Vis.label[2].rate	= lblRate2;
	Vis.label[3].rate	= lblRate3;
	Vis.label[4].rate	= lblRate4;
	Vis.label[5].rate	= lblRate5;
	Vis.label[6].rate	= lblRate6;
	Vis.label[7].rate	= lblRate7;

	Vis.label[0].sw		= lblSw0;
	Vis.label[1].sw		= lblSw1;
	Vis.label[2].sw		= lblSw2;
	Vis.label[3].sw		= lblSw3;
	Vis.label[4].sw		= lblSw4;
	Vis.label[5].sw		= lblSw5;
	Vis.label[6].sw		= lblSw6;
	Vis.label[7].sw		= lblSw7;

	//Create peak meter bitmap -----------------
	Vis.meters.hdc = CreateCompatibleDC(FrmMain->Canvas->Handle);
	Vis.meters.hbm = CreateCompatibleBitmap(FrmMain->Canvas->Handle, 152, 11);
	Vis.meters.hbmOld = SelectObject(Vis.meters.hdc, Vis.meters.hbm);

	r.left = 0;
	r.top = 0;
	r.right = 152;
	r.bottom = 11;
	hbBlack = CreateSolidBrush(0);				//Solid black brush
	FillRect(Vis.meters.hdc, &r, hbBlack);
	DeleteObject(hbBlack);

	hp = CreatePen(PS_SOLID, 0, 0xFF007F);		//Blue (-48dB to -12dB)
	hpOld = SelectObject(Vis.meters.hdc, hp);
	for (i=0; i<108; i+=2)
	{
		MoveToEx(Vis.meters.hdc, i, 0, NULL);
		LineTo(Vis.meters.hdc, i, 11);
	}
	DeleteObject(hp);

	hp = CreatePen(PS_SOLID, 0, 0x28FFFF);		//Yellow (-12dB to 0dB)
	SelectObject(Vis.meters.hdc, hp);
	for (; i<144; i+=2)
	{
		MoveToEx(Vis.meters.hdc, i, 0, NULL);
		LineTo(Vis.meters.hdc, i, 11);
	}
	DeleteObject(hp);

	hp = CreatePen(PS_SOLID, 0, 0x2800FF);		//Red (+0dB clipping)
	SelectObject(Vis.meters.hdc, hp);
	for (; i<152; i+=2)
	{
		MoveToEx(Vis.meters.hdc, i, 0, NULL);
		LineTo(Vis.meters.hdc, i, 11);
	}
	DeleteObject(hp);
	SelectObject(Vis.meters.hdc, hpOld);


	memset(Vis.meters.peak, 0, sizeof(Vis.meters.peak));
	Vis.meters.frame = 0;

	//Time bar ---------------------------------
	Vis.timeBar.marker = 0;

	//Waveform viewer / oscilloscope -----------
	NumSrc = 0;
	Octave = 4;
	Vis.rate = 0;
	Vis.lastPos = -1;

	DrawWave(NULL);


	//==========================================
	//Disable Stuff

	itmProf->Visible = (Cfg.apu.opt & SA_PROFILE);

	pbUpload->Visible = (Cfg.out.snes != 0);

	BtnEnabled(pbReset, 0);
	BtnEnabled(pbRun, 0);
	BtnEnabled(pbHalt, 0);
	BtnEnabled(pbAbort, 0);
	BtnEnabled(pbFF, 0);
	BtnEnabled(pbUpload, 0);
	BtnEnabled(pbSave, 0);
	BtnEnabled(pbWAV, 0);
	BtnEnabled(pbMIDI, 0);

	BtnEnabled(pbPlay, 0);
	BtnEnabled(pbStop, 0);
	BtnEnabled(pbImport, 0);
	BtnEnabled(pbExport, 0);

	txtIn0->Enabled = false;
	txtIn1->Enabled = false;
	txtIn2->Enabled = false;
	txtIn3->Enabled = false;

	txtOut0->Enabled = false;
	txtOut1->Enabled = false;
	txtOut2->Enabled = false;
	txtOut3->Enabled = false;

	Remaining = Cfg.time.remain;
	UseTimer = Cfg.time.enable;
	lblTotal->Font->Color = UseTimer ? clSilver : clGray;
	pbTimer->Tag |= UseTimer;
	itmTimer->Checked = UseTimer;
	pbBtnPaint(pbTimer);

	Scope = 0;
	SPCPaused = 0;
	Seeking = 0;

	//Pointers to voice labels for muting ------
	LblVoice[0]	= lblVoice0;
	LblVoice[1]	= lblVoice1;
	LblVoice[2]	= lblVoice2;
	LblVoice[3]	= lblVoice3;
	LblVoice[4]	= lblVoice4;
	LblVoice[5]	= lblVoice5;
	LblVoice[6]	= lblVoice6;
	LblVoice[7]	= lblVoice7;

	Apu.SetDSPDbg(DebugDSP, 0);					//Install DSP hook

	Apu.profile->tsc.host.acc = 0;
	Apu.profile->tsc.host.base = 0;
}


//**************************************************************************************************
// Save settings and delete anything created at run-time

void __fastcall TFrmMain::FormDestroy(TObject *sender)
{
	s32	i;


	if (Remaining != Cfg.time.remain)
		WritePrivateProfileString("Time", "remaining", Remaining ? "1" : "0", Cfg.ini);

	if (UseTimer != Cfg.time.enable)
		WritePrivateProfileString("Time", "enabled", UseTimer ? "1" : "0", Cfg.ini);

	WritePrivateProfileString("Visualize", "scope", Cfg.vis.scope ? "1" : "0", Cfg.ini);

	WritePrivateProfileString("Files", "columns", Cfg.col, Cfg.ini);

	if (Cfg.curFile != -1)
	{
		for (i='0'; i<'9'; i++)
		{
			WritePrivateProfileString("Files", (s8*)&i, Cfg.file[Cfg.curFile], Cfg.ini);
			if (--Cfg.curFile > 8) Cfg.curFile = 8;
		}
	}

	//Delete items used for peak meters --------
	SelectObject(Vis.meters.hdc, Vis.meters.hbmOld);
	DeleteObject(Vis.meters.hbm);
	DeleteDC(Vis.meters.hdc);

	delete dlgOpen;
	delete BmpCBtn;
}


//**************************************************************************************************
void __fastcall TFrmMain::FormMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	if (pbTime->Tag)
	{
		if (x < pbTime->Left || y < pbTime->Top ||
			x > (pbTime->Left + pbTime->Width) || y > (pbTime->Top + pbTime->Height))
		{
			pbTimeClearMarker(pbTime->Canvas);
			lblTimeSet->Caption = "";
			pbTime->Tag = 0;
		}
	}
}


//**************************************************************************************************
// Look for modified files and ask the user if they want to save them before exiting

void __fastcall TFrmMain::FormCloseQuery(TObject *sender, bool &canClose)
{
	s32	i;


	if (!lvwFile->Items->Count) return;

	for (i=0; i<lvwFile->Items->Count; i++)
	{
		if (((FileInfo*)lvwFile->Items->Item[i]->Data)->flags & F_SAVE)
		{
			i = MessageDlg("You have modified some files.  Do you want to save them before exiting?",
				mtWarning, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

			switch(i)
			{
			case(mrYes):
				itmSaveAClick(NULL);
				break;

			case(mrCancel):
				canClose = false;
				return;
			}

			break;
		}
	}

	itmCloseAClick(NULL);
}


//**************************************************************************************************
void __fastcall TFrmMain::WMDropFiles(TMessage &msg)
{
	OpenFiles(OT_DROP, (s8*)msg.WParam);
}


//**************************************************************************************************
bool __fastcall TFrmMain::WantChildKey(Controls::TControl* child, Messages::TMessage &msg)
{
	static b8	shift=0,ctrl=0,alt=0;
	s32			key;


	if (ActiveControl == txtIn0 ||				//Let the edit boxes respond themselves
		ActiveControl == txtIn1 ||
		ActiveControl == txtIn2 ||
		ActiveControl == txtIn3) return 0;

	switch(msg.Msg)
	{
	case(WM_KEYDOWN):
		switch(msg.WParam)
		{
		case(VK_SHIFT):							//Mark shift state
			shift = 1;
			return 0;
                                                           
		case(VK_CONTROL):						//Mark control state
			ctrl = 1;
			return 0;

		case(VK_MENU):							//Mark alt state
			alt = 1;
			return 0;

		case(VK_ADD):							//If key is num pad '+', increase mixing level
			key = trkAmp->Position;
			if (key > -180) trkAmp->Position = key - 1;
			return 1;

		case(VK_SUBTRACT):						//If key is num pad '-', decrease mixing level
			key = trkAmp->Position;
			if (key < 60) trkAmp->Position = key + 1;
			return 1;

		case(VK_MULTIPLY):						//If key is num pad '*', increase octave
			if (Octave < 9) Octave++;
			return 1;

		case(VK_DIVIDE):						//If key is num pad '/', decrease octave
			if (Octave > 1) Octave--;
			return 1;
		}

		//The following commands don't respond to repeating keys
		if (msg.LParamHi & KF_REPEAT) goto Remove;

		//Generate events that happen regardless of the active control
		switch(msg.WParam)
		{
		case(VK_ESCAPE):
			if (pbAbort->Enabled) itmAbortClick(NULL);
			if (pbStop->Enabled) StopNote();
			return 1;

		case(VK_F2):
			if (PCurFile && !(PCurFile->flags & F_READONLY))
				SendMessage(lvwFile->Handle, LVM_EDITLABEL, lvwFile->Selected->Index, 0);
			return 1;

		case(VK_BACK):
			if (pbReset->Enabled) itmResetClick(NULL);
			return 1;

		case('0'):								//Unmute all voices
			for (key=0; key<8; key++)
				VoiceMute(key, MUTE_OFF);
			return 1;
		}

		if (msg.WParam >= '1' && msg.WParam <= '8')	//Mute or solo a voice
		{
			key = msg.WParam - '1';
			VoiceMute(key, alt ? MUTE_SOLO : MUTE_TOGGLE);
			return 1;
		}

		if (State == ST_RUN && (msg.LParamHi & 0x1FF) == 0x29)
		{										//If a song is playing and the tilde key is pressed
			if (!(pbFF->Tag & 1))
			{
				pbFF->Tag |= 1;
				pbBtnPaint(pbFF);
			}

			return 1;
		}

		//Generate events that happen only if the source view is active
		if (ActiveControl == lvwSrc)
		{
			if (ctrl)
			{
				switch(msg.WParam | 0x20)
				{
				case('a'):
					for (key=0; key<lvwSrc->Items->Count; key++)
						lvwSrc->Items->Item[key]->Selected = true;
					return 1;

				case('i'):
					for (key=0; key<lvwSrc->Items->Count; key++)
						lvwSrc->Items->Item[key]->Selected = !lvwSrc->Items->Item[key]->Selected;
					return 1;
				}

				return 0;
			}

			switch(msg.WParam)
			{
			case(VK_INSERT):
				itmSShowClick(NULL);
				return 1;

			case(VK_DELETE):
				itmSHideClick(NULL);
				return 1;
			}

			if (pbPlay->Enabled)
			{
				switch(msg.WParam)
				{
				case(VK_RETURN):
					itmSetOptClick(NULL);
					return 1;

				case(VK_SPACE):
					PlayNote(4,0);
					return 1;
				}
			}
		}
		else									//Generate events that happen only if the source
		{										// view is NOT active
			if (ctrl)
			{
				switch(msg.WParam | 0x20)
				{
				case('a'):
					for (key=0; key<lvwFile->Items->Count; key++)
						lvwFile->Items->Item[key]->Selected = true;
					return 1;

				case('i'):
					for (key=0; key<lvwSrc->Items->Count; key++)
						lvwFile->Items->Item[key]->Selected = !lvwFile->Items->Item[key]->Selected;
					return 1;
				}

				return 0;
			}

			switch(msg.WParam)
			{
			case(VK_RETURN):
				itmEditClick(NULL);
				return 1;

			case(VK_DELETE):
				itmCloseClick(NULL);
				return 1;

			case(VK_SPACE):
				if (pbHalt->Enabled) itmHaltClick(NULL);
				else
				if (pbRun->Enabled) itmRunClick(NULL);
				return 1;
			}
		}

		//Generate events that happen only if the file view is NOT active
		if (ActiveControl != lvwFile)
		{
			if (pbPlay->Enabled)
			{
				key = msg.LParamHi & 0x1FF;

				if (key >= 0x10 && key <= 0x1B)
				{
					PlayNote(Octave, key-0x10);		//QWERTYUIOP[]
					return 1;
				}
				else
				if (key >= 0x1E && key <= 0x28)
				{
					PlayNote(Octave-1, key-0x1E);	//ASDFGHKJL;'
					return 1;
				}
				else
				if (key >= 0x2C && key <= 0x35)
				{
					PlayNote(Octave-2, key-0x2C);	//ZXCVBNM,./
					return 1;
				}
			}
		}

		goto Remove;

	case(WM_KEYUP):
		switch(msg.WParam)
		{
		case(VK_SHIFT):
			shift = 0;
			return 0;

		case(VK_CONTROL):
			ctrl = 0;
			return 0;

		case(VK_MENU):
			alt = 0;
			return 0;
		}

		if (State == ST_RUN && (msg.LParamHi & 0x1FF) == 0x29)
		{
			pbFF->Tag &= ~1;
			pbBtnPaint(pbFF);
			return 1;
		}

	case(WM_CHAR):								//Keep the application from processing custom keys
Remove:
		//Test regular keypresses --------------
		switch(msg.WParam)
		{
		case(VK_ADD):
		case(VK_SUBTRACT):
		case(VK_MULTIPLY):
		case(VK_DIVIDE):
		case(VK_ESCAPE):
		case(VK_RETURN):
		case(VK_SPACE):
		case(VK_BACK):
		case(VK_INSERT):
		case(VK_DELETE):
		case(VK_F2):
		case('0'):
		case('\\'):
			return 1;
		}

		if (msg.WParam >= '1' && msg.WParam <= '8') return 1;

		//...with control pressed --------------
		if (ctrl)
		{
			switch(msg.WParam | 0x20)
			{
			case('a'):
			case('i'):
				return 1;
			}
		}

		//Test scan codes ----------------------
		key = msg.LParamHi & 0x1FF;

		if (key == 0x29) return 1;

		if (ActiveControl != lvwFile)
		{
			if ((key >= 0x10 && key <= 0x1B) ||
				(key >= 0x1E && key <= 0x28) ||
				(key >= 0x2C && key <= 0x35)) return 1;
		}
	}

	return 0;
}


//**************************************************************************************************
void __fastcall TFrmMain::SetState(s32 s)
{
	s8			str[16];
	VisLabel	*pLabel;
	s32			i;
	TColor		c;


	if (s == State) return;

	switch(s)
	{
	case(ST_STOP):
		if (Vis.tid)
		{
			timeKillEvent(Vis.tid);
			Vis.tid = NULL;
		}

		WavStop();
		WavCallback = NULL;

		Vis.rate = 0;
		Vis.lastPos = -1;

		if (State == ST_RUN || State == ST_HALT)
		{
			if (Apu.profile) Apu.profile->tsc.host.acc += ReadTimeStamp() - Apu.profile->tsc.host.base;

			//Clear emulation panel --------------------
			pLabel = Vis.label;
			for (i=0; i<8; i++)
			{
				pLabel->volL->Caption = "";
				pLabel->volR->Caption = "";

				pLabel->mode->Caption = "";
				pLabel->env->Caption = "";
				pLabel->src->Caption = "";
				pLabel->rate->Caption = "";
				pLabel->sw->Caption = "";

				pLabel++;
			}

			lblMVolL->Caption = "";
			lblMVolR->Caption = "";
			lblEVolL->Caption = "";
			lblEVolR->Caption = "";

			pbMeter->Invalidate();

			ScopeClick(false);
			PrintTimer(0);

			SPCPaused = 0;
		}

		if (State == ST_PLAY)
		{
			DrawScope((u8*)-1);
		}
		
		State = (EmuState)s;
		EnableItem();
		break;

	case(ST_RUN):
		itmRun->Enabled		= false;
		itmHalt->Enabled	= true;

		BtnEnabled(pbRun, 0);
		BtnEnabled(pbHalt, 1);
		BtnEnabled(pbUpload, 0);

		if (State == ST_STOP)
		{
			itmAbort->Enabled	= true;
			itmImport->Enabled	= false;
			itmExport->Enabled	= false;
			itmSilence->Enabled	= false;
			itmWAV->Enabled		= false;
			itmMIDI->Enabled	= false;
			itmROM->Enabled		= false;

			BtnEnabled(pbAbort, 1);
			BtnEnabled(pbFF, 1);
			BtnEnabled(pbUpload, 0);
			BtnEnabled(pbWAV, 0);
			BtnEnabled(pbMIDI, 0);
			BtnEnabled(pbPlay, 0);
			BtnEnabled(pbStop, 0);
			BtnEnabled(pbImport, 0);

			ScopeClick(Cfg.vis.scope);

			if (Apu.profile) Apu.profile->tsc.host.base = ReadTimeStamp();

			WavCallback = PlaySPC;
			if (!WavStart()) break;
		}
		else
			WavPause(0);

		//Reset visualization labels -----------
		pLabel = Vis.label;
		for (i=0; i<8; i++)
		{
			pLabel->volL->Tag = 0x80000000;
			pLabel->volR->Tag = 0x80000000;
			pLabel->mode->Tag = 0x80000000;
			pLabel->env->Tag = 0x80000000;
			pLabel->src->Tag = 0x80000000;
			pLabel->rate->Tag = 0x80000000;
			pLabel->sw->Tag = 0x80000000;
			pLabel++;
		}

		lblMVolL->Tag = 0x80000000;
		lblMVolR->Tag = 0x80000000;
		lblEVolL->Tag = 0x80000000;
		lblEVolR->Tag = 0x80000000;

		Vis.second = 1;
		Vis.tid = timeSetEvent(20, 20, TimeSPC, 0, TIME_PERIODIC);
		State = (EmuState)s;
		break;

	case(ST_HALT):
		if (State == ST_STOP) return;

		WavPause(1);
		timeKillEvent(Vis.tid);
		Vis.tid = NULL;

		itmRun->Enabled		= true;
		itmHalt->Enabled	= false;

		BtnEnabled(pbRun, 1);
		BtnEnabled(pbHalt, 0);
		State = (EmuState)s;
		break;

	case(ST_PLAY):
		itmRun->Enabled		= false;
		itmImport->Enabled	= false;
		itmExport->Enabled	= false;
		itmSilence->Enabled	= false;
		itmWAV->Enabled		= false;
		itmMIDI->Enabled	= false;
		itmROM->Enabled		= false;

		BtnEnabled(pbRun, 0);
		BtnEnabled(pbUpload, 0);
		BtnEnabled(pbWAV, 0);
		BtnEnabled(pbMIDI, 0);
		BtnEnabled(pbStop, 1);
		BtnEnabled(pbImport, 0);

		WavCallback = PlaySrc2;
		WavStart();

		Vis.tid = timeSetEvent(20, 20, TimeSrc, 0, TIME_PERIODIC);
		State = (EmuState)s;
		break;
	}
}


//**************************************************************************************************
// Called every time a DSP register is written to.  Used to get the ADSR register setting for each
// sound source played.
/*
#define LOGLEN 16384
u32 logt[LOGLEN];
u8 logd[LOGLEN][2];
u32 logp = 0;

void savelog()
{
	s8	str[64];
	s32	fh;
	u32	i,j,k;

	fh = FileCreate("logd.txt");

	strcpy(str, "      Reg     0    1    2    3    4    5    6    7\n");
	FileWrite(fh, str, strlen(str));

	for (i=0; i<LOGLEN; i++)
	{
		sprintf(str, "%5i ", logt[i]);
		FileWrite(fh, str, strlen(str));
		
		memset(str, ' ', 48);

		if ((logd[i][0] & 0xF) < 10)
		{
			switch(logd[i][0] & 0xF)
			{
			case(0): sprintf(str, "VOLL "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%i\n", (s32)((s8)logd[i][1])); break;
			case(1): sprintf(str, "VOLR "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%i\n", (s32)((s8)logd[i][1])); break;
			case(2): sprintf(str, "PL   "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%02X\n", logd[i][1]); break;
			case(3): sprintf(str, "PH   "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%02X\n", logd[i][1]); break;
			case(4): sprintf(str, "SRCN "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%i\n", logd[i][1]); break;
			case(5): sprintf(str, "ADSR1"); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%02X\n", logd[i][1]); break;
			case(6): sprintf(str, "ADSR2"); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%02X\n", logd[i][1]); break;
			case(7): sprintf(str, "GAIN "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%02X\n", logd[i][1]); break;
			case(8): sprintf(str, "ENVX "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%i\n", (s32)((s8)logd[i][1])); break;
			case(9): sprintf(str, "OUTX "); sprintf(&str[((logd[i][0] >> 4) * 5) + 8], "%i\n", (s32)((s8)logd[i][1])); break;
			}

			str[5] = ' ';
		}
		else
		{
			switch(logd[i][0])
			{
			case(0x0C): sprintf(str, "MVOLL %2i\n", (s32)((s8)logd[i][1])); break;
			case(0x1C): sprintf(str, "MVOLR %2i\n", (s32)((s8)logd[i][1])); break;
			case(0x2C): sprintf(str, "EVOLL %2i\n", (s32)((s8)logd[i][1])); break;
			case(0x3C): sprintf(str, "EVOLR %2i\n", (s32)((s8)logd[i][1])); break;
			case(0x4C): sprintf(str, "KON     "); break;
			case(0x5C): sprintf(str, "KOF     "); break;
			case(0x6C): sprintf(str, "FLG   %02X\n", (s32)((s8)logd[i][1])); break;
			case(0x7C): sprintf(str, "ENDX  %02X\n", (s32)((s8)logd[i][1])); break;
			case(0x0D): sprintf(str, "EFB   %02X\n", (s32)((s8)logd[i][1])); break;
			case(0x2D): sprintf(str, "PMON    "); break;
			case(0x3D): sprintf(str, "NON     "); break;
			case(0x4D): sprintf(str, "EON     "); break;
			case(0x5D): sprintf(str, "DIR   %02X00\n", (s32)((s8)logd[i][1])); break;
			case(0x6D): sprintf(str, "ESA   %02X00\n", (s32)((s8)logd[i][1])); break;
			case(0x7D): sprintf(str, "EDL   %ims\n", (s32)((s8)logd[i][1]) << 4); break;
			}

			switch(logd[i][0])
			{
			case(0x4C):
			case(0x5C):
			case(0x2D):
			case(0x3D):
			case(0x4D):
				for (j=0; j<8; j++)
				{
					if (logd[i][1] & (1 << j))
						sprintf(&str[(j * 5) + 8], "---- \n");
					else
						sprintf(&str[(j * 5) + 8], "     \n");
				}
			}
		}

		FileWrite(fh, str, strlen(str));
	}

	FileClose(fh);
}
*/
void DebugDSP(volatile u8 *reg, volatile u8 val)
{
	Source		*pSrc;
	DSPVoice	*pVoice;
	u8			v,i;
	volatile u8 *p;

/*
	if (logp < LOGLEN)
	{
		logt[logp] = Apu.GetSPCTime() >> 1;
		logd[logp][0] = (u8)reg;
		logd[logp][1] = val;
		logp++;
	}
	else
	if (logp == LOGLEN)
	{
		savelog();
		logp++;
	}
*/

	if (!((u8)reg == offsetof(DSPReg, kon) && val)) return;	//If no voices getting keyed on

	p = &val;
	v = val;
	i = 1;
	pVoice = Apu.dsp->voice;
	do
	{
		if (v & 1)								//If a voice is getting keyed on...
		{
			pSrc = &PCurAPU->src[pVoice->srcn];

			if (pSrc->offset != *(u16*)&Apu.ram[((u32)Apu.dsp->dir << 8) | (pSrc->idx << 2)])
			{
				ScanSrc(pSrc);
				pSrc->flags |= SF_CHANGED;
			}

			if (pSrc->adsr == 0xE0FF)			//Do we have the register for this voice yet?
			{
				if (pVoice->adsr1 & 0x80)
					pSrc->adsr = *(u16*)&pVoice->adsr1;
				else
					pSrc->adsr = (u16)pVoice->gain << 8;
			}

			if (pSrc->flags & SF_MUTE) val &= ~i;
		}

		v >>= 1;
		i <<= 1;
		pVoice++;

	} while (v);

	*p = val;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu Items

//**************************************************************************************************
void __fastcall TFrmMain::DisableAll()
{
	BtnEnabled(pbReset, 0);
	BtnEnabled(pbRun, 0);
	BtnEnabled(pbHalt, 0);
	BtnEnabled(pbAbort, 0);
	BtnEnabled(pbFF, 0);
	BtnEnabled(pbUpload, 0);
	BtnEnabled(pbSave, 0);
	BtnEnabled(pbWAV, 0);
	BtnEnabled(pbMIDI, 0);
	BtnEnabled(pbPlay, 0);
	BtnEnabled(pbStop, 0);
	BtnEnabled(pbImport, 0);
	BtnEnabled(pbExport, 0);
	BtnEnabled(pbZoom, 0);

	lblTime->Caption = "";
	lblTotal->Caption = "";

	txtIn0->Enabled = false;
	txtIn1->Enabled = false;
	txtIn2->Enabled = false;
	txtIn3->Enabled = false;

	txtOut0->Enabled = false;
	txtOut1->Enabled = false;
	txtOut2->Enabled = false;
	txtOut3->Enabled = false;
}


//**************************************************************************************************
void __fastcall TFrmMain::EnableItem()
{
	TListItem	*pItem;
	TItemStates	itemState;

	FileInfo	*pFile;
	s32			selCount,allCount,i;
	b8			selSave,anySave,notROM;
	b8			valid,enable;


	itemState << isSelected;

	//File -------------------------------------
	selCount = lvwFile->SelCount;
	allCount = lvwFile->Items->Count;
	selSave = false;
	anySave = false;
	notROM = false;

	pItem = lvwFile->Selected;
	while (pItem)
	{
		pFile = (FileInfo*)pItem->Data;

		if (pFile->flags & F_SAVE)
		{
			selSave = true;
			anySave = true;
		}

		if (pFile->type < ID6_ROM) notROM = true;

		pItem = lvwFile->GetNextItem(pItem, sdAll, itemState);
	}

	if (!anySave)
	{
		for (i=0; i<allCount; i++)
			if (((FileInfo*)lvwFile->Items->Item[i]->Data)->flags & F_SAVE)
			{
				anySave = true;
				break;
			}
	}

	itmClose->Enabled	= selCount > 0;
	itmCloseA->Enabled	= allCount > 0;
	itmSave->Enabled	= selSave;
	itmSaveAs->Enabled	= selCount == 1;
	itmSaveA->Enabled	= anySave;

	BtnEnabled(pbSave, selSave);

	//Tools ------------------------------------
	itmEdit->Enabled = notROM && selCount > 0;

	valid = (notROM && State == ST_STOP);

	itmWAV->Enabled = valid && selCount > 0;
	BtnEnabled(pbWAV, valid && selCount > 0);

	itmMIDI->Enabled = valid && selCount == 1;
	BtnEnabled(pbMIDI, valid && selCount == 1);

	itmROM->Enabled = valid && selCount > 0;
	BtnEnabled(pbUpload, valid && selCount > 0);

	//Emulation --------------------------------
	enable = (State == ST_STOP || State == ST_HALT) &&
			 (PCurFile && PCurFile->type < ID6_ROM);
	itmRun->Enabled = enable;
	BtnEnabled(pbRun, enable);

	enable = (State == ST_RUN);
	itmHalt->Enabled = enable;
	BtnEnabled(pbHalt, enable);
	BtnEnabled(pbFF, enable);

	enable = PCurFile && PCurFile->type < ID6_ROM;
	itmReset->Enabled = enable;
	BtnEnabled(pbReset, enable);

	BtnEnabled(pbUpload, enable && State == ST_STOP);

	enable = (State == ST_RUN || State == ST_HALT);
	itmAbort->Enabled = enable;
	BtnEnabled(pbAbort, enable);

	itmDebug->Enabled	= !(FrmDSP && FrmDSP->Visible);
	itmDSP->Enabled		= !(FrmDebug && FrmDebug->Visible);

	EnableSrcItems();
}


void __fastcall TFrmMain::EnableSrcItems()
{
	TListItem	*pItem;
	TItemStates	itemState;
	s32			selCount;
	b8			valid;


	itemState << isSelected;
	selCount = lvwSrc->SelCount;
	valid = false;

	pItem = lvwSrc->Selected;
	while (pItem)
	{
		if (((Source*)pItem->Data)->offset != -1)
		{
			valid = true;
			break;
		}

		pItem = lvwSrc->GetNextItem(pItem, sdAll, itemState);
	}

	itmImport->Enabled	= (valid && selCount == 1 && State == ST_STOP);
	itmExport->Enabled	= valid;
	itmMute->Enabled	= valid;
	itmSilence->Enabled	= valid;
	itmSetOpt->Enabled	= valid && selCount == 1;
	itmRescan->Enabled	= PCurFile && PCurFile->type < ID6_ROM;

	BtnEnabled(pbPlay, valid && selCount == 1);
	BtnEnabled(pbStop, State == ST_PLAY);
	BtnEnabled(pbImport, valid && selCount == 1 && State == ST_STOP);
	BtnEnabled(pbExport, valid);
}


//**************************************************************************************************
void __fastcall TFrmMain::FileModified(u8 flag)
{
	PCurFile->flags |= flag;
	itmSave->Enabled = true;
	itmSaveA->Enabled = true;
	BtnEnabled(pbSave, 1);
}


//**************************************************************************************************
void __fastcall TFrmMain::RecentFiles()
{
	s8	str[256],*s,*e,*b;
	s32	f,i,j;


	if (Cfg.curFile == -1) return;

	sepF3->Visible = true;

	j = Cfg.curFile;
	f = 0;
	for (i=0; i<9; i++)
	{
		if (Cfg.file[j][0])
		{
			strcpy(str, Cfg.file[j]);

			s = 0;
			b = str;
			do
			{
				e = s;
				s = b;
				b = strchr(b+1, '\\');
			} while (b);

			s = strchr(str, '\\');
			if (s) s = strchr(s+1, '\\');
			if (s && s < e)
			{
				s++;
				*s++ = '.';
				*s++ = '.';
				*s++ = '.';
				strcpy(s, e);
			}

			ItmFile[f]->Caption = "&" + AnsiString(f+1) + " " + str;
			ItmFile[f]->Visible = true;
			ItmFile[f]->Tag = j;
			f++;
		}

		if (--j < 0) j = 8;
	}

	for (; f<9; f++)
		ItmFile[f]->Visible = false;
}


//**************************************************************************************************
// Adds a file, or moves an existing file, to the top of the recents list

void __fastcall TFrmMain::AddRecent(s8 *fn)
{
	s32	i,j;


	for (i=0; i<9; i++)							//Check if file is already in recents list
		if (!strcmp(Cfg.file[i], fn)) break;

	if (i != 9)									//If it is, move files in front down one place
	{
		while (i != (s32)Cfg.curFile)
		{
			j = i + 1;
			if (j > 8) j = 0;
			strcpy(Cfg.file[i], Cfg.file[j]);
			i = j;
		}
	}
	else										//Otherwise increase top position
		if (++Cfg.curFile > 8) Cfg.curFile = 0;

	strcpy(Cfg.file[Cfg.curFile], fn);			//Copy filename to top position
}


//**************************************************************************************************
// Open File(s)
//
// In:
//    type = Type of open operation to perform
//    fn   = OT_DIALOG - NULL
//           OT_DROP   - handle to a file drop operation
//           OT_PATH   - Path to directory or file (path only should have a trailing '\')
//           OT_RAR    - RAR file

s32 __stdcall RARCallback(u32 msg, long data, long p1, long p2)
{
	RARFile	*p;


	if (msg == UCM_PROCESSDATA && ((RARFile*)data)->size >= 0)
	{
		p = (RARFile*)data;

		if (p->alloc < p->size + p2)			//Is more space needed in the buffer?
		{
			p->pData = (s8*)realloc(p->pData, p->size + p2);
			p->alloc = p->size + p2;
		}

		memcpy(&p->pData[p->size], (s8*)p1, p2);
		p->size += p2;
	}

	return 0;
}

b8 __fastcall TFrmMain::OpenFiles(OpenType ot, s8 *fn = NULL)
{
	s8			str[256],*s;
	ID666		id6;
	ID6Type		type;
	ROMHdr		*pROM;

	ffblk	  	find;

	s32			fh,								//File handle
				numFiles,
				numItems;						//Number of items in file list view
	u32			idx;							//Index into file name array
	b8			success;
	TListItem	*pItem,*pFirst;
	SPCReg		*pReg;

	FileInfo	*pf;
	APUFile		*pa;
	ROMFile		*pr;
	SourceInfo	*pInfo;

	s32			i;

	RAROpenArchiveData	ropen;
	RARHeaderData		rhdr;
	HANDLE				rh;
	RARFile				rf;


	//Find the number of files to open ---------
	switch(ot)
	{
	case(OT_PATH):
		strcpy(str, fn);
		s = ::StrEnd(str);

		if (s[-1] != '\\')						//Is 'fn' a filename?
		{
			ot = OT_FILE;
			numFiles = 1;
		}
		else
		{
			strcat(fn, "*.*");					//Append a search wild card
			numFiles = -1;						//Number of files is unknown
		}
		break;

	case(OT_DIALOG):
		numFiles = dlgOpen->Files->Count;
		break;

	case(OT_DROP):
		numFiles = DragQueryFile((HDROP)fn, -1, NULL, 0);
		break;

	case(OT_RAR):
		memset(&ropen, 0, sizeof(RAROpenArchiveData));
		ropen.ArcName	= fn;
		ropen.OpenMode	= RAR_OM_LIST;

		rh = Rar.OpenArchive(&ropen);
		if (!rh) return 0;

		numFiles = 0;
		while(!Rar.ReadHeader(rh, &rhdr))
		{
			if ((rhdr.Flags & 0xE0) != 0xE0) numFiles++;
			if (Rar.ProcessFile(rh, RAR_SKIP, NULL, NULL)) break;
		}

		Rar.CloseArchive(rh);

		rf.pData = NULL;
		rf.alloc = 0;

		ropen.OpenMode	= RAR_OM_EXTRACT;
		rh = Rar.OpenArchive(&ropen);
		Rar.SetCallback(rh, RARCallback, (s32)&rf);

		AddRecent(fn);

		strcpy(str, fn);
		s = ::StrEnd(str);
		*s++ = '\\';
		break;
	}

	if (!numFiles) return 0;

	//==========================================
	// Begin Opening Files

	if (PCurFile)								//If a file is already loaded, save its state which
	{											// will be destroyed
		SetState(ST_STOP);						//Turn off any sound that may be playing

		SaveSrcPos();

		if (PCurFile->type < ID6_ROM)
		{
			Apu.SaveAPU(&PCurAPU->state.spcState, &PCurAPU->state.dspState);
			PCurFile = NULL;
			lvwFile->Tag = NULL;
		}
	}

	lvwFile->Items->BeginUpdate();
	numItems = lvwFile->Items->Count;

	i = numItems + numFiles;					//Increase AllocBy in file list view
	if (lvwFile->AllocBy < i) lvwFile->AllocBy = (i + 15) & ~15;

	success = 0;								//Nothing has been successfully opened yet
	pFirst = NULL;
	idx = 0;
	do
	{
		//Copy a filename into 'str' -----------
		switch(ot)
		{
		case(OT_PATH):
			if (idx ? findnext(&find) : findfirst(fn, &find, 0))
			{
				findclose(&find);

				if (success)
				{
					*ScanStrR(fn, '\\') = 0;
					*s = 0;
					AddRecent(str);
				}

				goto Done;
			}

			strcpy(s, find.ff_name);
			break;

		case(OT_DIALOG):
			if (dlgOpen->Files->Strings[idx].Length() > 255) continue;
			strcpy(str, dlgOpen->Files->Strings[idx].c_str());
			break;

		case(OT_DROP):
			DragQueryFile((HDROP)fn, idx, str, 255);
			break;

		case(OT_RAR):
			rf.size = 0;
			if (Rar.ReadHeader(rh, &rhdr) ||
				Rar.ProcessFile(rh, RAR_TEST, NULL, NULL)) goto Done;

			strcpy(s, rhdr.FileName);
			break;
		}

		//Check if file's already loaded -------
		for (i=0; i<numItems; i++)
		{
			if (!strcmpi(str, ((FileInfo*)lvwFile->Items->Item[i]->Data)->id6.file))
			{
				pItem = lvwFile->Items->Item[i];
				goto Loaded;
			}
		}

		//Get file type ------------------------
		if (ot == OT_RAR)
			type = id6.LoadTag(rf.pData, rf.size);
		else
			type = id6.LoadTag(str);

		switch(type)
		{
		case(ID6_UNK):							//If file type is unknown, it may be a RAR file
			if (ot != OT_RAR && Rar.GetDllVersion)
				success |= OpenFiles(OT_RAR, str);

		case(ID6_ERR):
			continue;
		}

		switch(type)
		{
		case(ID6_SPC):
		case(ID6_EXT):
			//Load file ------------------------
			pf = (FileInfo*)malloc(sizeof(APUFile));

			pa = (APUFile*)pf;
			pa->type	= type;
			pa->flags	= 0;
			pa->num		= 256;
			pa->pData	= pa->spc.ram;
			pa->size	= 65536;
			pa->pTop	= NULL;
			pa->id6		= id6;

			if (ot != OT_RAR)
			{
				fh = FileOpen(id6.file, fmOpenRead);
				FileRead(fh, &pa->spc.hdr, 0x10200);
				FileClose(fh);
			}
			else
			{
				memcpy(&pa->spc.hdr, rf.pData, 0x10200);
				strcpy(id6.file, str);
				strcpy(pa->id6.file, str);
				pa->flags = F_READONLY | F_ARCHIVE;
			}

			//Add SPC to file list -------------
			pItem = lvwFile->Items->Add();
			pItem->Caption = ScanStrR(id6.file,'\\');
			pItem->Data = pf;
			UpdateFile(pItem);

			//Create APU state -----------------
			if (id6.amp)
				Apu.SetDSPAmp(id6.amp);
			else
				Apu.SetDSPAmp(Cfg.mix.amp);

			Apu.LoadSPCFile(&pa->spc);

			pa->state.spcState.pRAM		= pa->state.ram;
			pa->state.dspState.pReg		= &pa->state.dsp;
			pa->state.dspState.pVoice	= pa->state.voice;
			pa->state.dspState.pEcho	= NULL;
			Apu.SaveAPU(&pa->state.spcState, &pa->state.dspState);

			ScanSrcDir(pa->src, &Apu.ram[(u32)Apu.dsp->dir << 8], Cfg.src.noDP,
					   Cfg.src.min, Cfg.src.max,
					   Cfg.src.linear, Cfg.src.silence, Cfg.src.range);
			break;

		case(ID6_ZST):
			continue;

		case(ID6_ROM):
		case(ID6_SWC):
		case(ID6_FIG):
		case(ID6_SF3):
			//Open file ------------------------
			fh = FileOpen(id6.file, fmOpenRead);
			i = FileSeek(fh, 0, 2) - FileSeek(fh, type == ID6_ROM ? 0 : 512, 0);

			//Add ROM to file list -------------
			pf = (FileInfo*)malloc(sizeof(ROMFile) + i);

			pr = (ROMFile*)pf;
			pr->type	= type;
			pr->flags	= 0;
			pr->pData	= pr->rom;
			pr->size	= i;
			pr->pTop	= NULL;
			pr->id6		= id6;

			pItem = lvwFile->Items->Add();
			pItem->Caption = ScanStrR(id6.file,'\\');
			pItem->Data = pf;
			UpdateFile(pItem);

			FileRead(fh, &pr->rom, i);
			FileClose(fh);

			pROM = (ROMHdr*)&pr->rom[0xFFC0];
			if ((pROM->crc ^ pROM->icrc) == 0xFFFF &&
				(pROM->makeup & 0xF) == 0) pr->flags |= F_HIROM;

			if ((Cfg.lcid & 0x3FF) != 0x11)
				Romaji(pr->id6.game, id6.game);

			pr->num = ScanROM(pr->src, pr->rom, 0, pr->size,
							  Cfg.src.min, Cfg.src.max,
							  Cfg.src.linear, Cfg.src.silence, Cfg.src.range);
			break;

		case(ID6_UNK):
		default:
			continue;
		}

		//Try to guess playback rate for each sound source
		//TODO: This should happen in the scanning subroutine
		if (type < ID6_ROM)
		{
			for (i=0; i<(s32)pf->num; i++)
			{
				pa->ord[i] = i;

				if (pf->src[i].loop != 0xFFFF)
				{
					pf->src[i].rate = (s32)(261.6255653 * ((pf->src[i].len - pf->src[i].loop) << 4));
					pf->src[i].rate /= (pf->src[i].rate / 32000) + 1;
				}
				pf->src[i].prog = 0;
			}

			pa->sel.Clear();
			pa->sel << 0;
		}
		else
		{
			for (i=0; i<(s32)pf->num; i++)
				pr->ord[i] = i;

			pr->sel.Clear();
			pr->sel << 0;
		}

		//Retrieve source info from table ------
		pInfo = PInfoStart;
		while (pInfo)
		{
			for (i=0; i<(s32)pf->num; i++)
			{
				if (pf->src[i].offset != -1 &&
					pf->src[i].crc == pInfo->crc &&
					*(u32*)&pf->src[i].len == *(u32*)&pInfo->len)
				{
					pf->src[i].flags |= SF_INFO;
					pf->src[i].rate	= pInfo->rate;
					pf->src[i].adsr	= pInfo->adsr;
					pf->src[i].prog	= pInfo->prog;
					pf->src[i].bank	= pInfo->bank;
				}
			}

			pInfo = pInfo->pNext;
		}

		if (ot != OT_RAR && numFiles != -1)		//Add file to recents if it was opened individually
			AddRecent(str);

Loaded:
		if (!success)							//Is this the first file successfully opened?
		{
			success = 1;

			if (lvwFile->SelCount <= 1)
			{
				pFirst = pItem;
				if (lvwFile->Selected) lvwFile->Selected->Selected = false;
			}
		}

	} while (++idx < (u32)numFiles);

Done:
	if (ot == OT_RAR)
	{
		Rar.CloseArchive(rh);
		if (rf.pData) free(rf.pData);
	}

	if (success)
	{
		if (pFirst)								//Change file selection
		{
			lvwFile->Selected = pFirst;
			lvwFile->ItemFocused = pFirst;
		}

		lvwFile->AlphaSort();
		lvwFile->Column[0]->Width = -2;

		RecentFiles();
	}

	lvwFile->Items->EndUpdate();

	lvwFileResize(NULL);

	return success;
}

__fastcall TOpenDlgEx::TOpenDlgEx(TComponent* owner) : TOpenDialog(owner)
{
  Template = "OPENDLGTEMPLATE";
}

void __fastcall TOpenDlgEx::WndProc(Messages::TMessage &msg)
{
	if (msg.Msg == WM_COMMAND &&
		msg.WParamHi == BN_CLICKED &&
		msg.WParamLo == 0x500)
	{
		SendMessage(GetParent(Handle), CDM_GETFOLDERPATH, 251, (s32)path);
		strcat(path, "\\");
		SetWindowLong(Handle, DWL_MSGRESULT, 1);
		SendMessage(GetParent(Handle), WM_CLOSE, 0, 0);
	}
	else
		TOpenDialog::WndProc(msg);
}

b8 __fastcall TOpenDlgEx::Execute()
{
  path[0] = 0;

  return TOpenDialog::Execute();
}


void __fastcall TFrmMain::dlgOpenShow(TObject *sender)
{
	RECT	r;
	HWND	parent,next,chk;
	s32		top;


	parent = GetParent(dlgOpen->Handle);
	top = 0;

	next = FindWindowEx(parent, NULL, "Button", NULL);
	do
	{
		GetWindowRect(next, &r);

		if (top < r.top)
		{
			top = r.top;
			chk = next;
		}

		next = FindWindowEx(parent, next, "Button", NULL);
	} while (next);

	SetWindowText(chk, "Open entire folder");
}


void __fastcall TFrmMain::itmOpenClick(TObject *sender)
{
	s8	str[256],*s;


	strcpy(str, PCurFile ? PCurFile->id6.file : Cfg.file[Cfg.curFile]);
	if (str[0])
	{
		s = strrchr(str, '\\');
		if (s) *s = 0;
		dlgOpen->InitialDir = str;
		dlgOpen->FileName = "";
	}

	if (dlgOpen->Execute()) OpenFiles(OT_DIALOG);
	else
	if (dlgOpen->path[0]) OpenFiles(OT_PATH, dlgOpen->path);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmCloseClick(TObject *sender)
{
	TLVSelectItemEvent	event;
	TListItem	*pItem,*pNext;
	TItemStates	selected;
	AnsiString	file;
	s32			i;


	selected << isSelected;

	//Check for any modified files -------------
	if (lvwFile->SelCount > 1)
	{
		pItem = lvwFile->Selected;
		while (pItem)
		{
			if (((FileInfo*)pItem->Data)->flags & F_SAVE) break;
			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		}

		if (pItem)
		{
			i = MessageDlg("You have modified some files.  Do you want to save them before closing?",
				mtWarning, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

			if (i == mrCancel) return;
			else
			if (i == mrYes) itmSaveAClick(NULL);
		}
	}
	else
	{
		if (PCurFile->flags & F_SAVE)
		{
			file = ScanStrR(PCurFile->id6.file, '\\');

			i = MessageDlg(AnsiString("Save changes to ") + file + "?",
				mtWarning, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

			if (i == mrCancel) return;
			else
			if (i == mrYes) itmSaveClick(NULL);
		}

		SetState(ST_STOP);
	}

	//Close selected files ---------------------
	lvwSrc->Items->BeginUpdate();				//Clear source listview
	lvwSrc->Items->Clear();
	lvwSrc->Items->EndUpdate();

	event = lvwFile->OnSelectItem;				//Disable OnSelectItem event
	lvwFile->OnSelectItem = NULL;

	lvwFile->Items->BeginUpdate();				//Close files
	if (lvwFile->Selected)
	{
		pItem = lvwFile->Selected;
		while (pItem)
		{
			pNext = lvwFile->GetNextItem(pItem, sdAll, selected);
			free(pItem->Data);
			pItem->Delete();
			pItem = pNext;
		}
	}
	else
	{
		free(PCurItem->Data);
		PCurItem->Delete();
	}

	lvwFile->Items->EndUpdate();

	PCurItem = NULL;
	PCurFile = NULL;
	PCurSrc = NULL;
	lvwFile->Tag = NULL;
	lvwFile->OnSelectItem = event;				//Restore event

	DrawWave(NULL);

	if (!lvwFile->Items->Count) DisableAll();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmCloseAClick(TObject *sender)
{
	TLVSelectItemEvent	event;
	s32	i;


	//Check for any modified files -------------
	if (sender)
	{
		for (i=0; i<lvwFile->Items->Count; i++)
		{
			if (((FileInfo*)lvwFile->Items->Item[i]->Data)->flags & F_SAVE)
			{
				i = MessageDlg("You have modified some files.  Do you want to save them before closing?",
					mtWarning, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

				if (i == mrCancel) return;
				else
				if (i == mrYes) itmSaveAClick(NULL);

				break;
			}
		}
	}

	//Close files ------------------------------
	SetState(ST_STOP);

	lvwSrc->Items->BeginUpdate();
	lvwSrc->Items->Clear();
	lvwSrc->Items->EndUpdate();

	event = lvwFile->OnSelectItem;
	lvwFile->OnSelectItem = NULL;

	i = lvwFile->Items->Count;
	while(i) free(lvwFile->Items->Item[--i]->Data);

	lvwFile->Items->BeginUpdate();
	lvwFile->Items->Clear();
	lvwFile->Items->EndUpdate();

	PCurItem = NULL;
	PCurFile = NULL;
	PCurSrc = NULL;
	lvwFile->Tag = NULL;
	lvwFile->OnSelectItem = event;

	DrawWave(NULL);

	DisableAll();
}


//**************************************************************************************************
b8 __fastcall Save(TListItem *pItem)
{
	FileInfo	*p;
	s32			fh;


	p = (FileInfo*)pItem->Data;

	if (p->flags & F_READONLY) return 0;

	//Write out ID666 tag ----------------------
	if (p->flags & F_TAG) p->id6.SaveTag(0, p->id6.IsExt());

	//Write out state/ROM data -----------------
	if (p->flags & F_STATE)
	{
		fh = FileOpen(p->id6.file, fmOpenWrite);
		if (fh == -1)
		{
			MessageDlg(AnsiString("Could not save ") + ScanStrR(p->id6.file, '\\'),
					   mtError, TMsgDlgButtons() << mbOK, 0);
			return 0;
		}

		if (p->type <= ID6_EXT)
		{
			FileSeek(fh, 37, 0);
			FileWrite(fh, &((APUFile*)p)->spc.hdr.reg, sizeof(SPCReg));
			FileSeek(fh, 256, 0);
			FileWrite(fh, ((APUFile*)p)->spc.ram, 0x10100);
		}
		else
		if (p->type >= ID6_ROM)
		{
			if (p->type > ID6_ROM) FileSeek(fh, 512, 0);
			FileWrite(fh, p->pData, p->size);
		}

		FileClose(fh);
	}

	if (p->flags & F_SAVE)
	{
		p->flags &= ~F_SAVE;
		pItem->Caption.Delete(pItem->Caption.Length() - 1, 2);
		return 1;
	}

	return 0;
}

void __fastcall TFrmMain::itmSaveClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;


	selected << isSelected;

	if (lvwFile->Selected)
	{
		pItem = lvwFile->Selected;
		do
		{
			Save(pItem);
			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);
	}
	else
		Save(PCurItem);

	lvwFile->Repaint();

	EnableItem();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSaveAsClick(TObject *sender)
{
	s8		hdr[512];
	s8		*s;
	s32		fh;
	ROMHdr	*pROM;
	u32		i;


	//Get current path and filename ------------
	s = strrchr(PCurFile->id6.file, '\\');
	*s++ = 0;									//Remove backslash and terminate path string
	dlgOpen->InitialDir = PCurFile->id6.file;
	dlgSave->FileName = s;
	s[-1] = '\\';								//Restore backslash

	//Set filter based on current file type ----
	if (PCurFile->type <= ID6_EXT)
	{
		dlgSave->Filter = "SNES-SPC700 Sound File (*.spc)|*.sp?";
		dlgSave->FilterIndex = 1;
	}
	else
	if (PCurFile->type == ID6_ZST)
	{
		dlgSave->Filter = "SNES-SPC700 Sound File (*.spc)|*.sp?|ZSNES Save State File (*.zst)|*.zst";
		dlgSave->FilterIndex = 2;
	}
	else
	if (PCurFile->type >= ID6_ROM)
	{
		dlgSave->Filter = "SNES ROM image (*.smc)|*.smc|Super WildCard (*.swc)|*.swc";//|Pro Fighter (*.fig)|*.fig";
		if (PCurFile->type == ID6_SWC) dlgSave->FilterIndex = 2;
//		else
//		if (PCurFile->type == ID6_FIG) dlgSave->FilterIndex = 3;
		else
			dlgSave->FilterIndex = 1;
	}

	//Save file --------------------------------
	if (!dlgSave->Execute()) return;

	if (dlgSave->FileName.Length() > 251)
	{
		MessageDlg("Path and filename too long", mtError, TMsgDlgButtons() << mbOK, 0);
		return;
	}

	strcpy(hdr, dlgSave->FileName.c_str());
	s = strrchr(hdr, '.');						//If filename doesn't have an extension, add one
	if (!s || strrchr(s, '\\'))
	{
		switch(dlgSave->FilterIndex)
		{
		case(1):
			if (PCurFile->type < ID6_ROM)
				strcat(hdr, ".spc");
			else
				strcat(hdr, ".smc");
			break;

		case(2):
			if (PCurFile->type < ID6_ROM)
				strcat(hdr, ".zst");
			else
				strcat(hdr, ".swc");
			break;

		case(3):
			strcat(hdr, ".fig");
			break;
		}
	}

	for (i=0; i<(u32)lvwFile->Items->Count; i++)
	{
		if (PCurFile != lvwFile->Items->Item[i]->Data &&
			!strcmpi(hdr, ((FileInfo*)lvwFile->Items->Item[i]->Data)->id6.file))
		{
			MessageDlg("A file with that filename is already open", mtError, TMsgDlgButtons() << mbOK, 0);
			return;
		}
	}

	fh = FileCreate(hdr);
	if (fh == -1)
	{
		MessageDlg(AnsiString("Could not save ") + ScanStrR(hdr, '\\'),
				   mtError, TMsgDlgButtons() << mbOK, 0);
		return;
	}

	strcpy(PCurFile->id6.file, hdr);

	if (PCurFile->type < ID6_ROM)
	{
		if (dlgSave->FilterIndex == 1)
		{
			FileSeek(fh, 37, 0);
			FileWrite(fh, &((APUFile*)PCurFile)->spc.hdr.reg, sizeof(SPCReg));
			FileSeek(fh, 256, 0);
			FileWrite(fh, ((APUFile*)PCurFile)->spc.ram, 0x10100);
			FileClose(fh);

			PCurFile->id6.SaveTag(0, PCurFile->id6.IsExt());
		}
	}
	else
	{
		switch(dlgSave->FilterIndex)
		{
		case(2):
			pROM = (ROMHdr*)&((ROMFile*)PCurFile)->rom[(PCurFile->flags & F_HIROM) ? 0xFFC0 : 0x7FC0];
			memset(hdr, 0, 512);

			*(u16*)&hdr[0] = PCurFile->size >> 13;

			if (pROM->sram == 0) hdr[2] = 0xC;	//No SRAM
			else
			if (pROM->sram == 1) hdr[2] = 0x8;	//16kb
			else
			if (pROM->sram <= 3) hdr[2] = 0x4;	//64kb

			if (pROM->makeup & 0xF) hdr[2] |= 0x30;

			hdr[8] = 0xAA;
			hdr[9] = 0xBB;
			hdr[10] = 4;

			FileWrite(fh, hdr, 512);
			break;

		case(3):
			pROM = (ROMHdr*)&((ROMFile*)PCurFile)->rom[(PCurFile->flags & F_HIROM) ? 0xFFC0 : 0x7FC0];
			memset(hdr, 0, 512);

			*(u16*)&hdr[0] = PCurFile->size >> 13;

			if (pROM->makeup & 0xF) hdr[3] = 0x80;

			if (pROM->mtype >= 3 && pROM->mtype <= 5)
				hdr[4] = pROM->sram ? 0xFD : 0x47;
			else
				hdr[4] = 0x77;

			if (pROM->mtype >= 3 && pROM->mtype <= 5 && pROM->sram)
				hdr[5] = 0x82;
			else
				hdr[5] = 0x83;

			FileWrite(fh, hdr, 512);
			break;

		}

		FileWrite(fh, PCurFile->pData, PCurFile->size);
		FileClose(fh);
	}

	PCurFile->flags &= ~F_SAVE;
	PCurItem->Caption = ScanStrR(PCurFile->id6.file, '\\');

	lvwFile->Repaint();
	EnableItem();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSaveAClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	s32			i;


	for (i=0; i<lvwFile->Items->Count; i++)
		Save(lvwFile->Items->Item[i]);

	lvwFile->Repaint();

	EnableItem();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmFileClick(TObject *sender)
{
	s32	i;


	i = ((TMenuItem*)sender)->Tag;
	if (!OpenFiles(OT_PATH, Cfg.file[i]))		//If the file couldn't be opened, remove it from the
	{											// recent list
		Cfg.file[i][0] = 0;

		if ((s32)Cfg.curFile == i)
			if (--Cfg.curFile > 8) Cfg.curFile = 8;

		RecentFiles();
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::itmExitClick(TObject *sender)
{
	Close();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmRunClick(TObject *sender)
{
	if (State == ST_PLAY) StopNote();

	SetState(ST_RUN);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmHaltClick(TObject *sender)
{
	if (FrmDSP && FrmDSP->Visible)
	{
		SPCPaused ^= 1;
		return;
	}

	if (State == ST_HALT || SPCPaused)
	{
		SetState(ST_RUN);
		SPCPaused = 0;
	}
	else
	{
		SetState(ST_HALT);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::itmResetClick(TObject *sender)
{
	u32	amp;


	if (State == ST_RUN) WavFlush();

	if (Cfg.mix.reset)							//Reset mixing level
	{
		if (PCurFile->id6.amp)
			amp = PCurFile->id6.amp;			//Use level from tag, if it has one
		else
			amp = Cfg.mix.amp;

		SetTrkAmp(amp);
		Apu.SetDSPAmp(amp);
	}

	Apu.LoadSPCFile(&PCurAPU->spc);

	if (FrmDSP && FrmDSP->Visible) FrmDSP->Reset();

	SetAPULength();

	txtIn0->Text = IntToHex((s32)Apu.ram[0xF4], 2);
	txtIn1->Text = IntToHex((s32)Apu.ram[0xF5], 2);
	txtIn2->Text = IntToHex((s32)Apu.ram[0xF6], 2);
	txtIn3->Text = IntToHex((s32)Apu.ram[0xF7], 2);

	txtOut0->Text = IntToHex((s32)Apu.GetAPUPort(0), 2);
	txtOut1->Text = IntToHex((s32)Apu.GetAPUPort(1), 2);
	txtOut2->Text = IntToHex((s32)Apu.GetAPUPort(2), 2);
	txtOut3->Text = IntToHex((s32)Apu.GetAPUPort(3), 2);

	if (State == ST_RUN) WavRestart(Apu.GetSPCTime());
}


//**************************************************************************************************
void __fastcall TFrmMain::itmAbortClick(TObject *sender)
{
	if (!(State == ST_RUN || State == ST_HALT)) return;

	SetState(ST_STOP);
	itmResetClick(NULL);
}


//**************************************************************************************************
void WAVRecord(TListView *lvwFile)
{
	FrmWav->Record(lvwFile);
}

void __fastcall TFrmMain::itmWAVClick(TObject *sender)
{
	HANDLE		th;
	u32			tID;
	DSPDebug	*oldDSP;


	if (!FrmWav) Application->CreateForm(__classid(TFrmWav), &FrmWav);
	if (!FrmProg) Application->CreateForm(__classid(TFrmProg), &FrmProg);

	if (FrmWav->ShowModal() == mrOk)
	{
		oldDSP = Apu.SetDSPDbg(NULL, 0);

		th = (HANDLE)CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)WAVRecord, lvwFile, 0, (LPDWORD)&tID);
		FrmProg->ShowTotal = true;
		FrmProg->AutoClose = false;
		FrmProg->Caption = "Recording DSP...";
		FrmProg->ShowModal();
		CloseHandle(th);						//Deallocate handle resources
		trkSpeedChange(NULL);
		trkPitchChange(NULL);

		Apu.SetDSPDbg(oldDSP, 0);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::itmMIDIClick(TObject *sender)
{
	Application->MessageBox("MIDI support not implemented.", "SPCTool", MB_OK);
/*
	frmMIDI->pCurAPU = (APUFile*)PCurFile;

	if (frmMIDI->ShowModal() == mrOk)
	{
		trkSpeedChange(NULL);
		trkPitchChange(NULL);
	}
*/
}


//**************************************************************************************************
void __fastcall TFrmMain::itmTimerClick(TObject *sender)
{
	pbTimer->Tag ^= 1;
	pbBtnPaint(pbTimer);
	pbTimerClick(NULL);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmDebugClick(TObject *sender)
{
	if (FrmDSP && FrmDSP->Visible) return;
	
	SetState(ST_STOP);
	FrmDebug->pState = PCurAPU;
	FrmDebug->ShowModal();

	if (FrmDebug->commit)
	{
		if (!(PCurFile->flags & F_SAVE)) PCurItem->Caption = PCurItem->Caption + " *";

		FileModified(F_STATE);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::itmDSPClick(TObject *sender)
{
	if (FrmDebug && FrmDebug->Visible) return;

	if (!FrmDSP) Application->CreateForm(__classid(TFrmDSP), &FrmDSP);
	FrmDSP->Show();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmProfClick(TObject *Sender)
{
	if (!FrmProf) Application->CreateForm(__classid(TFrmProf), &FrmProf);
	FrmProf->Show();
}


//**************************************************************************************************
void __fastcall TFrmMain::itmImportClick(TObject *sender)
{
	s32		f,l,i;
	Chunk	chk;
	SubChk	schk;
	s32		data,size,tsize;
	Source	*pSrc;
	s16		*pSmp;
	s8		str[8],*s;

	SourceInfo		info;
	WAVEFORMATEX	fmt;
	SmpChk			smp;


	//*** Todo - Display message to reset state ***

	if (!dlgImport->Execute()) return;

	f = FileOpen(dlgImport->FileName, fmOpenRead);
	if (f == -1) return;

	FileRead(f, &chk, sizeof(Chunk));			//Verify this is a RIFF file
	if (chk.id != 'RIFF') goto Done;

	pSrc = PCurSrc;
	switch(chk.type)
	{
	case('snes'):
		//======================================
		// Read in .BRR file

		while (FileRead(f, &chk, sizeof(Chunk)))
		{
			if (chk.id == 'LIST' && chk.type == 'srcd')
			{
				l = chk.size - 4;
				while (l > 0)
				{
					l -= FileRead(f, &schk, sizeof(SubChk));

					switch(schk.id)
					{
					case('info'):
					case('srci'):
						FileRead(f, &info, sizeof(SourceInfo));
						if (schk.size != sizeof(SourceInfo))
							FileSeek(f, (s32)(schk.size - sizeof(SourceInfo)), 1);
						l -= schk.size;
						break;

					case('data'):
						data = FileSeek(f, 0, 1);
						FileSeek(f, (s32)schk.size, 1);
						l -= schk.size;
						break;

					default:
						FileSeek(f, (s32)schk.size, 1);
						l -= schk.size;
					}
				}
			}
			else
				FileSeek(f, (s32)(chk.size - 4), 1);
		}

		if (info.len > pSrc->len)
		{
			if (pSrc->loop == 0xFFFF)
			{
				i = MessageDlg("The BRR file is too large to replace the current source safely.  " \
							   "Select 'Yes' to load it anyway, 'No' to truncate it.",
							   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
				if (i == mrCancel) goto Done;
				if (i == mrNo) info.len = pSrc->len;
			}
			else
			{
				if (MessageDlg("The BRR file is too large to replace the current source safely.  " \
							   "Do you want to load it anyway?  (You may overwrite necessary data.)",
							   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo) goto Done;
			}
		}

		memset(&PCurFile->pData[pSrc->offset], 0, pSrc->len * 9);	//Erase old sound source

		FileSeek(f, data + (s32)info.offset, 0);
		FileRead(f, &PCurFile->pData[pSrc->offset], info.len * 9);

		pSrc->len	= info.len;
		pSrc->loop	= info.loop;
		pSrc->crc	= info.crc;
		pSrc->rate	= info.rate;
		pSrc->adsr	= info.adsr;
		pSrc->prog	= info.prog;
		pSrc->bank	= info.bank;

		pSrc->flags &= ~0xF;
		for (i=0; i<pSrc->len; i++)
			pSrc->flags |= 1 << ((PCurFile->pData[pSrc->offset + (i * 9)] >> 2) & 3);

		break;

	case('WAVE'):
		data = 0;
		smp.cSampleLoops = 0;

		//======================================
		// Read in .WAV file

		while (FileRead(f, &schk, sizeof(SubChk)))
		{
			switch(schk.id)
			{
			case('fmt '):
				if (schk.size == sizeof(WAVEFORMAT))
				{
					FileRead(f, &fmt, sizeof(WAVEFORMAT));
					fmt.wBitsPerSample = fmt.nAvgBytesPerSec / fmt.nSamplesPerSec / fmt.nChannels;
				}
				else
				{
					FileRead(f, &fmt, sizeof(WAVEFORMATEX));
					FileSeek(f, (s32)(schk.size - sizeof(WAVEFORMATEX)), 1);
				}

				if (fmt.wFormatTag != WAVE_FORMAT_PCM ||
					fmt.nChannels != 1 ||
					(fmt.wBitsPerSample != 16 && fmt.wBitsPerSample != 8))
				{
					MessageDlg("WAV files must be 8- or 16-bit mono PCM", mtError, TMsgDlgButtons() << mbOK, 0);
					goto Done;
				}
				break;

			case('data'):
				size = schk.size;
				data = FileSeek(f, 0, 1);
				FileSeek(f, (s32)schk.size, 1);
				break;

			case('smpl'):
				if (schk.size <= sizeof(SmpChk))
				{
					FileRead(f, &smp, schk.size);
				}
				else
				{
					FileRead(f, &smp, sizeof(SmpChk));
					FileSeek(f, (s32)(schk.size - sizeof(SmpChk)), 1);
				}

				if (smp.cSampleLoops == 0) break;

				if (smp.cSampleLoops > 1)
				{
					MessageDlg("WAV file contains more than one loop", mtError, TMsgDlgButtons() << mbOK, 0);
					goto Done;
				}
				break;

			default:
				FileSeek(f, (s32)schk.size, 1);
			}
		}

		//======================================
		//Determine size of sound source

		tsize = size;
		if (fmt.wBitsPerSample == 16) tsize >>= 1;

		if (smp.cSampleLoops)
		{
			tsize = ((smp.loop.dwStart + 15) & ~15) +
					(((tsize - smp.loop.dwStart) + 15) &~15);

			//Adjust rate for middle C
			if (smp.dwMIDIUnityNote && smp.dwMIDIUnityNote != 60)
			{
				pSrc->rate = (s32)(((f64)fmt.nSamplesPerSec) /
							 pow(2.0, ((f64)smp.dwMIDIUnityNote + ((f64)smp.dwMIDIPitchFraction / 4294967296.0)) / 12.0) - 5.0);
			}
		}
		else
		{
			tsize = (tsize + 15) & ~15;
			pSrc->rate = fmt.nSamplesPerSec;
		}

		tsize += 16;

		//If source is too big, inform user ----
		if ((tsize >> 4) > pSrc->len)
		{
			i = MessageDlg("The WAV file is too large to replace the current source safely.  " \
						   "Select 'Yes' to load it anyway, 'No' to truncate it.",
						   mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

			if (i == mrCancel) goto Done;

			if (i == mrNo)
			{
				size = (pSrc->len - 1) << 4;
				if (fmt.wBitsPerSample == 16) size <<= 1;
			}
		}

		memset(&PCurFile->pData[pSrc->offset], 0, pSrc->len * 9);	//Erase old sound source

		//======================================
		// Create compacted sound source

		if (pSrc->rate < 7) pSrc->rate = 7;
		if (pSrc->rate > 128000) pSrc->rate = 128000;
		pSrc->adsr	= 0xE0FF;
		pSrc->prog	= (pSrc->loop != 0xFFFF) ? 0 : 0x80;
		pSrc->bank	= 0;

		//Read in sample data ------------------
		pSmp = (s16*)malloc(size);
		FileSeek(f, data, 0);
		FileRead(f, pSmp, size);

		//Pack samples -------------------------
//TODO: Apply new packing function
		i = 0;
//		if (smp.cSampleLoops) i |= BRR_LOOP;
//		if (fmt.wBitsPerSample == 8) i |= BRR_8BIT;
//		else size >>= 1;

//		Apu.PackSrc(&PCurFile->pData[pSrc->offset], pSmp, (u32*)&size, &smp.loop.dwStart, i);
		free(pSmp);

		pSrc->len = size;
		pSrc->loop = smp.loop.dwStart;

		//Calculate CRC ------------------------
		pSrc->crc = CRC16(pSmp, pSrc->len * 9);

		//Find filters used for compression ----
		pSrc->flags &= ~(SF_PLAYED|SF_FILTERS);

		for (i=0; i<pSrc->len; i++)
			pSrc->flags |= 1 << ((PCurFile->pData[pSrc->offset + (i * 9)] >> 2) & 3);

		break;
	}

	if (pSrc->loop != 0xFFFF)
	{
		if (PCurFile->type < ID6_ROM)
			*(u16*)&PCurFile->pData[(Apu.dsp->dir << 8) + (pSrc->idx << 2) + 2] =
															(u16)((pSrc->loop * 9) + pSrc->offset);
		else
			pSrc->loop = 0;
	}

	//Update source item -----------------------
	lvwSrc->Selected->SubItems->Strings[1] = pSrc->len << 4;

	if (pSrc->loop != 0xFFFF)
	{
		if (PCurFile->type < ID6_ROM)
			lvwSrc->Selected->SubItems->Strings[2] = pSrc->loop << 4;
		else
			lvwSrc->Selected->SubItems->Strings[2] = "yes";
	}
	else
		lvwSrc->Selected->SubItems->Strings[2] = " ";

	s = str;
	*(u32*)&s[0] = 0x20202020;
	*(u32*)&s[4] = 0x20202020;

	if (pSrc->flags & 1) *s++ = '0';
	else s += 2;
	if (pSrc->flags & 2) *s++ = '1';
	else s += 2;
	if (pSrc->flags & 4) *s++ = '2';
	else s += 2;
	if (pSrc->flags & 8) *s++ = '3';
	*s = 0;

	lvwSrc->Selected->SubItems->Strings[3] = str;

	DrawWave(pSrc);

	if (!(PCurFile->flags & F_SAVE)) PCurItem->Caption = PCurItem->Caption + " *";

	FileModified(F_STATE);

Done:
	FileClose(f);
}


//**************************************************************************************************
b8 __fastcall SaveBRR(AnsiString file, Source *pSrc)
{
	s32		fh,i;
	BRRFile	src;
	u16		*p;


	fh = FileCreate(file);
	if (fh == -1) return 0;

	i = ((pSrc->len * 9) + 3) & ~3;

	src.riff.id	   			= 'RIFF';
	src.riff.size			= i + sizeof(BRRFile) - 8;
	src.riff.type  			= 'snes';

	src.list.id	   			= 'LIST';
	src.list.size			= i + sizeof(BRRFile) - sizeof(Chunk) - 8;
	src.list.type  			= 'srcd';

	src.src.schk.id		 	= 'srci';
	src.src.schk.size	 	= sizeof(SourceInfo);
	src.src.info[0].offset	= 0;
	src.src.info[0].rate	= pSrc->rate;
	src.src.info[0].len		= pSrc->len;
	src.src.info[0].loop	= pSrc->loop;
	src.src.info[0].crc		= 0;
	src.src.info[0].prog	= pSrc->prog;
	src.src.info[0].adsr	= 0xE0FF;


	src.src.data.id			= 'data';
	src.src.data.size		= i;

	p = (u16*)&PCurFile->pData[pSrc->offset];

	src.src.info[0].crc			= CRC16(p, pSrc->len * 9);

	FileWrite(fh, &src, sizeof(BRRFile));
	FileWrite(fh, p, pSrc->len * 9);
	if ((pSrc->len * 9) & 3)
		FileWrite(fh, &i, 4 - ((pSrc->len * 9) & 3));

	FileClose(fh);

	return 1;
}

b8 __fastcall SaveWAV(AnsiString file, Source *pSrc)
{
	s32		fh;
	u32		i,blk;
	s32		offset,len,loop;
	s16		buf[16];
	s32		p1,p2;
	WavFile	wav;
	SmpChk	smp;


	fh = FileCreate(file);
	if (fh == -1) return 0;

	//Check for initial block of silence -------
	offset = pSrc->offset;
	len = pSrc->len;
	loop = pSrc->loop;

	if ((Apu.ram[pSrc->offset] & 0xC) == 0 &&
		(*(u32*)&Apu.ram[pSrc->offset+1] |
		 *(u32*)&Apu.ram[pSrc->offset+5]) == 0)
	{
		offset += 9;
		len--;
		loop--;
	}

	//Create the standard header ---------------
	wav.riff.id						= 'RIFF';
	wav.riff.size					= (len << 5) + sizeof(WavFile) - 8;
	wav.riff.type					= 'WAVE';

	wav.fmt.schk.id					= 'fmt ';
	wav.fmt.schk.size				= sizeof(WAVEFORMATEX);
	wav.fmt.data.wFormatTag			= WAVE_FORMAT_PCM;
	wav.fmt.data.nChannels			= 1;
	wav.fmt.data.nSamplesPerSec		= pSrc->rate;
	wav.fmt.data.nAvgBytesPerSec	= pSrc->rate * 2;
	wav.fmt.data.nBlockAlign		= 2;
	wav.fmt.data.wBitsPerSample		= 16;
	wav.fmt.data.cbSize				= 0;

	wav.data.id						= 'data';
	wav.data.size					= len << 5;

	//Create sampler chunk ---------------------
	if (pSrc->loop != 0xFFFF)
	{
		wav.riff.size			+= sizeof(SmpChk);

		smp.schk.id				= 'smpl';
		smp.schk.size			= sizeof(SmpChk) - 8;
		smp.dwManufacturer		= 0;
		smp.dwProduct			= 0;
		smp.dwSamplePeriod		= 1000000000 / pSrc->rate;
		smp.dwMIDIUnityNote		= 60;			//Mid-C
		smp.dwMIDIPitchFraction	= 0;
		smp.dwSMPTEFormat		= 0;
		smp.dwSMPTEOffset		= 0;
		smp.cSampleLoops		= 1;
		smp.cbSamplerData		= sizeof(SmpLoop);

		smp.loop.dwIdentifier	= 1;
		smp.loop.dwType		 	= 0;			//Forward looping
		smp.loop.dwStart	 	= loop << 4;
		smp.loop.dwEnd		 	= (len << 4) - 1;
		smp.loop.dwFraction	 	= 0;
		smp.loop.dwPlayCount 	= 0;			//Loop infinitely
	}

	//Write out WAV file -----------------------
	FileWrite(fh, &wav, sizeof(WavFile));

	blk = PCurFile->type < ID6_ROM ? offset : 0;
	p1 = p2 = 0;

	for (i=len; i; i--)							//Decompress one block at a time
	{
		Apu.UnpackSrc(buf, (void*)blk, 1, 0, &p1, &p2);
		blk += 9;
		FileWrite(fh, buf, 32);
	}

	if (loop != 0xFFFF) FileWrite(fh, &smp, sizeof(SmpChk));

	FileClose(fh);

	return 1;
}

void __fastcall TFrmMain::itmExportClick(TObject *sender)
{
	u32			i,l;
	s8			num[8];
	AnsiString	str;
	TListItem	*pItem;
	TItemStates	selected;


	//==========================================
	// Set filename in dialog

	str = PCurItem->Caption;
	l = (u32)ScanStrR((s8*)str.data(), '.') - (u32)str.data();
	if (l) str.SetLength(--l);
	else l = str.Length();

	if (lvwSrc->SelCount > 1)
		dlgExport->FileName = "(Filename will be ignored)";
	else
	{
		sprintf(num, "-%02i", PCurSrc->idx);
		str += num;

		dlgExport->FileName = str;
	}

	if (!dlgExport->Execute()) return;

	//==========================================
	// Save file(s)
	//
	// If only one source is selected, save it to the filename specified by the user.  Otherwise
	// use a naming convention based on the original filename and the source index.

	if (lvwSrc->SelCount == 1)
	{
		str = dlgExport->FileName;
		i = (u32)ScanStrR((s8*)str.data(), '.') - (u32)str.data();

		switch(dlgExport->FilterIndex)
		{
		case(1):
			if (!i) str += ".brr";
			SaveBRR(str, PCurSrc);
			break;

		case(2):
			if (!i) str += ".wav";
			SaveWAV(str, PCurSrc);
			break;
		}
	}
	else
	{
		selected << isSelected;
		switch(dlgExport->FilterIndex)
		{
		case(1):	//.brr
			pItem = lvwSrc->Selected;
			while (pItem)
			{
				if (((Source*)pItem->Data)->offset != -1)
				{
					if (PCurFile->type >= ID6_ROM)
						memcpy(Apu.ram, &PCurFile->pData[((Source*)pItem->Data)->offset], ((Source*)pItem->Data)->len * 9);

					str.SetLength(l);
					sprintf(num, "-%02i", ((Source*)pItem->Data)->idx);
					str += num;
					str += ".brr";
					SaveBRR(str, (Source*)pItem->Data);
				}
				pItem = lvwSrc->GetNextItem(pItem, sdAll, selected);
			}
			break;

		case(2):	//.wav
			pItem = lvwSrc->Selected;
			while (pItem)
			{
				if (((Source*)pItem->Data)->offset != -1)
				{
					if (PCurFile->type >= ID6_ROM)
						memcpy(Apu.ram, &PCurFile->pData[((Source*)pItem->Data)->offset], ((Source*)pItem->Data)->len * 9);

					str.SetLength(l);
					sprintf(num, "-%02i", ((Source*)pItem->Data)->idx);
					str += num;
					str += ".wav";
					SaveWAV(str, (Source*)pItem->Data);
				}
				pItem = lvwSrc->GetNextItem(pItem, sdAll, selected);
			}
			break;
		}
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::itmMuteClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	Source		*pSrc;
	u8			mute;


	selected << isSelected;

	pItem = FrmMain->lvwSrc->Selected;
	mute = (((Source*)pItem->Data)->flags & SF_MUTE) ^ SF_MUTE;
	do
	{
		pSrc = (Source*)pItem->Data;
		if (pSrc->offset != -1)
		{
			pSrc->flags &= ~SF_MUTE;
			pSrc->flags |= mute;
			lvwSrc->Selected->Update();
		}

		pItem = FrmMain->lvwSrc->GetNextItem(pItem, sdAll, selected);
	} while (pItem);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSilenceClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	Source		*pSrc;


	selected << isSelected;

	pItem = FrmMain->lvwSrc->Selected;
	do
	{
		pSrc = (Source*)pItem->Data;

		if (pSrc->offset != -1)
		{
			memset(&Apu.ram[pSrc->offset], 0, pSrc->len * 9);
			Apu.ram[pSrc->offset] = 0xC1;

			memset(&PCurAPU->spc.ram[pSrc->offset], 0, pSrc->len * 9);
			PCurAPU->spc.ram[pSrc->offset] = 0xC1;

			pSrc->len	= 1;
			pSrc->loop	= -1;
			pSrc->crc	= 0xAE6C;
			pSrc->flags	= 0x01;
			pSrc->rate	= 32000;
			pSrc->adsr	= 0xE0FF;
			pSrc->prog	= 0x80;
			pSrc->bank	= 0;

			pItem->SubItems->Strings[1] = "16";
			pItem->SubItems->Strings[2] = " ";
			pItem->SubItems->Strings[3] = "0";
		}

		pItem = FrmMain->lvwSrc->GetNextItem(pItem, sdAll, selected);
	} while (pItem);

	if (!(PCurFile->flags & F_SAVE)) PCurItem->Caption = PCurItem->Caption + " *";
	FileModified(F_STATE);

	if (!Scope) DrawWave(PCurSrc);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSetOptClick(TObject *sender)
{
	SourceInfo	*pInfo,*pNext;
	FileInfo	*pFile;
	Source		*pSrc;
	u32			i,j;


	if (!FrmSSI) Application->CreateForm(__classid(TFrmSSI), &FrmSSI);

	FrmSSI->txtRate->Tag = PCurSrc->rate;
	FrmSSI->grpType->ItemIndex = (PCurSrc->prog & 0x80) != 0;
	FrmSSI->prog = PCurSrc->prog & 0x7F;
	FrmSSI->txtOfs->Tag = PCurSrc->offset;
	FrmSSI->txtLoop->Tag = PCurSrc->loop;
	FrmSSI->txtOfs->Enabled = PCurFile->type < ID6_ROM;
	FrmSSI->txtLoop->Enabled = PCurFile->type < ID6_ROM;

	if (FrmSSI->ShowModal() != mrOk) return;

	PCurSrc->flags |= SF_INFO;
	PCurSrc->rate = FrmSSI->txtRate->Tag;
	PCurSrc->prog = FrmSSI->grpType->ItemIndex ? 0x80 : 0;
	PCurSrc->prog |= FrmSSI->prog;

	if (PCurSrc->offset != FrmSSI->txtOfs->Tag)
	{
		PCurSrc->offset = FrmSSI->txtOfs->Tag;

		i = (PCurAPU->spc.dsp.dir << 8) | (PCurSrc->idx << 2);
		*(u16*)&PCurAPU->spc.ram[i] = (u16)PCurSrc->offset;
		*(u16*)&Apu.ram[i] = (u16)PCurSrc->offset;

		i = (u16)PCurSrc->offset;
		while (!(PCurAPU->pData[i] & 1))
		{
			i = (i + 9) & 0xFFFF;
			if ((u16)(i - PCurSrc->offset) < 9)	//If source wraps around RAM and comes back to the
			{									// start, invalidate it.
				pSrc->offset = -1;
				break;
			}
		}

		if (pSrc->offset != -1)
		{
			pSrc->len = (i - PCurSrc->offset) / 9;

			pSrc->crc = CRC16(&PCurAPU->pData[PCurSrc->offset], pSrc->len * 9);

			pSrc->flags &= ~(SF_PLAYED|SF_FILTERS);
			for (i=0; i<pSrc->len; i++)
				pSrc->flags |= 1 << ((PCurFile->pData[pSrc->offset + (i * 9)] >> 2) & 3);
		}
	}

	if (PCurSrc->loop != FrmSSI->txtLoop->Tag)
	{
		PCurSrc->loop = FrmSSI->txtLoop->Tag;
		i = (PCurAPU->spc.dsp.dir << 8) | (PCurSrc->idx << 2);
		*(u16*)&PCurAPU->spc.ram[2+i] = (u16)PCurSrc->loop;
		*(u16*)&Apu.ram[2+i] = (u16)PCurSrc->loop;
	}


	PrintSrcInfo();

	//==========================================
	// Update global information list

	pNext = pInfo = PInfoStart;
	while (pNext)
	{
		if (pNext->crc >= PCurSrc->crc)
		{
			if (pNext->crc == PCurSrc->crc && *(u32*)&pNext->len == *(u32*)&PCurSrc->len)
			{
				pInfo = pNext;
				break;
			}
			else
			if (pNext->crc > PCurSrc->crc || *(u32*)&pNext->len > *(u32*)&PCurSrc->len)
			{
				pInfo = PInfoFree;				//Get first free info

				PInfoFree = PInfoFree->pNext;	//Remove from free list
				PInfoFree->pPrev = NULL;

				pInfo->pPrev = pNext->pPrev;	//Insert into used list
				pInfo->pNext = pNext;
				pNext->pPrev = pInfo;

				if (pInfo->pPrev)
					pInfo->pPrev->pNext = pInfo;
				else
					PInfoStart = pInfo;

				break;
			}
		}

		pInfo = pNext;
		pNext = pNext->pNext;
	}

	if (!pInfo)									//If list is empty
	{
		pInfo = PInfoStart = PInfoFree;
		PInfoFree = PInfoFree->pNext;

		pInfo->pNext = NULL;
	}
	else
	if (!pNext)									//If end of list was reached
	{
		if (!PInfoFree) return;					//If any free infos exist

		PInfoFree->pPrev = pInfo;
		pInfo->pNext = PInfoFree;

		pInfo = PInfoFree;

		PInfoFree = PInfoFree->pNext;
		PInfoFree->pPrev = NULL;

		pInfo->pNext = NULL;
	}

	pInfo->offset	= -1;
	pInfo->len		= PCurSrc->len;
	pInfo->loop		= PCurSrc->loop;
	pInfo->crc		= PCurSrc->crc;
	pInfo->_r1[0]	= 0;
	pInfo->_r1[1]	= 0;
	pInfo->_r1[2]	= 0;
	pInfo->rate		= PCurSrc->rate;
	pInfo->adsr		= PCurSrc->adsr;
	pInfo->prog		= PCurSrc->prog;
	pInfo->bank		= PCurSrc->bank;

	//Update same source in all other files ----
	for (i=0; i<lvwFile->Items->Count; i++)
	{
		pFile = (FileInfo*)lvwFile->Items->Item[i]->Data;

		for (j=0; j<pFile->num; j++)
		{
			pSrc = &pFile->src[j];

			if (pSrc->offset != -1 &&
				pSrc->crc == PCurSrc->crc &&
				*(u32*)&pSrc->len == *(u32*)&PCurSrc->len)
			{
				pSrc->flags	|= SF_INFO;
				pSrc->rate	= PCurSrc->rate;
				pSrc->adsr	= PCurSrc->adsr;
				pSrc->prog	= PCurSrc->prog;
				pSrc->bank	= PCurSrc->bank;
			}
		}
	}
}


//**************************************************************************************************
// Load Source Information File
//
// Any source options that are currently set will be reset

void __fastcall TFrmMain::itmLoadInfoClick(TObject *sender)
{
	FileInfo	*pFile;
	Source		*pSrc;
	SourceInfo	*pInfo;
	s32			fh,i,j,num;

	struct
	{
		Chunk	riff,list;
		SubChk	info;
	} hdr;


	if (!dlgOpenInfo->Execute()) return;

	fh = FileOpen(dlgOpenInfo->FileName, fmOpenRead);

	memset(&hdr, 0, sizeof(hdr));
	FileRead(fh, &hdr, sizeof(hdr));

	if (hdr.riff.id == 'RIFF' && hdr.riff.type == 'snes' &&
		hdr.list.id == 'LIST' && hdr.list.type == 'srcd' &&
		hdr.info.id == 'info')
	{
		num = hdr.info.size >> 5;
		if (num > 1024) num = 1024;

		for (i=0; i<num; i++)
		{
			FileRead(fh, &SrcInfo[i], sizeof(SourceInfo));
			SrcInfo[i].pPrev = &SrcInfo[i-1];
			SrcInfo[i].pNext = &SrcInfo[1+i];
		}

		SrcInfo[0].pPrev = NULL;
		SrcInfo[i-1].pNext = NULL;

		PInfoStart = num ? SrcInfo : NULL;
		PInfoFree = &SrcInfo[i];

		for (; i<1024; i++)
		{
			SrcInfo[i].pPrev = &SrcInfo[i-1];
			SrcInfo[i].pNext = &SrcInfo[1+i];
		}

		//Copy info to sources in loaded files
		for (i=0; i<lvwFile->Items->Count; i++)
		{
			pFile = (FileInfo*)lvwFile->Items->Item[i]->Data;

			for (j=0; j<pFile->num; j++)
			{
				pSrc = &pFile->src[j];

				if (pSrc->offset == -1) continue;

				pInfo = PInfoStart;
				while (pInfo)
				{
					pSrc->flags &= ~SF_INFO;

					if (pSrc->crc == pInfo->crc &&
						*(u32*)&pSrc->len == *(u32*)&pInfo->len)
					{
						pSrc->flags	|= SF_INFO;
						pSrc->rate	= pInfo->rate;
						pSrc->adsr	= pInfo->adsr;
						pSrc->prog	= pInfo->prog;
						pSrc->bank	= pInfo->bank;
					}

					pInfo = pInfo->pNext;
				}
			}
		}
	}

	FileClose(fh);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSaveInfoClick(TObject *sender)
{
	SourceInfo	src,*pInfo;
	s32			fh;

	struct
	{
		Chunk	riff,list;
		SubChk	info;
	} hdr;


	if (!dlgSaveInfo->Execute()) return;

	fh = FileCreate(dlgSaveInfo->FileName);

	//Create header ----------------------------
	hdr.riff.id		= 'RIFF';
	hdr.riff.size	= 24;
	hdr.riff.type	= 'snes';

	hdr.list.id		= 'LIST';
	hdr.list.size	= 12;
	hdr.list.type	= 'srcd';

	hdr.info.id		= 'info';
	hdr.info.size	= 0;

	FileWrite(fh, &hdr, sizeof(hdr));

	//Write out each source --------------------
	pInfo = PInfoStart;
	while (pInfo)
	{
		src = *pInfo;
		src.pPrev = NULL;
		src.pNext = NULL;
		FileWrite(fh, &src, sizeof(SourceInfo));
		hdr.info.size += sizeof(SourceInfo);
		pInfo = pInfo->pNext;
	}

	//Write out final header -------------------
	hdr.riff.size += hdr.info.size;
	hdr.list.size += hdr.info.size;

	FileSeek(fh, 0, 0);
	FileWrite(fh, &hdr, sizeof(hdr));
	FileClose(fh);
}


//**************************************************************************************************
b8 AddString(TStringList *sl, s8 *str)
{
	if (sl->IndexOf(str) != -1) return 0;

	sl->Append(str);

	return 1;									//Return true, if string didn't exist in the list
}

void __fastcall TFrmMain::itmEditClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	u32			u;
	ID666		*p1,*p2;
	s8			str[12];


	if (lvwFile->SelCount > 1)					//If more than one file is selected
	{
		u = -1;									//Flag all fields as updatable
		p1 = p2 = NULL;
		selected << isSelected;

		FrmTag->strSong->Clear();
		FrmTag->strGame->Clear();
		FrmTag->strArtist->Clear();
		FrmTag->strPub->Clear();
		FrmTag->strOST->Clear();
		FrmTag->strDumper->Clear();

		pItem = lvwFile->Selected;
		do
		{
			if (((APUFile*)pItem->Data)->type <= ID6_EXT)
			{
				if (!p1)						//If this is the first SPC save a pointer to its tag
				{
					p1 = &((APUFile*)pItem->Data)->id6;

					FrmTag->strSong->Append(p1->song);
					FrmTag->strGame->Append(p1->game);
					FrmTag->strArtist->Append(p1->artist);
					FrmTag->strPub->Append(p1->pub);
					FrmTag->strOST->Append(p1->ost);
					FrmTag->strDumper->Append(p1->dumper);
				}
				else
				{								//Compare with the first SPC for similarities
					p2 = &((APUFile*)pItem->Data)->id6;

					if (AddString(FrmTag->strSong, p2->song))		_Clear(u, EN_SONG);
					if (AddString(FrmTag->strGame, p2->game))		_Clear(u, EN_GAME);
					if (AddString(FrmTag->strArtist, p2->artist))	_Clear(u, EN_ARTIST);
					if (AddString(FrmTag->strDumper, p2->dumper))	_Clear(u, EN_DUMPER);
					if (strcmp(p1->comment, p2->comment))			_Clear(u, EN_CMNT);
					if (AddString(FrmTag->strOST, p2->ost))			_Clear(u, EN_OST);
					if (AddString(FrmTag->strPub, p2->pub))			_Clear(u, EN_PUB);
					if (p1->date.date != p2->date.date)				_Clear(u, EN_DATE);
					else
					if (strcmp(p1->datetxt, p2->datetxt))		_Clear(u, EN_DATE);
					if (p1->copy != p2->copy)	_Clear(u, EN_COPY);
					if (p1->disc != p2->disc)	_Clear(u, EN_DISC);
					if (p1->track != p2->track)	_Clear(u, EN_TRACK);
					if (p1->mute != p2->mute)	_Clear(u, EN_MUTE);
					if (p1->amp != p2->amp)		_Clear(u, EN_AMP);
					if (p1->emu != p2->emu)		_Clear(u, EN_EMU);
					if (p1->intro != p2->intro)	_Clear(u, EN_INTRO);
					if (p1->loop != p2->loop ||
						p1->loopx != p2->loopx)	_Clear(u, EN_LOOP);
					if (p1->end != p2->end)		_Clear(u, EN_END);
					if (p1->fade != p2->fade)	_Clear(u, EN_FADE);
				}
			}

			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);

		if (!p1) return;						//If no SPC's were found, return
		if (!p2) u = -1;						//If only one SPC, update all fields

		FrmTag->ShowBtn(false);					//Multiple files are selected, playing is disabled
		FrmTag->id6 = *p1;
		FrmTag->id6.file[0] = 0;
		FrmTag->pID6 = NULL;
		FrmTag->update = u;
		if (FrmTag->ShowModal() != mrOk) return;

		u = FrmTag->update;						//Get the fields the user wants updated
		if (!u) return;

		p1 = &FrmTag->id6;
		pItem = lvwFile->Selected;
		do
		{
			if (((APUFile*)pItem->Data)->type <= ID6_EXT)
			{
				p2 = &((APUFile*)pItem->Data)->id6;
				if (_Test(u, EN_SONG))		strcpy(p2->song, p1->song);
				if (_Test(u, EN_GAME))		strcpy(p2->game, p1->game);
				if (_Test(u, EN_ARTIST))	strcpy(p2->artist, p1->artist);
				if (_Test(u, EN_DUMPER))	strcpy(p2->dumper, p1->dumper);
				if (_Test(u, EN_CMNT))		strcpy(p2->comment, p1->comment);
				if (_Test(u, EN_OST))		strcpy(p2->ost, p1->ost);
				if (_Test(u, EN_PUB))		strcpy(p2->pub, p1->pub);
				if (_Test(u, EN_DATE))		p2->date	= p1->date;
				if (_Test(u, EN_COPY))		p2->copy	= p1->copy;
				if (_Test(u, EN_DISC))		p2->disc	= p1->disc;
				if (_Test(u, EN_TRACK))		p2->track	= p1->track;
				if (_Test(u, EN_MUTE))		p2->mute	= p1->mute;
				if (_Test(u, EN_AMP))		p2->amp		= p1->amp;
				if (_Test(u, EN_EMU))		p2->emu		= p1->emu;
				if (_Test(u, EN_INTRO))		p2->intro	= p1->intro;
				if (_Test(u, EN_LOOP))		p2->loop	= p1->loop;
				if (_Test(u, EN_LOOP))		p2->loopx	= p1->loopx;
				if (_Test(u, EN_END))		p2->end		= p1->end;
				if (_Test(u, EN_FADE))		p2->fade	= p1->fade;

				if (!(((APUFile*)pItem->Data)->flags & F_SAVE))
					pItem->Caption = pItem->Caption + " *";
				((APUFile*)pItem->Data)->flags |= F_TAG;

				UpdateFile(pItem);
			}

			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);

		EnableItem();
	}
	else
	{
		//Populate combo boxes -----------------
		FrmTag->strSong->Clear();
		FrmTag->strGame->Clear();
		FrmTag->strArtist->Clear();
		FrmTag->strPub->Clear();
		FrmTag->strOST->Clear();
		FrmTag->strDumper->Clear();

		//Display tag editor -------------------
		FrmTag->ShowBtn(true);
		FrmTag->id6 = PCurFile->id6;
		FrmTag->pID6 = &PCurFile->id6;
		FrmTag->update = -1;
		if (FrmTag->ShowModal() != mrOk) return;

		u = FrmTag->update;
		p1 = &FrmTag->id6;
		p2 = &PCurFile->id6;

		if (!u) return;

		if (_Test(u, EN_SONG))		strcpy(p2->song, p1->song);
		if (_Test(u, EN_GAME))		strcpy(p2->game, p1->game);
		if (_Test(u, EN_ARTIST))	strcpy(p2->artist, p1->artist);
		if (_Test(u, EN_DUMPER))	strcpy(p2->dumper, p1->dumper);
		if (_Test(u, EN_CMNT))		strcpy(p2->comment, p1->comment);
		if (_Test(u, EN_OST))		strcpy(p2->ost, p1->ost);
		if (_Test(u, EN_PUB))		strcpy(p2->pub, p1->pub);
		if (_Test(u, EN_DATE))		p2->date	= p1->date;
		if (_Test(u, EN_COPY))		p2->copy	= p1->copy;
		if (_Test(u, EN_DISC))		p2->disc	= p1->disc;
		if (_Test(u, EN_TRACK))		p2->track	= p1->track;
		if (_Test(u, EN_MUTE))		p2->mute	= p1->mute;
		if (_Test(u, EN_AMP))		p2->amp		= p1->amp;
		if (_Test(u, EN_EMU))		p2->emu		= p1->emu;
		if (_Test(u, EN_INTRO))		p2->intro	= p1->intro;
		if (_Test(u, EN_LOOP))		p2->loop	= p1->loop;
		if (_Test(u, EN_LOOP))		p2->loopx	= p1->loopx;
		if (_Test(u, EN_END))		p2->end		= p1->end;
		if (_Test(u, EN_FADE))		p2->fade	= p1->fade;

		PrintID6(p2);

		str[0] = '/';
		PrintTime(&str[1], p2->GetTotal());
		pbTime->Invalidate();
		lblTotal->Caption = str;
		lblTime->Left = lblTotal->Left - lblTime->Width;

		if (!(PCurFile->flags & F_SAVE)) PCurItem->Caption = PCurItem->Caption + " *";
		FileModified(F_TAG);

		UpdateFile(PCurItem);

		SetAPULength();
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::FillROMQueue()
{
	TListItem	*pItem;
	TItemStates	selected;
	u32			count;


	if (!FrmROM) Application->CreateForm(__classid(TFrmROM), &FrmROM);

	FrmROM->Initialize();

	selected << isSelected;

	//Check for any modified files -------------
	if (lvwFile->SelCount > 1)
	{
		count = 0;
		pItem = lvwFile->Selected;
		while (pItem)
		{
			if (((APUFile*)pItem->Data)->type <= ID6_ZST)
			{
				FrmROM->AddFile((APUFile*)pItem->Data);
				if (++count == 63)
				{
					MessageDlg("A ROM can only contain 63 SPC's",
								mtInformation, TMsgDlgButtons() << mbOK, 0);
					break;
				}
			}
			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		}
	}
	else
		FrmROM->AddFile(PCurAPU);
}

void __fastcall TFrmMain::itmROMClick(TObject *sender)
{
	FillROMQueue();
	if (FrmROM->ShowModal() == mrCancel) return;
}

void __fastcall TFrmMain::pbUploadClick(TObject *sender)
{
	FillROMQueue();
	FrmROM->chkPad->Checked = false;
	FrmROM->btnUploadClick(NULL);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmRenameClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	APUFile		*pFile;


	//Create rename dialog ---------------------
	if (!FrmRen)
	{
		Application->CreateForm(__classid(TFrmRen), &FrmRen);
		FrmRen->txtFormat->Text = Cfg.renFmt;
	}

	//Add files to dialog ----------------------
	if (lvwFile->SelCount > 1)					//If more than one file is selected
	{
		selected << isSelected;
		pItem = lvwFile->Selected;
		do
		{
			pFile = (APUFile*)pItem->Data;
			if (pFile->type <= ID6_EXT && !(pFile->flags & F_READONLY))
				FrmRen->AddFile(&pFile->id6, 0);

			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);
	}
	else
	if (PCurFile && PCurFile->type <= ID6_EXT && !(PCurFile->flags & F_READONLY))
		FrmRen->AddFile(&PCurFile->id6, 0);

	//Display dialog ---------------------------
	FrmRen->ShowModal();

	if (!CmpCopy(Cfg.renFmt, FrmRen->txtFormat->Text.c_str()))
		WritePrivateProfileString("Rename", "format", Cfg.renFmt, Cfg.ini);

	//Update local file list with new names ----
	if (lvwFile->SelCount > 1)
	{
		pItem = lvwFile->Selected;
		do
		{
			pFile = (APUFile*)pItem->Data;
			if (pFile->type <= ID6_EXT)
			{
				if (pFile->flags & F_SAVE)
					pItem->Caption = ScanStrR(pFile->id6.file, '\\') + AnsiString(" *");
				else
					pItem->Caption = ScanStrR(pFile->id6.file, '\\');
			}

			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);
	}
	else
	if (PCurFile)								//Rename can be run without any files open
	{
		if (PCurFile->flags & F_SAVE)
			PCurItem->Caption = ScanStrR(PCurFile->id6.file, '\\') + AnsiString(" *");
		else
			PCurItem->Caption = ScanStrR(PCurFile->id6.file, '\\');
	}

}


//**************************************************************************************************
void __fastcall TFrmMain::itmConfigClick(TObject *sender)
{
	FrmConfig = new TFrmConfig(FrmMain);
	FrmConfig->ShowModal();
	delete FrmConfig;
	FrmConfig = NULL;

	pbUpload->Visible = (Cfg.out.snes != 0);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmHelpClick(TObject *sender)
{
	WinHelp(FrmMain->Handle, Application->HelpFile.c_str(), HELP_FINDER, 0);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmAboutClick(TObject *sender)
{
	FrmAbout = new TFrmAbout(FrmMain);
	FrmAbout->ShowModal();
	delete FrmAbout;
	FrmAbout = NULL;
}


//**************************************************************************************************
void __fastcall TFrmMain::pbBtnPaint(TObject *sender)
{
	RECT		r;
	TPaintBox	*pb;
	TCanvas		*pCanvas;
	u32			i;


	pb = (TPaintBox*)sender;
	pCanvas = pb->Canvas;
	i = (pb->Tag >> 8) * CBTN_SIZE;

	if (pb->Enabled)
	{
		if (!(pb->Tag & BTN_STATE))				//Button is up
		{
			pCanvas->Pen->Color = CBtn.sh;
			pCanvas->PenPos = TPoint(0, CBTN_SIZE+2);
			pCanvas->LineTo(0,0);
			pCanvas->LineTo(CBTN_SIZE + 3,0);

			pCanvas->Pen->Color = CBtn.sd;
			pCanvas->LineTo(CBTN_SIZE + 3, CBTN_SIZE + 3);
			pCanvas->LineTo(-1, CBTN_SIZE + 3);

			pCanvas->Pen->Color = CBtn.sl;
			pCanvas->PenPos = TPoint(1, CBTN_SIZE + 1);
			pCanvas->LineTo(1, 1);
			pCanvas->LineTo(CBTN_SIZE + 2, 1);

			pCanvas->Pen->Color = CBtn.ss;
			pCanvas->LineTo(CBTN_SIZE + 2, CBTN_SIZE + 2);
			pCanvas->LineTo(0, CBTN_SIZE + 2);

			pCanvas->CopyRect(TRect(2, 2, CBTN_SIZE + 2, CBTN_SIZE + 2), BmpCBtn->Canvas,
							  TRect(0, i, CBTN_SIZE, i+CBTN_SIZE));
		}
		else
		{
			if (pb->Tag & BTN_TOGGLE)
			{
				pCanvas->Pen->Color = CBtn.sd;
				pCanvas->PenPos = TPoint(0, CBTN_SIZE + 2);
				pCanvas->LineTo(0, 0);
				pCanvas->LineTo(CBTN_SIZE + 3, 0);

				pCanvas->Pen->Color = CBtn.sh;
				pCanvas->LineTo(CBTN_SIZE + 3, CBTN_SIZE + 3);
				pCanvas->LineTo(-1, CBTN_SIZE + 3);

				pCanvas->Pen->Color = CBtn.ss;
				pCanvas->PenPos = TPoint(1, CBTN_SIZE + 1);
				pCanvas->LineTo(1, 1);
				pCanvas->LineTo(CBTN_SIZE + 2, 1);

				pCanvas->Pen->Color = CBtn.sl;
				pCanvas->LineTo(CBTN_SIZE + 2, CBTN_SIZE + 2);
				pCanvas->LineTo(0, CBTN_SIZE + 2);

				pCanvas->Pen->Color = CBtn.sf;	//Fill in part that won't get copied from bitmap
				pCanvas->PenPos = TPoint(2, CBTN_SIZE + 1);
				pCanvas->LineTo(2, 2);
				pCanvas->LineTo(CBTN_SIZE + 2, 2);

				pCanvas->CopyRect(TRect(3, 3, CBTN_SIZE + 2, CBTN_SIZE + 2), BmpCBtn->Canvas,
								  TRect(0, i, CBTN_SIZE-1, i+CBTN_SIZE-1));
			}
			else
			{
				pCanvas->Brush->Color = CBtn.sd;
				pCanvas->FrameRect(TRect(0, 0, CBTN_SIZE + 4, CBTN_SIZE + 4));

				pCanvas->Brush->Color = CBtn.ss;
				pCanvas->FrameRect(TRect(1, 1, CBTN_SIZE + 3, CBTN_SIZE + 3));

				pCanvas->Pen->Color = CBtn.sf;
				pCanvas->PenPos = TPoint(2, CBTN_SIZE + 1);
				pCanvas->LineTo(2, 2);
				pCanvas->LineTo(CBTN_SIZE + 2, 2);

				pCanvas->CopyRect(TRect(3, 3, CBTN_SIZE + 2, CBTN_SIZE + 2), BmpCBtn->Canvas,
								  TRect(0, i, CBTN_SIZE-1, i+CBTN_SIZE-1));
			}
		}
	}
	else
	{
		if (!(pb->Tag & BTN_STATE))				//Button is up
		{
			r.left = 0;
			r.top = 0;
			r.right = CBTN_SIZE + 4;
			r.bottom = CBTN_SIZE + 4;

			DrawFrameControl(pCanvas->Handle, &r, DFC_BUTTON, DFCS_BUTTONPUSH);
			pCanvas->CopyRect(TRect(2, 2, CBTN_SIZE + 2, CBTN_SIZE + 2), BmpCBtn->Canvas,
							  TRect(CBTN_SIZE, i, CBTN_SIZE*2, i+CBTN_SIZE));
		}
		else
		{
			pCanvas->Pen->Color = CBtn.ws;
			pCanvas->PenPos = TPoint(0, CBTN_SIZE + 2);
			pCanvas->LineTo(0, 0);
			pCanvas->LineTo(CBTN_SIZE + 3, 0);

			pCanvas->Pen->Color = CBtn.wh;
			pCanvas->LineTo(CBTN_SIZE + 3, CBTN_SIZE + 3);
			pCanvas->LineTo(-1, CBTN_SIZE + 3);

			pCanvas->Pen->Color = CBtn.wd;
			pCanvas->PenPos = TPoint(1, CBTN_SIZE + 1);
			pCanvas->LineTo(1, 1);
			pCanvas->LineTo(CBTN_SIZE + 2, 1);

			pCanvas->Pen->Color = CBtn.wl;
			pCanvas->LineTo(CBTN_SIZE + 2, CBTN_SIZE + 2);
			pCanvas->LineTo(0, CBTN_SIZE + 2);

			pCanvas->Pen->Color = CBtn.wf;
			pCanvas->PenPos = TPoint(2, CBTN_SIZE + 1);
			pCanvas->LineTo(2, 2);
			pCanvas->LineTo(CBTN_SIZE + 2, 2);

			pCanvas->CopyRect(TRect(3, 3, CBTN_SIZE + 2, CBTN_SIZE + 2), BmpCBtn->Canvas,
							  TRect(CBTN_SIZE, i, CBTN_SIZE*2-1, i+CBTN_SIZE-1));
		}
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::BtnMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (btn == mbLeft && shift.Contains(ssLeft))
	{
		if (((TPaintBox*)sender)->Tag & BTN_TOGGLE)
			((TPaintBox*)sender)->Tag ^= BTN_STATE;
		else
			((TPaintBox*)sender)->Tag |= BTN_STATE;

		((TPaintBox*)sender)->Repaint();
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::BtnMouseUp(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (btn == mbLeft)
	{
		if (((TPaintBox*)sender)->Tag & BTN_TOGGLE) return;

		((TPaintBox*)sender)->Tag &= ~BTN_STATE;
		((TPaintBox*)sender)->Repaint();
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::BtnEnabled(TPaintBox *pb, b8 state)
{
	if (state == pb->Enabled) return;
	pb->Enabled = state;
	pb->Repaint();
}


//**************************************************************************************************
void __fastcall TFrmMain::pnlTagResize(TObject *Sender)
{
	s32	c1,c2;


	//Column 2 ---------------------------------
	c2 = MulDiv(TagCol2, pnlTag->Width, TagWidth);
	lblCmntT->Left = c2;
	mmoCmnts->Left = c2;
	mmoCmnts->Width = pnlTag->Width - c2 - 4;

	//Column 1 ---------------------------------
	c1 = MulDiv(TagCol1, pnlTag->Width, TagWidth);
	lblGameT->Left = c1;
	lblGame->Left = lblGame->Tag + c1;
	lblGame->Width = c2 - lblGame->Left - 4;

	lblPubT->Left = c1;
	lblCopy->Left = lblCopy->Tag + c1;
	lblPub->Left = lblPub->Tag + c1;
	lblPub->Width = c2 - lblPub->Left - 4;

	lblDumperT->Left = lblDumperT->Tag + c1;
	lblDumper->Left = lblDumper->Tag + c1;
	lblDateT->Left = lblDateT->Tag + c1;
	lblDate->Left = lblDate->Tag + c1;
	lblEmuT->Left = lblEmuT->Tag + c1;
	lblEmu->Left = lblEmu->Tag + c1;

	//Column 0 ---------------------------------
	lblOST->Width = c1 - lblOST->Left - 4;
	lblSong->Width = c1 - lblSong->Left - 4;
	lblArtist->Width = c1 - lblArtist->Left - 4;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// File List

//**************************************************************************************************
void __fastcall TFrmMain::PrintID6(ID666 *pID6)
{
	s8	str[32];
	s32	i;
	b8	show;


	if (pID6)
	{
		//OST information ----------------------
		lblOST->Caption = pID6->ost;
		lblSong->Caption = pID6->song;
		lblArtist->Caption = pID6->artist;

		if (pID6->disc)
		{
			lblDiscT->Visible = true;

			lblDisc->Visible = true;
			lblDisc->Caption = pID6->disc;
			i = lblDisc->Left + lblDisc->Width;
		}
		else
		{
			lblDiscT->Visible = false;

			lblDisc->Visible = false;
			lblDisc->Caption = "";
			i = lblDisc->Left - lblTrackT->Width;
		}

		if (pID6->track)
		{
			lblTrackT->Visible = true;
			lblTrackT->Left = i;
			i += lblTrackT->Width;

			lblTrack->Visible = true;
			lblTrack->Left = i;
			lblTrack->Caption = pID6->track >> 8;
			if (pID6->track & 0xFF)
				lblTrack->Caption += AnsiString((s8)pID6->track);
		}
		else
		{
			lblTrackT->Visible = false;

			lblTrack->Visible = false;
			lblTrack->Caption = "";
		}

		//Game information ---------------------
		lblGame->Caption = pID6->game;

		if (pID6->copy)
		{
			lblCopy->Visible = true;
			lblPub->Caption = AnsiString(pID6->copy) + " " + pID6->pub;
			lblPub->Left = lblCopy->Left + lblCopy->Width;
		}
		else
		{
			lblCopy->Visible = false;
			lblPub->Caption = pID6->pub;
			lblPub->Left = lblPubT->Left + lblPubT->Width;
		}
		lblPub->Tag = lblPub->Left - lblPubT->Left;

		//Dumped -------------------------------
		if (pID6->dumper[0])
		{
			lblDumperT->Caption = "Dumped by  ";

			lblDumper->Visible = true;
			lblDumper->Caption = pID6->dumper;
			i = lblDumper->Left + lblDumper->Width;
			show = 1;
		}
		else
		{
			lblDumperT->Caption = "Dumped";

			lblDumper->Visible = false;
			i = lblDumper->Left;
			show = 0;
		}

//TODO
//		if (pID6->date.ToStr(str))
//			lblDate->Caption = str;
//		else
			lblDate->Caption = pID6->datetxt;

		if (lblDate->Caption.Length())
		{
			lblDateT->Visible = true;
			lblDateT->Left = i;
			lblDateT->Tag = i - lblPubT->Left;
			i += lblDateT->Width;

			lblDate->Visible = true;
			lblDate->Left = i;
			lblDate->Tag = i - lblPubT->Left;
			i += lblDate->Width;
			show = 1;
		}
		else
		{
			lblDateT->Visible = false;
			lblDate->Visible = false;
		}

		lblEmu->Caption = EmuList[pID6->emu];

		if (pID6->emu)
		{
			lblEmuT->Visible = true;
			lblEmuT->Left = i;
			lblEmuT->Tag = i - lblPubT->Left;
			i += lblEmuT->Width;

			lblEmu->Visible = true;
			lblEmu->Left = i;
			lblEmu->Tag = i - lblPubT->Left;
			i += lblEmu->Width;
			show = 1;
		}
		else
		{
			lblEmuT->Visible = false;
			lblEmu->Visible = false;
		}

		lblDumperT->Visible = show;

		if (i > lblCmntT->Left)
		{
			i = lblCmntT->Left - i;

			if (pID6->dumper[0])
			{
				lblDumper->Width -= i;
			}

			if (lblDateT->Visible)
			{
				lblDateT->Left -= i;
				lblDate->Left -= i;
			}

			if (pID6->emu)
			{
				lblEmuT->Left -= i;
				lblEmu->Left -= i;
			}
		}

		//Comments -----------------------------
		mmoCmnts->Text = pID6->comment;
	}
	else
	{
		lblSong->Caption = "";
		lblGame->Caption = "";
		lblArtist->Caption = "";
		lblPub->Caption = "";

		lblOST->Caption = "";
		lblDisc->Caption = "";
		lblTrack->Caption = "";
		lblDumper->Caption = "";
		lblDate->Caption = "";
		lblEmu->Caption = "";
		mmoCmnts->Text = "";
	}
}


void __fastcall TFrmMain::lvwFileSelectItem(TObject *sender, TListItem *item, bool selected)
{
	Source		*pSrc;
	TListItem	*pItem;
	u32			i,j,k;
	s8			str[12],f;
	TItemStates	stSelected;
	b8			canEdit,canPlay,canSave;


	//Do something based on selection count ----
	if (lvwFile->SelCount < 1) return;

	if (lvwFile->SelCount > 1)					//Disable everything
	{
		if (State == ST_RUN || State == ST_HALT) SetState(ST_STOP);

		PrintID6(NULL);
		if (PCurFile) SaveSrcPos();
		lvwSrc->Items->Clear();

		lblTime->Caption = "";
		lblTotal->Caption = "";

		txtIn0->Text = "";
		txtIn1->Text = "";
		txtIn2->Text = "";
		txtIn3->Text = "";

		txtOut0->Text = "";
		txtOut1->Text = "";
		txtOut2->Text = "";
		txtOut3->Text = "";

		txtIn0->Enabled = false;
		txtIn1->Enabled = false;
		txtIn2->Enabled = false;
		txtIn3->Enabled = false;

		txtOut0->Enabled = false;
		txtOut1->Enabled = false;
		txtOut2->Enabled = false;
		txtOut3->Enabled = false;

		PCurItem = NULL;
		PCurFile = NULL;
		PCurSrc = NULL;

		lvwFile->Tag = NULL;

		DrawWave(NULL);

		pbTime->Invalidate();
	}
	else
	{
		PCurItem = lvwFile->Selected;

		if (PCurFile == (FileInfo*)PCurItem->Data) return;

		if (State == ST_HALT) SetState(ST_STOP);
		if (State == ST_RUN)
		{
			WavFlush();
			visFrame = -1;
		}

		if (PCurFile && PCurFile->type < ID6_ROM)
			Apu.SaveAPU(&PCurAPU->state.spcState, &PCurAPU->state.dspState);

		if (PCurFile) SaveSrcPos();

		PCurFile = (FileInfo*)PCurItem->Data;
		lvwFile->Tag = (s32)PCurFile;

		if (PCurFile->type < ID6_ROM)
		{
			FrmMain->Caption = "SPCTool - [" + AnsiString(PCurFile->id6.file) + "]";

			Apu.RestoreAPU(&PCurAPU->state.spcState, &PCurAPU->state.dspState);

			SetTrkAmp(Apu.GetDSPAmp());

			for (i=0; i<8 ;i++)
				LblVoice[i]->Font->Color = (PCurAPU->state.voice[i].mFlg & 1) ? clGray : clSilver;

			if (State != ST_RUN)
			{
				txtIn0->Enabled = true;
				txtIn1->Enabled = true;
				txtIn2->Enabled = true;
				txtIn3->Enabled = true;

				txtOut0->Enabled = true;
				txtOut1->Enabled = true;
				txtOut2->Enabled = true;
				txtOut3->Enabled = true;
			}

			if (FrmDSP && FrmDSP->Visible) FrmDSP->Reset();
		}
		else
			SetState(ST_STOP);


		//======================================
		// Fill out sound source list

		lvwSrc->Items->BeginUpdate();
		lvwSrc->Items->Clear();

		pSrc = PCurFile->src;
		for (k=0; k<(u32)PCurFile->num; k++)
		{
			if (PCurFile->type < ID6_ROM) pSrc = &PCurAPU->src[PCurAPU->ord[k]];

			pSrc->pItem = pItem = lvwSrc->Items->Add();
			pItem->Caption = pSrc->idx;
			pItem->Data = pSrc;
			pItem->Selected = PCurAPU->sel.Contains(PCurAPU->ord[k]);

			if (pSrc->offset != -1)
			{
				pItem->SubItems->Add(AnsiString((s32)pSrc->offset));
				pItem->SubItems->Add(AnsiString(pSrc->len << 4));
				if (pSrc->loop != 0xFFFF)
					pItem->SubItems->Add(AnsiString(pSrc->loop << 4));
				else
					pItem->SubItems->Add(" ");

				f = pSrc->flags;
				j = 0;
				for (i=0;i<4;i++)
				{
					if (f & 1)
						str[j++] = i | 0x30;
					else
					{
						str[j++] = ' ';
						str[j++] = ' ';
					}
					f >>= 1;
				}
				str[j] = 0;
				pItem->SubItems->Add(str);
			}
/*
			else
			{
				pItem->SubItems->Add("     ");
				pItem->SubItems->Add("     ");
				pItem->SubItems->Add("     ");
				pItem->SubItems->Add("    ");
			}
*/
			pSrc++;
		}

		lvwSrc->Items->EndUpdate();

		if (PCurFile->pTop)
		{
			if (PCurFile->type < ID6_ROM)
				pSrc = &PCurFile->src[((APUFile*)PCurFile)->ord[255]];
			else
				pSrc = &PCurFile->src[((ROMFile*)PCurFile)->ord[PCurFile->num]];

//			lvwSrc->FindData(0, (void*)pSrc, 1, 1)->MakeVisible(0);
//			lvwSrc->FindData(0, (void*)PCurFile->pTop, 1, 1)->MakeVisible(0);
			SendMessage(lvwSrc->Handle, LVM_ENSUREVISIBLE, (WPARAM)255, 0);
			SendMessage(lvwSrc->Handle, LVM_ENSUREVISIBLE, (WPARAM)PCurFile->pTop, 0);
		}

		//Print ID666 tag information ----------
		PrintID6(&PCurFile->id6);

		//Print song time ----------------------
		pbTime->Invalidate();

		if (PCurFile->type < ID6_ROM)
		{
			i = PCurAPU->id6.GetTotal();

			SetAPULength();
			PrintTimer(PCurAPU->state.spcState.t64Cnt);

			str[0] = '/';
			PrintTime(&str[1], i);
			lblTotal->Caption = str;
			lblTime->Left = lblTotal->Left - lblTime->Width;

			txtIn0->Text = IntToHex((s32)Apu.ram[0xF4], 2);
			txtIn1->Text = IntToHex((s32)Apu.ram[0xF5], 2);
			txtIn2->Text = IntToHex((s32)Apu.ram[0xF6], 2);
			txtIn3->Text = IntToHex((s32)Apu.ram[0xF7], 2);

			if (State == ST_RUN) WavRestart(Apu.GetSPCTime());
			else
			{
				txtOut0->Text = IntToHex((s32)Apu.GetAPUPort(0), 2);
				txtOut1->Text = IntToHex((s32)Apu.GetAPUPort(1), 2);
				txtOut2->Text = IntToHex((s32)Apu.GetAPUPort(2), 2);
				txtOut3->Text = IntToHex((s32)Apu.GetAPUPort(3), 2);
			}

		}
	}

	EnableItem();
}


//**************************************************************************************************
// Don't allow editing if file is read-only

void __fastcall TFrmMain::lvwFileEditing(TObject *sender, TListItem *pItem, bool &allowEdit)
{
	if (PCurFile->flags & F_READONLY) allowEdit = 0;
}


//**************************************************************************************************
// Rename file

void __fastcall TFrmMain::lvwFileEdited(TObject *sender, TListItem *item, AnsiString &str)
{
	s8	old[256];
	s8	*path,*file;
	s32	len;


	path = PCurFile->id6.file;		  	 		//path -> path and filename in ID666 tag
	file = ScanStrR(path, '\\');				//file -> filename in ID666 tag
	len = file - path;							//len = length of pathname

	if (!str.Length())
	{
		str = file;
		return;
	}

	if (len + str.Length() < 256)				//If complete path is < 256 characters
	{
		CopyStr(old, path);						//Copy path and old filename
		CopyStr(file, str.c_str());				//Copy new filename
		if (!RenameFile(old, path))				//If rename failed
		{
			CopyStr(file, &old[len]);			//Restore filename
			str = file;							//Undo user's change
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound Source Directory

//**************************************************************************************************
// Custom draw to make text white on sources that have played

void __fastcall AddSrc(TListItem *pItem)
{
	Source	*pSrc;
	u32		i,j;
	u8		f;
	s8		str[5];


	pSrc = (Source*)pItem->Data;
	ScanSrc(pSrc);

	if (pSrc->loop != 0xFFFF)
	{
		pSrc->rate = (s32)(261.6255653 * ((pSrc->len - pSrc->loop) << 4));
		pSrc->rate /= (pSrc->rate / 32000) + 1;
	}

	pItem->SubItems->Strings[0] = (s32)pSrc->offset;
	pItem->SubItems->Strings[1] = pSrc->len << 4;
	if (pSrc->loop != 0xFFFF)
		pItem->SubItems->Strings[2] = pSrc->loop << 4;
	else
		pItem->SubItems->Strings[2] = " ";

	f = pSrc->flags;
	j = 0;
	for (i=0;i<4;i++)
	{
		if (f & 1)
			str[j++] = i | 0x30;
		else
		{
			str[j++] = ' ';
			str[j++] = ' ';
		}
		f >>= 1;
	}
	str[j] = 0;
	pItem->SubItems->Strings[3] = str;
}


void __fastcall SrcChange(TListItem *pItem)
{
	s8		str[8],*s;
	Source	*pSrc;


	pSrc = (Source*)pItem->Data;

	s = str;
	*(u32*)&s[0] = 0x20202020;
	*(u32*)&s[4] = 0x20202020;

	if (pSrc->flags & 1) *s++ = '0';
	else s += 2;
	if (pSrc->flags & 2) *s++ = '1';
	else s += 2;
	if (pSrc->flags & 4) *s++ = '2';
	else s += 2;
	if (pSrc->flags & 8) *s++ = '3';
	*s = 0;

	pItem->SubItems->Strings[0] = (s32)pSrc->offset;
	pItem->SubItems->Strings[1] = (s32)pSrc->len << 4;
	if (pSrc->loop < pSrc->len)
		pItem->SubItems->Strings[2] = (s32)pSrc->loop << 4;
	else
	if (pSrc->loop == 0xFFFF)
		pItem->SubItems->Strings[2] = " ";
	else
		pItem->SubItems->Strings[2] = "...";
	pItem->SubItems->Strings[3] = str;

	if (!Scope && pItem->Selected) DrawWave(pSrc);
}


void __fastcall TFrmMain::lvwSrcCustomDrawItem(TCustomListView *pList, TListItem *pItem, TCustomDrawState state, bool &draw)
{
	TRect	r;
	TCanvas	*pCanvas;
	Source	*pSrc;


	//Source must be marked as played and not have any special states (focus rectangle, etc.)
	pSrc = (Source*)pItem->Data;
	if (!(pSrc->flags & (SF_PLAYED | SF_MUTE))) return;

	if (pSrc->offset == -1) AddSrc(pItem);
	if (pSrc->flags & SF_CHANGED)
	{
                try
                {
		SrcChange(pItem);
     		pSrc->flags &= ~SF_CHANGED;
                }
                catch (EStringListError* err)
                {
                        return;
                }
	}

	if (pItem->Selected) return;

	draw = false;								//We'll handle the drawing

	r = pItem->DisplayRect(drBounds);
	pCanvas = pList->Canvas;
	pCanvas->Font->Color = (pSrc->flags & SF_MUTE) ? clGray : clWhite;

	r.left = pItem->Left;
	r.right = r.left + pList->Column[0]->Width;
	pCanvas->TextRect(r, r.left + 2, r.top, pItem->Caption);

	r.left = r.right;
	r.right += pList->Column[1]->Width;
	pCanvas->TextRect(r, r.right - pCanvas->TextWidth(pItem->SubItems->Strings[0]) - 8,
					  r.top, pItem->SubItems->Strings[0]);
	r.left = r.right;
	r.right += pList->Column[2]->Width;
	pCanvas->TextRect(r, r.left + 4, r.top, pItem->SubItems->Strings[1]);

	r.left = r.right;
	r.right += pList->Column[3]->Width;
	pCanvas->TextRect(r, r.left + 4, r.top, pItem->SubItems->Strings[2]);

	r.left = r.right;
	r.right += pList->Column[4]->Width;
	pCanvas->TextRect(r, r.left + 4, r.top, pItem->SubItems->Strings[3]);
}


//**************************************************************************************************
void __fastcall TFrmMain::lvwSrcExit(TObject *sender)
{
	if (State == ST_PLAY) StopNote();
}


//**************************************************************************************************
// Saves the display positions of all of the items in the sound source list view

void __fastcall TFrmMain::SaveSrcPos()
{
	TListItem	*pItem;
	APUFile		*pa;
	ROMFile		*pr;
	u32			i,j;


	if (PCurFile->type < ID6_ROM)
	{
		pa = (APUFile*)PCurFile;
		pa->sel.Clear();

		for (j=0; j<256; j++)
		{
			pItem = lvwSrc->Items->Item[j];

			i = ((Source*)pItem->Data)->idx;

			pa->ord[j] = i;
			if (pItem->Selected) pa->sel << i;
		}
	}
	else
	{
		pr = (ROMFile*)PCurFile;
		pr->sel.Clear();

		for (j=0; j<(u32)PCurFile->num; j++)
		{
			pItem = lvwSrc->Items->Item[j];

			i = ((Source*)pItem->Data)->idx;

			pr->ord[j] = i;
			if (pItem->Selected) pr->sel << i;
		}
	}

//	PCurFile->pTop = (Source*)lvwSrc->TopItem->Data;
//	if (PCurFile->pTop == (Source*)lvwSrc->Items->Item[0]->Data) PCurFile->pTop = NULL;
i = SendMessage(lvwSrc->Handle, LVM_GETTOPINDEX, 0, 0);
PCurFile->pTop = (Source*)i;
}


//**************************************************************************************************
// Play Sound Source

static void CALLBACK TimeSrc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	TCanvas		*pCanvas;
	u32			x,d;

	if (!Vis.rate)
	{
		FrmMain->pbStopClick(NULL);
		return;
	}

	x = MulDiv(Vis.rate, WavPos(), Cfg.dsp.rate);
	d = DWSrc.len << 4;
	if (x > d)
	{
		x -= DWSrc.loop << 4;
		x %= (DWSrc.len - DWSrc.loop) << 4;
		x += DWSrc.loop << 4;
	}

	if (FrmMain->barWave->Enabled)
	{
		d = FrmMain->barWave->Position << 4;
		if ((x >= d) && (x < d + FrmMain->pbWave->Width))
			x -= d;
		else
			x = -1;
	}
	else
		x = MulDiv(x, FrmMain->pbWave->Width, DWSrc.len << 4);


	pCanvas = FrmMain->pbWave->Canvas;
	pCanvas->Lock();
	pCanvas->Pen->Mode = pmNot;

	if (Vis.lastPos != -1)
	{
		pCanvas->PenPos = TPoint(Vis.lastPos, 0);
		pCanvas->LineTo(Vis.lastPos, 64);
	}

	if (x != -1)
	{
		pCanvas->PenPos = TPoint(x, 0);
		pCanvas->LineTo(x, 64);
	}

	Vis.lastPos = x;

	pCanvas->Pen->Mode = pmCopy;
	pCanvas->Unlock();
}

static u32 PlaySrc2(void *pOut, u32 len, u32 buf)
{
	if (pOut) return (u32)Apu.PlaySrc(pOut, len / (2 * abs(Cfg.dsp.bits) >> 3), 0) - (u32)pOut;

	Vis.rate = 0;

	return 0;
}

void __fastcall TFrmMain::PlayNote(s32 octave, s32 note)
{
	s32	key,rate;
	s8	str[32];


	key = ((octave - 4) * 12) + note;

        //for (rate=0;rate==0;)
	rate = PCurSrc->rate * pow(2.0, key / 12.0);

	if (pbStop->Enabled) WavFlush();

	rate = (s32)Apu.PlaySrc((void*)PCurSrc->offset, PCurSrc->loop, rate);

	sprintf(str, "%iHz (%s%i)", rate, NoteLst[note], octave);
	lblRate->Caption = str;
	Vis.rate = rate;

	if (pbStop->Enabled)
		WavRestart(0);
	else
		SetState(ST_PLAY);
}

void __fastcall TFrmMain::StopNote()
{
	SetState(ST_STOP);
	lblRate->Caption = AnsiString(PCurSrc->rate) + "Hz";
}


//**************************************************************************************************
// Column Sort

static s32 __stdcall SortProc(long item1, long item2, long order)
{
	Source	*p1,*p2;
	s32		cmp;


	p1 = (Source*)((TListItem*)item1)->Data;
	p2 = (Source*)((TListItem*)item2)->Data;

	if ((order & 0x7F) == 0)
	{
		if (order >= 0)
			return p1->idx - p2->idx;
		else
			return p2->idx - p1->idx;
	}

	if (p1->offset == -1) return 1;
	if (p2->offset == -1) return -1;

	switch (order & 0x7F)
	{
	case(1):
		cmp = p1->offset - p2->offset;
		break;

	case(2):
		cmp = p1->len - p2->len;
		break;

	case(3):
		if (p1->loop == 0xFFFF) cmp = -1;
		else
		if (p2->loop == 0xFFFF) cmp = 1;
		else
			cmp = p1->loop - p2->loop;
		break;

	case(4):
		cmp = (p1->flags & 0xF) - (p2->flags & 0xF);
		break;
	}

	if (order >= 0) return cmp;
	else return -cmp;
}


void __fastcall TFrmMain::lvwSrcColumnClick(TObject *sender, TListColumn *column)
{
	static s8	colIdx = 0;
	u32			i;


	if ((colIdx & 0x7F) == column->Index) colIdx ^= 0x80;
	else colIdx = column->Index;

	((TCustomListView *)sender)->CustomSort(SortProc, colIdx);
/*
	if (pCurEdit && pCurEdit->type <= ID6_ZST)
	{
		for (i=0; i<256; i++)
			srcItems[((Source*)lvwSrc->Items->Item[i]->Data)->idx] = lvwSrc->Items->Item[i];
	}
*/
}


//**************************************************************************************************
// Select Sound Source
//
// In:
//    pSrc -> Sound source to draw (-1 redraw last source, NULL erases the box and draws nothing)

void __fastcall TFrmMain::lvwSrcSelectItem(TObject *sender, TListItem *item, bool selected)
{
	//Update valid sources selected ------------
	if (!selected)
	{
		if (((Source*)item->Data)->offset != -1) NumSrc--;
		goto Done;								//If being deselected, nothing else needs to be done
	}
	else
	if (((Source*)item->Data)->offset != -1) NumSrc++;

	//Stop any playing notes -------------------
	if (State == ST_PLAY) StopNote();

	//Update source panel ----------------------
	if (lvwSrc->SelCount == 1 && ((Source*)item->Data)->offset != -1)
	{
		PCurSrc = (Source*)item->Data;

		if (PCurFile->type >= ID6_ROM)			//If a ROM file, copy source data into local APU RAM
			memcpy(Apu.ram, &PCurFile->pData[PCurSrc->offset], PCurSrc->len * 9);

		if (!Scope)								//If scope isn't being drawn, draw source
		{
			if (PCurSrc->len << 4 > pbWave->Width)	//If source is larger than view, enable zoom
			{
				pbZoom->Enabled = true;
				barWave->Position = 0;
				if (pbZoom->Tag & BTN_STATE)	//If zoom is enabled, set max position
				{
					barWave->Enabled = true;
					barWave->Max = PCurSrc->len - 1;// - (pbWave->Width >> 4);
				}
			}
			else
			{
				barWave->Enabled = false;
				pbZoom->Enabled = false;
			}

			DrawWave(PCurSrc);
		}

		PrintSrcInfo();

		itmSetOpt->Enabled = true;
	}
	else
	{
		PCurSrc = NULL;

		barWave->Enabled = false;
		pbZoom->Enabled = false;

		DrawWave(NULL);

		lblRate->Caption = "";
		lblProg->Caption = "";
		lblBank->Caption = "";
		lblType->Caption = "";
		lblADSR->Caption = "";
		lblCRC->Caption  = "";
	}

Done:
	EnableSrcItems();
}


//**************************************************************************************************
void __fastcall TFrmMain::lvwSrcDblClick(TObject *sender)
{
	if (PCurSrc)
		itmSetOptClick(sender);
	else
		AddSrc(lvwSrc->Selected);
}


//**************************************************************************************************
void __fastcall TFrmMain::popSrcPopup(TObject *sender)
{
	popSrc->Items->Items[0]->Visible = (lvwSrc->SelCount != NumSrc);
	popSrc->Items->Items[1]->Visible = (NumSrc != 0);
	popSrc->Items->Items[2]->Visible = itmMute->Enabled;
	popSrc->Items->Items[3]->Visible = itmSilence->Enabled;
	popSrc->Items->Items[4]->Visible = itmSetOpt->Enabled;
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSShowClick(TObject *sender)
{
	TListItem	*pItem;
	TItemStates	selected;


	selected << isSelected;

	pItem = lvwSrc->Selected;
	while (pItem)
	{
		if (((Source*)pItem->Data)->offset == -1) AddSrc(pItem);

		pItem = lvwSrc->GetNextItem(pItem, sdAll, selected);
	}
}



//**************************************************************************************************
void __fastcall TFrmMain::itmSHideClick(TObject *Sender)
{
	TListItem	*pItem;
	TItemStates	selected;
	Source		*pSrc;


	selected << isSelected;

	pItem = lvwSrc->Selected;
	while (pItem)
	{
		pSrc = (Source*)pItem->Data;

		if (pSrc->offset != -1)
		{
			pSrc->offset = -1;
			pItem->SubItems->Strings[0] = "     ";
			pItem->SubItems->Strings[1] = "     ";
			pItem->SubItems->Strings[2] = "     ";
			pItem->SubItems->Strings[3] = "    ";
		}

		pItem = lvwSrc->GetNextItem(pItem, sdAll, selected);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Emulation Pane

//**************************************************************************************************
static b8	TrkAmpChg = 1;

void __fastcall TFrmMain::SetTrkAmp(u32 amp)
{
	TrkAmpChg = 0;
	trkAmp->Position = (s32)-YLog2(60.0, amp / 65536.0);
	trkAmp->Tag = amp;
	TrkAmpChg = 1;
}

void __fastcall TFrmMain::trkAmpChange(TObject *sender)
{
	s32	dB;


	dB = -trkAmp->Position;
	lblAmp->Caption = FormatFloat("0.0'dB'", dB/10.0);

	if (TrkAmpChg)
	{
		trkAmp->Tag = (s32)(65536.0 * pow(2.0, dB/60.0));
		Apu.SetDSPAmp(trkAmp->Tag);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::PrintTimer(u32 time)
{
	s8	str[12];

//if ((time / 640) >= 1942) itmDebugClick(NULL);

	if (Remaining && UseTimer)
	{
		str[0] = '-';
		PrintTime(&str[1], PCurAPU->id6.GetTotal() - time);
	}
	else
		PrintTime(str, time);

	lblTime->Caption = str;
}


//**************************************************************************************************
// Draw Peak Meter
//
// In:
//    length = Length of peak meter returned from SNESAPU.VMax2dB()
//    top    = Y position of meter
//    height = Height of bar (in pixels)

static void DrawMeter(s32 length, s32 top, s32 height)
{
	s32		width;


	width = ((length * 3) >> 16) - 144;			//48dB, three pixels per decible
	_Clamp(width, 0, 152);

	BitBlt(FrmMain->pbMeter->Canvas->Handle, 0, top, width, height, Vis.meters.hdc, 0, 0, SRCCOPY);
}


void __fastcall TFrmMain::pbMeterPaint(TObject *Sender)
{
	u32	i;
	s32	*p;


	if (Cfg.vis.vu && State == ST_RUN)
	{
		p = Vis.meters.peak[Vis.meters.frame];
		Vis.meters.frame ^= 1;

		for (i=0; i<8; i++)
		{
			DrawMeter(*p++, LblVoice[i]->Top, 5);
			DrawMeter(*p++, LblVoice[i]->Top + 6, 5);
		}

		DrawMeter(*p++, FrmMain->lblChnL->Top, 11);
		DrawMeter(*p++, FrmMain->lblChnR->Top, 11);
	}
}


//**************************************************************************************************
#define	_TBSection(color) \
		if (r.left != r.right) \
		{ \
			pCanvas->Brush->Color = (TColor)color; \
			pCanvas->FillRect(r); \
		}

static void __fastcall SetTimeBarPos(u32 time, u32 total)
{
	static u32	x;
	u32			i;
	TCanvas		*pCanvas;


	if (time >= total && time != -1) return;

	pCanvas = FrmMain->pbTime->Canvas;

	if (time == -1) goto Redraw;

	i = x;
	x = MulDiv(FrmMain->pbTime->Width, time, total);

	if (i != x)
	{
		if (i < Vis.timeBar.loop) pCanvas->Pen->Color = (TColor)0x600080;
		else
		if (i < Vis.timeBar.end) pCanvas->Pen->Color = (TColor)0x800060;
		else
		if (i < Vis.timeBar.fade) pCanvas->Pen->Color = (TColor)0x806000;
		else
			pCanvas->Pen->Color = (TColor)0x608000;

		pCanvas->PenPos = TPoint(i, 0);
		pCanvas->LineTo(i, FrmMain->pbTime->Height);

Redraw:
		pCanvas->Pen->Color = clWhite;
		pCanvas->PenPos = TPoint(x, 0);
		pCanvas->LineTo(x, FrmMain->pbTime->Height);
	}
}

static void CALLBACK TimeSPC(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	VisLabel	*pLabel;
	VisVoice	*pVoice;
	Source		*pSrc;
	u32			i;
	u32			x,y;
	s32			amp;
	s8			str[12],*t;
	u8			*p;
	TPoint		newSrcP[8];
	static b8	odd;


	if (Seeking)
	{
		i = Apu.GetSPCTime();
		FrmMain->PrintTimer(i);
		SetTimeBarPos(i, PCurAPU->id6.GetTotal());
		return;
	}

	if (--Vis.second == 0)
	{
		Vis.second = 50;
		if (FrmProf && FrmProf->Visible) FrmProf->UpdateProf();
	}

	if (visFrame >= (u32)Cfg.out.size / 20) return;	//If all frames have been displayed, return
	Vis.pFrame = &Vis.frame[visBuffer][visFrame++];	//Get address of visualization data

	//Get current time -------------------------
	if (UseTimer && Vis.pFrame->time > Length)	//Stop, if end of song has been reached
	{
		FrmMain->itmAbortClick(NULL);
		return;
	}

	FrmMain->PrintTimer(Vis.pFrame->time);		//Print current time
	SetTimeBarPos(Vis.pFrame->time, PCurFile->id6.GetTotal());

	amp = (s32)Apu.GetDSPAmp();
	if (FrmMain->trkAmp->Tag != amp) FrmMain->SetTrkAmp(amp);

	//One-time visualizations ------------------
	if (visFrame == 1)
	{
		if (FrmDSP && FrmDSP->Visible) FrmDSP->Update();
	}

	odd ^= 1;
	if (Cfg.vis.fps < 50 && odd) return;		//Drop odd frames for 25Hz

	FrmMain->txtOut0->Text = IntToHex((s32)Apu.GetAPUPort(0), 2);
	FrmMain->txtOut1->Text = IntToHex((s32)Apu.GetAPUPort(1), 2);
	FrmMain->txtOut2->Text = IntToHex((s32)Apu.GetAPUPort(2), 2);
	FrmMain->txtOut3->Text = IntToHex((s32)Apu.GetAPUPort(3), 2);

	//Visualize each voice ---------------------
	pLabel = Vis.label;
	for (i=0; i<8; i++)
	{
		pVoice = &Vis.pFrame->voice[i];

		if (pLabel->volL->Tag != pVoice->volL)
		{
			pLabel->volL->Caption = pVoice->volL;
			pLabel->volL->Tag = pVoice->volL;
		}

		if (pLabel->volR->Tag != pVoice->volR)
		{
			pLabel->volR->Caption = pVoice->volR;
			pLabel->volR->Tag = pVoice->volR;
		}

		if (pLabel->mode->Tag != pVoice->mode)
		{
			switch(pVoice->mode & ENVM_MODE)
			{
			case(ENV_DEC):
				pLabel->mode->Caption = "Dec";
				break;

			case(ENV_EXP):
				pLabel->mode->Caption = "Exp";
				break;

			case(ENV_INC):
				pLabel->mode->Caption = "Inc";
				break;

			case(ENV_BENT):
				pLabel->mode->Caption = "Bent";
				break;

			case(ENV_DIR):
				pLabel->mode->Caption = "Direct";
				break;

			case(ENV_REL):
			case(ENV_SUST):
			case(ENV_ATTACK):
			case(ENV_DECAY):
				pLabel->mode->Caption = "ADSR";
				break;
			}

			pLabel->mode->Tag = pVoice->mode;
		}

		if (pLabel->env->Tag != pVoice->env)
		{
			pLabel->env->Caption = pVoice->env;
			pLabel->env->Tag = pVoice->env;
		}

		if (pLabel->src->Tag != pVoice->src)
		{
			pLabel->src->Caption = pVoice->src;
			pLabel->src->Tag = pVoice->src;
		}

		if (pLabel->rate->Tag != pVoice->rate)
		{
			pLabel->rate->Caption = pVoice->rate;
			pLabel->rate->Tag = pVoice->rate;
		}

		if (pLabel->sw->Tag != pVoice->sw)
		{
			*(u32*)str = 0x202020;
			if (pVoice->sw & 1) str[0] = 'E';
			if (pVoice->sw & 2) str[1] = 'P';
			if (pVoice->sw & 4) str[2] = 'N';
			pLabel->sw->Caption = str;

			pLabel->sw->Tag = pVoice->sw;
		}

		if (!(pVoice->mode & 0x80))
		{
			pSrc = &PCurFile->src[pVoice->src];

			if (!(pSrc->flags & SF_PLAYED)) pSrc->flags |= (SF_PLAYED | SF_CHANGED);

//			if (pSrc->flags & SF_CHANGED) pSrc->pItem->Update();
		}

		pLabel++;
	}

	if (FrmMain->lblMVolL->Tag != Vis.pFrame->mvolL) FrmMain->lblMVolL->Caption = Vis.pFrame->mvolL;
	if (FrmMain->lblMVolR->Tag != Vis.pFrame->mvolR) FrmMain->lblMVolR->Caption = Vis.pFrame->mvolR;
	if (FrmMain->lblEVolL->Tag != Vis.pFrame->evolL) FrmMain->lblEVolL->Caption = Vis.pFrame->evolL;
	if (FrmMain->lblEVolR->Tag != Vis.pFrame->evolR) FrmMain->lblEVolR->Caption = Vis.pFrame->evolR;

	if (Cfg.vis.vu)
	{
		memcpy(Vis.meters.peak[Vis.meters.frame], Vis.pFrame->peak, 9*8);
		FrmMain->pbMeter->Invalidate();
	}

	//Oscilloscope -----------------------------
	if (Scope) DrawScope(WavMakeScope(Vis.pFrame->pBuf));
}


static u32 PlaySPC(void *pOut, u32 len, u32 buf)
{
	s32			i,j,frameSize;
	u32			out;
	VisFrame	*pVis;


	if (!pOut)
	{
		Vis.frame[buf][0].time = Length + 1;
		return 0;
	}
	
	frameSize = (20 * Cfg.dsp.rate) / 1000;
	pVis = Vis.frame[buf];
	out = 0;
	do
	{
		pVis->time = Apu.GetSPCTime();
		pVis->pBuf = pOut;

		//Get visualized data ------------------
		Apu.VMax2dB(pVis->peak, 0);				//Convert vMax values into decibles
		for (i=0;i<8;i++)						//Values for each voice
		{
			if (Cfg.dsp.mix != MIX_FLOAT)
			{
				pVis->voice[i].volL = Apu.voice[i].mChnL;
				pVis->voice[i].volR = Apu.voice[i].mChnR;
			}
			else
			{
				pVis->voice[i].volL = *(f32*)&Apu.voice[i].mChnL * 128.0f;
				pVis->voice[i].volR = *(f32*)&Apu.voice[i].mChnR * 128.0f;
			}

			pVis->voice[i].mode = (Apu.voice[i].eMode & 0xF) | ((Apu.voice[i].mFlg & 8) << 4);
			pVis->voice[i].env = Apu.voice[i].eVal >> 4;
			pVis->voice[i].src = Apu.dsp->voice[i].srcn;
			pVis->voice[i].rate = MulDiv(Apu.voice[i].mRate, Cfg.dsp.rate, 65536);
			pVis->voice[i].sw = (Apu.dsp->eon >> i) & 1;
			pVis->voice[i].sw |= ((Apu.dsp->pmon >> i) & 1) << 1;
			pVis->voice[i].sw |= ((Apu.dsp->non >> i) & 1) << 2;
		}

		pVis->mvolL = Apu.dsp->mvolL;			//Main volumes
		pVis->mvolR = Apu.dsp->mvolR;
		pVis->evolL = Apu.dsp->evolL;
		pVis->evolR = Apu.dsp->evolR;
		pVis++;

		//Emulate APU --------------------------
		if (SPCPaused)
			i = (u32)Apu.EmuDSP(pOut, frameSize) - (u32)pOut;
		else
			i = (u32)Apu.EmuAPU(pOut, 0, frameSize) - (u32)pOut;

		(u32)pOut += i;
		out += i;

		if (FrmMain->pbFF->Tag & 1) Apu.EmuAPU(NULL, ((Cfg.time.skip - 1) * APU_CLK) / 50, 0);
	} while (out < len);

	return out;
}


//**************************************************************************************************
// Mute Voice

static void __fastcall VoiceMute(u32 voice, u32 state)
{
	if (state == MUTE_SOLO)
	{
		for (s32 i=0; i<8; i++)
			LblVoice[i]->Font->Color = clGray;
	}

	LblVoice[voice]->Font->Color = Apu.SetDSPVoiceMute(voice, state) ? clGray : clSilver;
}

void __fastcall TFrmMain::lblVoiceMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	s32	i,j,k;


	i = ((TLabel*)sender)->Tag;
	switch(btn)
	{
	case(mbLeft):
		VoiceMute(i, MUTE_TOGGLE);
		break;

	case(mbRight):
		k = 0;
		for (j=7; j>=0; j--)
		{
			k <<= 1;
			k |= Apu.SetDSPVoiceMute(j, MUTE_GET);
		}
		k ^= 0xFF;

		if (i == BitScanF(k) && i == BitScanR(k))
		{
			for (j=0; j<8; j++)
				VoiceMute(j, MUTE_OFF);
		}
		else
			VoiceMute(i, MUTE_SOLO);
		break;
	}
}


//**************************************************************************************************
// Switch Time Display

void __fastcall TFrmMain::lblTimeClick(TObject *sender)
{
	Remaining ^= 1;
}


//**************************************************************************************************
void __fastcall TFrmMain::popTimePopup(TObject *sender)
{
	s8	str[12];


	popTime->Items->Items[0]->Visible = Apu.GetSPCTime();
	PrintTime(str, SeekPos);
	popTime->Items->Items[1]->Caption = "Set starting position to " + AnsiString(str);
}


//**************************************************************************************************
static void FindKON(volatile u8 *reg, volatile u8 val)
{
	if ((u8)reg != 0x4C || !val) return;		//Wait for a non-zero value to be written to KON

	*(u16*)PCurAPU->spc.hdr.reg.pc	= PCurAPU->state.spcState.pc;
	PCurAPU->spc.hdr.reg.a			= PCurAPU->state.spcState.a;
	PCurAPU->spc.hdr.reg.x			= PCurAPU->state.spcState.x;
	PCurAPU->spc.hdr.reg.y			= PCurAPU->state.spcState.y;
	PCurAPU->spc.hdr.reg.sp			= PCurAPU->state.spcState.sp;
	PCurAPU->spc.hdr.reg.psw		= *(u8*)&PCurAPU->state.spcState.psw;

	memcpy(PCurAPU->spc.ram, Apu.ram, 0x10000);
	memcpy(PCurAPU->spc.dsp.reg, Apu.dsp, 0x80);
	memcpy(PCurAPU->spc.xram, &Apu.ram[-0x80], 0x40);
	PCurAPU->spc.dsp.kon = val;

	Apu.SetDSPDbg(NULL, 0);

	return;
}

void __fastcall TFrmMain::itmTSetCurClick(TObject *sender)
{
	SPCDebug	*oldSPC;
	DSPDebug	*oldDSP;


	//Save current state of the APU ------------
	*(u16*)PCurAPU->spc.hdr.reg.pc	= PCurAPU->state.spcState.pc;
	PCurAPU->spc.hdr.reg.a			= PCurAPU->state.spcState.a;
	PCurAPU->spc.hdr.reg.x			= PCurAPU->state.spcState.x;
	PCurAPU->spc.hdr.reg.y			= PCurAPU->state.spcState.y;
	PCurAPU->spc.hdr.reg.sp			= PCurAPU->state.spcState.sp;
	PCurAPU->spc.hdr.reg.psw		= *(u8*)&PCurAPU->state.spcState.psw;

	memcpy(PCurAPU->spc.ram, Apu.ram, 0x10000);
	memcpy(PCurAPU->spc.dsp.reg, Apu.dsp, 0x80);
	memcpy(PCurAPU->spc.xram, &Apu.ram[-0x80], 0x40);

	//Search for the next key on event ---------
	oldDSP = Apu.SetDSPDbg(FindKON, 0);
	Apu.EmuAPU(NULL, APU_CLK, 0);				//Emulate for one second
	Apu.SetDSPDbg(oldDSP, 0);

//	*Apu.t64Cnt = 0;							//Reset clock counter

	//Mark file as modified --------------------
	if (!(PCurFile->flags & F_SAVE)) PCurItem->Caption = PCurItem->Caption + " *";

	FileModified(F_STATE);
}


//**************************************************************************************************
void __fastcall TFrmMain::itmSetPosClick(TObject *sender)
{
	SeekSong(SeekPos);
	itmTSetCurClick(sender);
}


//**************************************************************************************************
void __fastcall TFrmMain::pbTimePaint(TObject *Sender)
{
	TRect		r;
	TCanvas		*pCanvas;						//-> timebar's canvas
	u32			i,t,w;


	pCanvas = pbTime->Canvas;

	//Clear bar if no song is selected ---------
	if (!PCurAPU)
	{
		pCanvas->FillRect(TRect(0, 0, pbTime->Width, pbTime->Height));
		return;
	}

	//Draw bar sections ------------------------
	t = PCurAPU->id6.GetTotal();
	w = pbTime->Width;

	r.top = 0;
	r.bottom = pbTime->Height;

	if (PCurAPU->id6.HasTime())
		i = PCurAPU->id6.intro;
	else
		i = ID666::defSong;

	r.left = 0;
	r.right = MulDiv(w, i, t);
	Vis.timeBar.loop = r.right;
	_TBSection(0x600080)						//Intro

	i += PCurAPU->id6.loop * PCurAPU->id6.loopx;
	if ((s32)PCurAPU->id6.end < 0) i += PCurAPU->id6.end;
	r.left = r.right;
	r.right = MulDiv(w, i, t);
	Vis.timeBar.end = r.right;
	_TBSection(0x800060)						//Loop

	if ((s32)PCurAPU->id6.end >= 0) i += PCurAPU->id6.end;
	r.left = r.right;
	r.right = MulDiv(w, i, t);
	Vis.timeBar.fade = r.right;
	_TBSection(0x806000)						//End

	r.left = r.right;
	r.right = w;
	_TBSection(0x608000)						//Fade

	pCanvas->Brush->Color = clBtnFace;
}


//**************************************************************************************************
void __fastcall TFrmMain::pbTimeClearMarker(TCanvas *pCanvas)
{
	u32			x;

	x = Vis.timeBar.marker;
	if (x < Vis.timeBar.loop) pCanvas->Pen->Color = (TColor)0x600080;
	else
	if (x < Vis.timeBar.end) pCanvas->Pen->Color = (TColor)0x800060;
	else
	if (x < Vis.timeBar.fade) pCanvas->Pen->Color = (TColor)0x806000;
	else
		pCanvas->Pen->Color = (TColor)0x608000;

	pCanvas->PenPos = TPoint(x, 0);
	pCanvas->LineTo(x, pbTime->Height);

}

void __fastcall TFrmMain::pbTimeMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	TCanvas		*pCanvas;
	s8			str[12];


	if (!PCurAPU || Seeking) return;

	pCanvas = pbTime->Canvas;

	pbTimeClearMarker(pCanvas);

	if (shift.Contains(ssShift))
	{
		if ((u32)x < Vis.timeBar.loop)
		{
			SeekPos = 0;
			x = 0;
		}
		else
		if ((u32)x < Vis.timeBar.end)
		{
			SeekPos = PCurAPU->id6.intro;
			x = Vis.timeBar.loop;
		}
		else
		if ((u32)x < Vis.timeBar.fade)
		{
			SeekPos = PCurAPU->id6.intro + (PCurAPU->id6.loop * PCurAPU->id6.loopx);
			x = Vis.timeBar.end;
		}
		else
		{
			SeekPos = PCurAPU->id6.GetSong();
			x = Vis.timeBar.fade;
		}
	}
	else
		SeekPos = MulDiv(x, PCurAPU->id6.GetTotal(), pbTime->Width);

	pCanvas->Pen->Color = clWhite;
	pCanvas->PenPos = TPoint(x, 0);
	pCanvas->LineTo(x, pbTime->Height);

	PrintTime(str, SeekPos);
	lblTimeSet->Caption = str;

	Vis.timeBar.marker = x;

	pbTime->Tag = 1;
}


//**************************************************************************************************
void __fastcall TFrmMain::pbTimeMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (!Seeking && btn == mbLeft && State == ST_RUN)
	{
		Seeking = 1;
		SeekSong(SeekPos);
		Seeking = 0;

		return;
	}

	if (PCurAPU && btn == mbRight && State != ST_RUN)
	{
		popTime->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::pbTimerClick(TObject *sender)
{
	s8	str[32];


	UseTimer = pbTimer->Tag & 1;

	itmTimer->Checked = UseTimer;
	lblTotal->Font->Color = UseTimer ? clSilver : clGray;

	if (PCurAPU)
	{
		SetAPULength();
		PrintTimer(PCurAPU->state.spcState.t64Cnt);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::ScopeClick(b8 enable)
{
	if (!Cfg.vis.vu) enable = false;
	if (Scope == enable) return;

	Scope = enable;

	if (Scope)
	{
		barWave->Enabled = false;
		pbZoom->Enabled = false;

		if (!Vis.tid && State == ST_HALT) DrawScope((u8*)-1);
	}
	else
	{
		//Draw selected sound source ---------------
		if (PCurSrc && PCurSrc->len << 4 > pbWave->Width)	//Can zoom be enabled?
		{
			pbZoom->Enabled = true;
			barWave->Position = 0;
			if (pbZoom->Tag & BTN_STATE)
			{
				barWave->Enabled = true;
				barWave->Max = PCurSrc->len - 1;// - (pbWave->Width >> 4);
			}
		}

		DrawWave(PCurSrc);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::trkSpeedChange(TObject *sender)
{
	f64	f;

	f = Pow2((f64)trkSpeed->Position / 48.0);
	lblSpeed->Caption = FloatToStrF(f, ffFixed, 3, 2) + "x";
	Apu.SetAPUSmpClk((s32)(65536.0 * f));
}


//**************************************************************************************************
void __fastcall TFrmMain::trkPitchChange(TObject *sender)
{
	f64	f;

	f = (f64)trkPitch->Position / 4.0;
	lblPitch->Caption = FloatToStrF(f, ffFixed, 3, 2);
	Apu.SetDSPPitch((s32)(Pow2(f / 12.0) * 32000.0));
}


//**************************************************************************************************
void __fastcall TFrmMain::txtInChange(TObject *sender)
{
	TEdit	*p;
	u8	 	h,l;


	p = (TEdit*)sender;

	if (p->Text.Length() != 2) return;			//Field must have two characters

	h = (p->Text[1] | 0x20) - 0x30;				//Convert uppercase to lower and convert digits to
	l = (p->Text[2] | 0x20) - 0x30;				// their binary value

	if (h > 9)									//If h > 9, it may have been 'A' - 'F'
	{
		h -= 0x27;
		if (h < 10 || h > 15) return;
	}

	if (l > 9)
	{
		l -= 0x27;
		if (l < 10 || l > 15) return;
	}

	Apu.SetAPUPort(p->Tag, (h << 4) | l);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Emulation Pane (Source Related)


//**************************************************************************************************
static void __fastcall DrawWave(Source *pSrc)
{
	static Source	*pLast;


	if ((s32)pSrc == -1)
	{
		if (PCurFile) pSrc = pLast;
		else pSrc = NULL;
	}

	if (pSrc)
	{
		pLast = pSrc;
		DWSrc.offset = pSrc->offset;
		DWSrc.len = pSrc->len;
		DWSrc.loop = pSrc->loop;
	}
	else
		DWSrc.offset = 0;

	FrmMain->pbWave->Invalidate();
}


//**************************************************************************************************
static void __fastcall DrawScope(u8 *p)
{
	if ((s32)p != -1) DWSrc.pScope = p;
	FrmMain->pbWave->Invalidate();
}


//**************************************************************************************************
void __fastcall TFrmMain::pbWavePaint(TObject *Sender)
{
	TCanvas	*pCanvas;
	u8		*p;
	u32		blk;								//Current block to decompress
	s32		p1,p2;								//Previous two samples
	s16		smp[20];							//Temp buffer of 20 samples
	u32		l,s;								//Length of data, current sample in smp
	s32		x,y,y1;
	u32		xp,xd,n;							//X position [16.16], X delta [16.16]


	pCanvas = pbWave->Canvas;

	//Clear image ------------------------------
	pCanvas->Pen->Color = (TColor)0x404040;
	pCanvas->PenPos = TPoint(0, 31);
	pCanvas->LineTo(pbWave->Width, 31);

	pCanvas->Pen->Color = (TColor)0xFF0000;


	//==========================================
	// Draw waveform

	if (Scope && (State == ST_RUN || State == ST_HALT))
	{
		p = DWSrc.pScope;
		pCanvas->PenPos = TPoint(0, *p);

		l = pbWave->Width;
		for (x=0; x<l; x++)
			pCanvas->LineTo(x, *p++);
	}
	else
	if (DWSrc.offset)
	{
		blk = PCurFile->type < ID6_ROM ? DWSrc.offset : 0;
		l = DWSrc.len;
		p1 = 0;
		p2 = 0;

		xp = 0;
		y = 31;

		if ((DWSrc.len << 4) >= pbWave->Width)
		{
			if (FrmMain->barWave->Enabled)
			{
				xd = 0x10000;
				l = pbWave->Width >> 4;

				for (s=FrmMain->barWave->Position; s; s--)
				{
					Apu.UnpackSrc(smp, (void*)blk, 1, 0, &p1, &p2);
					blk += 9;
				}
			}
			else
				xd = (pbWave->Width << 12) / DWSrc.len;

			do
			{
				if (FrmMain->barWave->Enabled)
					pCanvas->Pen->Color = CBlkFilter[(Apu.ram[blk] >> 2) & 3];

				Apu.UnpackSrc(smp, (void*)blk, 1, 0, &p1, &p2);
				blk += 9;

				for (s=0; s<16; s++)
				{
					x = xp >> 16;
					xp += xd;
					pCanvas->PenPos = TPoint(x, y);

					y1 = y;
					y = -(smp[s] >> 10) + 31;    //Ending pixel on Y axis
					pCanvas->LineTo(x, y + (y == y1));
				}
			} while (--l);

			if (FrmMain->barWave->Enabled)		//Find location of loop point
			{
				x = -1;

				if (DWSrc.loop >= FrmMain->barWave->Position &&
					DWSrc.loop <  FrmMain->barWave->Position + (pbWave->Width >> 4))
				{
					x = (DWSrc.loop - FrmMain->barWave->Position) << 4;
				}
			}
			else
				x = (xd * DWSrc.loop) >> 12;
		}
		else  //Interpolate
		{
			xd = (DWSrc.len << 20) / pbWave->Width;

			smp[16] = 0;
			smp[17] = 0;
			smp[18] = 0;
			smp[19] = 0;
			x = 0;
			while (1)
			{
				pCanvas->Pen->Color = CBlkFilter[(Apu.ram[blk] >> 2) & 3];

				*(u32*)&smp[0] = *(u32*)&smp[16];
				*(u32*)&smp[2] = *(u32*)&smp[18];
				Apu.UnpackSrc(&smp[4], (void*)blk, 1, 0, &p1, &p2);
				blk += 9;

				n = xp & 0x100000;
				do
				{
					pCanvas->PenPos = TPoint(x, y);

					y1 = y;
					s = (xp >> 16) & 0xF;
					y = (xp & 0xFFFF) >> 8;
					xp += xd;

					y = (smp[0+s] * Gaussian[y][0]) +
						(smp[1+s] * Gaussian[y][1]) +
						(smp[2+s] * Gaussian[y][2]) +
						(smp[3+s] * Gaussian[y][3]);
					y = -(y >> 25) + 31;

					pCanvas->LineTo(x, y + (y == y1));

					if (++x == pbWave->Width) goto Done;
				} while ((xp & 0x100000) == n);
			}
Done:
			x = (DWSrc.loop << 20) / xd;
		}

		//Draw loop point ----------------------
		if (DWSrc.loop != 0xFFFF && x != -1)
		{
			pCanvas->Pen->Color = clYellow;
			for (y=0; y<64; y+=2)
			{
				pCanvas->PenPos = TPoint(x, y);
				pCanvas->LineTo(x, y + 1);
			}
		}
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::pbWaveMouseDown(TObject *Sender, TMouseButton btn, TShiftState shift, int x, int y)
{
	if (State == ST_RUN || State == ST_HALT)
	{
		Cfg.vis.scope ^= 1;
		ScopeClick(Cfg.vis.scope);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::barWaveChange(TObject *sender)
{
	if (lvwSrc->SelCount == 1)
	{
		if (barWave->Position > (PCurSrc->len - barWave->PageSize))
		{
			barWave->Position = PCurSrc->len - barWave->PageSize;
			return;
		}

		if (PCurSrc->offset != -1 && PCurSrc->len << 4 >= pbWave->Width)
			DrawWave(PCurSrc);
	}
}


//**************************************************************************************************
void __fastcall TFrmMain::pbPlayClick(TObject *sender)
{
	PlayNote(4,0);
}


//**************************************************************************************************
void __fastcall TFrmMain::pbStopClick(TObject *sender)
{
	StopNote();
}


//**************************************************************************************************
void __fastcall TFrmMain::pbZoomClick(TObject *sender)
{
	b8	enabled;


	//Hack:  Setting the Max position automatically enables the scrollbar, but keeps the Enabled
	//       flag set to false.  So I use the zoom button's state to determine if the scrollbar
	//       should be enabled, and set Max only when it's true.

	enabled = pbZoom->Tag & BTN_STATE;

	barWave->Position = 0;
	barWave->Enabled = enabled;
	if (enabled) barWave->Max = PCurSrc->len - 1;// - (pbWave->Width >> 4);

	DrawWave(PCurSrc);
}


//**************************************************************************************************
void __fastcall TFrmMain::PrintSrcInfo()
{
	lblRate->Caption = AnsiString(PCurSrc->rate) + "Hz";

	lblCRC->Caption = IntToHex((s32)PCurSrc->crc, 4);

	if (PCurSrc->flags & SF_INFO)
	{
		if (PCurSrc->prog & 0x80)
		{
			lblTProg->Caption = "Note";
			lblProg->Caption = DrumLst[(PCurSrc->prog & 0x7F) / 12][(PCurSrc->prog & 0x7F) % 12];
			lblType->Caption = "Drum";
		}
		else
		{
			lblTProg->Caption = "Program";
			lblProg->Caption = ProgLst[PCurSrc->prog >> 3][PCurSrc->prog & 7];
			lblType->Caption = "Tone";
		}

		lblBank->Caption = PCurSrc->bank;
	}
	else
	{
		lblTProg->Caption = "Program";
		lblProg->Caption = "";
		lblType->Caption = "";
		lblBank->Caption = "";
	}

	if (PCurSrc->flags & SF_PLAYED)
	{
		if (PCurSrc->adsr & 0x80)
		{
			lblTADSR->Caption = "ADSR";
			lblADSR->Caption = "a " + IntToHex((s32)(PCurSrc->adsr & 0xF), 1) +
							   "   d " + IntToHex((s32)((PCurSrc->adsr >> 4) & 0x7), 1) +
							   "   l " + IntToHex((s32)((PCurSrc->adsr >> 13) & 0x7), 1) +
							   "   s " + IntToHex((s32)((PCurSrc->adsr >> 8) & 0x1F), 2);
		}
		else
		{
			lblTADSR->Caption = "Gain";
			if (PCurSrc->adsr & 0x8000)
			{
				switch(PCurSrc->adsr >> 13)
				{
				case(4):
					lblADSR->Caption = "Dec " + AnsiString((PCurSrc->adsr >> 8) & 0x1F);
					break;

				case(5):
					lblADSR->Caption = "Exp " + AnsiString((PCurSrc->adsr >> 8) & 0x1F);
					break;

				case(6):
					lblADSR->Caption = "Inc " + AnsiString((PCurSrc->adsr >> 8) & 0x1F);
					break;

				case(7):
					lblADSR->Caption = "Bent " + AnsiString((PCurSrc->adsr >> 8) & 0x1F);
					break;
				}
			}
			else
				lblADSR->Caption = "Direct " + AnsiString(PCurSrc->adsr >> 8);
		}
	}
	else
	{
		lblTADSR->Caption = "ADSR";
		lblADSR->Caption = "";
	}
}


//**************************************************************************************************
// Resize the columns in the file list

void __fastcall TFrmMain::lvwFileResize(TObject *sender)
{
	s32	i;


	if (lvwFile->Align == alLeft)
	{
		i = lvwFile->Width - 4;
		if (lvwFile->VisibleRowCount < lvwFile->Items->Count) i -= GetSystemMetrics(SM_CXVSCROLL);
		lvwFile->Column[0]->Width = i;
	}
	else
	{
		lvwFile->Columns->BeginUpdate();

		for (i=0; i<lvwFile->Columns->Count; i++)
			lvwFile->Column[i]->Width = -2;

		lvwFile->Columns->EndUpdate();
	}
}


//**************************************************************************************************
// Show or hide the tag colmuns if the 'File' column is clicked on

void __fastcall TFrmMain::lvwFileColumnClick(TObject *sender, TListColumn *pCol)
{
	s32			i,j;
	TControl	*p;


	if (pCol->Index) return;

	j = FrmMain->ControlCount;

	if (lvwFile->Width == FrmMain->ClientWidth)	//Restore to normal file list
	{
		lvwFile->OnColumnRightClick = NULL;
		lvwFile->Visible = false;
		lvwFile->Align = alLeft;

		for (i=1; i<lvwFile->Columns->Count; i++)
			lvwFile->Column[i]->Width = 0;

		lvwFile->Width = pgcSrc->Left - 3;

		for (i=0; i<j; i++)
			FrmMain->Controls[i]->Visible = true;
	}
	else										//Expand to ID666 tag listing
	{
		for (i=0; i<j; i++)
		{
			p = FrmMain->Controls[i];
			if (p == pnlTag || p == bvlTag || p == lvwFile) continue;
			p->Visible = false;
		}

		lvwFile->Align = alClient;

		for (i=0; i<lvwFile->Columns->Count; i++)
			lvwFile->Column[i]->Width = -2;

		lvwFile->OnColumnRightClick = lvwFileColumnRightClick;
	}
}


//**************************************************************************************************
// Update the tag columns for a file in the file list

void __fastcall TFrmMain::UpdateFile(TListItem *pItem)
{
	s8			str[32];
	u32			i,t;
	ID666		*pID6;


	pID6 = &((FileInfo*)pItem->Data)->id6;
	pItem->SubItems->Clear();
	for (i=1; i<lvwFile->Columns->Count; i++)
	{
		switch(lvwFile->Column[i]->Tag)
		{
		case('S'):
			pItem->SubItems->Add(pID6->song);
			break;

		case('G'):
			pItem->SubItems->Add(pID6->game);
			break;

		case('A'):
			pItem->SubItems->Add(pID6->artist);
			break;

		case('P'):
			pItem->SubItems->Add(pID6->pub);
			break;

		case('O'):
			pItem->SubItems->Add(pID6->ost);
			break;

		case('T'):
			str[0] = 0;

			if (pID6->disc)
			{
				if (pID6->track)
					sprintf(str, "%i-%02i", pID6->disc, pID6->track >> 8);
				else
					sprintf(str, "%i", pID6->disc);
			}
			else
			if (pID6->track)
				sprintf(str, "%2i", pID6->track >> 8);

			pItem->SubItems->Add(str);
			break;

		case('U'):
			pItem->SubItems->Add(pID6->dumper);
			break;

		case('D'):
			str[0] = 0;
//TODO
//			if (pID6->date.ToStr(str))
//				pItem->SubItems->Add(str);
//			else
				pItem->SubItems->Add(pID6->datetxt);
			break;

		case('E'):
			if ((u8)pID6->emu < XID6_EMULIST)
				pItem->SubItems->Add(EmuList[pID6->emu]);
			else
				pItem->SubItems->Add("");
			break;

		case('C'):
			pItem->SubItems->Add(pID6->comment);
			break;

		case('L'):
			t = pID6->GetTotal();
			sprintf(str, "%i:%02i", t / XID6_TICKSMIN, (t % XID6_TICKSMIN) / XID6_TICKSSEC);
			pItem->SubItems->Add(str);
			break;

		default:
			pItem->SubItems->Add("");
		}
	}
}


//**************************************************************************************************
// Display a popup of the possible columns to display

void __fastcall TFrmMain::lvwFileColumnRightClick(TObject *sender, TListColumn *pCol, const TPoint &point)
{
	popFile->Tag = pCol->Index;
	popFile->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
}


//**************************************************************************************************
// An item in the file list popup menu was clicked on

void __fastcall TFrmMain::itmFClick(TObject *sender)
{
	TMenuItem	*pItem;
	TListColumn	*pCol;
	s8			*s;
	u32			c,i;


	lvwFile->Items->BeginUpdate();

	pItem = (TMenuItem*)sender;
	if (pItem->Checked)
	{
		pItem->Checked = false;

		for (c=1; c<lvwFile->Columns->Count; c++)
			if (lvwFile->Column[c]->Tag == pItem->Tag)
			{
				lvwFile->Columns->Delete(c--);
				break;
			}

		for (i=0; i<lvwFile->Items->Count; i++)
			lvwFile->Items->Item[i]->SubItems->Delete(c);

		s = strchr(Cfg.col, (s8)pItem->Tag);
		strcpy(s, s+1);
	}
	else
	{
		pItem->Checked = true;

		lvwFile->Columns->Add();

		for (i=lvwFile->Columns->Count-1; i>popFile->Tag; i--)
		{
			lvwFile->Column[i]->Caption = lvwFile->Column[i-1]->Caption;
			lvwFile->Column[i]->Tag = lvwFile->Column[i-1]->Tag;
			lvwFile->Column[i]->Alignment = lvwFile->Column[i-1]->Alignment;
		}

		lvwFile->Column[popFile->Tag+1]->Caption = pItem->Caption;
		lvwFile->Column[popFile->Tag+1]->Tag = pItem->Tag;
		lvwFile->Column[popFile->Tag+1]->Alignment = (pItem->Tag == 'L') ? taRightJustify : taLeftJustify;

		for (i=0; i<lvwFile->Items->Count; i++)
			UpdateFile(lvwFile->Items->Item[i]);

		s = ::StrEnd(Cfg.col);
		s[1] = 0;
		i = s - Cfg.col;
		while (i)
		{
			s[0] = s[-1];
			if (i == popFile->Tag) break;
			s--;
			i--;
		}

		*s = pItem->Tag;
	}

	for (i=0; i<lvwFile->Columns->Count; i++)
		lvwFile->Column[i]->Width = -2;

	lvwFile->Items->EndUpdate();
}


void __fastcall TFrmMain::pgcSrcChange(TObject *sender)
{
	SourceInfo	*pInfo;
	TListItem	*pItem;
	AnsiString	str;


	if (pgcSrc->ActivePageIndex)
	{
		pInfo = PInfoStart;
		while(pInfo)
		{
			pItem = lvwInfo->Items->Add();
			pItem->Caption = IntToHex(pInfo->crc, 4);
			pItem->SubItems->Append((s32)pInfo->len << 4);
			if (pInfo->loop != 0xFFFF)
				str = (s32)pInfo->loop << 4;
			else
				str = "";
			pItem->SubItems->Append(str);
			pItem->SubItems->Append(pInfo->rate);
			str = pInfo->prog & 0x7F;
			if (pInfo->prog & 0x80) str += "(D)";
			pItem->SubItems->Append(str);

			pInfo = pInfo->pNext;
		}
	}
	else
	{
		lvwInfo->Items->Clear();
	}
}


//**************************************************************************************************
void __fastcall HotKey(WORD key, TShiftState shift)
{
	switch(key)
	{
	case(VK_F9):
		if (shift.Contains(ssShift))
		{
			if (FrmMain->pbReset->Enabled) FrmMain->itmResetClick(NULL);
		}
		else
		{
			if (FrmMain->pbRun->Enabled) FrmMain->itmRunClick(NULL);
		}
		break;

	case(VK_F10):
		if (FrmMain->pbHalt->Enabled) FrmMain->itmHaltClick(NULL);
		break;

	case(VK_F11):
		if (FrmMain->pbTimer->Enabled) FrmMain->itmTimerClick(NULL);
		break;

	case(VK_F12):
		if (FrmMain->pbAbort->Enabled) FrmMain->itmAbortClick(NULL);
		break;
	}
}


//**************************************************************************************************
void __fastcall SeekSong(u32 time)
{
	u32	t;


	t = Apu.GetSPCTime();

	if (State == ST_RUN)
	{
		WavFlush();
		visFrame = (u32)Cfg.out.size / 20;
	}

	if (t > time)
	{
		Apu.LoadSPCFile(&PCurAPU->spc);

		if (FrmDSP && FrmDSP->Visible) FrmDSP->Reset();

		SetAPULength();
                Apu.SeekAPU(time, 0);
	}
        else
        {
	        Apu.SeekAPU(time - t, 0);
        }

	if (State == ST_RUN) WavRestart(t);
}


//**************************************************************************************************
void __fastcall SetAPULength()
{
	if (UseTimer)
	{
		if (Apu.GetSPCTime() > PCurAPU->id6.GetSong())
			Length = Apu.SetDSPLength(Apu.GetSPCTime(), PCurAPU->id6.GetFade());
		else
			Length = Apu.SetDSPLength(PCurAPU->id6.GetSong(), PCurAPU->id6.GetFade());
	}
	else
		Apu.SetDSPLength(-1, 0);
}


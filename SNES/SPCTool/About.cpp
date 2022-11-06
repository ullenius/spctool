//**************************************************************************************************

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"About.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

extern HINSTANCE	HAPU;

TFrmAbout *FrmAbout;


//**************************************************************************************************
__fastcall TFrmAbout::TFrmAbout(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmAbout::FormShow(TObject *sender)
{
	s8					l,*s;
	u32					i,size;
	void				*pInfo;
	VS_FIXEDFILEINFO	*pVer;
	s8				 	str[MAX_PATH];


	//SPCTool.exe version ----------------------
	size = GetFileVersionInfoSize("SPCTool.exe", 0);
	pInfo = malloc(size);
	GetFileVersionInfo("SPCTool.exe", 0, size, pInfo);

	VerQueryValue(pInfo, "\\", (void**)&pVer, &size);

	sprintf(str, "SPC Tool v%i.%02i build %i",
			pVer->dwFileVersionMS >> 16,
			(u16)pVer->dwFileVersionMS,
			(u16)pVer->dwFileVersionLS);

	free(pInfo);

	lblVersion->Caption = str;


	//SNESAPU.DLL version ----------------------
	s = str;
	sprintf(s, "SNESAPU.DLL v%01i.%02i", Cfg.apu.ver >> 24, (Cfg.apu.ver >> 16) & 0xFF);
	s += strlen(s);

	l = (s8)(Cfg.apu.ver >> 8);
	if (l) *s++ = l + 0x60;

	sprintf(s, " build %i (v%01i.%02i", (u8)Cfg.apu.ver, Cfg.apu.min >> 24, (Cfg.apu.min >> 16) & 0xFF);
	s += strlen(s);

	l = (s8)(Cfg.apu.min >> 8);
	if (l) *s++ = l + 0x60;

	i = (APU_CLK / 1000) / Cfg.apu.clk;
	sprintf(s, ") %i.%03iMHz", i / 1000, i % 1000);

	lblAPUVer->Caption	= str;
	

	//SNESAPU.DLL build options ----------------
	lblDebug->Caption	= (Cfg.apu.opt & SA_DEBUG) ? "yes" : "no";
	lblHalfC->Caption	= (Cfg.apu.opt & SA_HALFC) ? "emulated" : "disabled";
	lblIPLW->Caption	= (Cfg.apu.opt & SA_IPLW) ? "writable" : "read-only";
	lblSpeed->Caption	= (Cfg.apu.opt & SA_SPEED) ? "enabled" : "disabled";

	lblDSPInt->Caption	= (Cfg.apu.opt & SA_DSPINTEG) ? "yes" : "no";
	lblProf->Caption	= (Cfg.apu.opt & SA_PROFILE) ? "enabled" : "disabled";
	switch(Cfg.apu.opt & (SA_MAINPPM | SA_VOICEPPM))
	{
	case(0):			lblPPM->Caption = "disabled"; break;
	case(SA_MAINPPM):	lblPPM->Caption = "main only"; break;
	case(SA_VOICEPPM):	lblPPM->Caption = "voice only"; break;
	default:			lblPPM->Caption = "main && voice"; break;
	}
	lblStereo->Caption	= (Cfg.apu.opt & SA_STEREO) ? "enabled" : "disabled";
}


//**************************************************************************************************
void __fastcall TFrmAbout::imgBackgroundMouseMove(TObject *sender, TShiftState Shift, int x, int y)
{
	if (Screen->Cursor == crHandPoint) Screen->Cursor = oldCursor;
}


//**************************************************************************************************
void __fastcall TFrmAbout::lblURLMouseMove(TObject *sender, TShiftState shift, int x, int y)
{
	if (Screen->Cursor != crHandPoint)
	{
		oldCursor = Screen->Cursor;
		Screen->Cursor = crHandPoint;
	}
}


//**************************************************************************************************
void __fastcall TFrmAbout::lblURLClick(TObject *sender)
{
	ShellExecute(Application->Handle, NULL, "http://www.alpha-ii.com", NULL, NULL, 0);
}


//**************************************************************************************************
void __fastcall TFrmAbout::lblAPUVerClick(TObject *Sender)
{
	s8	str[MAX_PATH];

	GetModuleFileName(HAPU, str, MAX_PATH);
	Application->MessageBox(str, "Location of SNESAPU.DLL", MB_OK);
}


//**************************************************************************************************
void __fastcall TFrmAbout::lblCloseClick(TObject *sender)
{
	Close();
}


//**************************************************************************************************
void __fastcall TFrmAbout::FormKeyPress(TObject *sender, char &key)
{
	if (key == VK_ESCAPE) Close();
}

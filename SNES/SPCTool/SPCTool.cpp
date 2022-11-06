/***************************************************************************************************
*                                                           Copyright (C)2004 Alpha-II Productions *
***************************************************************************************************/

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"Config.h"
#include	"WaveOut.h"

USERES("SPCTool.res");
USEUNIT("..\..\A2Math.c");
USEUNIT("..\..\A2Nihon.c");
USEUNIT("..\..\A2RIFF.cpp");
USEUNIT("..\..\A2Str.c");
USEUNIT("..\ID666.cpp");
USEUNIT("..\SWC.cpp");
USEUNIT("WaveOut.cpp");
USEFORM("Main.cpp", FrmMain);
USEFORM("About.cpp", FrmAbout);
USEFORM("Config.cpp", FrmConfig);
USEFORM("DSPEdit.cpp", FrmDSP);
USEFORM("Debug.cpp", FrmDebug);
USEFORM("MIDI.cpp", FrmMIDI);
USEFORM("Map.cpp", FrmMap);
USEFORM("Progress.cpp", FrmProg);
USEFORM("ROM.cpp", FrmROM);
USEFORM("Rename.cpp", FrmRen);
USEFORM("SSI.cpp", FrmSSI);
USEFORM("Tag.cpp", FrmTag);
USEFORM("WAV.cpp", FrmWav);
USE("Buttons.h", File);
USE("ROMCode.h", File);
USERC("OpenDialog.rc");
USEFORM("Profile.cpp", FrmProf);
USEUNIT("Assembly.cpp");
//---------------------------------------------------------------------------
const s8	NClk[32][12]={
			"0Hz",
			"15.625Hz",	"20.83Hz",	"25Hz",
			"31.25Hz",	"41.67Hz",	"50Hz",
			"62.5Hz",	"83.33Hz",	"100Hz",
			"125Hz",	"166.67Hz",	"200Hz",
			"250Hz",	"333.33Hz",	"400Hz",
			"500Hz",	"666.67Hz",	"800Hz",
			"1kHz",		"1.33kHz",	"1.6kHz",
			"2kHz",		"2.67kHz",	"3.2kHz",
			"4kHz",		"5.33kHz",	"6.4kHz",
			"8kHz",		"10.67kHz",
			"16kHz",
			"32kHz"};

SAPUData	Apu;
UnrarFunc	Rar;
Config		Cfg;
BtnColors	CBtn;

HINSTANCE	HAPU,HUnrar;


//**************************************************************************************************
#define	SAPU_NEWEST	0x02280006
#define	SAPU_OLDEST	0x02280006

HINSTANCE InitSAPU()
{
	HINSTANCE			hDLL;
	u32					i,size;
	void				*pInfo;
	VS_FIXEDFILEINFO	*pVer;
	s8				 	str[MAX_PATH];



	//Load SNESAPU.DLL -------------------------
	hDLL = LoadLibrary("SNESAPU.DLL");	 	//Try to load SNESAPU
	if (hDLL)
	{
		Cfg.apu.ver = 0;
		Cfg.apu.min = 0;

		size = GetFileVersionInfoSize("SNESAPU.DLL", 0);
		if (size)
		{
			pInfo = malloc(size);
			GetFileVersionInfo("SNESAPU.DLL", 0, size, pInfo);

			VerQueryValue(pInfo, "\\", (void**)&pVer, &size);
			Cfg.apu.ver = ((pVer->dwFileVersionMS <<  8) & 0xFF000000) |
						  ((pVer->dwFileVersionMS << 16) & 0x00FF0000) |
						  ((pVer->dwFileVersionLS >>  8) & 0x0000FF00) |
						  ( pVer->dwFileVersionLS        & 0x000000FF);

			Cfg.apu.min = ((pVer->dwProductVersionMS <<  8) & 0xFF000000) |
						  ((pVer->dwProductVersionMS << 16) & 0x00FF0000) |
						  ((pVer->dwProductVersionLS >>  8) & 0x0000FF00) |
						  ( pVer->dwProductVersionLS        & 0x000000FF);
			free(pInfo);
		}
	}
	else
	{
		Application->MessageBox("Could not load SNESAPU.DLL.  This file needs to be in your " \
								"SPCTool directory in order for SPCTool to run.  " \
								"You can download SNESAPU.DLL from http://www.alpha-ii.com",
								"SPCTool", MB_OK | MB_ICONERROR);
		return NULL;
	}


	//Check version requirments ----------------
	if (Cfg.apu.ver < SAPU_OLDEST)
	{
		Application->MessageBox("Your copy of SNESAPU.DLL is too old to work with this " \
								"version of SPCTool.  Please get version 0.95 or later from " \
								"http://www.alpha-ii.com",
								"SPCTool", MB_OK | MB_ICONERROR);
		FreeLibrary(hDLL);
		return NULL;
	}
	else
	if (Cfg.apu.min > SAPU_NEWEST)
	{
		Application->MessageBox("This version of SPCTool is too old to work with your copy of "\
								"SNESAPU.DLL.  Please get the latest version of SPC Tool from "\
								"http://www.alpha-ii.com",
								"SPCTool", MB_OK | MB_ICONERROR);
		FreeLibrary(hDLL);
		return NULL;
	}

	//Get pointers to needed functions ---------
	(void*)Apu.GetAPUData		= (void*)GetProcAddress(hDLL,"GetAPUData");
	(void*)Apu.ResetAPU			= (void*)GetProcAddress(hDLL,"ResetAPU");
	(void*)Apu.LoadSPCFile		= (void*)GetProcAddress(hDLL,"LoadSPCFile");
	(void*)Apu.SaveAPU			= (void*)GetProcAddress(hDLL,"SaveAPU");
	(void*)Apu.RestoreAPU		= (void*)GetProcAddress(hDLL,"RestoreAPU");
	(void*)Apu.SetAPUOpt		= (void*)GetProcAddress(hDLL,"SetAPUOpt");
	(void*)Apu.SetAPUSmpClk		= (void*)GetProcAddress(hDLL,"SetAPUSmpClk");
	(void*)Apu.EmuAPU			= (void*)GetProcAddress(hDLL,"EmuAPU");
	(void*)Apu.SeekAPU			= (void*)GetProcAddress(hDLL,"SeekAPU");
	(void*)Apu.SetSPCDbg		= (void*)GetProcAddress(hDLL,"SetSPCDbg");
	(void*)Apu.SetAPURAM		= (void*)GetProcAddress(hDLL,"SetAPURAM");
	(void*)Apu.SetAPUPort		= (void*)GetProcAddress(hDLL,"SetAPUPort");
	(void*)Apu.GetAPUPort		= (void*)GetProcAddress(hDLL,"GetAPUPort");
	(void*)Apu.GetSPCTime		= (void*)GetProcAddress(hDLL,"GetSPCTime");
	(void*)Apu.EmuSPC			= (void*)GetProcAddress(hDLL,"EmuSPC");
	(void*)Apu.GetProcInfo		= (void*)GetProcAddress(hDLL,"GetProcInfo");
	(void*)Apu.SetDSPDbg		= (void*)GetProcAddress(hDLL,"SetDSPDbg");
	(void*)Apu.SetDSPAmp		= (void*)GetProcAddress(hDLL,"SetDSPAmp");
	(void*)Apu.GetDSPAmp		= (void*)GetProcAddress(hDLL,"GetDSPAmp");
	(void*)Apu.SetDSPAAR		= (void*)GetProcAddress(hDLL,"SetDSPAAR");
	(void*)Apu.SetDSPLength		= (void*)GetProcAddress(hDLL,"SetDSPLength");
	(void*)Apu.SetDSPPitch		= (void*)GetProcAddress(hDLL,"SetDSPPitch");
	(void*)Apu.SetDSPStereo		= (void*)GetProcAddress(hDLL,"SetDSPStereo");
	(void*)Apu.SetDSPEFBCT		= (void*)GetProcAddress(hDLL,"SetDSPEFBCT");
	(void*)Apu.SetDSPVoiceMute	= (void*)GetProcAddress(hDLL,"SetDSPVoiceMute");
	(void*)Apu.SetDSPReg		= (void*)GetProcAddress(hDLL,"SetDSPReg");
	(void*)Apu.EmuDSP			= (void*)GetProcAddress(hDLL,"EmuDSP");
	(void*)Apu.PlaySrc			= (void*)GetProcAddress(hDLL,"PlaySrc");
	(void*)Apu.UnpackSrc		= (void*)GetProcAddress(hDLL,"UnpackSrc");
	(void*)Apu.PackSrc			= (void*)GetProcAddress(hDLL,"PackSrc");
	(void*)Apu.VMax2dB			= (void*)GetProcAddress(hDLL,"VMax2dB");

	Cfg.apu.opt = (u32)Apu.GetAPUData(SADATA_OPTIONS);
	Cfg.apu.clk = (u8)Cfg.apu.opt;
	Apu.ram		= (u8*)Apu.GetAPUData(SADATA_RAM);
	Apu.dsp		= (DSPReg*)Apu.GetAPUData(SADATA_DSP);
	Apu.voice	= (Voice*)Apu.GetAPUData(SADATA_VOICE);
	Apu.profile	= (APUProf*)Apu.GetAPUData(SADATA_PROFILE);

	if (!(Cfg.apu.opt & SA_DEBUG))
	{
		Application->MessageBox("Your copy of SNESAPU.DLL doesn't have debugging ability.  You "\
								"will not be able to make MIDI files, mute sound sources, or use "\
								"the DSP register editor or SPC700 debugger.",
								"SPCTool", MB_OK | MB_ICONWARNING);
	}

	//Verify host processor --------------------
	if (!Apu.GetProcInfo())
	{
		Application->MessageBox("SPCTool requires a CPU with MMX capabilities.",
								"SPCTool", MB_OK | MB_ICONERROR);
		FreeLibrary(hDLL);
		return NULL;
	}

	return hDLL;
}


//**************************************************************************************************
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GetModuleFileName(NULL, Cfg.ini, MAX_PATH);
	*ScanStrR(Cfg.ini, '\\') = 0;
	strcpy(Cfg.hlp, Cfg.ini);
	strcat(Cfg.ini, "SPCTool.ini");
	strcat(Cfg.hlp, "Context.hlp");
	LoadConfig();

	if (Cfg.ver < 0x7000)
	{
		if (Application->MessageBox("This version of SPCTool is a work-in-progress (WIP).  By "\
									"continuing you acknowledge that this program is incomplete "\
									"and contains bugs, and that the author is not responsible "\
									"for any damages caused by using this program.",
									"SPCTool", MB_OKCANCEL | MB_ICONINFORMATION) == ID_CANCEL)
			return 0;

		WritePrivateProfileString("SPCTool", "version", "28672", Cfg.ini);
	}

	HAPU = InitSAPU();
	if (!HAPU) return 0;

	Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());
	Apu.SetDSPAAR(Cfg.mix.aar, Cfg.mix.threshold, Cfg.mix.min, Cfg.mix.max);
	Apu.SetDSPStereo(Cfg.dsp.stereo);
	Apu.SetDSPEFBCT(Cfg.dsp.efbct);

	HUnrar = LoadLibrary("unRar.dll");
	if (HUnrar)
	{
		(void*)Rar.OpenArchive		= (void*)GetProcAddress(HUnrar, "RAROpenArchive");
		(void*)Rar.CloseArchive		= (void*)GetProcAddress(HUnrar, "RARCloseArchive");
		(void*)Rar.ReadHeader		= (void*)GetProcAddress(HUnrar, "RARReadHeader");
		(void*)Rar.ProcessFile		= (void*)GetProcAddress(HUnrar, "RARProcessFile");
		(void*)Rar.SetCallback		= (void*)GetProcAddress(HUnrar, "RARSetCallback");
		(void*)Rar.GetDllVersion	= (void*)GetProcAddress(HUnrar, "RARGetDllVersion");
	}

	WavInit();
	WavReset();

//	try
//	{
		Application->Initialize();
		Application->Title = "SPCTool";
		Application->CreateForm(__classid(TFrmMain), &FrmMain);
		Application->CreateForm(__classid(TFrmDebug), &FrmDebug);
		Application->CreateForm(__classid(TFrmTag), &FrmTag);
		Application->Run();
//	}
//	catch (Exception &exception)
//	{
//		Application->ShowException(&exception);
//	}

	WavQuit();
	if (HUnrar) FreeLibrary(HUnrar);
	FreeLibrary(HAPU);

	return 0;
}


//**************************************************************************************************
void __fastcall GetFileName(s8 *fn, FileInfo *pFile, s8 *ext)
{
	s8	*s,*d;


	strcpy(fn, pFile->id6.file);

	if (pFile->flags & F_ARCHIVE)
	{
		s = ScanStrR(fn, '\\');
		s[-1] = 0;
		strcpy(ScanStrR(s, '.'), ext);
		strcpy(ScanStrR(fn, '\\'), s);
	}
	else
	{
		s = ScanStrR(fn, '\\');
		d = ScanStrR(s, '.');

		if (s == d)
		{
			d = ::StrEnd(s);
			*d++ = '.';
		}

		strcpy(d, ext);
	}
}


//**************************************************************************************************
u32 CfgDSP::Opts()
{
	u32	i = 0;

	if (analog) i |= OPT_ANALOG;
	if (oldSmp) i |= OPT_OLDSMP;
	if (surround) i |= OPT_SURND;
	if (reverse) i |= OPT_REVERSE;
	if (disEcho) i |= OPT_NOECHO;
	if (filter) i |= OPT_FILTER;

	return i;
}

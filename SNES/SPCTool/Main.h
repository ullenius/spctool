/***************************************************************************************************
* Form: Main                                                                                       *
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

#ifndef	MainH
#define	MainH

#include	<Classes.hpp>
#include	<Controls.hpp>
#include	<StdCtrls.hpp>
#include	<Forms.hpp>
#include	<ComCtrls.hpp>
#include	<ExtCtrls.hpp>
#include	<Grids.hpp>
#include	<Menus.hpp>
#include	<Dialogs.hpp>
#include	<Buttons.hpp>
#include	<Graphics.hpp>
#include	<ImgList.hpp>

typedef enum {OT_PATH, OT_DIALOG, OT_DROP, OT_RAR, OT_FILE} OpenType;

typedef enum {ST_STOP, ST_RUN, ST_HALT, ST_PLAY} EmuState;

class TOpenDlgEx : public TOpenDialog
{
private:
	void	__fastcall WndProc(Messages::TMessage &msg);

public:
	s8		path[256];
	
			__fastcall TOpenDlgEx(TComponent* owner);
	b8		__fastcall Execute();
};

class TFrmMain : public TForm
{
__published:	// IDE-managed Components
	//Menus ------------------------------------
	TMainMenu	*mainMenu;

	TMenuItem	*mnuFile;
	TMenuItem	*itmOpen;
	TMenuItem	*itmClose;
	TMenuItem	*itmCloseA;
	TMenuItem	*sepF1;
	TMenuItem	*itmSave;
	TMenuItem	*itmSaveAs;
	TMenuItem	*itmSaveA;
	TMenuItem	*sepF2;
	TMenuItem	*itmFile1;
	TMenuItem	*itmFile2;
	TMenuItem	*itmFile3;
	TMenuItem	*itmFile4;
	TMenuItem	*itmFile5;
	TMenuItem	*itmFile6;
	TMenuItem	*itmFile7;
	TMenuItem	*itmFile8;
	TMenuItem	*itmFile9;
	TMenuItem	*sepF3;
	TMenuItem	*itmExit;

	TMenuItem	*mnuEmu;
	TMenuItem	*itmRun;
	TMenuItem	*itmHalt;
	TMenuItem	*itmReset;
	TMenuItem	*itmAbort;
	TMenuItem	*sepE1;
	TMenuItem	*itmTimer;
	TMenuItem	*sepE2;
	TMenuItem	*itmDebug;
	TMenuItem	*itmDSP;

	TMenuItem	*mnuSource;
	TMenuItem	*itmImport;
	TMenuItem	*itmExport;
	TMenuItem	*itmMute;
	TMenuItem	*itmSilence;
	TMenuItem	*sepS1;
	TMenuItem	*itmSetOpt;
	TMenuItem *itmSaveInfo;
	TMenuItem	*sepS2;
	TMenuItem	*itmRescan;

	TMenuItem	*mnuTools;
	TMenuItem	*itmEdit;
	TMenuItem	*itmWAV;
	TMenuItem	*itmMIDI;
	TMenuItem	*itmROM;
	TMenuItem	*itmRename;
	TMenuItem	*sepT1;
	TMenuItem	*itmConfig;

	TMenuItem	*mnuHelp;
	TMenuItem	*itmHelp;
	TMenuItem	*sepH1;
	TMenuItem	*itmAbout;

	TPopupMenu	*popTime;
	TMenuItem	*itmTSetCur;
	TMenuItem	*itmSetPos;

	TPopupMenu	*popFile;
	TMenuItem	*itmFSong;
	TMenuItem	*itmFGame;
	TMenuItem	*itmFArtist;
	TMenuItem	*itmFPub;
	TMenuItem	*itmFOST;
	TMenuItem	*itmFTrack;
	TMenuItem	*itmFDumper;
	TMenuItem	*itmFDate;
	TMenuItem	*itmFEmu;
	TMenuItem	*itmFCmnt;
	TMenuItem	*itmFLen;

	TPopupMenu	*popSrc;
	TMenuItem	*itmSOptions;
	TMenuItem	*itmSShow;
	TMenuItem	*itmSMute;
	TMenuItem	*itmSSilence;

	//ID666 Tag --------------------------------
	TPanel		*pnlTag;
	TLabel		*lblSongT;
	TLabel		*lblGameT;
	TLabel		*lblArtistT;
	TLabel		*lblPubT;
	TLabel		*lblOSTT;
	TLabel		*lblDiscT;
	TLabel		*lblTrackT;
	TLabel		*lblDumperT;
	TLabel		*lblEmuT;
	TLabel		*lblDateT;
	TLabel		*lblCmntT;
	TLabel		*lblSong;
	TLabel		*lblGame;
	TLabel		*lblArtist;
	TLabel		*lblCopy;
	TLabel		*lblPub;
	TLabel		*lblOST;
	TLabel		*lblDisc;
	TLabel		*lblTrack;
	TLabel		*lblDumper;
	TLabel		*lblDate;
	TLabel	   	*lblEmu;
	TMemo		*mmoCmnts;
	TBevel		*bvlTag;

	//Buttons ----------------------------------
	TPaintBox	*pbReset;
	TPaintBox	*pbRun;
	TPaintBox	*pbHalt;
	TPaintBox	*pbAbort;
	TPaintBox	*pbFF;
	TPaintBox	*pbUpload;
	TPaintBox	*pbOpen;
	TPaintBox	*pbSave;
	TPaintBox	*pbWAV;
	TPaintBox	*pbMIDI;
	TPaintBox	*pbTimer;

	TPaintBox	*pbPlay;
	TPaintBox	*pbStop;
	TPaintBox	*pbImport;
	TPaintBox	*pbExport;
	TPaintBox	*pbZoom;
	TPaintBox *pbTime;

	TListView	*lvwFile;
	TSaveDialog	*dlgSave;
	TOpenDialog	*dlgImport;
	TSaveDialog	*dlgExport;
	TPanel *pnlSrc;
	TLabel *lblTRate;
	TLabel *lblTType;
	TLabel *lblTProg;
	TLabel *lblType;
	TBevel *bvlSep;
	TTrackBar *trkAmp;
	TLabel *lblCLeft;
	TLabel *lblCRight;
	TLabel *lblCMode;
	TLabel *lblCEnv;
	TLabel *lblCSrc;
	TLabel *lblCRate;
	TLabel *lblCSw;
	TLabel *lblRate;
	TLabel *lblProg;
	TBevel *bvlTime;
	TLabel *lblTBank;
	TLabel *lblBank;
	TEdit *txtIn0;
	TEdit *txtIn1;
	TEdit *txtIn2;
	TEdit *txtIn3;
	TEdit *txtOut0;
	TEdit *txtOut1;
	TEdit *txtOut2;
	TEdit *txtOut3;
	TLabel *lblIn;
	TLabel *lblOut;
	TLabel *lblTADSR;
	TLabel *lblADSR;
	TLabel *lblTCRC;
	TLabel *lblCRC;
	TTrackBar *trkSpeed;
	TTrackBar *trkPitch;
	TLabel *lblSpeed;
	TLabel *lblPitch;
	TImage *Image1;
	TImage *Image2;
	TPageControl *pgcSrc;
	TTabSheet *tabSrc;
	TTabSheet *tabInfo;
	TListView *lvwSrc;
	TSaveDialog *dlgSaveInfo;
	TListView *lvwInfo;
	TMenuItem *itmLoadInfo;
	TOpenDialog *dlgOpenInfo;
	TPanel *pnlMeter;
	TPaintBox *pbMeter;
	TLabel *lblVoice0;
	TLabel *lblVoice1;
	TLabel *lblVoice2;
	TLabel *lblVoice3;
	TLabel *lblVoice4;
	TLabel *lblVoice5;
	TLabel *lblVoice6;
	TLabel *lblVoice7;
	TLabel *lblChnL;
	TLabel *lblChnR;
	TPanel *pnlWave;
	TScrollBar *barWave;
	TPaintBox *pbWave;
	TPanel *pnlVol;
	TLabel *lblVolL0;
	TLabel *lblVolR0;
	TLabel *lblVolL1;
	TLabel *lblVolR1;
	TLabel *lblVolL2;
	TLabel *lblVolR2;
	TLabel *lblVolL3;
	TLabel *lblVolR3;
	TLabel *lblVolL4;
	TLabel *lblVolR4;
	TLabel *lblVolL5;
	TLabel *lblVolR5;
	TLabel *lblVolL6;
	TLabel *lblVolR6;
	TLabel *lblVolL7;
	TLabel *lblVolR7;
	TLabel *lblMVolL;
	TLabel *lblEVolL;
	TLabel *lblMVolR;
	TLabel *lblEVolR;
	TPanel *pnlVoice;
	TLabel *lblTotal;
	TLabel *lblTime;
	TLabel *lblAmpT;
	TLabel *lblAmp;
	TLabel *lblTimeSet;
	TLabel *lblSw7;
	TLabel *lblRate7;
	TLabel *lblSrc7;
	TLabel *lblEnv7;
	TLabel *lblMode7;
	TLabel *lblMode6;
	TLabel *lblEnv6;
	TLabel *lblSrc6;
	TLabel *lblRate6;
	TLabel *lblSw6;
	TLabel *lblSw5;
	TLabel *lblRate5;
	TLabel *lblSrc5;
	TLabel *lblEnv5;
	TLabel *lblMode5;
	TLabel *lblMode4;
	TLabel *lblEnv4;
	TLabel *lblSrc4;
	TLabel *lblRate4;
	TLabel *lblSw4;
	TLabel *lblSw3;
	TLabel *lblRate3;
	TLabel *lblSrc3;
	TLabel *lblEnv3;
	TLabel *lblMode3;
	TLabel *lblMode2;
	TLabel *lblEnv2;
	TLabel *lblSrc2;
	TLabel *lblRate2;
	TLabel *lblSw2;
	TLabel *lblSw1;
	TLabel *lblRate1;
	TLabel *lblSrc1;
	TLabel *lblEnv1;
	TLabel *lblMode1;
	TLabel *lblMode0;
	TLabel *lblEnv0;
	TLabel *lblSrc0;
	TLabel *lblRate0;
	TLabel *lblSw0;
	TMenuItem *itmSHide;
	TMenuItem *itmProf;

	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall FormCloseQuery(TObject *sender, bool &canClose);
	void	__fastcall FormDestroy(TObject *sender);
	void	__fastcall itmOpenClick(TObject *sender);
	void	__fastcall itmCloseClick(TObject *sender);
	void	__fastcall itmCloseAClick(TObject *sender);
	void	__fastcall itmSaveClick(TObject *sender);
	void	__fastcall itmSaveAsClick(TObject *sender);
	void	__fastcall itmSaveAClick(TObject *sender);
	void	__fastcall itmConfigClick(TObject *sender);
	void	__fastcall itmExitClick(TObject *sender);
	void	__fastcall itmRunClick(TObject *sender);
	void	__fastcall itmHaltClick(TObject *sender);
	void	__fastcall itmResetClick(TObject *sender);
	void	__fastcall itmAbortClick(TObject *sender);
	void	__fastcall itmWAVClick(TObject *sender);
	void	__fastcall itmTimerClick(TObject *sender);
	void	__fastcall itmDebugClick(TObject *sender);
	void	__fastcall itmDSPClick(TObject *sender);
	void	__fastcall itmImportClick(TObject *sender);
	void	__fastcall itmExportClick(TObject *sender);
	void	__fastcall itmSetOptClick(TObject *sender);
	void	__fastcall itmEditClick(TObject *sender);
	void	__fastcall itmAboutClick(TObject *sender);

	void	__fastcall lblVoiceMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y);
	void	__fastcall lvwSrcColumnClick(TObject *sender, TListColumn *col);
	void	__fastcall lvwFileSelectItem(TObject *sender, TListItem *item, bool selected);
	void	__fastcall lvwFileEdited(TObject *sender, TListItem *item, AnsiString &s);
	void	__fastcall lvwSrcSelectItem(TObject *sender, TListItem *Item, bool selected);
	void	__fastcall BtnMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y);
	void	__fastcall BtnMouseUp(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y);
	void	__fastcall pbZoomClick(TObject *sender);
	void	__fastcall barWaveChange(TObject *sender);
	void	__fastcall pbBtnPaint(TObject *sender);
	void	__fastcall trkAmpChange(TObject *sender);
	void	__fastcall lvwSrcExit(TObject *sender);
	void	__fastcall lblTimeClick(TObject *sender);
	void	__fastcall pbTimerClick(TObject *sender);
	void	__fastcall itmRenameClick(TObject *sender);
	void	__fastcall itmFileClick(TObject *sender);
	void	__fastcall lvwSrcCustomDrawItem(TCustomListView *sender, TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
	void	__fastcall pbStopClick(TObject *sender);
	void	__fastcall pbPlayClick(TObject *sender);
	void	__fastcall itmSilenceClick(TObject *sender);
	void	__fastcall itmSShowClick(TObject *sender);
	void	__fastcall popSrcPopup(TObject *sender);
	void	__fastcall lvwSrcDblClick(TObject *sender);
	void	__fastcall itmROMClick(TObject *sender);
	void	__fastcall txtInChange(TObject *sender);
	void	__fastcall trkSpeedChange(TObject *sender);
	void	__fastcall trkPitchChange(TObject *sender);
	void	__fastcall itmMuteClick(TObject *sender);
	void	__fastcall dlgOpenShow(TObject *sender);
	void	__fastcall pbTimeMouseMove(TObject *sender, TShiftState Shift, int X, int Y);
	void	__fastcall pbTimeMouseDown(TObject *sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void	__fastcall popTimePopup(TObject *sender);
	void	__fastcall itmTSetCurClick(TObject *sender);
	void	__fastcall itmSetPosClick(TObject *sender);
	void	__fastcall itmHelpClick(TObject *sender);
	void	__fastcall lvwFileColumnClick(TObject *sender, TListColumn *Column);
	void	__fastcall lvwFileColumnRightClick(TObject *sender, TListColumn *Column, const TPoint &Point);
	void	__fastcall itmFClick(TObject *sender);
	void	__fastcall lvwFileResize(TObject *sender);
	void	__fastcall lvwFileEditing(TObject *sender, TListItem *Item, bool &AllowEdit);
	void	__fastcall itmMIDIClick(TObject *sender);
	void	__fastcall pbUploadClick(TObject *Sender);
	void	__fastcall itmSaveInfoClick(TObject *Sender);
	void	__fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void	__fastcall pgcSrcChange(TObject *Sender);
	void	__fastcall itmLoadInfoClick(TObject *Sender);
	void __fastcall pbMeterPaint(TObject *Sender);
	void __fastcall pbWavePaint(TObject *Sender);
	void __fastcall pbWaveMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall pbTimePaint(TObject *Sender);
	void __fastcall pnlTagResize(TObject *Sender);
	void __fastcall itmSHideClick(TObject *Sender);
	void __fastcall itmProfClick(TObject *Sender);

private:	// User declarations
	TOpenDlgEx	*dlgOpen;
	TWndMethod	oldBevelProc;

	void	__fastcall CreateParams(TCreateParams &params);
	void	__fastcall BevelProc(TMessage &msg);
	void	__fastcall WMDropFiles(TMessage &msg);

#pragma	warn -8027								//Turn off warning produced by BEGIN_MESSAGE_MAP
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_DROPFILES, TMessage, WMDropFiles)
END_MESSAGE_MAP(TForm)
#pragma	warn .8027

	void	__fastcall RecentFiles();
	void	__fastcall AddRecent(s8 *fn);
	b8		__fastcall OpenFiles(OpenType type, s8 *fn);
	void	__fastcall UpdateFile(TListItem *pItem);
	void	__fastcall DisableAll();
	void	__fastcall EnableItem();
	void	__fastcall EnableSrcItems();
	void	__fastcall FileModified(u8 flag);
	void	__fastcall BtnEnabled(TPaintBox *pb, b8 state);
	void	__fastcall PrintSrcInfo();
	void	__fastcall PlayNote(s32 octave, s32 note);
	void	__fastcall StopNote();
	void	__fastcall SetState(s32 s);
	bool	__fastcall WantChildKey(Controls::TControl* child, Messages::TMessage &msg);
	void	__fastcall SaveSrcPos();
	void	__fastcall FillROMQueue();
	void	__fastcall ScopeClick(b8 enable);
	void	__fastcall pbTimeClearMarker(TCanvas *pCanvas);


public:		// User declarations
			__fastcall TFrmMain(TComponent* Owner);
	void	__fastcall PrintID6(ID666 *pID6);
	void	__fastcall PrintTimer(u32 time);
	void	__fastcall DrawStuff();
	void	__fastcall SetTrkAmp(u32 amp);

	friend void	__fastcall HotKey(WORD key, TShiftState shift);
	friend void	__fastcall SeekSong(u32 time);
};


//**************************************************************************************************

extern PACKAGE TFrmMain *FrmMain;

#endif

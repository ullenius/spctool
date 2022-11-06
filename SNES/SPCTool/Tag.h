//**************************************************************************************************

#ifndef EditH
#define EditH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

typedef enum
{
	FT_TEXT,									//Control is a TEdit
	FT_MEMO,									//   "  "    a TMemo
	FT_COMBO,									//   "  "    a TComboBox
	FT_MUTE,									//   "  "    mute checkboxes
	FT_INTRO,									//   "  "    introduction text boxes
	FT_LOOP,									//   "  "    loop text boxes
	FT_END,										//   "  "    end text boxes
	FT_FADE										//   "  "    fade text boxes
} FieldType;

typedef struct
{
	TPaintBox	*pb;							//-> Update button
	TControl	*control;						//-> Component
	TLabel		*label;							//-> Label in standard ID666 tag
	b8			update;							//Field should be updated
	FieldType	type;							//Type of component
	u8			_r[2];
} ID6Field;

typedef enum
{
	EN_SONG,
	EN_GAME,
	EN_ARTIST,
	EN_PUB,
	EN_COPY,
	EN_OST,
	EN_DISC,
	EN_TRACK,
	EN_CMNT,
	EN_DUMPER,
	EN_DATE,
	EN_EMU,
	EN_MUTE,
	EN_AMP,
	EN_INTRO,
	EN_LOOP,
	EN_END,
	EN_FADE,
	EN_TOTAL
} UpdateField;


class TFrmTag : public TForm
{
__published:	// IDE-managed Components
	TLabel		*lblSong;
	TLabel		*lblGame;
	TLabel		*lblArtist;
	TLabel		*lblPub;
	TLabel		*lblCopy;
	TLabel		*lblOST;
	TLabel		*lblDisc;
	TLabel		*lblTrack;
	TLabel		*lblCmnt;
	TLabel		*lblDumper;
	TLabel		*lblDate;
	TLabel		*lblEmu;
	TLabel		*lblMute;
	TLabel		*lblMute1;
	TLabel		*lblMute8;
	TLabel		*lblAmp;
	TLabel		*lblIntro;
	TLabel		*lblLoop;
	TLabel		*lblEnd;
	TLabel		*lblFade;
	TLabel		*lblIntroSep;
	TLabel		*lblLoopSep;
	TLabel		*lblLoopX;
	TLabel		*lblEndSep;

	TPopupMenu	*popDiff;
	TPaintBox	*pbSongD;
	TPaintBox	*pbGameD;
	TPaintBox	*pbArtistD;
	TPaintBox	*pbPubD;
	TPaintBox	*pbOSTD;
	TPaintBox	*pbDumperD;

	TPaintBox	*pbSong;
	TPaintBox	*pbGame;
	TPaintBox	*pbArtist;
	TPaintBox	*pbPub;
	TPaintBox	*pbCopy;
	TPaintBox	*pbOST;
	TPaintBox	*pbDisc;
	TPaintBox	*pbTrack;
	TPaintBox	*pbCmnt;
	TPaintBox	*pbDumper;
	TPaintBox	*pbDate;
	TPaintBox	*pbEmu;
	TPaintBox	*pbMute;
	TPaintBox	*pbAmp;
	TPaintBox	*pbIntro;
	TPaintBox	*pbLoop;
	TPaintBox	*pbEnd;
	TPaintBox	*pbFade;

	TEdit		*txtSong;
	TEdit		*txtGame;
	TEdit		*txtArtist;
	TEdit		*txtPub;
	TEdit		*txtCopy;
	TEdit		*txtOST;
	TEdit		*txtDisc;
	TEdit		*txtTrack;
	TMemo		*mmoCmnt;
	TEdit		*txtDump;
	TEdit		*txtDate;
	TComboBox	*cboEmu;
	TCheckBox	*chkMute1;
	TCheckBox	*chkMute2;
	TCheckBox	*chkMute3;
	TCheckBox	*chkMute4;
	TCheckBox	*chkMute5;
	TCheckBox	*chkMute6;
	TCheckBox	*chkMute7;
	TCheckBox	*chkMute8;
	TEdit		*txtAmp;
	TButton		*btnIntro;
	TButton		*btnLoop;
	TButton		*btnEnd;
	TButton		*btnFade;
	TEdit		*txtIntroMin;
	TEdit		*txtIntroSec;
	TEdit		*txtLoopMin;
	TEdit		*txtLoopSec;
	TEdit		*txtLoopNum;
	TEdit		*txtEndMin;
	TEdit		*txtEndSec;
	TEdit		*txtFadeSec;


	TBevel		*bvlTime;

	TLabel		*lblTime;
	TLabel		*lblTimeSep;

	TButton		*btnTime;
	TEdit		*txtTimeMin;
	TEdit		*txtTimeSec;
	TCheckBox	*chkExt;
	TCheckBox	*chkBin;

	TEdit		*txtMuteFocus;

	TButton		*btnID6;
	TButton		*btnOK;
	TButton		*btnCancel;


	TBevel		*bvlID6;

	TLabel		*lblISongT;
	TLabel		*lblIGameT;
	TLabel		*lblIArtistT;
	TLabel		*lblICommentT;
	TLabel		*lblIDumperT;
	TLabel		*lblIDateT;
	TLabel		*lblIEmuT;
	TLabel		*lblISong2T;
	TLabel		*lblIFadeT;

	TLabel		*lblISong;
	TLabel		*lblIGame;
	TLabel		*lblIArtist;
	TLabel		*lblICmnt;
	TLabel		*lblIDumper;
	TLabel		*lblIDate;
	TLabel		*lblIEmu;
	TLabel		*lblISong2;
	TLabel		*lblIFade;


	void	__fastcall btnOKClick(TObject *sender);
	void	__fastcall chkExtClick(TObject *sender);
	void	__fastcall btnID6Click(TObject *sender);
	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall ID6Change(TObject *sender);
	void	__fastcall pbChkPaint(TObject *sender);
	void	__fastcall pbChkMouseDown(TObject *sender, TMouseButton btn, TShiftState shift, int x, int y);
	void	__fastcall pbChkMouseMove(TObject *sender, TShiftState shift, int x, int y);
	void	__fastcall FormKeyDown(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall FormKeyPress(TObject *sender, char &key);
	void	__fastcall FormKeyUp(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall txtMuteFocusKeyDown(TObject *sender, WORD &key, TShiftState shift);
	void	__fastcall txtMuteFocusEnter(TObject *sender);
	void	__fastcall txtMuteFocusExit(TObject *sender);
	void	__fastcall FormShow(TObject *sender);
	void	__fastcall txtCopyExit(TObject *sender);
	void	__fastcall txtAmpExit(TObject *sender);
	void	__fastcall txtDiscExit(TObject *sender);
	void	__fastcall txtTrackExit(TObject *sender);
	void	__fastcall txtDateEnter(TObject *sender);
	void	__fastcall txtDateKeyPress(TObject *sender, char &key);
	void	__fastcall txtDateExit(TObject *sender);
	void	__fastcall btnTimeClick(TObject *sender);
	void	__fastcall txtSecExit(TObject *sender);
	void	__fastcall txtLoopNumExit(TObject *sender);
	void	__fastcall txtMinExit(TObject *sender);
	void	__fastcall FormPaint(TObject *sender);
	void	__fastcall pbDiffClick(TObject *sender);
	void	__fastcall itmClick(TObject *sender);
	void	__fastcall pbDiffPaint(TObject *sender);
	void	__fastcall pbDiffMouseDown(TObject *sender, TMouseButton button, TShiftState shift, int x, int y);
	void	__fastcall pbDiffMouseUp(TObject *sender, TMouseButton button, TShiftState shift, int x, int y);
	void	__fastcall FormClose(TObject *sender, TCloseAction &action);
	void	__fastcall btnIntroClick(TObject *sender);
	void	__fastcall btnLoopClick(TObject *sender);
	void	__fastcall btnEndClick(TObject *sender);
	void	__fastcall btnFadeClick(TObject *sender);
	void	__fastcall FormDestroy(TObject *Sender);

private:
	void	__fastcall pbChkState(ID6Field *pb);
	void	__fastcall chkMuteFocus(b8 draw);
//	b8		__fastcall StrDate(s8 *str, A2Date &date);
	void	__fastcall TxtPaint(TEdit *txt, TPaintBox *pb);

public:
	ID666	id6,*pID6;							//ID666 tag to display in editor, -> original tag
	u32		update;								//Flags for fields to be updated

	TStringList	*strSong,*strGame,*strArtist,*strPub,*strOST,*strDumper;

			__fastcall TFrmTag(TComponent* Owner);
	void	__fastcall ShowBtn(b8 enable);
	void	__fastcall PrintTotal();
};

void __fastcall PrintTime(s8 *str, u32 ticks);

extern PACKAGE TFrmTag *FrmTag;



#endif

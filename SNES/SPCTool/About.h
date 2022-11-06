//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmAbout : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblVersion;
	TLabel *lblCopy;
	TImage *imgBackground;
	TLabel *lblClose;
	TLabel *lblDebugT;
	TLabel *lblHalfCT;
	TLabel *lblIPLWT;
	TLabel *lblSpeedT;
	TLabel *lblDebug;
	TLabel *lblHalfC;
	TLabel *lblIPLW;
	TLabel *lblSpeed;
	TLabel *lblPPMT;
	TLabel *lblProfT;
	TLabel *lblStereoT;
	TLabel *lblPPM;
	TLabel *lblProf;
	TLabel *lblStereo;
	TLabel *lblURL;
	TLabel *lblAPUVer;
	TLabel *lblDSPIntT;
	TLabel *lblDSPInt;
	void __fastcall lblCloseClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall lblURLClick(TObject *Sender);
	void __fastcall lblURLMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall imgBackgroundMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall lblAPUVerClick(TObject *Sender);
private:	// User declarations
	TCursor	oldCursor;

public:		// User declarations
	__fastcall TFrmAbout(TComponent* Owner);
};

extern PACKAGE TFrmAbout *FrmAbout;

#endif

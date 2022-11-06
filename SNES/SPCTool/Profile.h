//---------------------------------------------------------------------------

#ifndef ProfileH
#define ProfileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmProf : public TForm
{
__published:	// IDE-managed Components
	TTabControl *tabProf;
	TListView *lvwProf;
	TButton *btnReset;
	TButton *btnResetAll;
	void __fastcall tabProfChange(TObject *Sender);
	void __fastcall lvwProfColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall btnResetClick(TObject *Sender);
	void __fastcall btnResetAllClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall lvwProfData(TObject *Sender, TListItem *Item);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	u8	instOrder[256];
	u8	branchOrder[28];
	u8	funcOrder[16];
	u8	dspOrder[128];

public:		// User declarations
	__fastcall TFrmProf(TComponent* Owner);
	void __fastcall UpdateProf();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmProf *FrmProf;
//---------------------------------------------------------------------------
#endif


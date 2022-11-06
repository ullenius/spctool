//**************************************************************************************************

#ifndef ProgressH
#define ProgressH

#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

class ProgBar
{
	friend	class TFrmProg;

private:
	void	DrawBox();
	void	DrawBar();
	void	Paint();

	f64		Total;								//Total amount for 100 percent of bar width
	TCanvas	*PCanvas;							//Canvas of TPaintBox used for drawing bar
	s32		Width,Height;						//Size of box (in pixels)
	s32		Length;								//Visible length of progress bar (in pixels)
	b8		Print;								//Bar's progress percentage should be printed

public:
			ProgBar(TPaintBox *p);
	void	Reset(f64 total);
	void	Set(f64 progress);
};


class TFrmProg : public TForm
{
__published:	// IDE-managed Components
	TLabel		*lblInfo;
	TLabel		*lblStatus;
	TLabel		*lblProgress;
	TPaintBox	*pbCurrent;
	TPaintBox	*pbTotal;
	TButton		*btnCancel;

	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall FormDestroy(TObject *sender);
	void	__fastcall FormShow(TObject *sender);
	void	__fastcall btnCancelClick(TObject *sender);
	void	__fastcall pbPaint(TObject *sender);

private:

public:
	ProgBar	*BarCurrent,*BarTotal;

	b8		ShowTotal;
	b8		AutoClose;
	b8		Cancel;

			__fastcall TFrmProg(TComponent* Owner);
};

extern PACKAGE TFrmProg *FrmProg;
#endif


//**************************************************************************************************

#ifndef MapH
#define MapH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

extern u8	MemMap[65536];


class TFrmMap : public TForm
{
__published:	// IDE-managed Components
	TPanel		*pnlMap;
	TPaintBox	*pbMap;

	TPanel		*pnlKey;
	TLabel		*lblCode;
	TLabel		*lblData;
	TLabel		*lblDir;
	TLabel		*lblSrc;
	TLabel		*lblEcho;
	TLabel		*lblIPL;
	TLabel		*lblFunc;
	TLabel		*lblFF;

	TLabel		*lblTOfs;
	TLabel		*lblOfs;
	TLabel		*lblMap;

	TButton		*btnMap;
	TButton		*btnClose;

	void	__fastcall FormShow(TObject *sender);
	void	__fastcall btnMapClick(TObject *sender);
	void	__fastcall pbMapMouseMove(TObject *sender, TShiftState shift, int x, int y);
	void	__fastcall pbMapPaint(TObject *sender);

private:	// User declarations

public:		// User declarations
	APUFile	*pState;

			__fastcall TFrmMap(TComponent* owner);
};

extern PACKAGE TFrmMap *FrmMap;
#endif


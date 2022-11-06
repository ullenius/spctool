//---------------------------------------------------------------------------
#ifndef DebugH
#define DebugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

typedef enum									//Thread command
{
	CMD_WAIT,									//Wait for signaling event
	CMD_STEP,									//Step through next instruction
	CMD_OUT,									//Step out of function
	CMD_OVER,									//Step over next instruction
	CMD_RUN,									//Execute
	CMD_QUIT,									//Don't break execution
} DebugCmd;

typedef struct
{
	s8	mnemonic[6];							//Instruction mnemonic
	u8	op1;									//Operand 1 encoding (see above)
	u8	op2;									//Operand 2 encoding
} Instruction;

extern const Instruction	InsList[256];


//---------------------------------------------------------------------------
class TFrmDebug : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlRAM;
	TLabel *lblRAMX0;
	TLabel *lblRAMX1;
	TLabel *lblRAMX2;
	TLabel *lblRAMX3;
	TLabel *lblRAMX4;
	TLabel *lblRAMX5;
	TLabel *lblRAMX6;
	TLabel *lblRAMX7;
	TLabel *lblRAMX8;
	TLabel *lblRAMX9;
	TLabel *lblRAMXA;
	TLabel *lblRAMXB;
	TLabel *lblRAMXC;
	TLabel *lblRAMXD;
	TLabel *lblRAMXE;
	TLabel *lblRAMXF;
	TLabel *lblRAMY0;
	TLabel *lblRAMY1;
	TLabel *lblRAMY2;
	TLabel *lblRAMY3;
	TLabel *lblRAMY4;
	TLabel *lblRAMY5;
	TLabel *lblRAMY6;
	TLabel *lblRAMY7;
	TLabel *lblRAMY8;
	TLabel *lblRAMY9;
	TLabel *lblRAMYA;
	TLabel *lblRAMYB;
	TLabel *lblRAMYC;
	TLabel *lblRAMYD;
	TLabel *lblRAMYE;
	TLabel *lblRAMYF;

	TPanel *pnlDSP;
	TLabel *lblDSPY0;
	TLabel *lblDSPY1;
	TLabel *lblDSPY2;
	TLabel *lblDSPY3;
	TLabel *lblDSPY4;
	TLabel *lblDSPY5;
	TLabel *lblDSPY6;
	TLabel *lblDSPY7;
	TLabel *lblDSPX0;
	TLabel *lblDSPX1;
	TLabel *lblDSPX2;
	TLabel *lblDSPX4;
	TLabel *lblDSPX6;
	TLabel *lblDSPX7;
	TLabel *lblDSPX8;
	TLabel *lblDSPX9;
	TLabel *lblDSPXA;
	TLabel *lblDSPXB;
	TLabel *lblDSPXC;
	TLabel *lblDSPXD;
	TLabel *lblDSPXE;
	TLabel *lblDSPXF;
	TLabel *lblDSPX3;
	TLabel *lblRAM;
	TLabel *lblDSP;
	TLabel *lblSPC;
	TLabel *lblDisasm;
	TStatusBar *Status;
	TListBox *lstDisasm;
	TPanel *pnlSPC;
	TMainMenu *mnuMain;
	TMenuItem *mnuOther;
	TMenuItem *mnuDebug;
	TMenuItem *mnuHelp;
	TMenuItem *itmExit;
	TMenuItem *itmBrkAddr;
	TMenuItem *itmBrkData;
	TMenuItem *itmRun;
	TMenuItem *sepRun1;
	TMenuItem *itmOver;
	TMenuItem *itmInto;
	TMenuItem *sepRun2;
	TMenuItem *itmReset;
	TMenuItem *itmHelp;
	TMenuItem *itmLog;
	TScrollBar *barRAM;
	TLabel *lblPCT;
	TLabel *lblYAT;
	TLabel *lblXT;
	TLabel *lblSPT;
	TLabel *lblPSWT;
	TLabel *lblUpT;
	TPopupMenu *popRAM;
	TMenuItem *itmRAMDisasm;
	TMenuItem *itmRAMSetPC;
	TLabel *lblUp0;
	TLabel *lblUp1;
	TLabel *lblUp2;
	TLabel *lblCyc;
	TLabel *lblDSPX5;
	TMenuItem *itmOut;
	TMenuItem *itmStop;
	TMenuItem *itmCommit;
	TLabel *lblInT;
	TLabel *lblOutT;
	TMenuItem *itmRAMBreak;
	TMenuItem *itmRunTo;
	TMenuItem *sepO1;
	TMenuItem *Goto1;
	TLabel *lblOut0;
	TLabel *lblOut1;
	TLabel *lblOut2;
	TLabel *lblOut3;
	TMenuItem *itmGotoPC;
	TMenuItem *itmGotoSP;
	TButton *Button1;
	TListBox *lstBrkAddr;
	TListBox *lstBrkData;
	TLabel *lblBrkAddr;
	TLabel *lblBrkData;
	TMenuItem *sepRun3;
	TMenuItem *itmMap;
	TPopupMenu *popBreak;
	TMenuItem *itmBreak;
	TPaintBox *pbDSP;
	TPaintBox *pbRAM;
	TLabel *lblIn0;
	TLabel *lblIn1;
	TLabel *lblIn2;
	TLabel *lblIn3;
	TLabel *lblPC;
	TLabel *lblYA;
	TLabel *lblX;
	TLabel *lblSP;
	TLabel *lblPSW;
        TTimer *Timer1;
	void __fastcall FormCreate(TObject *sender);
	
	void __fastcall barRAMChange(TObject *sender);

	void __fastcall FormShow(TObject *sender);

	void __fastcall itmHelpClick(TObject *sender);
	void __fastcall itmExitClick(TObject *sender);
	void __fastcall itmRAMDisasmClick(TObject *sender);
	void __fastcall popRAMPopup(TObject *sender);
	void __fastcall itmRAMSetPCClick(TObject *sender);
	void __fastcall lblRAMXDblClick(TObject *sender);
	void __fastcall barRAMScroll(TObject *sender, TScrollCode ScrollCode,
          int &ScrollPos);
	void __fastcall FormClose(TObject *sender, TCloseAction &Action);
	void __fastcall itmIntoClick(TObject *sender);
	void __fastcall itmOverClick(TObject *sender);
	void __fastcall itmBrkDataClick(TObject *sender);
	void __fastcall itmRunClick(TObject *sender);
	void __fastcall itmStopClick(TObject *sender);
	void __fastcall lstDisasmDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall itmBrkAddrClick(TObject *sender);
	void __fastcall itmRunToClick(TObject *sender);
	void __fastcall itmGotoPCClick(TObject *sender);
	void __fastcall itmGotoSPClick(TObject *sender);
	void __fastcall itmOutClick(TObject *sender);
	void __fastcall lstBrkDataDblClick(TObject *sender);
	void __fastcall lstBrkAddrDblClick(TObject *sender);
	void __fastcall itmCommitClick(TObject *sender);
	void __fastcall itmMapClick(TObject *sender);
	void __fastcall itmRAMBreakClick(TObject *sender);
	void __fastcall itmBreakClick(TObject *sender);
	void __fastcall FormDestroy(TObject *sender);
	void __fastcall pbDSPPaint(TObject *sender);
	void __fastcall pbDSPMouseMove(TObject *sender, TShiftState Shift, int X,
          int Y);
	void __fastcall pbDSPDblClick(TObject *sender);
	void __fastcall pbRAMMouseMove(TObject *sender, TShiftState Shift, int X,
          int Y);
	void __fastcall pbRAMPaint(TObject *sender);
	void __fastcall pbRAMDblClick(TObject *sender);
	void __fastcall pbDSPMouseDown(TObject *sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall pbRAMMouseDown(TObject *sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall pnlDSPEnter(TObject *sender);
	void __fastcall pnlDSPExit(TObject *sender);
	void __fastcall pnlRAMEnter(TObject *sender);
	void __fastcall pnlRAMExit(TObject *sender);
	void __fastcall pbDSPClick(TObject *sender);
	void __fastcall pbRAMClick(TObject *sender);
	void __fastcall pnlSPCEnter(TObject *sender);
	void __fastcall pnlSPCExit(TObject *sender);
	void __fastcall lblSPCClick(TObject *sender);
	void __fastcall lblSPCMouseMove(TObject *sender, TShiftState Shift,
          int X, int Y);
	void __fastcall lblSPCMouseDown(TObject *sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);

private:	// User declarations
	b8		  	handled;
	AnsiString	comment;

	bool	__fastcall WantChildKey(Controls::TControl* child, Messages::TMessage &msg);
	void	__fastcall SetCmd(DebugCmd cmd);
	static void		   DebugSPCs(volatile u8 *pc, volatile u16 ya, volatile u8 x, volatile SPCFlags psw, volatile u8 *sp, volatile u32 cnt);
	void	__fastcall DebugSPC();
	void	__fastcall Run();
	void	__fastcall Stop();
	void	__fastcall Disassembly(u16 pc);
	u16		__fastcall Disassemble(AnsiString &line, u16 pc);

	void	__fastcall FlipPSW(u8 mask);
	void	__fastcall SetRAMPos(u32 p);
	void	__fastcall RepaintCell(u32 r);
	void	__fastcall RestoreReg(TEdit *pTxt);
	b8		__fastcall IsVisible(u16 i);
	void	__fastcall Watch(u8 *p);
	void	__fastcall SetDisasm(u16 addr);
	void	__fastcall SetPC(u16 addr);

	static s32 MsgProcSPC(HWND hwnd, u32 msg, s32 w, s32 l);
	void	__fastcall KeyDownSPC(s32 key);
	void	__fastcall SelectSPC();

	static s32 MsgProcRAM(HWND hwnd, u32 msg, s32 w, s32 l);
	void	__fastcall KeyDownRAM(s32 key);
	void	__fastcall SetAPURAM(u16 addr, u8 val);
	void	__fastcall PrintRAM(u32 x, u32 y);
	void	__fastcall SelectRAM();
	void	__fastcall lblRAMYUpdate();

	static s32 MsgProcDSP(HWND hwnd, u32 msg, s32 w, s32 l);
	void	__fastcall KeyDownDSP(s32 key);
	void	__fastcall SetDSPReg(u8 reg, u8 val);
	void	__fastcall PrintDSP(u8 r, u8 v);
	void	__fastcall SelectDSP();

public:		// User declarations
	APUFile	*pState;
	b8		commit;

			__fastcall TFrmDebug(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmDebug *FrmDebug;
//---------------------------------------------------------------------------

s8*	__fastcall PrintDP(s8 *pStr, u8 a);

#endif

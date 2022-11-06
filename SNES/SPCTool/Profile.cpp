//**************************************************************************************************

#include	<vcl.h>
#include	"SPCTool.h"
#pragma hdrstop
#include	"Profile.h"
#include	"Assembly.h"


////////////////////////////////////////////////////////////////////////////////////////////////////

static const s8 Inst[256][16] = {
	"NOP",
	"TCALL 0",
	"SET1 dp.0",
	"BBS dp.0,rel",
	"OR A,dp",
	"OR A,!abs",
	"OR A,(X)",
	"OR A,[dp+X]",
	"OR A,#imm",
	"OR dp,dp",
	"OR1 C,mem.bit",
	"ASL dp",
	"ASL !abs",
	"PUSH PSW",
	"TSET1 !abs",
	"BRK",

	"BPL rel",
	"TCALL 1",
	"CLR1 dp.0",
	"BBC dp.0,rel",
	"OR A,dp+X",
	"OR A,!abs+X",
	"OR A,!abs+Y",
	"OR A,[dp]+Y",
	"OR dp,#imm",
	"OR (X),(Y)",
	"DECW dp",
	"ASL dp+X",
	"ASL A",
	"DEC X",
	"CMP X,!abs",
	"JMP [!abs+X]",

	"CLRP",
	"TCALL 2",
	"SET1 dp.1",
	"BBS dp.1,rel",
	"AND A,dp",
	"AND A,!abs",
	"AND A,(X)",
	"AND A,[dp+X]",
	"AND A,#imm",
	"AND dp,dp",
	"OR1 C,/mem.bit",
	"ROL dp",
	"ROL !abs",
	"PUSH A",
	"CBNE dp,rel",
	"BRA rel",

	"BMI rel",
	"TCALL 3",
	"CLR1 dp.1",
	"BBC dp.1,rel",
	"AND A,dp+X",
	"AND A,!abs+X",
	"AND A,!abs+Y",
	"AND A,[dp]+Y",
	"AND dp,#imm",
	"AND (X),(Y)",
	"INCW dp",
	"ROL dp+X",
	"ROL A",
	"INC X",
	"CMP X,dp",
	"CALL !abs",

	"SETP",
	"TCALL 4",
	"SET1 dp.2",
	"BBS dp.2,rel",
	"EOR A,dp",
	"EOR A,!abs",
	"EOR A,(X)",
	"EOR A,[dp+X]",
	"EOR A,#imm",
	"EOR dp,dp",
	"AND1 C,mem.bit",
	"LSR dp",
	"LSR !labs",
	"PUSH X",
	"TCLR1 !abs",
	"PCALL up",

	"BVC rel",
	"TCALL 5",
	"CLR1 dp.2",
	"BBC dp.2,rel",
	"EOR A,dp+X",
	"EOR A,!abs+X",
	"EOR A,!abs+Y",
	"EOR A,[dp]+Y",
	"EOR dp,#imm",
	"EOR (X),(Y)",
	"CMPW YA,dp",
	"LSR dp+X",
	"LSR A",
	"MOV X,A",
	"CMP Y,!abs",
	"JMP !abs",

	"CLRC",
	"TCALL 6",
	"SET1 dp.3",
	"BBS dp.3,rel",
	"CMP A,dp",
	"CMP A,!abs",
	"CMP A,(X)",
	"CMP A,[dp+X]",
	"CMP A,#imm",
	"CMP dp,dp",
	"AND1 C,/mem.bit",
	"ROR dp",
	"ROR !abs",
	"PUSH Y",
	"DBNZ dp,rel",
	"RET",

	"BVS rel",
	"TCALL 7",
	"CLR1 dp.3",
	"BBC dp.3,rel",
	"CMP A,dp+X",
	"CMP A,!abs+X",
	"CMP A,!abs+Y",
	"CMP A,[dp]+Y",
	"CMP dp,#imm",
	"CMP (X),(Y)",
	"ADDW YA,dp",
	"ROR dp+X",
	"ROR A",
	"MOV A,X",
	"CMP Y,dp",
	"RETI",

	"SETC",
	"TCALL 8",
	"SET1 dp.4",
	"BBS dp.4,rel",
	"ADC A,dp",
	"ADC A,!abs",
	"ADC A,(X)",
	"ADC A,[dp+X]",
	"ADC A,#imm",
	"ADC dp,dp",
	"EOR1 C,mem.bit",
	"DEC dp",
	"DEC !abs",
	"MOV Y,#imm",
	"POP PSW",
	"MOV dp,#imm",

	"BCC rel",
	"TCALL 9",
	"CLR1 dp.4",
	"BBC dp.4,rel",
	"ADC A,dp+X",
	"ADC A,!abs+X",
	"ADC A,!abs+Y",
	"ADC A,[dp]+Y",
	"ADC dp,#imm",
	"ADC (X),(Y)",
	"SUBW YA,dp",
	"DEC dp+X",
	"DEC A",
	"MOV X,SP",
	"DIV YA,X",
	"XCN A",

	"EI",
	"TCALL 10",
	"SET1 dp.5",
	"BBS dp.5,rel",
	"SBC A,dp",
	"SBC A,!abs",
	"SBC A,(X)",
	"SBC A,[dp+X]",
	"SBC A,#imm",
	"SBC dp,dp",
	"MOV1 C,mem.bit",
	"INC dp",
	"INC !abs",
	"CMP Y,#imm",
	"POP A",
	"MOV (X)+,A",

	"BCS rel",
	"TCALL 11",
	"CLR1 dp.5",
	"BBC dp.5,rel",
	"SBC A,dp+X",
	"SBC A,!abs+X",
	"SBC A,!abs+Y",
	"SBC A,[dp]+Y",
	"SBC dp,#imm",
	"SBC (X),(Y)",
	"MOVW YA,dp",
	"INC dp+X",
	"INC A",
	"MOV SP,X",
	"DAA A",
	"MOV A,(X)+",

	"DI",
	"TCALL 12",
	"SET1 dp.6",
	"BBS dp.6,rel",
	"MOV dp,A",
	"MOV !abs,A",
	"MOV (X),A",
	"MOV [dp+X],A",
	"CMP X,#imm",
	"MOV !abs,X",
	"MOV1 mem.bit,C",
	"MOV dp,Y",
	"MOV !abs,Y",
	"MOV X,#imm",
	"POP X",
	"MUL YA",

	"BNE rel",
	"TCALL 13",
	"CLR1 dp.6",
	"BBC dp.6,rel",
	"MOV dp+X,A",
	"MOV !abs+X,A",
	"MOV !abs+Y,A",
	"MOV [dp]+Y,A",
	"MOV dp,X",
	"MOV dp+Y,X",
	"MOVW dp,YA",
	"MOV dp+X,Y",
	"DEC Y",
	"MOV A,Y",
	"CBNE dp+X,rel",
	"DAS A",

	"CLRV",
	"TCALL 14",
	"SET1 dp.7",
	"BBS dp.7,rel",
	"MOV A,dp",
	"MOV A,!abs",
	"MOV A,(X)",
	"MOV A,[dp+X]",
	"MOV A,#imm",
	"MOV X,!abs",
	"NOT1 mem.bit",
	"MOV Y,dp",
	"MOV Y,!abs",
	"NOTC",
	"POP Y",
	"SLEEP",

	"BEQ rel",
	"TCALL 15",
	"CLR1 dp.7",
	"BBC dp.7,rel",
	"MOV A,dp+X",
	"MOV A,!abs+X",
	"MOV A,!abs+Y",
	"MOV A,[dp]+Y",
	"MOV X,dp",
	"MOV X,dp+Y",
	"MOV dp,dp",
	"MOV Y,dp+X",
	"INC Y",
	"MOV Y,A",
	"DBNZ Y,rel",
	"STOP"};

static const u8 BranchOp[28] = {
	0x10,0x30,0x50,0x70,0x90,0xB0,0xD0,0xF0,
	0x13,0x33,0x53,0x73,0x93,0xB3,0xD3,0xF3,
	0x03,0x23,0x43,0x63,0x83,0xA3,0xC3,0xE3,
	0x2E,0xDE,0xFE,0x6E};

static const s8 Branch[28][16] = {
	"BPL",
	"BMI",
	"BVC",
	"BVS",
	"BCC",
	"BCS",
	"BNE",
	"BEQ",
	"BBC.0",
	"BBC.1",
	"BBC.2",
	"BBC.3",
	"BBC.4",
	"BBC.5",
	"BBC.6",
	"BBC.7",
	"BBS.0",
	"BBS.1",
	"BBS.2",
	"BBS.3",
	"BBS.4",
	"BBS.5",
	"BBS.6",
	"BBS.7",
	"CBNE dp",
	"CBNE dp+X",
	"DBNZ Y",
	"DBNZ dp"};

static const s8 Func[16][16] = {
	"F0 Test",
	"F1 Control",
	"F2 DSP Addr",
	"F3 DSP Data",
	"F4 Port-0",
	"F5 Port-1",
	"F6 Port-2",
	"F7 Port-3",
	"F8 unused",
	"F9 unused",
	"FA Timer-0",
	"FB Timer-1",
	"FC Timer-2",
	"FD Counter-0",
	"FE Counter-1",
	"FF Counter-2"};

static const s8 DSPRegs[128][10] = {
	"VOL(L)-0",
	"VOL(R)-0",
	"P(L)-0",
	"P(H)-0",
	"SRCN-0",
	"ADSR1-0",
	"ADSR2-0",
	"GAIN-0",
	"ENVX-0",
	"OUTX-0",
	"(0A)",
	"(0B)",
	"MVOL(L)",
	"EFB",
	"(0E)",
	"C0",
	"VOL(L)-1",
	"VOL(R)-1",
	"P(L)-1",
	"P(H)-1",
	"SRCN-1",
	"ADSR1-1",
	"ADSR2-1",
	"GAIN-1",
	"ENVX-1",
	"OUTX-1",
	"(1A)",
	"(1B)",
	"MVOL(R)",
	"(1D)",
	"(1E)",
	"C1",
	"VOL(L)-2",
	"VOL(R)-2",
	"P(L)-2",
	"P(H)-2",
	"SRCN-2",
	"ADSR1-2",
	"ADSR2-2",
	"GAIN-2",
	"ENVX-2",
	"OUTX-2",
	"(2A)",
	"(2B)",
	"EVOL(L)",
	"PMON",
	"(2E)",
	"C2",
	"VOL(L)-3",
	"VOL(R)-3",
	"P(L)-3",
	"P(H)-3",
	"SRCN-3",
	"ADSR1-3",
	"ADSR2-3",
	"GAIN-3",
	"ENVX-3",
	"OUTX-3",
	"(3A)",
	"(3B)",
	"EVOL(R)",
	"NON",
	"(3E)",
	"C3",
	"VOL(L)-4",
	"VOL(R)-4",
	"P(L)-4",
	"P(H)-4",
	"SRCN-4",
	"ADSR1-4",
	"ADSR2-4",
	"GAIN-4",
	"ENVX-4",
	"OUTX-4",
	"(4A)",
	"(4B)",
	"KON",
	"EON",
	"(4E)",
	"C4",
	"VOL(L)-5",
	"VOL(R)-5",
	"P(L)-5",
	"P(H)-5",
	"SRCN-5",
	"ADSR1-5",
	"ADSR2-5",
	"GAIN-5",
	"ENVX-5",
	"OUTX-5",
	"(5A)",
	"(5B)",
	"KOF",
	"DIR",
	"(5E)",
	"C5",
	"VOL(L)-6",
	"VOL(R)-6",
	"P(L)-6",
	"P(H)-6",
	"SRCN-6",
	"ADSR1-6",
	"ADSR2-6",
	"GAIN-6",
	"ENVX-6",
	"OUTX-6",
	"(6A)",
	"(6B)",
	"FLG",
	"ESA",
	"(6E)",
	"C6",
	"VOL(L)-7",
	"VOL(R)-7",
	"P(L)-7",
	"P(H)-7",
	"SRCN-7",
	"ADSR1-7",
	"ADSR2-7",
	"GAIN-7",
	"ENVX-7",
	"OUTX-7",
	"(7A)",
	"(7B)",
	"ENDX",
	"EDL",
	"(7E)",
	"C7"};

#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmProf *FrmProf;


//**************************************************************************************************
__fastcall TFrmProf::TFrmProf(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmProf::FormCreate(TObject *Sender)
{
	u32	i;

	for (i=0; i<sizeof(instOrder); i++) instOrder[i] = i;
	for (i=0; i<sizeof(branchOrder); i++) branchOrder[i] = i;
	for (i=0; i<sizeof(funcOrder); i++) funcOrder[i] = i;
	for (i=0; i<sizeof(dspOrder); i++) dspOrder[i] = i;
}


//**************************************************************************************************
void __fastcall TFrmProf::FormShow(TObject *Sender)
{
	tabProfChange(NULL);
}


//**************************************************************************************************
void __fastcall TFrmProf::btnResetClick(TObject *Sender)
{
	switch(tabProf->TabIndex)
	{
	case(0):
		memset(Apu.profile->exec, 0, sizeof(Apu.profile->exec));
		break;

	case(1):
		memset(&Apu.profile->branch, 0, sizeof(Apu.profile->branch));
		break;

	case(2):
		memset(&Apu.profile->func, 0, sizeof(Apu.profile->func));
		break;

	case(3):
		memset(&Apu.profile->dsp, 0, sizeof(Apu.profile->dsp));
		break;

	case(4):
		memset(&Apu.profile->tsc, 0, sizeof(Apu.profile->tsc));
		Apu.profile->tsc.host.base =
		Apu.profile->tsc.apu.base =
		Apu.profile->tsc.spc700.base =
		Apu.profile->tsc.dsp.base = ReadTimeStamp();
		break;
	}

	UpdateProf();
}


//**************************************************************************************************
void __fastcall TFrmProf::btnResetAllClick(TObject *Sender)
{
	memset(Apu.profile, 0, sizeof(APUProf));
	Apu.profile->tsc.host.base =
	Apu.profile->tsc.apu.base =
	Apu.profile->tsc.spc700.base =
	Apu.profile->tsc.dsp.base = ReadTimeStamp();

	UpdateProf();
}


//**************************************************************************************************
void __fastcall TFrmProf::tabProfChange(TObject *sender)
{
	s32	i;


	lvwProf->Items->BeginUpdate();
	lvwProf->Items->Clear();

	switch (tabProf->TabIndex)
	{
	case(0):
		lvwProf->Items->Count = _CountOf(Inst);
		lvwProf->Columns->Items[0]->Caption = "Op";
		lvwProf->Columns->Items[1]->Caption = "Instruction";
		lvwProf->Columns->Items[1]->Alignment = taLeftJustify;
		lvwProf->Columns->Items[2]->Caption = "Exec Count";
		lvwProf->Columns->Items[3]->Caption = "";
		break;

	case(1):
		lvwProf->Items->Count = _CountOf(Branch);
		lvwProf->Columns->Items[0]->Caption = "Instruction";
		lvwProf->Columns->Items[1]->Caption = "Count";
		lvwProf->Columns->Items[1]->Alignment = taRightJustify;
		lvwProf->Columns->Items[2]->Caption = "Taken";
		lvwProf->Columns->Items[3]->Caption = "Not taken";
		break;

	case(2):
		lvwProf->Items->Count = _CountOf(Func);
		lvwProf->Columns->Items[0]->Caption = "Register";
		lvwProf->Columns->Items[1]->Caption = "Read";
		lvwProf->Columns->Items[1]->Alignment = taRightJustify;
		lvwProf->Columns->Items[2]->Caption = "Write";
		lvwProf->Columns->Items[3]->Caption = "Write 16";
		break;

	case(3):
		lvwProf->Items->Count = _CountOf(DSPRegs) + 1;
		lvwProf->Columns->Items[0]->Caption = "Register";
		lvwProf->Columns->Items[1]->Caption = "Read";
		lvwProf->Columns->Items[1]->Alignment = taRightJustify;
		lvwProf->Columns->Items[2]->Caption = "Write";
		lvwProf->Columns->Items[3]->Caption = "Affected";
		break;

	case(4):
		lvwProf->Items->Count = 3;
		lvwProf->Columns->Items[0]->Caption = "Processor";
		lvwProf->Columns->Items[1]->Caption = "APU";
		lvwProf->Columns->Items[1]->Alignment = taRightJustify;
		lvwProf->Columns->Items[2]->Caption = "CPU";
		lvwProf->Columns->Items[3]->Caption = "Per Call";
		break;
	}


	if (tabProf->TabIndex)
	{
		i = (lvwProf->Width - (GetSystemMetrics(SM_CXVSCROLL) + 4)) / 4;
		lvwProf->Columns->Items[0]->Width = i + 12;
		lvwProf->Columns->Items[1]->Width = i - 4;
		lvwProf->Columns->Items[2]->Width = i - 4;
		lvwProf->Columns->Items[3]->Width = i - 4;
	}
	else
	{
		i = (lvwProf->Width - (GetSystemMetrics(SM_CXVSCROLL) + 4)) / 10;
		lvwProf->Columns->Items[0]->Width = i;
		lvwProf->Columns->Items[1]->Width = i * 4;
		lvwProf->Columns->Items[2]->Width = i * 3;
		lvwProf->Columns->Items[3]->Width = i * 2;
	}

	lvwProf->Items->EndUpdate();
}


//**************************************************************************************************
void __fastcall TFrmProf::lvwProfData(TObject *Sender, TListItem *pItem)
{
	u32	i,j;
	s8	str[16];
	u64	t;


	switch (tabProf->TabIndex)
	{
	case(0):
		i = instOrder[pItem->Index];
		pItem->Caption = IntToHex((s32)i, 2);
		pItem->SubItems->Append(Inst[i]);
		sprintf(str, "%i", Apu.profile->exec[i]);
		pItem->SubItems->Append(str);
		pItem->SubItems->Append("");
		break;

	case(1):
		i = branchOrder[pItem->Index];
		pItem->Caption = Branch[i];

		j = Apu.profile->exec[BranchOp[i]];
		if (j)
		{
			sprintf(str, "%i", j);
			pItem->SubItems->Append(str);

			j = (Apu.profile->branch.bxx[i] * 1000) / j;
			sprintf(str, "%i.%i", j / 10, j % 10);
			pItem->SubItems->Append(str);

			j = 1000 - j;
			sprintf(str, "%i.%i", j / 10, j % 10);
			pItem->SubItems->Append(str);
		}
		else
		{
			pItem->SubItems->Append("0");
			pItem->SubItems->Append("0.0");
			pItem->SubItems->Append("0.0");
		}
		break;

	case(2):
			i = funcOrder[pItem->Index];
			pItem->Caption = Func[i];
			pItem->SubItems->Append(Apu.profile->func.read[i]);
			pItem->SubItems->Append(Apu.profile->func.write[i]);
			pItem->SubItems->Append(Apu.profile->func.write16[i]);
		break;

	case(3):
		if (pItem->Index == 0)
		{
			pItem->Caption = "CatchUp";
			pItem->SubItems->Append(Apu.profile->dsp.update.read);
			pItem->SubItems->Append(Apu.profile->dsp.update.num - Apu.profile->dsp.update.read);
			if (Apu.profile->dsp.update.num)
			{
				j = (Apu.profile->dsp.update.gen * 1000) / Apu.profile->dsp.update.num;
				sprintf(str, "%i.%i", j / 10, j % 10);
				pItem->SubItems->Append(str);
			}
			else
				pItem->SubItems->Append("0.0");
		}
		else
		{
			i = dspOrder[pItem->Index - 1];
			pItem->Caption = DSPRegs[i];
			pItem->SubItems->Append(Apu.profile->dsp.read[i]);

			j = Apu.profile->dsp.write[i];
			if (j)
			{
				pItem->SubItems->Append(j);

				j = (Apu.profile->dsp.update.reg[i] * 1000) / j;
				sprintf(str, "%i.%i", j / 10, j % 10);
				pItem->SubItems->Append(str);
			}
			else
			{
				pItem->SubItems->Append("0");
				pItem->SubItems->Append("0.0");
			}
		}
		break;

	case(4):
		t = (ReadTimeStamp() - Apu.profile->tsc.host.base) + Apu.profile->tsc.host.acc;

		switch(pItem->Index)
		{
		case(0):
			pItem->Caption = "APU";

			pItem->SubItems->Append("100.0");

			if (Apu.profile->tsc.apu.acc)
			{
				j = (u32)((Apu.profile->tsc.apu.acc * 10000) / t);
				sprintf(str, "%i.%02i", j / 100, j % 100);
				pItem->SubItems->Append(str);
			}
			else
				pItem->SubItems->Append("0.0");
			break;

		case(1):
			pItem->Caption = "SPC700";

			if (Apu.profile->tsc.apu.acc)
			{
				j = (u32)((Apu.profile->tsc.spc700.acc * 10000) / Apu.profile->tsc.apu.acc);
				sprintf(str, "%i.%02i", j / 100, j % 100);
				pItem->SubItems->Append(str);

				j = (u32)((Apu.profile->tsc.spc700.acc * 10000) / t);
				sprintf(str, "%i.%02i", j / 100, j % 100);
				pItem->SubItems->Append(str);


			}
			else
			{
				pItem->SubItems->Append("0.0");
				pItem->SubItems->Append("0.0");
			}
			break;

		case(2):
			pItem->Caption = "DSP";

			if (Apu.profile->tsc.apu.acc)
			{
				j = (u32)((Apu.profile->tsc.dsp.acc * 10000) / Apu.profile->tsc.apu.acc);
				sprintf(str, "%i.%02i", j / 100, j % 100);
				pItem->SubItems->Append(str);

				j = (u32)((Apu.profile->tsc.dsp.acc * 10000) / t);
				sprintf(str, "%i.%02i", j / 100, j % 100);
				pItem->SubItems->Append(str);
			}
			else
			{
				pItem->SubItems->Append("0.0");
				pItem->SubItems->Append("0.0");
			}
			break;
		}
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmProf::UpdateProf()
{
	s32	i;

	lvwProf->Items->BeginUpdate();
	i = lvwProf->TopItem->Index;
	lvwProf->UpdateItems(i, i+lvwProf->VisibleRowCount);
	lvwProf->Items->EndUpdate();
}


//**************************************************************************************************
void __fastcall BubbleSort(u8 *pOrder, u32 *pItems, u32 num, b8 desc)
{
	s32	x,y;
	u8	i;

	if (desc)
	{
		for (x=num-1; x>0; x--)
			for (y=0; y<x; y++)
				if (pItems[pOrder[y]] < pItems[pOrder[y+1]])
				{
					i = pOrder[y];
					pOrder[y] = pOrder[y+1];
					pOrder[y+1] = i;
				}
	}
	else
	{
		for (x=num-1; x>0; x--)
			for (y=0; y<x; y++)
				if (pItems[pOrder[y]] > pItems[pOrder[y+1]])
				{
					i = pOrder[y];
					pOrder[y] = pOrder[y+1];
					pOrder[y+1] = i;
				}
	}
}

#define _SortIndex(x) \
	if (c < 0) \
		for (i=_CountOf(x)-1, j=0; i>=0; i--) x[i] = j++; \
	else \
		for (i=0; i<_CountOf(x); i++) x[i] = i; \

void __fastcall TFrmProf::lvwProfColumnClick(TObject *sender, TListColumn *pCol)
{
	s8	t,c;
	s32	i,j;
	u32	d;
	u32	percent[128];


	t = tabProf->TabIndex;
	c = (s32)tabProf->Tabs->Objects[t];

	if ((c & 0x7F) == pCol->Index) c ^= 0x80;
	else c = pCol->Index | 0x80;

	tabProf->Tabs->Objects[t] = (TObject*)c;


	switch(t)
	{
	case(0):
		switch(c & 0x7F)
		{
		case(0):
			_SortIndex(instOrder)
			break;

		case(1):
			break;

		case(2):
			BubbleSort(instOrder, Apu.profile->exec, _CountOf(instOrder), (c < 0));
			break;
		}
		break;

	case(1):
		switch(c & 0x7F)
		{
		case(0):
			_SortIndex(branchOrder)
			break;

		case(1):
			for (i=0; i<_CountOf(branchOrder); i++)
				percent[i] = Apu.profile->exec[BranchOp[branchOrder[i]]];

			BubbleSort(branchOrder, percent, _CountOf(branchOrder), (c < 0));
			break;

		case(2):
			for (i=0; i<_CountOf(branchOrder); i++)
			{
				d = Apu.profile->exec[BranchOp[branchOrder[i]]];
				percent[i] = d ? (Apu.profile->branch.bxx[i] * 1000) / d : 0;
			}

			BubbleSort(branchOrder, percent, _CountOf(branchOrder), (c < 0));
			break;

		case(3):
			for (i=0; i<_CountOf(branchOrder); i++)
			{
				d = Apu.profile->exec[BranchOp[branchOrder[i]]];
				percent[i] = d ? (Apu.profile->branch.bxx[i] * 1000) / d : 0;
			}

			BubbleSort(branchOrder, percent, _CountOf(branchOrder), (c >= 0));
			break;
		}
		break;

	case(2):
		switch(c & 0x7F)
		{
		case(0):
			_SortIndex(funcOrder)
			break;

		case(1):
			BubbleSort(funcOrder, Apu.profile->func.read, _CountOf(funcOrder), (c < 0));
			break;

		case(2):
			BubbleSort(funcOrder, Apu.profile->func.write, _CountOf(funcOrder), (c < 0));
			break;

		case(3):
			BubbleSort(funcOrder, Apu.profile->func.write16, _CountOf(funcOrder), (c < 0));
			break;
		}
		break;

	case(3):
		switch(c & 0x7F)
		{
		case(0):
			_SortIndex(dspOrder)
			break;

		case(1):
			BubbleSort(dspOrder, Apu.profile->dsp.read, _CountOf(dspOrder), (c < 0));
			break;

		case(2):
			BubbleSort(dspOrder, Apu.profile->dsp.write, _CountOf(dspOrder), (c < 0));
			break;

		case(3):
			for (i=0; i<_CountOf(dspOrder); i++)
			{
				d = Apu.profile->dsp.write[i];
				percent[i] = d ? (Apu.profile->dsp.update.reg[i] * 1000) / d : 0;
			}

			BubbleSort(dspOrder, percent, _CountOf(dspOrder), (c >= 0));
			break;
		}
		break;
	}

	UpdateProf();
}

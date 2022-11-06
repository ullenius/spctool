//**************************************************************************************************

#include	<vcl.h>
#include	"SPCTool.h"
#pragma hdrstop
#include	"Debug.h"
#include	"Map.h"


#pragma package(smart_init)
#pragma resource "*.dfm"

static const TColor MapColor[9] =
		{clBlack, clBlue, clLime, clAqua, clRed, clFuchsia, clYellow, clWhite, clGray};


u8	MemMap[65536];

static struct									//Registers in the SPC700
{
	u8			*pc;
	u8			*sp;
	u8			y;
	u8			x;
	SPCFlags	psw;
} Reg;

static u8	Src[32];


TFrmMap *FrmMap;


static void	DebugSPC(volatile u8 *pc, volatile u16 ya, volatile u8 x, volatile SPCFlags psw,
					 volatile u8 *sp, volatile u32 cnt);
static void	DebugDSP(volatile u8 *reg, volatile u8 val);


//**************************************************************************************************
__fastcall TFrmMap::TFrmMap(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmMap::FormShow(TObject *Sender)
{
	u8	*p1,*p2;
	u32	i;


	btnMap->Caption = "&Map";
	btnMap->ModalResult = mrNone;

	p1 = Apu.ram;
	p2 = MemMap;
	for (i=0; i<0x10000; i++)
		*p2++ = (*p1++ != 0xFF) ? 8 : 0;
}


//**************************************************************************************************
void __fastcall TFrmMap::pbMapMouseMove(TObject *Sender, TShiftState shift, int x, int y)
{
	lblOfs->Caption = IntToHex(((y >> 1) << 8) | (x >> 1), 4);
}


//**************************************************************************************************
void __fastcall TFrmMap::pbMapPaint(TObject *Sender)
{
	TRect	r;
	s32		x,y;
	u8		*p,m;


	p = MemMap;
	r.top = 0;
	r.bottom = 2;
	for (y=0; y<256; y++)
	{
		r.left = 0;
		r.right = 2;
		x = 256;

		do
		{
			m = *p;
			pbMap->Canvas->Brush->Color = MapColor[m];
			while (x-- && MapColor[*++p] == m) r.right += 2;

			pbMap->Canvas->FillRect(r);

			r.left = r.right;
			r.right += 2;
		}
		while (x > 0);

		r.top = r.bottom;
		r.bottom += 2;
	}
}


//**************************************************************************************************
void __fastcall TFrmMap::btnMapClick(TObject *Sender)
{
	SPCDebug	*oldSPC;
	DSPDebug	*oldDSP;
	SPCReg		*pReg;
	s32			i,x,y;
	u8			*p;


	if (btnMap->ModalResult != mrNone) return;
	
	//Restore original state -------------------
	oldSPC = Apu.SetSPCDbg(DebugSPC, 0);
	oldDSP = Apu.SetDSPDbg(DebugDSP, 0);

	Apu.LoadSPCFile(&pState->spc);

	Apu.SetAPUOpt(MIX_INT, 2, 16, 32000, INT_NONE, OPT_NOECHO);

	//==========================================
	// Generate memory map

	memset(MemMap, 0, 0x10000);					//Erase map

	//Emulate entire song ----------------------
	i = pState->id6.GetTotal();
	do
	{
		Apu.EmuAPU(NULL, APU_CLK, 0);
		lblMap->Caption = "Mapping:  " + AnsiString((s32)Apu.GetSPCTime() * 100 / i) + "%";
		lblMap->Repaint();
	}
	while (Apu.GetSPCTime() < i);
	lblMap->Caption = "";

	//Mark sound source usage ------------------
	for (x=0; x<256; x++)
	{
		if (Src[x >> 3] & (1 << (x & 7)))
		{
			p = &MemMap[((u32)Apu.dsp->dir << 8) | (x << 2)];
			for (i=0; i<4; i++)
			{
				if (!*p) *p = 4;
				p++;
			}
		}
	}

	for (x=0; x<256; x++)
	{
		if (Src[x >> 3] & (1 << (x & 7)))
		{
			p = &MemMap[pState->src[x].offset];
			i = pState->src[x].len * 9;
			while (i--)
			{
				if (!*p) *p = 5;
				p++;
			}
		}
	}

	//Mark echo region -------------------------
	if (!(Apu.dsp->flg & FLG_ECEN))
	{
		p = &MemMap[(u32)Apu.dsp->esa << 8];
		if (Apu.dsp->edl & 0xF)
			i = (u32)Apu.dsp->edl << 11;
		else
			i = 4;

		while (i--)
		{
			if (!*p) *p = 6;
			p++;
		}
	}

	//Mark IPL ROM -----------------------------
	if (Apu.ram[0xF1] & 0x80)
	{
		p = &MemMap[0xFFC0];
		for (i=0; i<64; i++)
		{
			if (!*p) *p = 3;
			p++;
		}
	}

	//Mark function registers ------------------
	p = &MemMap[0xF0];
	for (i=0; i<16; i++)
	{
		if (!*p) *p = 7;
		p++;
	}

	//Mark non-FF data -------------------------
	p = MemMap;
	for (i=0; i<65536; i++)
	{
		if (!*p && Apu.ram[i] != 0xFF) *p = 8;
		p++;
	}

	pbMap->Repaint();

	//Restore DSP options ----------------------
	Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());

	Apu.SetDSPDbg(oldDSP, 0);
	Apu.SetSPCDbg(oldSPC, 0);

	btnMap->Caption = "O&K";
	btnMap->ModalResult = mrOk;
}


//**************************************************************************************************
void __fastcall MarkData(u8 op)
{
	u32	d;


	switch(op >> 6)
	{
	case(0):
		switch((op >> 4) & 3)
		{
		case(0):
			switch(*Reg.pc)
			{
			case(0x0F):	//BRK
				MemMap[(u16)Reg.sp] |= 1;
				MemMap[((u8)Reg.sp - 1) | 0x100] |= 1;
				MemMap[((u8)Reg.sp - 2) | 0x100] |= 1;
				break;

			case(0x7F):	//RETI
				MemMap[((u8)Reg.sp + 3) | 0x100] |= 1;

			case(0x6F):	//RET
				MemMap[((u8)Reg.sp + 1) | 0x100] |= 1;
				MemMap[((u8)Reg.sp + 2) | 0x100] |= 1;
				break;
			}
			break;

		case(1):
			if (op & 8)
			{
				d = Reg.x;

				if ((op & 7) == 2) d = Reg.y;
				if (Reg.psw.p) d |= 0x100;

				MemMap[d] |= 1;
			}
			else
			{
				if ((*Reg.pc & 0x9F) == 0x0D)	//PUSH
				{
					MemMap[(u16)Reg.sp] |= 1;
				}
				else
				if ((*Reg.pc & 0x9F) == 0x8E)	//POP
				{
					MemMap[((u8)Reg.sp + 1) | 0x100] |= 1;
				}
			}
			break;

		case(2):	//TCALL
			MemMap[(u16)Reg.sp] |= 1;
			MemMap[((u8)Reg.sp - 1) | 0x100] |= 1;

			d = 0xFFC0 + ((15 - (*Reg.pc & 0xF)) << 1);
			MemMap[d] |= 1;
			MemMap[1+d] |= 1;
			break;
		}
		break;

	case(1):
		Reg.pc++;

		switch((op >> 4) & 3)
		{
		case(0):
			if (op & 8)	//PCALL
			{
				MemMap[(u16)Reg.sp] |= 1;
				MemMap[((u8)Reg.sp - 1) | 0x100] |= 1;
			}
			break;

		case(2):
			switch(op & 7)
			{
			case(0):
			case(2):
				d = Reg.pc[0];
				break;

			case(1):
				d = Reg.pc[1];
				break;

			case(3):
				d = Reg.pc[-1];
				break;
			}

			if (Reg.psw.p) d |= 0x100;

			MemMap[d] |= 1;
			if (op & 8) MemMap[1+d] |= 1;
			break;

		case(3):
			d = *Reg.pc;
			if (Reg.psw.p) d |= 0x100;

			if (op & 4)
			{
				if (op & 2)
				{
					MemMap[d] |= 1;
					MemMap[1+d] |= 1;

					d = *(u16*)&Apu.ram[d];
					d += Reg.y;
				}
				else
				{
					d += Reg.x;

					MemMap[d] |= 1;
					MemMap[1+d] |= 1;

					d = *(u16*)&Apu.ram[d];
				}
			}
			else
				d += (op & 2) ? Reg.y : Reg.x;

			MemMap[d] |= 1;
			break;
		}
		break;

	case(2):
		Reg.pc++;
		
		switch((op >> 4) & 3)
		{
		case(2):
			if (op & 7) MemMap[*(u16*)Reg.pc & 0x1FFF] |= 1;
			else
			if (!(op & 8)) MemMap[*(u16*)Reg.pc] |= 1;

			break;

		case(3):
			d = *(u16*)Reg.pc;

			if (op & 4)	//JMP [abs+X]
			{
				d += Reg.x;

				MemMap[(u16)d] |= 1;
				MemMap[(u16)(1+d)] |= 1;
			}
			else
			{
				d += (op & 3) ? Reg.y : Reg.x;
				MemMap[(u16)d] |= 1;
			}
			break;
		}
		break;
	}
}


//**************************************************************************************************
void DebugSPC(volatile u8 *pc, volatile u16 ya, volatile u8 x, volatile SPCFlags psw,
			  volatile u8 *sp, volatile u32 cnt)
{
	u8	*p;
	u8	op;


	Reg.pc	= (u8*)pc;
	Reg.y	= ya >> 8;
	Reg.x	= x;
	Reg.psw	= psw;
	Reg.sp	= (u8*)sp;

	p = &MemMap[(u16)Reg.pc];
	*p |= 2;									//Mark the instruction

	//Opcode 1 ---------------------------------
	op = InsList[*Reg.pc].op1;
	switch(op >> 6)								//Mark the opcode bytes
	{
	case(2):
		p[2] |= 2;

	case(1):
		p[1] |= 2;
		p++;

		MarkData(op);							//Mark any bytes accessed by the instruction
	}

	//Opcode 2 ---------------------------------
	op = InsList[*Reg.pc].op2;
	switch(op >> 6)
	{
	case(2):
		p[2] |= 2;

	case(1):
		p[1] |= 2;

		MarkData(op);
	}
}


//**************************************************************************************************
void DebugDSP(volatile u8 *reg, volatile u8 val)
{
	DSPVoice	*pVoice;
	u8			kon;


	if (!((u8)reg == 0x4C && val)) return;		//Are any voices getting keyed on?

	pVoice = Apu.dsp->voice;
	kon = val;
	do
	{
		if (kon & 1) Src[pVoice->srcn >> 3] |= 1 << (pVoice->srcn & 7);

		kon >>= 1;
		pVoice++;

	} while (kon);
}

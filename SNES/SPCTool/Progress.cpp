//---------------------------------------------------------------------------

#include <vcl.h>
#include "SPCTool.h"
#pragma hdrstop

#include "Progress.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmProg *FrmProg;


//**************************************************************************************************
__fastcall TFrmProg::TFrmProg(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmProg::FormCreate(TObject *Sender)
{
	BarCurrent = new ProgBar(pbCurrent);
	BarTotal = new ProgBar(pbTotal);
	
	pbCurrent->Tag = (s32)BarCurrent;
	pbTotal->Tag = (s32)BarTotal;

	ShowTotal = 0;
	AutoClose = 0;
}


//**************************************************************************************************
void __fastcall TFrmProg::FormDestroy(TObject *Sender)
{
	delete BarTotal;
	delete BarCurrent;
}


//**************************************************************************************************
void __fastcall TFrmProg::FormShow(TObject *Sender)
{
	Cancel = 0;

	if (ShowTotal)
	{
		btnCancel->Top = 88;
		ClientHeight = 113;
		pbTotal->Visible = true;
		pbTotal->Enabled = true;
		BarCurrent->Print = false;
		BarTotal->Print = true;
	}
	else
	{
		btnCancel->Top = 68;
		ClientHeight = 93;
		pbTotal->Visible = false;
		pbTotal->Enabled = false;
		BarCurrent->Print = true;
		BarTotal->Print = false;
	}
}


//**************************************************************************************************
void __fastcall TFrmProg::btnCancelClick(TObject *Sender)
{
	Cancel = 1;

	if (AutoClose) ModalResult = mrCancel;
}


//**************************************************************************************************
void __fastcall TFrmProg::pbPaint(TObject *sender)
{
	((ProgBar*)((TPaintBox*)sender)->Tag)->Paint();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Progress Bar Class

//**************************************************************************************************
ProgBar::ProgBar(TPaintBox *p)
{
	PCanvas = p->Canvas;
	Width = p->Width;
	Height = p->Height;

	PCanvas->Brush->Style = bsSolid;
}


//**************************************************************************************************
void ProgBar::Reset(f64 total)
{
	Total = total;
	Length = 1;
	Set(0.0);
}


//**************************************************************************************************
void ProgBar::Set(f64 progress)
{
	s32	i;


	_Clamp(progress, 0.0, Total); 				//Clamp 'progress' between 0.0 and Total
	progress /= Total;

	i = progress * Width;						//Calculate integer width of bar
	if (i == Length) return;					//If the bar hasn't changed, return

	if (Print) FrmProg->lblProgress->Caption = AnsiString((s32)(progress * 100.0f)) + "%";

	PCanvas->Lock();
	if (i < Length) DrawBox();					//If the bar got smaller...
	Length = i;									//Set new size
	DrawBar();
	PCanvas->Unlock();
}


//**************************************************************************************************
void ProgBar::Paint()
{
	PCanvas->Lock();
	DrawBox();
	DrawBar();
	PCanvas->Unlock();
}


//**************************************************************************************************
void ProgBar::DrawBox()
{
	//Face -------------------------------------
	PCanvas->Brush->Color = CBtn.wf;
	PCanvas->FillRect(TRect(1, 1, Width - 1, Height - 1));

	//Shadow -----------------------------------
	PCanvas->Pen->Color = CBtn.ws;
	PCanvas->PenPos = TPoint(0, Height - 2);
	PCanvas->LineTo(0, 0);
	PCanvas->LineTo(Width - 1, 0);

	//Highlight --------------------------------
	PCanvas->Pen->Color = CBtn.wh;
	PCanvas->LineTo(Width - 1, Height - 1);
	PCanvas->LineTo(-1, Height - 1);
}


//**************************************************************************************************
void ProgBar::DrawBar()
{
	if (Length < 4) return;

	//Face -------------------------------------
	if (Length > 4)
	{
		PCanvas->Brush->Color = CBtn.sf;
		PCanvas->FillRect(TRect(2, 2, Length - 2, Height - 2));
	}

	//Highlight --------------------------------
	PCanvas->Pen->Color = CBtn.sh;
	PCanvas->PenPos = TPoint(0, Height - 2);
	PCanvas->LineTo(0, 0);
	PCanvas->LineTo(Length - 1, 0);

	//Dark shadow ------------------------------
	PCanvas->Pen->Color = CBtn.sd;
	PCanvas->LineTo(Length - 1, Height - 1);
	PCanvas->LineTo(-1, Height - 1);

	//Light ------------------------------------
	PCanvas->Pen->Color = CBtn.sl;
	PCanvas->PenPos = TPoint(1, Height - 3);
	PCanvas->LineTo(1, 1);
	PCanvas->LineTo(Length - 2, 1);

	//Shadow -----------------------------------
	PCanvas->Pen->Color = CBtn.ss;
	PCanvas->LineTo(Length - 2, Height - 2);
	PCanvas->LineTo(0, Height - 2);
}

//---------------------------------------------------------------------------

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"mmsystem.h"
#include	"mmreg.h"
#include	"Wav.h"
#include	"Progress.h"
#include	"A2Math.h"
#include	"Help\Index.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmWav *FrmWav;


//**************************************************************************************************
__fastcall TFrmWav::TFrmWav(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmWav::FormCreate(TObject *sender)
{
	grpMode->ItemIndex = 0;
	dsp = Cfg.dsp;
}


//**************************************************************************************************
void __fastcall TFrmWav::grpModeClick(TObject *sender)
{
	if (grpMode->Tag == 2)						//If switching to another mode, save current setting
	{
		dsp.rate		= cboRate->Tag;
		dsp.bits		= cboRate->Tag;
		dsp.inter		= cboInter->Tag;
		dsp.analog		= chkAnalog->Checked;
		dsp.filter		= chkFilter->Checked;
		dsp.oldSmp		= chkOldSmp->Checked;
		dsp.disEcho		= chkNoEcho->Checked;
		dsp.stereo		= trkStereo->Tag;
		dsp.efbct		= trkEcho->Tag;
		grpNorm->Tag	= grpNorm->ItemIndex;
	}

	switch(grpMode->ItemIndex)
	{
	case(0):
		grpNorm->ItemIndex	= 2;
		cboRate->ItemIndex	= 1;
		cboRate->Tag		= 44100;
		cboBits->ItemIndex	= 1;
		cboBits->Tag		= 16;
		cboInter->ItemIndex	= 4;
		cboInter->Tag		= 4;
		chkMix->Checked		= 1;
		chkAnalog->Checked	= 0;
		chkFilter->Checked	= 1;
		chkNoEcho->Checked	= 0;
		chkOldSmp->Checked	= 0;
		trkStereo->Position	= 50;
		trkEcho->Position	= -50;

		grpMode->Tag		= 0;
		ClientHeight		= bvlAdvanced->Top - 2;
		break;

	case(1):
		grpNorm->ItemIndex	= 2;
		cboRate->ItemIndex	= 1;
		cboRate->Tag		= 44100;
		cboBits->ItemIndex	= 0;
		cboBits->Tag		= 16;
		cboInter->ItemIndex	= 3;
		cboInter->Tag		= 3;
		chkMix->Checked		= 0;
		chkAnalog->Checked	= 1;
		chkFilter->Checked	= 0;
		chkNoEcho->Checked	= 0;
		chkOldSmp->Checked	= 0;
		trkStereo->Position	= 50;
		trkEcho->Position	= 100;

		grpMode->Tag		= 1;
		ClientHeight		= bvlAdvanced->Top - 2;
		break;

	case(2):
		grpNorm->ItemIndex	= grpNorm->Tag;
		
		if (dsp.bits == 16)
		{
			cboBits->ItemIndex	= 0;
			cboBits->Tag		= 16;
		}
		else
		{
			cboBits->ItemIndex	= 1;
			cboBits->Tag		= -32;
		}

		cboRate->Text		= dsp.rate;
		cboRate->Tag		= dsp.rate;
		cboInter->ItemIndex	= dsp.inter;
		cboInter->Tag		= dsp.inter;

		if (dsp.mix == MIX_FLOAT)
		{
			chkMix->Checked		= 1;
			chkAnalog->Checked	= 0;
		}
		else
		{
			chkMix->Checked		= 0;
			chkAnalog->Checked	= dsp.analog;
		}

		chkFilter->Checked	= dsp.filter;
		chkNoEcho->Checked	= dsp.disEcho;
		chkOldSmp->Checked	= dsp.oldSmp;
		trkStereo->Position	= (dsp.stereo * 100) >> 16;
		trkEcho->Position	= (dsp.efbct * 100) >> 15;

		grpMode->Tag		= 2;
		ClientHeight		= chkOldSmp->Top + chkOldSmp->Height + 3;
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmWav::cboRateExit(TObject *sender)
{
	s32	i;


	i = cboRate->Text.ToIntDef(44100);
	_Clamp(i, 8000, 192000);

	cboRate->Text = i;
	cboRate->Tag = i;
}


//**************************************************************************************************
void __fastcall TFrmWav::cboBitsExit(TObject *sender)
{
	switch(cboBits->ItemIndex)
	{
	case(0):
		cboBits->Tag = 16;
		break;

	case(1):
		cboBits->Tag = -32;
		break;
	}
}


//**************************************************************************************************
void __fastcall TFrmWav::cboInterExit(TObject *sender)
{
	cboInter->Tag = cboInter->ItemIndex;
}


//**************************************************************************************************
void __fastcall TFrmWav::chkMixClick(TObject *sender)
{
	chkAnalog->Enabled = !chkMix->Checked;
}


//**************************************************************************************************
void __fastcall TFrmWav::chkAnalogClick(TObject *sender)
{
	chkMix->Enabled = !chkAnalog->Checked;
}


//**************************************************************************************************
void __fastcall TFrmWav::trkStereoChange(TObject *sender)
{
	s32	i;


	i = trkStereo->Position;
	lblStereo->Caption = i;
	trkStereo->Tag = (i << 16) / 100;
}


//**************************************************************************************************
void __fastcall TFrmWav::trkEchoChange(TObject *sender)
{
	AnsiString	str;
	s32			i;


	i = trkEcho->Position;
	if (i < 0) str = "-";
	str += 100 - abs(i);
	str += "%";
	lblEcho->Caption = str;
	trkEcho->Tag = (i << 15) / 100;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Create .WAV Files

//**************************************************************************************************
// Find Normalized Amplification Level
//
// In:
//    p    -> APU saved state
//
// Out:
//    Amplification value for song, normalized to 0dB

s32 __fastcall TFrmWav::FindNorm(APUFile *p)
{
	s32	song,t;


	//Restore APU ------------------------------
	Apu.LoadSPCFile(&p->spc);
	song = p->id6.GetSong();
	Apu.SetDSPLength(song, 0);

	//Reset progress bar -----------------------
	FrmProg->BarCurrent->Reset(song);
	FrmProg->lblInfo->Caption = ScanStrR(p->id6.file, '\\');
	FrmProg->lblStatus->Caption = "Finding peak volume...";

	//Search for peak sample -------------------
	do
	{
		Apu.EmuAPU(pBuf, 0, 32000);

		t = Apu.GetSPCTime();
		FrmProg->BarCurrent->Set((f64)t);
		FrmProg->BarTotal->Set(totalProg + t);
	} while (t < song && !FrmProg->Cancel);

	totalProg += song;							//Add song length to total progress

	return Apu.GetDSPAmp();
}


//**************************************************************************************************
s32 __fastcall TFrmWav::GenerateWav(APUFile *p)
{
	s8		str[260];
	s32		fh,size;
	u32		t,total;


	//Create file ------------------------------
	GetFileName(str, p, "wav");
	fh = FileCreate(str);
	if (fh == -1) goto Error;

	FileSeek(fh, (s32)sizeof(WavFile), 0);
	wav.data.size = 0;

	//Reset APU --------------------------------
	Apu.LoadSPCFile(&p->spc);

	if (amp)
	{
		Apu.SetDSPAmp(amp);						//Set amplification to normalized level
	}
	else
	{
		if (p->id6.amp)							//If tag contains amp level, use it
			Apu.SetDSPAmp(p->id6.amp);
		else
		{
			size = p->state.dspState.amp;
			Apu.SetDSPAmp(size & 0xFFFFFFF);
			if (size < 0) Apu.SetDSPAAR((size >> 28) & 3, -1, -1, -1);
		}
	}

	Apu.SetDSPLength(p->id6.GetSong(), p->id6.GetFade());
	total = p->id6.GetTotal();

	//Reset progress bar -----------------------
	FrmProg->BarCurrent->Reset(total);

	FrmProg->lblInfo->Caption = ScanStrR(str, '\\');
	FrmProg->lblStatus->Caption = "Creating WAV file...";

	//Emulate APU ------------------------------
	while (1)
	{
		if (FrmProg->Cancel) goto Delete;

		Apu.EmuAPU(pBuf, 0, wav.fmt.data.nSamplesPerSec);

		t = Apu.GetSPCTime();
		if (t >= (u32)total) break;

		size = FileWrite(fh, pBuf, wav.fmt.data.nAvgBytesPerSec);
		if (size == -1) goto Error;

		wav.data.size += size;
		FrmProg->BarCurrent->Set(t);
		FrmProg->BarTotal->Set(totalProg + t);
	}

	totalProg += total;
	FrmProg->BarCurrent->Set(t);
	FrmProg->BarTotal->Set(totalProg);

	size = FileWrite(fh, pBuf,
					 (wav.fmt.data.nSamplesPerSec * (XID6_TICKSSEC - (t - total)) / XID6_TICKSSEC) *
					 wav.fmt.data.nBlockAlign);

	if (size == -1) goto Error;

	wav.data.size += size;
	wav.riff.size = wav.data.size + sizeof(WavFile) - 8;

	FileSeek(fh, 0, 0);
	FileWrite(fh, &wav, sizeof(WavFile));
	FileClose(fh);

	return 0;

Error:											//Out of disk space
	FileClose(fh);
	return -1;

Delete:											//User clicked on 'Cancel'
	FileClose(fh);
	DeleteFile(str);
	return -2;
}


//**************************************************************************************************
void TFrmWav::Record(TListView *lvwFile)
{
	TItemStates	selected;
	TListItem	*pItem;
	APUFile		*pCurAPU;
	f64			total,ampAcc;
	s32			ampCount;


	//Create a WAV header ------------------
	wav.riff.id						= 'RIFF';
	wav.riff.type					= 'WAVE';

	wav.fmt.schk.id					= 'fmt ';
	wav.fmt.schk.size				= sizeof(WAVEFORMATEX);
	wav.fmt.data.wFormatTag			= (cboBits->Tag == -32) ? WAVE_FORMAT_IEEE_FLOAT : WAVE_FORMAT_PCM;
	wav.fmt.data.nChannels			= 2;
	wav.fmt.data.nSamplesPerSec		= cboRate->Tag;
	wav.fmt.data.wBitsPerSample		= abs(cboBits->Tag);
	wav.fmt.data.nBlockAlign		= (wav.fmt.data.wBitsPerSample * 2) >> 3;
	wav.fmt.data.nAvgBytesPerSec	= wav.fmt.data.nSamplesPerSec * wav.fmt.data.nBlockAlign;
	wav.fmt.data.cbSize				= 0;

	wav.data.id						= 'data';

	//Change DSP options -------------------
	Apu.SetAPUOpt(chkMix->Checked ? MIX_FLOAT : MIX_INT,
				  2,
				  cboBits->Tag,
				  cboRate->Tag,
				  cboInter->ItemIndex,
				  (chkAnalog->Checked ? OPT_ANALOG : 0) |
				  (chkOldSmp->Checked ? OPT_OLDSMP : 0) |
				  (chkNoEcho->Checked ? OPT_NOECHO : 0) |
				  (chkFilter->Checked ? OPT_FILTER : 0));
	Apu.SetAPUSmpClk(0x10000);
	Apu.SetDSPPitch(32000);
	Apu.SetDSPStereo(trkStereo->Tag);
	Apu.SetDSPEFBCT(trkEcho->Tag);
	

	//==========================================
	// Create WAV files

	pBuf = malloc(192000 * 8);
	totalProg = 0.0;
	amp = 0;

	if (lvwFile->SelCount > 1)
	{
		selected << isSelected;

		//Get total time of songs --------------
		total = 0.0;
		pItem = lvwFile->Selected;
		do
		{
			if (((FileInfo*)pItem->Data)->type <= ID6_ZST)
			{
				total += ((FileInfo*)pItem->Data)->id6.GetTotal();
				if (grpNorm->ItemIndex) total += ((FileInfo*)pItem->Data)->id6.GetSong();
			}

			pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
		} while (pItem);

		FrmProg->BarTotal->Reset(total);

		switch(grpNorm->ItemIndex)
		{
		case(1):
			pItem = lvwFile->Selected;
			do
			{
				if (((APUFile*)pItem->Data)->type <= ID6_ZST)
				{
					Apu.SetAPUOpt(-1, -1, -1, 32000, -1, -1);
					Apu.SetDSPAmp(0x100000);
					Apu.SetDSPAAR(AAR_ON, 32768, 0, 0x100000);
					amp = FindNorm((APUFile*)pItem->Data);
					if (FrmProg->Cancel) goto Cancel;
					Apu.SetAPUOpt(-1, -1, -1, wav.fmt.data.nSamplesPerSec, -1, -1);

					if (GenerateWav((APUFile*)pItem->Data) < 0) goto Cancel;
				}

				pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
			} while (pItem);
			break;

		case(2):
			ampCount = 0;
			ampAcc = 0.0;

			Apu.SetAPUOpt(-1, -1, -1, 32000, -1, -1);

			pItem = lvwFile->Selected;
			do
			{
				if (((FileInfo*)pItem->Data)->type <= ID6_ZST)
				{
					Apu.SetDSPAmp(0x100000);
					Apu.SetDSPAAR(AAR_ON, 32768, 0, 0x100000);
					ampAcc += FindNorm((APUFile*)pItem->Data);
					ampCount++;
				}

				if (FrmProg->Cancel) goto Cancel;

				pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
			} while (pItem);

			Apu.SetAPUOpt(-1, -1, -1, wav.fmt.data.nSamplesPerSec, -1, -1);

			amp = (s32)(ampAcc / ampCount);

		case(0):
			pItem = lvwFile->Selected;
			do
			{
				if (((APUFile*)pItem->Data)->type <= ID6_ZST)
				{
					if (GenerateWav((APUFile*)pItem->Data) < 0) goto Cancel;
				}

				pItem = lvwFile->GetNextItem(pItem, sdAll, selected);
			} while (pItem);

			break;
		}
	}
	else
	{
		pCurAPU = (APUFile*)lvwFile->Tag;

		Apu.SaveAPU(&pCurAPU->state.spcState, &pCurAPU->state.dspState);

		if (grpNorm->ItemIndex)
		{
			FrmProg->BarTotal->Reset(pCurAPU->id6.GetTotal() + pCurAPU->id6.GetSong());
			Apu.SetAPUOpt(-1, -1, -1, 32000, -1, -1);
			Apu.SetDSPAmp(0x100000);
			Apu.SetDSPAAR(AAR_ON, 32768, 0, 0x100000);
			amp = FindNorm(pCurAPU);
			if (FrmProg->Cancel) goto Cancel;
			Apu.SetAPUOpt(-1, -1, -1, wav.fmt.data.nSamplesPerSec, -1, -1);
		}
		else
			FrmProg->BarTotal->Reset(pCurAPU->id6.GetTotal());

		GenerateWav(pCurAPU);

		Apu.RestoreAPU(&pCurAPU->state.spcState, &pCurAPU->state.dspState);
	}

Cancel:
	free(pBuf);

	//Restore DSP options ----------------------
	Apu.SetAPUOpt(Cfg.dsp.mix, 2, Cfg.dsp.bits, Cfg.dsp.rate, Cfg.dsp.inter, Cfg.dsp.Opts());
	Apu.SetDSPAAR(Cfg.mix.aar, Cfg.mix.threshold, Cfg.mix.min, Cfg.mix.max);
	Apu.SetDSPStereo(Cfg.dsp.stereo);
	Apu.SetDSPEFBCT(Cfg.dsp.efbct);
	//Speed and pitch get restored by caller

	FrmProg->ModalResult = mrOk;				//Close progress dialog
}

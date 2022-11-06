//---------------------------------------------------------------------------

#ifndef LogH
#define LogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#pragma pack(push, 2)
typedef struct
{
	SubChk			schk;						//'fmt '
	WAVEFORMATEX	data;
} WavHdr;

typedef struct
{
	Chunk	riff;								//'RIFF' 'WAVE'
	WavHdr	fmt;
	SubChk	data;								//'data'
} WavFile;

typedef struct
{
	u32	dwIdentifier;
	u32	dwType;
	u32	dwStart;
	u32	dwEnd;
	u32	dwFraction;
	u32	dwPlayCount;
} SmpLoop;

typedef struct
{
	SubChk	schk;								//'smpl'
	u32		dwManufacturer;
	u32		dwProduct;
	u32		dwSamplePeriod;
	u32		dwMIDIUnityNote;
	u32		dwMIDIPitchFraction;
	u32		dwSMPTEFormat;
	u32		dwSMPTEOffset;
	u32		cSampleLoops;
	u32		cbSamplerData;
	SmpLoop	loop;
} SmpChk;
#pragma	pack(pop)


//---------------------------------------------------------------------------
class TFrmWav : public TForm
{
__published:
	TRadioGroup	*grpMode;
	TButton		*btnOK;
	TButton		*btnCancel;

	TBevel		*bvlAdvanced;
	TRadioGroup	*grpNorm;

	TLabel		*lblRate;
	TLabel		*lblBits;
	TLabel		*lblInter;
	TComboBox	*cboBits;
	TComboBox	*cboRate;
	TComboBox	*cboInter;

	TCheckBox	*chkMix;
	TCheckBox	*chkAnalog;
	TCheckBox	*chkFilter;
	TCheckBox	*chkNoEcho;
	TCheckBox	*chkOldSmp;

	TLabel		*lblStereoT;
	TLabel		*lblStereo;
	TTrackBar	*trkStereo;
	TLabel		*lblEchoT;
	TLabel		*lblEcho;
	TTrackBar	*trkEcho;

	void	__fastcall FormCreate(TObject *sender);
	void	__fastcall grpModeClick(TObject *sender);
	void	__fastcall cboRateExit(TObject *sender);
	void	__fastcall cboBitsExit(TObject *sender);
	void	__fastcall cboInterExit(TObject *sender);
	void	__fastcall chkMixClick(TObject *sender);
	void	__fastcall chkAnalogClick(TObject *sender);
	void	__fastcall trkStereoChange(TObject *sender);
	void	__fastcall trkEchoChange(TObject *sender);

private:
	CfgDSP	dsp;								//Placeholder for user defined settings

	WavFile	wav;								//.WAV file header
	void	*pBuf;								//Temporary buffer for output
	s32		totalProg;							//Total for progress dialog
	u32		amp;

	s32		__fastcall FindNorm(APUFile *p);
	s32		__fastcall GenerateWav(APUFile *p);

public:
			__fastcall TFrmWav(TComponent* Owner);
	void	Record(TListView *lvwFile);
};


extern PACKAGE TFrmWav *FrmWav;
#endif


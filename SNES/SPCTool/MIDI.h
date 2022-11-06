//---------------------------------------------------------------------------

#ifndef MIDIH
#define MIDIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#pragma pack(push, 1)
typedef struct
{
	u32	time;
	u8	reg;
	union
	{
		struct
		{
			u8	flags;
			struct
			{
				u8	volL,volR;
				u16	pitch;
				u8	srcn;
			} voice[8];
		};

		struct {u8 volL,volR;};

		u16	pitch;
	};
} RegLog;
#pragma pack(pop)

//---------------------------------------------------------------------------
class TFrmMIDI : public TForm
{
__published:
	TTrackBar		*trkTempo;
	TGroupBox		*grpTempo;
	TRadioButton	*rdoT0;
	TRadioButton	*rdoT1;
	TRadioButton	*rdoT2;
	TRadioButton	*rdoUser;
	TButton			*btnOK;
	TButton			*btnCancel;

	void	__fastcall trkTempoChange(TObject *Sender);
	void	__fastcall FormShow(TObject *Sender);
	void	__fastcall btnOKClick(TObject *Sender);

private:
	u32		__fastcall CreateMIDI(s8 *pMidi, s8 *pData);

public:
	APUFile	*pCurAPU;

			__fastcall TFrmMIDI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMIDI *FrmMIDI;
//---------------------------------------------------------------------------
#endif


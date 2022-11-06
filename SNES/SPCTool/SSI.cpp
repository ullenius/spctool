//**************************************************************************************************

#include	<vcl.h>
#include	"SPCTool.h"
#pragma	hdrstop
#include	"SSI.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//**************************************************************************************************
// Global Data

const s8	ProgLst[16][8][18]={
	{"Piano 1",          "Piano 2",      "Piano 3",        "Honky-tonk",     "Electric Piano 1","Electric Piano 2","Harpsichord",  "Clavinet"},
	{"Celesta",          "Glockenspiel", "Music Box",      "Vibraphone",     "Marimba",         "Xylophone",       "Tubular Bell", "Santur"},
	{"Organ 1",          "Organ 2",      "Organ 3",        "Church Organ",   "Reed Organ",      "Accordion",       "Harmonica",    "Bandneon"},
	{"Nylon Guitar",     "Steel Guitar", "Jazz Guitar",    "Clean Guitar",   "Muted Guitar",    "Overdrive",       "Distortion",   "Harmonics"},
	{"Acoustic Bass",    "Fingered Bass","Picked Bass",    "Fretless Bass",  "Slap Bass 1",     "Slap Bass 2",     "Synth Bass 1", "Synth Bass 2"},
	{"Violin",           "Viola",        "Cello",          "Contrabass",     "Tremolo String",  "Pizzicato String","Harp",         "Timpani"},
	{"Strings",          "Slow Strings", "Synth Strings 1","Synth Strings 2","Choir Aahs",      "Voice Oohs",      "SynVox",       "Orchestra Hit"},
	{"Trumpet",          "Trombone",     "Tuba",           "Muted Trumpet",  "French Horn",     "Brass",           "Synth Brass 1","Synth Brass 2"},
	{"Soprano Sax",      "Alto Sax",     "Tenor Sax",      "Baritone Sax",   "Oboe",            "English Horn",    "Bassoon",      "Clarinet"},
	{"Piccolo",          "Flute",        "Recorder",       "Pan Flute",      "Bottle Blow",     "Shakuhachi",      "Whistle",      "Ocarina"},
	{"Square Wave",      "Saw Wave",     "Synth Calliope", "Chiffer Lead",   "Charang",         "Solo Vox",        "5th Saw Wave", "Bass & Lead"},
	{"Fantasia",         "Warm Pad",     "Polysynth",      "Space Voice",    "Bowed Glass",     "Metal Pad",       "Halo Pad",     "Sweep Pad"},
	{"Ice Rain",         "Soundtrack",   "Crystal",        "Atmosphere",     "Brightness",      "Goblin",          "Echo Drops",   "Star Theme"},
	{"Sitar",            "Banjo",        "Shamisen",       "Koto",           "Kalimba",         "Bag Pipe",        "Fiddle",       "Shannai"},
	{"Tinkle Bell",      "Agogo",        "Steel Drum",     "Wood Block",     "Taiko",           "Melo Tom",        "Synth Drum",   "Reverse Cymbal"},
	{"Guitar Fret Noise","Breath Noise", "Seashore",       "Bird",           "Telephone",       "Helicopter",      "Applause",     "Gun Shot"}};

const s8	DrumLst[11][12][20]={
	{"C-(-1)",          "C#(-1)",        "D-(-1)",         "D#(-1)",         "E-(-1)",         "F-(-1)",          "F#(-1)",			"G-(-1)",		"G#(-1)",		"A-(-1)",			"A#(-1)",		"B-(-1)"},
	{"C-0",             "C#0",           "D-0",            "D#0",            "E-0",            "F-0",             "F#0",			"G-0",			"G#0",			"A-0",				"A#0",			"B-0"},
	{"C-1",             "C#1",           "D-1",            "D#1",            "E-1",            "F-1",             "F#1",			"G-1",			"G#1",			"A-1",				"A#1",			"Bass Drum 2"},
	{"Bass Drum 1",     "Side Stick",    "Snare Drum 1",   "Hand Clap",      "Snare Drum 2",   "Low Tom 2",       "Closed Hi-Hat",	"Low Tom 1",	"Pedal Hi-Hat",	"Mid Tom 2",		"Open Hi-Hat",	"Mid Tom 1"},
	{"High Tom 2",      "Crash Cymbal 1","High Tom 1",     "Ride Cymbal 1",  "Chinese Cymbal", "Ride Cymbal Bell","Tambourine",		"Splash Cymbal","Cowbell",		"Crash Cymbal 2",	"Vibra-slap",	"Ride Cymbal 2"},
	{"High Bongo",      "Low Bongo",     "Mute High Conga","Open High Conga","Low Conga",      "High Timbale",    "Low Timbale",	"High Agogo",	"Low Agogo",	"Cabasa",			"Maracas",		"Short High Whistle"},
	{"Long Low Whistle","Short Guiro",   "Long Guiro",     "Claves",         "High Wood Block","Low Wood Block",  "Mute Culca",		"Open Culca",	"Mute Triangle","Open Triangle",	"A#5",			"B-5"},
	{"C-6",             "C#6",           "D-6",            "D#6",            "E-6",            "F-6",             "F#6",			"G-6",			"G#6",			"A-6",				"A#6",			"B-6"},
	{"C-7",             "C#7",           "D-7",            "D#7",            "E-7",            "F-7",             "F#7",			"G-7",			"G#7",			"A-7",				"A#7",			"B-7"},
	{"C-8",             "C#8",           "D-8",            "D#8",            "E-8",            "F-8",             "F#8",			"G-8",			"G#8",			"A-8",				"A#8",			"B-8"},
	{"C-9",             "C#9",           "D-9",            "D#9",            "E-9",            "F-9",             "F#9",			"G-9",			"",				"",					"",				""}};

static const s8 Group[16][12]={
	"Piano","Chromatic","Organ",     "Guitar",   "Bass",    "Orchestra","Ensemble",  "Brass",
	"Reed", "Pipe",     "Synth Lead","Synth Pad","Synth FX","Ethnic",   "Percussive","Sound FX"};


TFrmSSI *FrmSSI;


//**************************************************************************************************
__fastcall TFrmSSI::TFrmSSI(TComponent* Owner) : TForm(Owner)
{
}


//**************************************************************************************************
void __fastcall TFrmSSI::FormShow(TObject *sender)
{
	txtRate->Text = txtRate->Tag;
	grpTypeClick(NULL);

	txtOfs->Text = IntToHex(txtOfs->Tag, 4);
	if (txtLoop->Tag == 0xFFFF)
	{
		txtLoop->Text = "(no loop)";
		txtLoop->Enabled = false;
		lblLoop->Enabled = false;
	}
	else
	{
		txtLoop->Text = txtLoop->Tag << 4;
		txtLoop->Enabled = true;
		lblLoop->Enabled = true;
	}
}


//**************************************************************************************************
void __fastcall TFrmSSI::btnOKClick(TObject *sender)
{
	if (grpType->ItemIndex)
		prog = (lstGroup->ItemIndex * 12) + lstInstr->ItemIndex;
	else
		prog = (lstGroup->ItemIndex << 3) + lstInstr->ItemIndex;
}


//**************************************************************************************************
void __fastcall TFrmSSI::grpTypeClick(TObject *sender)
{
	s32	i;


	lstGroup->Items->Clear();

	if (grpType->ItemIndex)
	{
		for (i=-1; i<10; i++)
			lstGroup->Items->Add("Octave " + AnsiString(i));

		lstGroup->ItemIndex = prog / 12;
		lstGroupClick(NULL);
		lstInstr->ItemIndex = prog % 12;
	}
	else
	{
		for (i=0; i<16; i++)
			lstGroup->Items->Add(Group[i]);

		lstGroup->ItemIndex = prog >> 3;
		lstGroupClick(NULL);
		lstInstr->ItemIndex = prog & 7;
	}
}


//**************************************************************************************************
void __fastcall TFrmSSI::txtRateExit(TObject *sender)
{
	s32	i;

	i = txtRate->Text.ToIntDef(txtRate->Tag);
	if (i < 7) i = 7;
	if (i > 128000) i = 128000;

	txtRate->Tag = i;
	txtRate->Text = AnsiString(i) + "Hz";
}


//**************************************************************************************************
void __fastcall TFrmSSI::lstGroupClick(TObject *sender)
{
	u32	i,j;
	s8	str[32];


	lstInstr->Items->Clear();
	j = lstGroup->ItemIndex;
	if (grpType->ItemIndex)
	{
		if (lstGroup->ItemIndex < 10)
			for (i=0; i<12; i++)
			{
				sprintf(str, "%i %s", (j * 12) + i, DrumLst[j][i]);
				lstInstr->Items->Add(str);
			}
		else
			for (i=0; i<8; i++)
			{
				sprintf(str, "%i %s", 120 + i, DrumLst[j][i]);
				lstInstr->Items->Add(str);
			}
	}
	else
	{
		for (i=0; i<8; i++)
		{
			sprintf(str, "%i %s", (j << 3) + i + 1, ProgLst[j][i]);
			lstInstr->Items->Add(str);
		}
	}
}


//**************************************************************************************************
void __fastcall TFrmSSI::txtOfsExit(TObject *sender)
{
	txtOfs->Tag = Hex2Int(txtOfs->Text.c_str());
	txtOfs->Text = IntToHex(txtOfs->Tag, 4);
}


//**************************************************************************************************
void __fastcall TFrmSSI::txtLoopExit(TObject *Sender)
{
	s32		i;


	i = txtLoop->Text.ToIntDef(-1) >> 4;

	if (i >= 0 && i <= 7280) txtLoop->Tag = i;
	
	txtLoop->Text = txtLoop->Tag << 4;
}


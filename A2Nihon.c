/***************************************************************************************************
* ���{��̃X�g�����O�E�t�@���N�V����                                                               *
*                                                   Copyright (C)2002 �A���t�@�c�@�v���_�N�V�����Y *
***************************************************************************************************/

#if defined	__BORLANDC__
#pragma	inline
#pragma	warn -rvl
#endif

#include	"Types.h"
#include	"A2Str.h"

//  �Љ��� A1 - DF
//
//	� � � � �  A1-A5
//	        �  A6    WO
//	� � � � �  A7-AB a-o
//	�   �   �  AC-AE ya-yo
//	�          AF    tsu
//  �          B0
//	� � � � �  B1-B5 A-O
//	� � � � �  B6-BA KA-KO
//	� � � � �  BB-BF SA-SO
//	� � � � �  C0-C4 TA-TO
//	� � � � �  C5-C9 NA-NO
//	� � � � �  CA-CE HA-HO
//	� � � � �  CF-D3 MA-MO
//	�   �   �  D4-D6 YA-YO
//  � � � � �  D7-DB RA-RO
//  �       �  DC-DD WA N
//	� �        DE-DF

//**************************************************************************************************
//�@�\�̃��[�}�ɑ�������

static const char kRomaji[][4]={				//Characters B1 - DD
	"A",  "I",  "U",  "E",  "O",
	"KA", "KI" ,"KU", "KE", "KO",
	"SA", "SHI","SU", "SE", "SO",
	"TA", "CHI","TSU","TE", "TO",
	"NA", "NI", "NU", "NE", "NO",
	"HA", "HI", "FU", "HE", "HO",
	"MA", "MI", "MU", "ME", "MO",
	"YA",       "YU",       "YO",
	"RA", "RI", "RU", "RE", "RO",
	"WA",
	"N"};

static const char kDakuten[][4]={				//Characters B6 - CE paired with DE
	"GA", "GI", "GU", "GE", "GO",
	"ZA", "JI", "ZU", "ZE", "ZO",
	"DA", "JI", "ZU", "DE", "DO",
	"",   "",   "",   "",   "",
	"BA", "BI", "BU", "BE", "BO"};

static const char kHandakuten[][4]={			//Characters CA - CE paired with DF
	"PA", "PI", "PU", "PE", "PO"};

static const char kYA[][3][4]={					//Characters B7, BC, C1, C6, CB, D0, and D8
	{"KYA","KYU","KY0"},						// paired with AC - AE
	{"SHA","SHU","SHO"},
	{"CHA","CHU","CHO"},
	{"NYA","NYU","NYO"},
	{"HYA","HYU","HYO"},
	{"MYA","MYU","MYO"},
	{"RYA","RYU","RYO"},
	{"GYA","GYU","GYO"},						//  " " B7, BC, and CB with DE
	{"JA", "JU", "JO"},
	{"BYA","BYU","BYO"},
	{"PYA","PYU","PYO"}};						//  " " CB with DF

static const char kForeign[][4]={				//Characters paired with A7 - AB
	"",   "WI", "",   "WE", "WO",
	"KWA","KWI","",   "KWE","",
	"TSA","",   "",   "TSE","TSO",
	"FA", "FI", "",   "FE", "FO",
	"VA", "VI", "",   "VE", "VO"};

static const char kPunc[]=".\"\", "; 			//Characters A1 - A5


//**************************************************************************************************
//�@�Љ����̓��[�}���ɕϊ�����

void Romaji(u8 *t, u8 *s)
{
	s32	i;
	u8	c;
	b8	dc=0;


	while(*s)
	{
		c = *s++;

		//Regular ASCII ========================
		if (c<=0xA0 || c>=0xE0) *t++ = c;		//If c is regular ASCII, pass it through
		else
		//Large Katakana =============== [B1-DD]
		if (c>='�' && c<='�')					//If c is large Katakana...
		{
			//Tenten ---------------------------
			if (*s=='�')
			{
				s++;
				if (c>='�' && c<='�')			//c must be KA - HO
				{
					if (*s>='�' && *s<='�')		//If s is small YA - YO
					{
						switch (c)
						{
						case('�'):				//If c is GI
							CopyStr((char*)t, kYA[7][*s-'�']);
							break;

						case('�'):				//If c is JI
							CopyStr((char*)t, kYA[8][*s-'�']);
							break;

						case('�'):				//If c is BI
							CopyStr((char*)t, kYA[9][*s-'�']);

						case('�'):			  	// VU
							if (*s=='�') CopyStr((char*)t,"VU");
							break;

						case('�'):			  	// DE
							if (*s=='�') CopyStr((char*)t,"DYU");
							break;

						case('�'):			  	// DO
							if (*s=='�') CopyStr((char*)t,"DU");
							break;
						}
						s++;
					}
					else
					if (*s>='�' && *s<='�')		//If s is small A - O
					{
						switch(c)
						{
						case('�'):			  	// VU
							CopyStr((char*)t, kForeign[20+*s-'�']);
							break;

						case('�'):			  	// DE
							if (*s=='�') CopyStr((char*)t,"DI");
							break;

						case('�'):			  	// JI
							if (*s=='�') CopyStr((char*)t,"JE");
							break;
						}
						s++;
					}
					else
						CopyStr((char*)t, kDakuten[c-'�']);
				}
			}
			else
			//Hantenten ------------------------
			if (*s=='�')
			{
				s++;
				if (c>='�' && c<='�')			//c must be HA - HO
				{
					if (*s>='�' && *s<='�')		//If s is a small YA - YO, c must be PI
					{
						if (c=='�')
							CopyStr((char*)t, kYA[10][*s-'�']);
						s++;
					}
					else
						CopyStr((char*)t, kHandakuten[c-'�']);
				}
			}
			else
			//Small YA - YO --------------------
			if (*s>='�' && *s<='�')
			{
				switch (c)
				{
				case('�'):						//If c is KI, SHI, CHI, NI, HI, or MI
				case('�'):
				case('�'):
				case('�'):
				case('�'):
				case('�'):
					CopyStr((char*)t, kYA[(c-'�')/5][*s-'�']);
					break;

				case('�'):						// RI
					CopyStr((char*)t, kYA[6][*s-'�']);
					break;

				case('�'):						// TO
					if (*s=='�') CopyStr((char*)t,"TU");
					break;

				case('�'):						// FU
					if (*s=='�') CopyStr((char*)t,"FYU");
					break;
				}

				s++;
			}
			else
			//Small A - O ----------------------
			if (*s>='�' && *s<='�')
			{
				switch(c)
				{
				case('�'):						// U
					CopyStr((char*)t,kForeign[*s-'�']);

				case('�'):						// KU
					CopyStr((char*)t,kForeign[5+*s-'�']);
					break;

				case('�'):						// TSU
					CopyStr((char*)t,kForeign[10+*s-'�']);
					break;

				case('�'):						// FU
					CopyStr((char*)t,kForeign[15+*s-'�']);
					break;

				case('�'):						// TE
					if (*s=='�') CopyStr((char*)t,"TI");
					break;

				default:
					if (*s!='�') break;

					switch(c)
					{
					case('�'):					// I
						CopyStr((char*)t,"YE");
						break;

					case('�'):					// KI
						CopyStr((char*)t,"KYE");
						break;

					case('�'):					// SHI
						CopyStr((char*)t,"SHE");
						break;

					case('�'):					// CHI
						CopyStr((char*)t,"CHE");
						break;

					case('�'):					// HI
						CopyStr((char*)t,"HYE");
						break;
					}
				}
				s++;
			}
			//No special characters ------------
			else
			{
				if (t[-1]=='N' && c<='�')		//Don't confuse N'A - N'O with NA - NO
					*t++ = '\'';
				CopyStr((char*)t, kRomaji[c-'�']);
			}

			if (dc)								//If this sound is a double consonant
			{
				dc = 0;

				for (i=1+StrEnd((char*)t)-t; i; i--)	//Duplicate first Roman character
					t[i] = t[i-1];

				if (*t=='C') *t = 'T';			//CHI becomes TCHI
			}

			t = StrEnd((char*)t);
		}
		else
		//Small TSU (double consonant) ==== [AF]
		if (c=='�')
			dc = 1;
		else
		//Long vowel ====================== [B0]
		if (c=='�' && t[-1]!='N')
			*t++ = *(t-1);						//Copy last vowel
		else
		//Punctuation ================== [A1-A5]
		if (c>='�' && c<='�')
			*t++ = kPunc[c-'�'];
		else
		//WO ============================== [A6]
		if (c=='�') *t++ = 'O';
	}

	*t = 0;
}


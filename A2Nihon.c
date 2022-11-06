/***************************************************************************************************
* 日本語のストリング・ファンクション                                                               *
*                                                   Copyright (C)2002 アルファツ　プロダクションズ *
***************************************************************************************************/

#if defined	__BORLANDC__
#pragma	inline
#pragma	warn -rvl
#endif

#include	"Types.h"
#include	"A2Str.h"

//  片仮名 A1 - DF
//
//	｡ ｢ ｣ ､ ･  A1-A5
//	        ｦ  A6    WO
//	ｧ ｨ ｩ ｪ ｫ  A7-AB a-o
//	ｬ   ｭ   ｮ  AC-AE ya-yo
//	ｯ          AF    tsu
//  ｰ          B0
//	ｱ ｲ ｳ ｴ ｵ  B1-B5 A-O
//	ｶ ｷ ｸ ｹ ｺ  B6-BA KA-KO
//	ｻ ｼ ｽ ｾ ｿ  BB-BF SA-SO
//	ﾀ ﾁ ﾂ ﾃ ﾄ  C0-C4 TA-TO
//	ﾅ ﾆ ﾇ ﾈ ﾉ  C5-C9 NA-NO
//	ﾊ ﾋ ﾌ ﾍ ﾎ  CA-CE HA-HO
//	ﾏ ﾐ ﾑ ﾒ ﾓ  CF-D3 MA-MO
//	ﾔ   ﾕ   ﾖ  D4-D6 YA-YO
//  ﾗ ﾘ ﾙ ﾚ ﾛ  D7-DB RA-RO
//  ﾜ       ﾝ  DC-DD WA N
//	ﾞ ﾟ        DE-DF

//**************************************************************************************************
//　表のローマに相当する

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
//　片仮名はローマ字に変換する

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
		if (c>='ｱ' && c<='ﾝ')					//If c is large Katakana...
		{
			//Tenten ---------------------------
			if (*s=='ﾞ')
			{
				s++;
				if (c>='ｶ' && c<='ﾎ')			//c must be KA - HO
				{
					if (*s>='ｬ' && *s<='ｮ')		//If s is small YA - YO
					{
						switch (c)
						{
						case('ｷ'):				//If c is GI
							CopyStr((char*)t, kYA[7][*s-'ｬ']);
							break;

						case('ｼ'):				//If c is JI
							CopyStr((char*)t, kYA[8][*s-'ｬ']);
							break;

						case('ﾋ'):				//If c is BI
							CopyStr((char*)t, kYA[9][*s-'ｬ']);

						case('ｳ'):			  	// VU
							if (*s=='ｮ') CopyStr((char*)t,"VU");
							break;

						case('ﾃ'):			  	// DE
							if (*s=='ｭ') CopyStr((char*)t,"DYU");
							break;

						case('ﾄ'):			  	// DO
							if (*s=='ｭ') CopyStr((char*)t,"DU");
							break;
						}
						s++;
					}
					else
					if (*s>='ｧ' && *s<='ｫ')		//If s is small A - O
					{
						switch(c)
						{
						case('ｳ'):			  	// VU
							CopyStr((char*)t, kForeign[20+*s-'ｧ']);
							break;

						case('ﾃ'):			  	// DE
							if (*s=='ｨ') CopyStr((char*)t,"DI");
							break;

						case('ｼ'):			  	// JI
							if (*s=='ｪ') CopyStr((char*)t,"JE");
							break;
						}
						s++;
					}
					else
						CopyStr((char*)t, kDakuten[c-'ｶ']);
				}
			}
			else
			//Hantenten ------------------------
			if (*s=='ﾟ')
			{
				s++;
				if (c>='ﾊ' && c<='ﾎ')			//c must be HA - HO
				{
					if (*s>='ｬ' && *s<='ｮ')		//If s is a small YA - YO, c must be PI
					{
						if (c=='ﾋ')
							CopyStr((char*)t, kYA[10][*s-'ｬ']);
						s++;
					}
					else
						CopyStr((char*)t, kHandakuten[c-'ﾊ']);
				}
			}
			else
			//Small YA - YO --------------------
			if (*s>='ｬ' && *s<='ｮ')
			{
				switch (c)
				{
				case('ｷ'):						//If c is KI, SHI, CHI, NI, HI, or MI
				case('ｼ'):
				case('ﾁ'):
				case('ﾆ'):
				case('ﾋ'):
				case('ﾐ'):
					CopyStr((char*)t, kYA[(c-'ｷ')/5][*s-'ｬ']);
					break;

				case('ﾘ'):						// RI
					CopyStr((char*)t, kYA[6][*s-'ｬ']);
					break;

				case('ﾄ'):						// TO
					if (*s=='ｭ') CopyStr((char*)t,"TU");
					break;

				case('ﾌ'):						// FU
					if (*s=='ｭ') CopyStr((char*)t,"FYU");
					break;
				}

				s++;
			}
			else
			//Small A - O ----------------------
			if (*s>='ｧ' && *s<='ｫ')
			{
				switch(c)
				{
				case('ｳ'):						// U
					CopyStr((char*)t,kForeign[*s-'ｧ']);

				case('ｸ'):						// KU
					CopyStr((char*)t,kForeign[5+*s-'ｧ']);
					break;

				case('ﾂ'):						// TSU
					CopyStr((char*)t,kForeign[10+*s-'ｧ']);
					break;

				case('ﾌ'):						// FU
					CopyStr((char*)t,kForeign[15+*s-'ｧ']);
					break;

				case('ﾃ'):						// TE
					if (*s=='ｨ') CopyStr((char*)t,"TI");
					break;

				default:
					if (*s!='ｪ') break;

					switch(c)
					{
					case('ｲ'):					// I
						CopyStr((char*)t,"YE");
						break;

					case('ｷ'):					// KI
						CopyStr((char*)t,"KYE");
						break;

					case('ｼ'):					// SHI
						CopyStr((char*)t,"SHE");
						break;

					case('ﾁ'):					// CHI
						CopyStr((char*)t,"CHE");
						break;

					case('ﾋ'):					// HI
						CopyStr((char*)t,"HYE");
						break;
					}
				}
				s++;
			}
			//No special characters ------------
			else
			{
				if (t[-1]=='N' && c<='ｵ')		//Don't confuse N'A - N'O with NA - NO
					*t++ = '\'';
				CopyStr((char*)t, kRomaji[c-'ｱ']);
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
		if (c=='ｯ')
			dc = 1;
		else
		//Long vowel ====================== [B0]
		if (c=='ｰ' && t[-1]!='N')
			*t++ = *(t-1);						//Copy last vowel
		else
		//Punctuation ================== [A1-A5]
		if (c>='｡' && c<='･')
			*t++ = kPunc[c-'｡'];
		else
		//WO ============================== [A6]
		if (c=='ｦ') *t++ = 'O';
	}

	*t = 0;
}


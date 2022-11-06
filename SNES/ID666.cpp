/***************************************************************************************************
* ID666 Tag Manager                                                                                *
*                                                      Copyright (C)2001-2005 Alpha-II Productions *
***************************************************************************************************/

#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"Types.h"
#include	"A2Str.h"
#include	"ID666.h"

//File header text
const s8			EmuList[XID6_EMULIST][12] =
					{"unknown","ZSNES","Snes9x","ZST2SPC","ETC","SNEShout","ZSNESW"};

static const s8 	SPCFileID[] = {"SNES-SPC700 Sound File Data v0.30"};
static const s8 	SPCTerm[4] = {0x1a,0x1a,0x1a,0x1e};
static const s8 	xid6[4] = {'x','i','d','6'};
static const s8 	ZSTFileID[] = {"ZSNES Save State File"};
static const s8		SFCFileID[] = {"GAME DOCTOR SF 3"};
static const u32	ROMOffset[4] = {0x7FC0, 0xFFC0, 0x81C0, 0x101C0};
#if ID666_ROM
static const char	ROMMaker[256][32];
#endif
#if 0
static const u8		IDList[] = {
		XID6_SONG,
		XID6_GAME,
		XID6_ARTIST,
		XID6_DUMPER,
		XID6_DATE,
		XID6_EMU,
		XID6_CMNTS,
		XID6_INTRO,
		XID6_LOOP,
		XID6_END,
		XID6_FADE,
		XID6_MUTE,
		XID6_LOOPX,
		XID6_AMP,
		XID6_OST,
		XID6_DISC,
		XID6_TRACK,
		XID6_PUB,
		XID6_COPY};
#endif

static ROMHdr		ROMInfo;

u32	ID666::defSong;
u32	ID666::defFade;
b8	ID666::preferBin;


////////////////////////////////////////////////////////////////////////////////////////////////////

//**************************************************************************************************
ID666::ID666()
{
	bin = false;

	file[0] = 0;

	mute = 0;
	amp = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Functions

//**************************************************************************************************
// Get ID666 Tag
//
// Retrieves the individual components of the ID666 tag from an SPC header
//
// Note:
//    This function does not modify any extended fields
//
// In:
//    -> SPC header containing ID666 tag

void ID666::GetID666(SPCHdr &spc)
{
	s32	i,j,k;
	u32	d,m,y;
	s8	str[36];
	s8	songStr[4];								//Song in seconds
	s8	fadeStr[6];								//Fade in ms
	s8*	t;


	//Copy known fields ------------------------
	str[32] = 0;
	memcpy(song, spc.song, 32);
	song[32] = 0;
	memcpy(game, spc.game, 32);
	game[32] = 0;
	memcpy(dumper, spc.dumper, 16);
	dumper[16] = 0;
	memcpy(comment, spc.comment, 32);
	comment[32] = 0;
	memcpy(songStr, spc.songLen, 3);
	memcpy(fadeStr, spc.fadeLen, 5);
	mute = 0;
	amp = 0;
	date.date = 0;

	//Determine tag type -----------------------
	i = IsText(songStr, 3);
	j = IsText(fadeStr, 5);
	k = IsText(spc.date, 11);
	if (!(i | j | k))							//If no times or date, use default
	{
		if (spc.chnDis == 1 && spc.emulator == 0)	//Was the file dumped with ZSNES?
			bin = true;
		else
			bin = preferBin;
	}
	else
	if (i != -1 && j != -1)						//If no time, or time is text
	{
		if (k > 0)
			bin = false;						//If date is text, then tag is text
		else
		if (k == 0)								//No date
			bin = preferBin;					//Times could still be binary (ex. 56 bin = '8' txt)
		else
		if (k == -1)							//Date contains invalid characters
		{
			if (!((u32*)&spc.date)[1])			//If bytes 4-7 are 0's, it's probably a ZSNES dump
				bin = true;
			else
				bin = false;					//Otherwise date may contain alpha characters
		}
	}
	else
		bin = true;								//If time is not text, tag is binary

	//Get fields dependant on tag type ---------
	if (bin)
	{
		j = *(u32*)(fadeStr);
		if (j > 59999) j = 59999;

		i = *(u16*)(songStr);
		if (i > 959) i = 959;

		intro = i;
		fade = j;

		memcpy(artist, spc.artist-1, 32);
		artist[32] = 0;
		emu = spc.chnDis;

		datetxt[0] = 0;
		y = *(u16*)(&spc.date[2]);
		m = *(u8*)(&spc.date[1]);
		d = *(u8*)(&spc.date[0]);
	}
	else
	{
		intro = atoi(songStr);
		fade = atoi(fadeStr);
		if (intro > 959) intro = 959;
		if (fade > 59999) fade = 59999;

		memcpy(artist, spc.artist, 32);
		artist[32] = 0;
		emu = spc.emulator;
		if (emu>=0x30 && emu<=0x39)
			emu -= 0x30;
		else
			emu = 0;

		memcpy(datetxt, spc.date, 11);
		y = 0;
		strcpy(str, datetxt);
		if (str[0] != 0)
		{
			t = strchr(str, '/');
			if (t && *(++t)!=0)
			{
				d = atoi(t);
				t = strchr(t, '/');
				if (t && *(++t)!=0)
				{
					y = atoi(t);
					m = atoi(str);
				}
			}
		}
	}
	SetDate(y, m, d);

	//Change song/fade length to ticks ---------
	intro *= XID6_TICKSSEC;
	fade <<= 6;

	if (!intro) fade = 0;
}


//**************************************************************************************************
// Get Extended ID666 Tag
//
// Reads the sub-chunks in an extended ID666 tag chunk
//
// In:
//    pTag -> Buffer containing XID6 chunk
//    size  = Size of buffer
//
// Out:
//    True, if file has an XID6 tag

b8 ID666::GetXID6(XID6Field* pTag, s32 size)
{
	s32		i,r;
#ifdef	TIDY
	s32			sb;
	XID6Chk*	ptb;
	b8			align;


	ptb = pTag;
	sb = size;
	align = 0;

TryAgain:
	pTag = ptb;
	size = sb;
#endif
	while(size > 0)								//While there's data left to be read
	{
#ifdef	TIDY
		r = pTag->length;
		if (align) r = (r + 3) & ~3;
#else
		r = (pTag->size + 3) & ~3;
#endif

		size -= 4;

		switch(pTag->type)
		{
		case(XID6_TSTR):
			if (pTag->size < 1 || pTag->size > 0x100) break;

			switch(pTag->id)
			{
			case(XID6_SONG):
				memcpy(song, pTag->data, size > r ? r : size);
				song[pTag->size-1] = 0;			//Force string to null terminate, incase tag is bad
				break;

			case(XID6_GAME):
				memcpy(game, pTag->data, size > r ? r : size);
				game[pTag->size-1] = 0;
				break;

			case(XID6_ARTIST):
				memcpy(artist, pTag->data, size > r ? r : size);
				artist[pTag->size-1] = 0;
				break;

			case(XID6_PUB):
				memcpy(pub, pTag->data, size > r ? r : size);
				pub[pTag->size-1] = 0;
				break;

			case(XID6_OST):
				memcpy(ost, pTag->data, size > r ? r : size);
				ost[pTag->size-1] = 0;
				break;

			case(XID6_DUMPER):
				memcpy(dumper, pTag->data, size > r ? r : size);
				dumper[pTag->size-1] = 0;
				break;

			case(XID6_CMNTS):
				memcpy(comment, pTag->data, size > r ? r : size);
				comment[pTag->size-1] = 0;
				break;
#ifdef	TIDY
			default:
				if (align && size == 4) goto Done;
#endif
			}
			break;

		case(XID6_TINT):
			if (pTag->size != 4) break;

			switch(pTag->id)
			{
			case(XID6_DATE):
				i = *pTag->data;
				SetDate(i>>16, (i>>8) & 0xFF, i & 0xFF);
				break;

			case(XID6_INTRO):
				intro = *pTag->data;
				if (intro > XID6_MAXTICKS) intro = XID6_MAXTICKS;
				break;

			case(XID6_LOOP):
				loop = *pTag->data;
				if (loop > XID6_MAXTICKS) loop = XID6_MAXTICKS;
				break;

			case(XID6_END):
				end = *pTag->data;
				if (end > XID6_MAXTICKS) end = XID6_MAXTICKS;
				break;

			case(XID6_FADE):
				fade = *pTag->data;
				if (fade > XID6_TICKSMIN-1) fade = XID6_TICKSMIN-1;
				break;

			case(XID6_AMP):
				amp = *pTag->data;
				if (amp < 32768) amp = 32768;
				if (amp > 524288) amp = 524288;
				break;
#ifdef	TIDY
			default:
				if (align && size == 4) goto Done;
#endif
			}
			break;

		case(XID6_TSIZE):
			switch(pTag->id)
			{
			case(XID6_EMU):
				emu = (s8)pTag->size;
				break;

			case(XID6_DISC):
				disc = (u8)pTag->size;
				if (disc > 9) disc = 9;
				break;

			case(XID6_TRACK):
				track = pTag->size;
				if (((track>>8)-1) > 98) track = 0;
				break;

			case(XID6_COPY):
				copy = pTag->size;
				break;

			case(XID6_MUTE):
				mute = (u8)pTag->size;
				break;

			case(XID6_LOOPX):
				loopx = pTag->size;
				if (loopx < 1) loopx = 1;
				if (loopx > 9) loopx = 9;
				break;

			case(XID6_AMP):						//Old way of storing
				amp = (u32)pTag->size << 12;
				if (amp < 32768) amp = 32768;
				if (amp > 524288) amp = 524288;
				break;
#ifdef	TIDY
			default:
				if (align && size == 4) goto Done;
#endif
			}
			break;
#ifdef	TIDY
		default:
			if (align && size == 4) goto Done;
#endif
		}

		if (pTag->type != XID6_TSIZE)
		{
			pTag = (XID6Field*)((s32)pTag + r);
			size -= r;
		}

		pTag++;									//Skip sub-chunk header
	}

Done:
	if (end < -((s32)loop))						//If end is negative and |end| > loop
		end = loop ? (end % (s32)loop) : 0;

#ifdef	TIDY
	if (size != 0)
	{
		if (!align)
		{
			align = 1;
			goto TryAgain;
		}

		if (size == 4) printf("Bad tail?  ");
		else           printf("**corrupt  ");
	}
#endif

	return (size == 0);
}


//**************************************************************************************************
// Is Number in Text Form?
//
// Scans a string for characters other than '0'-'9', '\', and NULL
//
// In:
//    str-> Character string to scan
//    len = Length of string
//
// Out:
//    -1, if string contains invalid characters
//    length of string, otherwise

s32 ID666::IsText(s8* str, u32 length)
{
	u32	c = 0;

	while (c<length && ((str[c]>=0x30 && str[c]<=0x39) || str[c]=='/')) c++;
	if (c==length || str[c]==0)
		return c;
	else
		return -1;
}


//**************************************************************************************************
// Write XID6 Size
//
// Writes a 4-byte subchunk header, if 'data' is !0.
//
// In:
//    pChk-> Buffer
//    id   = Subchunk ID
//    data = 16-bit data to be written in the subchunk header
//
// Out:
//    -> next subchunk position

XID6Field* ID666::WriteSize(XID6Field* pChk, u8 id, u16 data)
{
	if (data != 0)
	{
		pChk->id = id;
		pChk->type = XID6_TSIZE;
		pChk->size = data;
		pChk++;
	}

	return pChk;
}


//**************************************************************************************************
// Write XID6 Integer
//
// Writes a 4-byte subchunk, if data is !0.
//
// In:
//    pChk-> Buffer
//    id   = Subchunk ID
//    data = 32 bits of data to be written
//
// Out:
//    -> next subchunk position

XID6Field* ID666::WriteInt(XID6Field* pChk, u8 id, u32 data)
{
	if (data != 0)
	{
		pChk->id = id;
		pChk->type = XID6_TINT;
		pChk->size = 4;

		((u32*)pChk)[1] = data;
		pChk += 2;
	}

	return pChk;
}


//**************************************************************************************************
// Write XID6 String
//
// Writes a string subchunk.  Strings must be <= 256 bytes including NULL.  Empty strings will not
// be written.
//
// In:
//    pChk-> Buffer
//    id   = Subchunk ID
//    str -> String to write
//
// Out:
//    -> next subchunk position

XID6Field* ID666::WriteStr(XID6Field* pChk, u8 id, const s8* str)
{
	u32		i,j;


	i = strlen(str);
	if (i != 0)
	{
		j = (i + 4) >> 2;

		pChk->id = id;
		pChk->type = XID6_TSTR;
		pChk->size = i + 1;

		((u32*)pChk)[j] = 0;					//Set last u32 to 0 to force NULL teminator
		memcpy(&pChk[1], str, i);				//Copy characters from string
		pChk += j + 1;
	}

	return pChk;
}
/*
XID6Chk* ID666::WriteWStr(XID6Chk* pChk, u8 id, const s8* str)
{
	u32		i,j;


	i = wcslen(str);
	if (i != 0)
	{
		i <<= 1;
		j = (i + 4) >> 2;

		pChk->id = id;
		pChk->type = XID6_TWSTR;
		pChk->size = i + 2;

		((u32*)pChk)[j] = 0;
		memcpy(&pChk[1], str, i);
		pChk += j + 1;
	}

	return pChk;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions

//**************************************************************************************************
ID6Type ID666::LoadTag(const s8* fn, u32 size)
{
	ID6Type		type;
	FILE*		fh;
	u32			i,n;
	XID6Field*	pTag;
	SPCHdr		spc;


	if (!size)
		type = IsSPC(fn, fh);
	else
		type = IsSPC(fn, size);

	if (type > 0)
	{
		if (!size) strcpy(file, fn);

		if (type==ID6_SPC || type==ID6_EXT)
		{
			bin = false;

			if (!size)
			{
				fread(&spc, sizeof(SPCHdr), 1, fh);
				GetID666(spc);
			}
			else
				GetID666(*(SPCHdr*)fn);

			ost[0]	= 0;
			pub[0]	= 0;
			copy	= 0;
			disc	= 0;
			track	= 0;
			amp		= 0;
			loop	= 0;
			end		= 0;
			loopx	= 1;
			mute	= 0;

			if (type == ID6_EXT)
			{
				if (!size)
				{
					fseek(fh, 0x10204, SEEK_SET);
					fread(&i, 1, 4, fh);

					//Truncate header size, if it's corrupt
					fseek(fh, 0, SEEK_END);
					n = ftell(fh) - 0x10208;
					if (i > n) i = n;

					//Load xid6 chunk into memory
					pTag = (XID6Field*)malloc((i + 3) & ~3);
					fseek(fh, 0x10208, SEEK_SET);
					fread(pTag, i, 1, fh);
					GetXID6(pTag, i);
					free(pTag);
				}
				else
				{
					i = *(u32*)&fn[0x10204];
					if (i > (size - 0x10208)) i = size - 0x10208;

					GetXID6((XID6Field*)&fn[0x10208], i);
				}
			}
		}
		else
		{
			song[0]		= 0;
			game[0]		= 0;
			artist[0]	= 0;
			dumper[0]	= 0;
			comment[0]	= 0;
			ost[0]		= 0;
			pub[0]		= 0;
			datetxt[0]	= 0;
			date.date	= 0;
			copy		= 0;
			track		= 0;
			disc		= 0;
			amp			= 0;
			mute		= 0;
			emu			= 0;

			intro		= defSong;
			loop		= 0;
			loopx		= 1;
			end			= 0;
			fade		= defFade;

#if	ID666_ROM
			if (type>=ID6_ROM && type<=ID6_SF3)
			{
				game[21] = 0;
				memcpy(game, romID.name, 21);
				strcpy(pub, romMaker[romID.maker]);
			}
#endif
		}

		if (!size) fclose(fh);
	}

	return type;
}


//**************************************************************************************************
#if 0
static b8 Search(u8 type)
{
	for (int i=0; i<sizeof(IDList); i++)
		if (type == IDList[i]) return 1;

	return 0;
}
#endif

b8 ID666::SaveTag(b8 bin, b8 ext)
{
	u32*		pBuf;
	XID6Field*	p;
	FILE*		fh;
	u32			size,xsize,i,j;
//	u32			s;
//	b8			u;
	u32			xbuf[2048];


	//Load SPC ---------------------------------
	fh = fopen(file, "rb");
	if (!fh) return 0;
	fseek(fh, 0, SEEK_END);
	size = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	pBuf = (u32*)malloc(size);
	fread(pBuf, size, 1, fh);
	fclose(fh);

	//Write ID666 tag --------------------------
	ToSPC(*(SPCHdr*)pBuf, bin);					//Create SPC header

	//Write SPC back out -----------------------
	fh = fopen(file, "wb");
	if (!fh) return 0;
	fwrite(pBuf, 0x10200, 1, fh);

	//Write extended tag -----------------------
	if (ext)
	{
		p = (XID6Field*)&xbuf[2];
		if (strlen(song) > 32)		p = WriteStr(p, XID6_SONG, song);
		if (strlen(game) > 32)		p = WriteStr(p, XID6_GAME, game);
		if (strlen(artist) > 32)	p = WriteStr(p, XID6_ARTIST, artist);
		if (strlen(dumper) > 16)	p = WriteStr(p, XID6_DUMPER, dumper);
		if (strlen(comment) > 32 ||
			strchr(comment, '\n'))	p = WriteStr(p, XID6_CMNTS, comment);
		//*** Date and emulator will always be written in the original tag ***
//		p = WriteXChk(p, XID6_EMU, id6.emu);
//		id6.date.GetDate(y,m,d);
//		p = WriteInt(p, XID6_DATE, (y<<16) | ((m&0xFF)<<8) | (d&0xFF));

		if (ost) p = WriteStr(p, XID6_OST, ost);
		p = WriteSize(p, XID6_DISC, disc);
		p = WriteSize(p, XID6_TRACK, track);
		p = WriteSize(p, XID6_COPY, copy);
		p = WriteStr(p, XID6_PUB, pub);
		p = WriteSize(p, XID6_MUTE, mute);
		p = WriteInt(p, XID6_AMP, amp);

		if (end < (s32)(0 - loop))				//If 'end' is negative, it can't be more than loop
			end = loop ? (end % (s32)loop) : 0;

		p = WriteInt(p, XID6_INTRO, intro);
		p = WriteInt(p, XID6_LOOP, loop);
		p = WriteInt(p, XID6_END, end);
		p = WriteInt(p, XID6_FADE, fade);
		if (loopx > 1)
			p = WriteSize(p, XID6_LOOPX, loopx);

		xsize = (u32)p - (u32)xbuf - 8;
		if (xsize)
		{
			xbuf[0] = *(u32*)xid6;				//Chunk ID
			xbuf[1] = xsize;					//Chunk size
			fwrite(xbuf, 1, xsize + 8, fh);
		}
	}

	//Write out any extra data -----------------
	//For forward compatibility, any data at the end of the file, in addition to an xid6 chunk, is
	//copied.  If the file contained an xid6 chunk, the chunk is scanned for unknown fields (fields
	//that may be added in the future) and those are copied as well.

	if (size > 0x10200)							//Was file larger than regular SPC size?
	{
		i = 0x10200;

		//First check if the file had an xid6 chunk
		if (size >= 0x10208 &&
			pBuf[0x10200 >> 2] == *(u32*)xid6)
		{
			j = pBuf[0x10204 >> 2];

			//If the size of the xid6 chunk is invalid, we can't assume anything.
			if (!(j & 3) && j <= (size - 0x10208))
			{
				i += j + 8;						//Skip past the xid6 chunk
#if 0
				//This code copies any unknown ID666 fields to make sure future fields will remain
				// intact, even in if the editor doesn't support them yet.  However, I haven't
				// gotten around to testing this code, and I don't see the xid6 chunk changing any
				// time in the near future.
				if (ext)						//If the user wanted to save the xid6 chunk, search
				{								// for and copy potentially unknown (future) fields.
					p = (XID6Field*)&pBuf[0x10208 >> 2];
					j = 0x10208;
					u = 0;
					while (j < size)
					{
						s = (p->size + 3) & ~3;
						if (Search(p->id))
						{
							if (p->type)
							{
								p += s >> 2;
								j += s;
							}

							p++;
							j += 4;
						}
						else
						{
							if (p->type)
							{
								j += 4 + s;

								if (j <= size)
								{
									fwrite(p, 1, 4 + s, fh);
									xsize += 4 + s;
									u = 1;
								}

								p += 1 + (s >> 2);
							}
							else
							{
								fwrite(p, 1, 4, fh);
								xsize += 4;
								j += 4;
								p++;
								u = 1;
							}
						}
					}

					if (u)
					{
						j = ftell(fh);
						fseek(fh, 0x10204, SEEK_SET);
						fwrite(&xsize, 4, 1, fh);
						fseek(fh, j, SEEK_SET);
					}
				}
#endif
			}
			else
				i = size;						//xid6 chunk was invalid, ingnore any trailing data
		}

		//If any extra data remains, write it out
		if (i < size) fwrite(&pBuf[i >> 2], 1, size-i, fh);
	}

	fclose(fh);
	free(pBuf);

	return 1;
}


//**************************************************************************************************
ID666* ID666::operator=(const ID666& id6)
{
	strcpy(file, id6.file);

	bin		= id6.bin;

	strcpy(song, id6.song);
	strcpy(game, id6.game);
	strcpy(artist, id6.artist);
	strcpy(dumper, id6.dumper);
	strcpy(datetxt, id6.datetxt);
	date	= id6.date;
	emu		= id6.emu;
	strcpy(comment, id6.comment);
	strcpy(ost,id6.ost);
	strcpy(pub, id6.pub);
	copy	= id6.copy;
	disc	= id6.disc;
	track	= id6.track;
	mute	= id6.mute;
	amp		= id6.amp;

	intro	= id6.intro;
	loop	= id6.loop;
	end		= id6.end;
	fade	= id6.fade;
	loopx	= id6.loopx;

	return this;
}


//**************************************************************************************************
u32 ID666::GetSong()
{
	u32 time = intro + (loop * loopx) + end;

	return time ? time : defSong;
}


//**************************************************************************************************
u32 ID666::GetFade()
{
	return (intro | loop | end) ? fade : defFade;
}


//**************************************************************************************************
u32 ID666::GetTotal()
{
	u32	time = intro + (loop * loopx) + end + fade;

	return time ? time : defSong+defFade;
}


//**************************************************************************************************
b8 ID666::SetDate(u32 year, u8 month, u8 day)
{
	//Validate fields --------------------------
	if (year < 100)								//Add century, if not included
	{
		year += 1900;
		if (year < 1998) year += 100;
	}

	if (year < 1998 || year > 9999) year = 0;	//Invaild year
	if (month < 1 || month > 12) year = 0;		//Invalid month
	if (day < 1 || day > 31) year = 0;			//Invalid day

	if (year == 1998)	  						//SPC's didn't exist before 15 Apr 1998
		if ((month==4 && day<15) || month<4) year = 0;

	//Set date ---------------------------------
	if (year)
	{
		date.year = (s16)year;
		date.month = (s8)month;
		date.day = (s8)day;
		datetxt[0] = 0;

		return true;
	}
	else
		return false;
}


//**************************************************************************************************
// Create SPC Header from ID666
//
// Reduces the attributes of ID666 into a writeable form for the SPC header
//
// Notes:
//    Song and fade length are taken from songMin, songSec, and fadeSec (see FixTime).
//    No range checking is performed on any numeric values.
//
// In:
//    spc -> SPC header containing ID666 tag
//    bin  = Create a binary tag
//
// Out:
//    nothing

void ID666::ToSPC(SPCHdr &spc, b8 bin)
{
	s8*	cr;
	u32	i;

	strcpy(spc.fTag, SPCFileID);
	memcpy(spc.tTerm, SPCTerm,4);
	memset(spc.song, 0, 0xa5);

	if (bin)
	{
		i = strlen(artist);
		if (i > 32) i = 32;
		memcpy(spc.artist-1, artist, i);

		*(u32*)(&spc.songLen[0]) = intro / XID6_TICKSSEC;
		*(u32*)(&spc.fadeLen[0]) = fade >> 6;

		if (date.date)
		{
			*(u32*)&spc.date = date.date;
		}

		spc.chnDis = emu;
		spc.artist[31] = 0;
	}
	else
	{
		if (date.date)
		{
			sprintf(spc.date, "%i/%i/%i", date.month, date.day, date.year);
		}
		else
			strcpy(spc.date, datetxt);

		if (HasTime())
		{
			sprintf(spc.songLen, "%i", intro / XID6_TICKSSEC);
			sprintf(spc.fadeLen, "%i", fade >> 6);
		}

		i = strlen(artist);
		if (i > 32) i = 32;
		memcpy(spc.artist, artist, i);
		spc.emulator = emu + 0x30;
		spc.chnDis = 0;
	}

	i = strlen(song);
	if (i > 32) i = 32;
	memcpy(spc.song, song, i);

	i = strlen(game);
	if (i > 32) i = 32;
	memcpy(spc.game, game, i);

	i = strlen(dumper);
	if (i > 16) i = 16;
	memcpy(spc.dumper, dumper, i);

	i = strlen(comment);
	if (i > 32) i = 32;
	memcpy(spc.comment, comment, i);

	cr = strchr(spc.comment, '\n');
	if (cr) *cr = 0;
}


//**************************************************************************************************
b8 ID666::IsExt()
{
	return	(strlen(song) > 32) ||
			(strlen(game) > 32) ||
			(strlen(artist) > 32) ||
			(strlen(dumper) > 32) ||
			(strlen(comment) > 32) || //TODO: Check for CR
			(intro % XID6_TICKSSEC) ||
			(ost[0]	!= 0) ||
			(pub[0]	!= 0) ||
			(copy	!= 0) ||
			(disc	!= 0) ||
			(track	!= 0) ||
			(mute	!= 0) ||
			(amp	!= 0) ||
			(loop	!= 0) ||
			(end	!= 0);
}


//**************************************************************************************************
b8 ID666::HasTime()
{
	return (intro | loop | end | fade) != 0;
}


//**************************************************************************************************
// Parse Title Format String

//--------------------------------------------------------------------------------------------------
// Copy ID666 Field
//
// Copies the string form of an ID666 field to a destination string.  A specific number of
// characters will be copied if the user specified a constraint.
//
// In:
//    pDest -> Destination string
//    pSrc  -> String to copy
//    ppFmt -> Pointer to character after field specifier in format string
//
// Out:
//    -> end of pDest string

static s8* CopyField(s8* pDest, const s8* pSrc, const s8** ppFmt)
{
	const s8*	s;
	
	u32	i;
	s8	f;


	i = 0;										//Required length = 0
	f = **ppFmt;								//f = length type

	if (f=='<' || f=='=' || f=='>')				//Is a length specified?
	{
		s = (*ppFmt)+1;
		i = atoi(s);							//i = number of characters to copy
		while (*s >= '0' && *s <= '9') s++;		//Skip *len past digits
		*ppFmt = s;
	}

	if (!i) return CopyStr(pDest, pSrc);		//If no length, perform a normal copy

	switch(f)
	{
	case('<'):
		return CopyStrL(pDest, pSrc, i);

	case('>'):
		pDest = CopyStrL(pDest, pSrc, 0-i);
		if (strlen(pSrc) <= i) return pDest;
		return CopyStr(pDest, pSrc+i);

	case('='):
		return CopyStrL(pDest, pSrc, 0-i);
	}

	return NULL;
}


//--------------------------------------------------------------------------------------------------
// Find End of Conditional Statement
//
// In:
//    str   -> Format string
//    pIf   -> Length of "if" portion of condition
//    pElse -> Length of "else" portion of condition
//
// Out:
//    Number of characters in condition

static u32 FindEnd(const s8* str, u32* pIf, u32* pElse)
{
	u32	len,i,j;
	s8	c;
	b8	or;


	*pIf = 0;
	*pElse = 0;

	len = 0;
	or = 0;
	while (*str)
	{
		switch(*str++)
		{
		case('%'):
			c = *str++ | 0x20;
			len += 2;

			if (*str == '[' && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'd')))
			{
				len++;
				i = FindEnd(++str, &j, &j);
				len += i;
				str += i;
			}
			break;

		case(']'):
			if (or)
			{
				*pElse = len;
				len += *pIf + 2;
			}
			else
			{
				*pIf = len;
				len += 1;
			}

			return len;

		case('|'):
			if (!or)
			{
				*pIf = len;
				len = 0;
				or = 1;
				break;
			}

		default:
			len++;
		}
	}

	return 0;
}

#define	_CopyPath() \
			if (cnd) \
			{ \
				cpy |= ToStr(&str, pFmt, f); \
				pFmt += cnd; \
				cnd = 0; \
			}\
			else

#define	_CopyField(_field, _string, _printf) \
			if (cnd) \
			{ \
				cpy |= (_field) ? ToStr(&str, pFmt, f) : ToStr(&str, &pFmt[f+1], e); \
				pFmt += cnd; \
				cnd = 0;\
			} \
			else \
			if (_field) \
			{ \
				_printf; \
				str = CopyField(str, _string, &pFmt); \
				cpy |= 1; \
			} \
			else \
			{ \
				*str++ = '?'; \
			}

b8 ID666::ToStr(s8** ppStr, const s8* pFmt, u32 len)
{
	u32	i;
	u32	cnd,f,e;							//Length of statement, and contained if and else
	s8*	str;
	const s8*	end;
	s8	c;
	b8	cpy=0;								//True, if any fields were copied
	s8	temp[12];


	cnd = 0;								//Length of conditional statement is 0
	str = *ppStr;
	end = &pFmt[len];
	while (pFmt < end)
	{
		if (*pFmt != '%')					//Is current character a field delimiter?
		{
			*str++ = *pFmt++;
			continue;
		}

		c = pFmt[1];
		pFmt += 2;							//Move pointer to first char after field specifier

		//Check for special characters -----
		switch(c)
		{
		case('%'):
			*str++ = '%';
			continue;

		case('<'):
			*str++ = '<';
			continue;

		case('='):
			*str++ = '=';
			continue;

		case('>'):
			*str++ = '>';
			continue;

		case('['):
			*str++ = '[';
			continue;

		case(']'):
			*str++ = ']';
			continue;

		case('|'):
			*str++ = '|';
			continue;
		}

		//Check for unknown format specifier ---
		c |= 0x20;
		if ((c < '0' || c > '9') && (c < 'a' || c > 'd'))
		{
			*str++ = '%';
			pFmt--;
			continue;
		}

		if (pFmt[0] == '[') cnd = FindEnd(++pFmt, &f, &e);

		switch(c)
		{
		case('0'):							//Path
			_CopyPath()
			{
				CopyField(str, file, &pFmt);
				str = ScanStrR(str, '\\');
				cpy |= 1;
			}
			break;

		case('1'):							//Filename
			_CopyPath()
			{
				str = CopyField(str, ScanStrR(file, '\\'), &pFmt);
				cpy |= 1;
			}
			break;

		case('2'):							//Song str
			_CopyField(song[0], song,);
			break;

		case('3'):							//Game str
			_CopyField(game[0], game,);
			break;

		case('4'):							//Artist
			_CopyField(artist[0], artist,);
			break;

		case('5'):							//Publisher
			_CopyField(pub[0], pub,);
			break;

		case('6'):							//Copyright
			_CopyField(copy, temp, sprintf(temp, "©%i", copy));
			break;

		case('7'):							//OST str
			_CopyField(ost[0], ost,);
			break;

		case('8'):							//OST disc
			_CopyField(disc, temp, sprintf(temp, "%i", disc));
			break;

		case('9'):							//OST track
			_CopyField(track, temp, sprintf(temp, "%i%c", track >> 8, track & 0xFF));
			break;

		case('a'):							//Dumper
			_CopyField(dumper[0], dumper,);
			break;

		case('b'):							//Date dumped
			if (cnd)
			{
				cpy |= date.date ? ToStr(&str, pFmt, f) : ToStr(&str, &pFmt[f+1], e);
				pFmt += cnd;
				cnd = 0;
			}
			else
			if (date.date)
			{
				sprintf(temp, "%i.%02i.%02i", date.year, date.month, date.day);
				str = CopyField(str, temp, &pFmt);
				cpy |= 1;
			}
			else
				_CopyField(datetxt[0], datetxt,);
			break;

		case('c'):							//Emulator used
			_CopyField(emu <= 6, EmuList[emu],);
			break;

		case('d'):							//Comment
			_CopyField(comment[0], comment,);
			break;
		}
	}

	*str = 0;

	if (cpy)
	{
		*ppStr = str;
    	return 1;
	}

	return 0;
}

b8 ID666::ToStr(s8* str, const s8* format)
{
	if (ToStr(&str, format, strlen(format))) return 1;

	CopyStr(str, ScanStrR(file, '\\'));
	return 0;
}


//**************************************************************************************************
// Does File Contain SPC Data?

ID6Type IsSPC(const s8* fn, FILE*& fh)
{
	s8		header[28];
	s32		i,l;


	//Open file and read header ----------------
	fh = fopen(fn,"rb");
	if (!fh) return ID6_ERR;

	fread(header, 28, 1, fh);
	fseek(fh, 0, SEEK_END);
	l = ftell(fh);
	fseek(fh, 0 ,SEEK_SET);

	if (l < 0x10200) goto Done;					//If file is too small to be anything

	//Look for SPC header ----------------------
	if (CmpMem(header, SPCFileID, 28))
	{
		if (l < 0x1020C) return ID6_SPC;		//Return normal SPC

		fseek(fh, 0x10200, SEEK_SET);			//Look for extended information
		fread(&header, 1, 8, fh);
		fseek(fh, 0, SEEK_SET);

		if (*(u32*)&header[0] == *(u32*)xid6 &&
			*(u32*)&header[4] >= 4) return ID6_EXT;

		return ID6_SPC;
	}

	//Look for ZST header ----------------------
	if (CmpMem(header, ZSTFileID, 21))
	{
		if (l < 266879)	goto Done;				//Return invalid, if file is too small

		return ID6_ZST;
	}

	//Is it a ROM image? -----------------------
#if	ID666_ROM
	if (l < 0x40000) goto Done;					//If file is too small to be a valid ROM (2Mbit)

	for (i=0; i<4; i++)
	{
		fseek(fh, romTag[i], SEEK_SET);
		fread(&romID, 1, 32, fh);

		if ((romID.crc^romID.icrc) == 0xFFFF &&
			(romID.makeup & 0xF) == (i & 1)) break;
	}
	if (i == 4) goto Done;

	fseek(fh,0,SEEK_SET);

	if (i < 2) return ID6_ROM;

	if (*(u32*)&header[8] == 0x4BBAA) return ID6_SWC;

	if (CmpMem(header, sfcID, 16)) return ID6_SF3;

	if ((header[2] ==   64 || header[2] == 0) &&
		(header[3] == -128 || header[3] == 0)) return ID6_FIG;

	return ID6_ROMH;
#endif

Done:
	fclose(fh);
	fh = NULL;

	return ID6_UNK;
}

ID6Type IsSPC(const s8* pFile, u32 size)
{
	s32	i;


	if (size < 0x10200) goto Done;

	//Look for SPC header ----------------------
	if (CmpMem(pFile, SPCFileID, 28))
	{
		if (size < 0x1020C) return ID6_SPC;

		if (*(u32*)&pFile[0x10200] == *(u32*)xid6 &&
			*(u32*)&pFile[0x10204] >= 4) return ID6_EXT;

		return ID6_SPC;
	}

	//Look for ZST header ----------------------
	if (CmpMem(pFile, ZSTFileID, 21))
	{
		if (size < 266879) goto Done;

		return ID6_ZST;
	}

	//Is it a ROM image? -----------------------
#if	ID666_ROM
	if (size < 0x40000) goto Done;

	for (i=0; i<4; i++)
	{
		memcpy(&romID, &pFile[romTag[i]], sizeof(ROMHdr));

		if ((romID.crc^romID.icrc) == 0xFFFF &&
			(romID.makeup & 0xF) == (i & 1)) break;
	}
	if (i == 4) goto Done;

	if (i < 2) return ID6_ROM;

	if (*(u32*)&pFile[8] == 0x4BBAA) return ID6_SWC;

	if (CmpMem(pFile, sfcID, 16)) return ID6_SF3;

	if ((pFile[2] ==   64 || pFile[2] == 0) &&
		(pFile[3] == -128 || pFile[3] == 0)) return ID6_FIG;

	return ID6_ROMH;
#endif

Done:
	return ID6_UNK;
}


//**************************************************************************************************
// String to Ticks

u32 Str2Ticks(const s8* cstr)
{
	s8	str[4];
	s8*	d;
	s32	s,m;


	s = atoi(cstr);								//Get integer value of number left of decimal
	if (s < 0) return 0;						//Time can't be negative
	if (s > 59) return XID6_TICKSMIN - 1;		//Time can't be greater than 1 minute

	m = 0;
	d = strchr((char*)cstr, '.');
	if (d++)									//If there's a decimal point, get fraction
	{
		*(u32*)&str[0] = 0x303030;				//Initialize str to "000"

		for (m=0; m<3; m++)
		{
			if (*d >= '0' && *d <= '9') str[m] = *d++;
			else break;
		}

		m = atoi(str);

		return (s * XID6_TICKSSEC) + (m * XID6_TICKSMS);
	}
	else
	{
		d = strchr((char*)cstr, ':');
		if (d)
		{
			m = atoi(d+1);
			if (m < 0) m = 0;
			if (m > XID6_TICKSSEC - 1) m = XID6_TICKSSEC - 1;
		}

		return (s * XID6_TICKSSEC) + m;
	}
}

#if ID666_ROM
static const s8 romMaker[256][32]={
	"N/A",
	"Nintendo",
	"(2)",
	"Imagineer-Zoom",
	"(4)",
	"Zamuse",
	"Falcom",
	"(7)",
	"Capcom",
	"HOT-B",
	"Jaleco",
	"Coconuts",
	"Rage Software",
	"(13)",
	"Technos",
	"Mebio Software",
	"(16)",
	"(17)",
	"Gremlin Graphics",
	"Electronic Arts",
	"(20)",
	"COBRA Team",
	"Human/Field",
	"KOEI",
	"Hudson Soft",
	"(25)",
	"Yanoman",
	"(27)",
	"Tecmo",
	"(29)",
	"Open System",
	"Virgin Games",
	"KSS",
	"Sunsoft",
	"POW",
	"Micro World",
	"(36)",
	"(37)",
	"Enix",
	"Loriciel/Electro Brain",
	"Kemco",
	"Seta Co.,Ltd.",
	"(42)",
	"(43)",
	"(44)",
	"Visit Co.,Ltd.",
	"(46)",
	"(47)",
	"(48)",
	"Carrozzeria",
	"Dynamic",
	"Nintendo",
	"Magifact",
	"Hect",
	"(54)",
	"(55)",
	"(56)",
	"(57)",
	"(58)",
	"(59)",
	"Empire Software",
	"Loriciel",
	"(62)",
	"(63)",
	"Seika Corp.",
	"UBI Soft",
	"(66)",
	"(67)",
	"(68)",
	"(69)",
	"System 3",
	"Spectrum Holobyte",
	"(72)",
	"Irem",
	"(74)",
	"Raya Systems/Sculptured Software",
	"Renovation Products",
	"Malibu Games/Black Pearl",
	"(78)",
	"U.S. Gold",
	"Absolute Entertainment",
	"Acclaim",
	"Activision",
	"American Sammy",
	"GameTek",
	"Hi Tech Expressions",
	"LJN Toys",
	"(87)",
	"(88)",
	"(89)",
	"Mindscape",
	"(91)",
	"(92)",
	"Tradewest",
	"(94)",
	"American Softworks Corp.",
	"Titus",
	"Virgin Interactive Entertainment",
	"Maxis",
	"(99)",
	"(100)",
	"(101)",
	"(102)",
	"Ocean",
	"(104)",
	"Electronic Arts",
	"(106)",
	"Laser Beam",
	"(108)",
	"(109)",
	"Elite",
	"Electro Brain",
	"Infogrames",
	"Interplay",
	"LucasArts",
	"Parker Brothers",
	"(116)",
	"STORM",
	"(118)",
	"(119)",
	"THQ Software",
	"Accolade Inc.",
	"Triffix Entertainment",
	"(123)",
	"Microprose",
	"(125)",
	"(126)",
	"Kemco",
	"Misawa",
	"Teichio",
	"Namco Ltd.",
	"Lozc",
	"Koei",
	"(133)",
	"Tokuma Shoten Intermedia",
	"(135)",
	"DATAM-Polystar",
	"(137)",
	"(138)",
	"Bullet-Proof Software",
	"Vic Tokai",
	"(141)",
	"Character Soft",
	"I''Max",
	"Takara",
	"CHUN Soft",
	"Video System Co., Ltd.",
	"BEC",
	"(148)",
	"Varie",
	"(150)",
	"Kaneco",
	"(152)",
	"Pack in Video",
	"Nichibutsu",
	"TECMO",
	"Imagineer Co.",
	"(157)",
	"(158)",
	"(159)",
	"Telenet",
	"(161)",
	"(162)",
	"(163)",
	"Konami",
	"K.Amusement Leasing Co.",
	"(166)",
	"Takara",
	"(168)",
	"Technos Jap.",
	"JVC",
	"(171)",
	"Toei Animation",
	"Toho",
	"(174)",
	"Namco Ltd.",
	"(176)",
	"ASCII Co. Activison",
	"BanDai America",
	"(179)",
	"Enix",
	"(181)",
	"Halken",
	"(183)",
	"(184)",
	"(185)",
	"Culture Brain",
	"Sunsoft",
	"Toshiba EMI",
	"Sony Imagesoft",
	"(190)",
	"Sammy",
	"Taito",
	"(193)",
	"Kemco",
	"Square",
	"Tokuma Soft",
	"Data East",
	"Tonkin House",
	"(199)",
	"KOEI",
	"(201)",
	"Konami USA",
	"NTVIC",
	"(204)",
	"Meldac",
	"Pony Canyon",
	"Sotsu Agency/Sunrise",
	"Disco/Taito",
	"Sofel",
	"Quest Corp.",
	"Sigma",
	"(212)",
	"(213)",
	"Naxat",
	"(215)",
	"Capcom Co., Ltd.",
	"Banpresto",
	"Tomy",
	"Acclaim",
	"(220)",
	"NCS",
	"Human Entertainment",
	"Altron",
	"Jaleco",
	"(225)",
	"Yutaka",
	"(227)",
	"T&ESoft",
	"EPOCH Co.,Ltd.",
	"(230)",
	"Athena",
	"Asmik",
	"Natsume",
	"King Records",
	"Atlus",
	"Sony Music Entertainment",
	"(237)",
	"IGS",
	"(239)",
	"(240)",
	"Motown Software",
	"Left Field Entertainment",
	"Beam Software",
	"Tec Magik",
	"(245)",
	"(246)",
	"(247)",
	"(248)",
	"Cybersoft",
	"(250)",
	"(251)",
	"(252)",
	"(253)",
	"(254)",
	"Hudson Soft"};
#endif

//**************************************************************************************************
void	__fastcall HotKey(WORD key, TShiftState shift);
void	__fastcall SeekSong(u32 time);
void	__fastcall SetAPULength();
b8		__fastcall SaveBRR(AnsiString file, Source *pSrc);
b8		__fastcall SaveWAV(AnsiString file, Source *pSrc);
void	ScanSrc(Source *pSrc);
void	ScanSrcDir(Source table[256], u8*, u8, u32, u32, b8, b8, u8);
u32		ScanROM(Source*, u8*, u32, u32, u32, u32, b8, b8, u8);
u16		__fastcall CRC16(void *p, u32 len);
u64		__fastcall ReadTimeStamp();

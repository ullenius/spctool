u16		__fastcall CRC16(void *p, u32 len);
u64		__fastcall ReadTimeStamp();
void	DivMod(u32 *q, u32 *r, u32 d);
s8*		CopyCase(s8 *pDest, const s8 *pSrc);
void	ScanSrc(Source *pSrc);
void	ScanSrcDir(Source table[256], u8*, u8, u32, u32, b8, b8, u8);
u32		ScanROM(Source*, u8*, u32, u32, u32, u32, b8, b8, u8);

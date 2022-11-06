b8		__fastcall CmpChkID(void*, u32);
b8		__fastcall CmpChkType(void*, u32);
inline void* EnterChk(void **p)
{
	void	*pRet;

	pRet = *p;
	*((u32*)&pRet) += ((u32*)pRet)[1];
	*((u32*)p) += 12;

	return pRet;
}

#define	_SkipChk(p) *(u32*)&p += ((u32*)p)[1];

u16		__fastcall ByteSwap16(u16);
void	__fastcall ByteSwap16(u16*);
void	__fastcall ByteSwap16(u16*, u32);
u32		__fastcall ByteSwap32(u32);
void	__fastcall ByteSwap32(u32*);
void	__fastcall ByteSwap32(u32*, u32);

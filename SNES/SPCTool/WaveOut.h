extern u32	visBuffer;
extern u32	visFrame;

extern u32	(*WavCallback)(void*,u32,u32);
extern u8*	(*WavMakeScope)(void*);

void	WavInit();
void	WavReset();
void	WavQuit();

b8		WavStart();
void	WavStop();

b8		WavPause(b8 state);
void	WavFlush();
void	WavRestart(u32 time);

u32		WavPos();
s32		WavTime();
b8		IsWavOpen();


/***************************************************************************************************
* SNES(tm) APU Emulator DLL                                                                        *
*                                                      Copyright (C)2001-2005 Alpha-II Productions *
***************************************************************************************************/

#include	"Types.h"
#include	"SPC700.h"
#include	"DSP.h"
#include	"APU.h"


//**************************************************************************************************
// Variables

typedef enum SADataType;
static u32	ApuOpt;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions

//**************************************************************************************************
extern "C" b8 _DllMainCRTStartup(u32 hinst, u32 dwReason, u32 lpReserved)
{
	switch (dwReason)
	{
	case 1:	//DLL_PROCESS_ATTACH
		ApuOpt = InitAPU();
		ResetAPU();
		break;

	case 0:	//DLL_PROCESS_DETACH
		ShutAPU();
		break;
	}

	return 1;
}


//**************************************************************************************************
void* GetAPUData(SADataType dt)
{
	switch(dt)
	{
	case(-1):	return (void*)ApuOpt;									//SADATA_OPTIONS
	case(0):	return (void*)pAPURAM;									//SADATA_RAM
	case(1):	return (void*)&dsp;										//SADATA_DSP
	case(2):	return (void*)&mix;										//SADATA_VOICE
	case(3):	return (void*)((ApuOpt & SA_PROFILE) ? &profile : 0);	//SADATA_PROFILE
	default:	return (void*)0;
	}
}
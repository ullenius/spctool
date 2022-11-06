# Microsoft Developer Studio Project File - Name="SNESAPU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SNESAPU - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SNESAPU.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SNESAPU.mak" CFG="SNESAPU - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SNESAPU - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SNESAPU - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SNESAPU - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SNESAPU_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gz /W3 /O2 /Ob2 /I "../.." /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /FAs /Fr /FD /opt:nowin98 /c
# SUBTRACT CPP /u
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /version:2.40 /dll /pdb:none /machine:I386 /nodefaultlib /out:"C:\Windows\System32\SNESAPU.DLL" /implib:"SNESAPU.lib"

!ELSEIF  "$(CFG)" == "SNESAPU - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SNESAPU_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G3 /Gz /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SNESAPU_EXPORTS" /FAs /FD /GZ /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /version:0.96 /dll /pdb:none /debug /debugtype:both /machine:I386 /out:"C:\Program Files\SPCTool\SNESAPU.DLL"

!ENDIF 

# Begin Target

# Name "SNESAPU - Win32 Release"
# Name "SNESAPU - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APU.Asm

!IF  "$(CFG)" == "SNESAPU - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__APU_A="..\..\Macro.Inc"	"APU.Inc"	"DSP.Inc"	"SPC700.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\APU.Asm
InputName=APU

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm -l $(InputName).Lst

# End Custom Build

!ELSEIF  "$(CFG)" == "SNESAPU - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__APU_A="..\..\Macro.Inc"	"APU.Inc"	"DSP.Inc"	"SPC700.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\APU.Asm
InputName=APU

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSP.Asm

!IF  "$(CFG)" == "SNESAPU - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__DSP_A="..\..\Macro.Inc"	"APU.Inc"	"DSP.Inc"	"SPC700.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\DSP.Asm
InputName=DSP

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm -l $(InputName).Lst

# End Custom Build

!ELSEIF  "$(CFG)" == "SNESAPU - Win32 Debug"

USERDEP__DSP_A="..\..\Macro.Inc"	"DSP.Inc"	"SPC700.Inc"	"APU.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\DSP.Asm
InputName=DSP

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm -l $(InputName).Lst

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SNESAPU.cpp
# End Source File
# Begin Source File

SOURCE=.\SNESAPU.def
# End Source File
# Begin Source File

SOURCE=.\SPC700.Asm

!IF  "$(CFG)" == "SNESAPU - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__SPC70="..\..\Macro.Inc"	"APU.Inc"	"DSP.Inc"	"SPC700.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Release
InputPath=.\SPC700.Asm
InputName=SPC700

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm -l $(InputName).Lst

# End Custom Build

!ELSEIF  "$(CFG)" == "SNESAPU - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SPC70="..\..\Macro.Inc"	"SPC700.Inc"	"DSP.Inc"	"APU.Inc"	
# Begin Custom Build - Assembling $(InputPath)
IntDir=.\Debug
InputPath=.\SPC700.Asm
InputName=SPC700

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	E:\snes\nasmw -Xvc -DWIN32 -DSTDCALL -O1 -f win32 -o $(IntDir)\$(InputName).obj $(InputName).Asm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Version.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APU.h
# End Source File
# Begin Source File

SOURCE=.\DSP.h
# End Source File
# Begin Source File

SOURCE=.\SNESAPU.h
# End Source File
# Begin Source File

SOURCE=.\SPC700.h
# End Source File
# Begin Source File

SOURCE=..\..\Types.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

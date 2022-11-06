/***************************************************************************************************
* File:       Dialog Docking Library                                                               *
* Platform:   Windows C++                                                                          *
* Programmer: Anti Resonance                                                                       *
*                                                                                                  *
* This program is free software; you can redistribute it and/or modify it under the terms of the   *
* GNU General Public License as published by the Free Software Foundation; either version 2 of     *
* the License, or (at your option) any later version.                                              *
*                                                                                                  *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;        *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* See the GNU General Public License for more details.                                             *
*                                                                                                  *
* You should have received a copy of the GNU General Public License along with this program;       *
* if not, write to the Free Software Foundation, Inc.                                              *
* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                                        *
*                                                                                                  *
* ------------------------------------------------------------------------------------------------ *
* Revision History:                                                                                *
*                                                                                                  *
* 04.10.27                                                                                         *
* + Turned A2Dock into a class                                                                     *
* + Added CenterDialog()                                                                           *
* - Updated IsVisible() to make sure the title bar, not just the border, of a dialog is visible    *
* - Added ENUM_CAST to fix a casting discrepancy between VC and BCB                                *
*                                                                                                  *
* 04.10.23                                                                                         *
* - Changed InitDock() to use existing USER32.DLL instead of loading a new instance                *
* - Updated function pointers to compile under the picky VC++                                      *
*                                                                                                  *
* 04.06.26                                                                                         *
* + Added IsVisible()                                                                              *
* + Updated IsDocked() to ignore other windows if pDock->thread is NULL                            *
*                                                                                                  *
* 03.04.06                                                                                         *
* + Added GetWindowAlpha()                                                                         *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/


#define	DOCK_MAXWIN	16							//Max number of windows to test for docking against


//**************************************************************************************************
// Public Variables

// Pointers to functions in USER32.DLL (see A2Dock::Init())
extern BOOL	(__stdcall *PGetWindowInfo)(HWND,PWINDOWINFO);							//Win98
extern BOOL	(__stdcall *PEnumDisplayMonitors)(HDC,LPCRECT,MONITORENUMPROC,LPARAM);	//Win98
extern void	(__stdcall *PGetLayeredWindowAttributes)(HWND,COLORREF*,BYTE*,DWORD*);	//Win2k
extern BOOL	(__stdcall *PSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD);		//Win2k


class A2Dock
{
private:
	static	BOOL __stdcall EnumThreadWndProc(HWND hwnd, LPARAM lParam);
	static	BOOL __stdcall EnumThreadWndProcA(HWND hwnd, LPARAM lParam);
	static	BOOL __stdcall DesktopEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT pRect, LPARAM usr);

	HWND	hDialog;							//Handle of dialog to dock/test
	HWND	hRoot;								//Handle of root window
	u32		thread;								//Handle of thread to enumerate windows from

	u32		windows; 							//Total windows enumerated
	u32		monitors;							//Total monitors enumerated plus windows
	RECT	rect[DOCK_MAXWIN];					//Bounding rectangles for each window and monitor

	u32		maxWin;								//Maximum number of windows that can be enumerated
	u8		*pAlpha;

	RECT	adj;								//Amount to adjust real position from mouse coord

public:
			A2Dock();


	//**********************************************************************************************
	// Initialize Function Pointers
	//
	// The docking class uses functions that were added to USER32.DLL in versions of Windows after
	// 95.  You do not need call Init(), but not doing so will make A2Dock behave as it would in a
	// Windows 95 evironment.
	//
	// Because Init() is a static function, you can call it to gain access to the extra functions
	// of USER32 without instantiating a copy of the A2Dock class.
	//
	// Init() only needs to be called once during the initialization of your application.
	//
	// Out:
	//    true, if at least Windows 98 functionality was found

	static b8 Init();


	//**********************************************************************************************
	// Set Dialog Handle
	//
	// All functions within A2Dock need a window handle
	//
	// In:
	//    hWnd = Handle to dialog to test or move

	void __fastcall SetDialog(HWND hWnd);


	//**********************************************************************************************
	// Set Root Window Handle
	//
	// The handle to the root window, or the application's main window, is needed by IsDocked(),
	// IsDockedPath(), PrepareMove(), and GetWindowAlpha().

	void __fastcall SetRootWindow(HWND hWnd);


	//**********************************************************************************************
	// Is Dialog Visible?
	//
	// Determines if a dialog's title bar is visible on the current monitor configuration
	//
	// In:
	//    hWnd = Handle to dialog to test for visibility
	//
	// Out:
	//    true, if the dialog is visible

	b8 __fastcall IsVisible();


	//**********************************************************************************************
	// Center Dialog on the Main Monitor
	//
	// Centers a dialog on the primary display device.  If the dialog is too big to be displayed,
	// it's resized to the devices display dimensions.
	//
	// In:
	//   hWnd  = Handle to dialog to center
	//   pRect-> Rectangle to store new coordinates in

	void __fastcall CenterDialog(RECT *pRect);


	//**********************************************************************************************
	// Is Dialog Docked to Window?
	//
	// Determines if a dialog is docked to, and only to, the root window.
	//
	// Notes:  This function does not count corner docking
	//         Windows 95 cannot test for docking against other windows
	//
	// In:
	//    pDock -> hWindow = Handle to dialog to test for docking
	//             hRoot   = Handle to window to test for docking against
	//             thread  = Thread of handle process (only needed if testing for docking against
	//                       other windows, otherwise set to NULL)
	//
	// Out:
	//    true, if dialog is only docked to root window

	b8 __fastcall IsDocked();


	//**********************************************************************************************
	// Is Dialog Indirectly Docked to Root Window?
	//
	// Determines if a dialog is docked to the root window, either directly or through a path of
	// other windows.
	//
	// Note:  Windows 95 can only test for docking against the root window
	//
	// In:
	//    pDock -> hWindow = Handle to dialog to test for docking
	//             hRoot   = Handle to window to test for docking against
	//             thread  = Thread handle of process
	//
	// Out:
	//    true, if dialog is docked

	b8 __fastcall IsDockedPath();


	//**********************************************************************************************
	// Get Relative Position
	//
	// After calling IsDockedPath() or IsDocked(), these functions will return the relative distance
	// from the root window.
	//
	// In:
	//    pos = Position of source window
	//
	// Out:
	//    Relative position from root window

	inline s32 __fastcall GetRelX(s32 pos) {return pos - rect[0].left;}
	inline s32 __fastcall GetRelY(s32 pos) {return pos - rect[0].top;}


	//**********************************************************************************************
	// Prepare to Move Dialog
	//
	// Gathers the positions of all the windows associated with the root window.
	//
	// Call this function when your dialog receives the WM_ENTERSIZEMOVE message.
	//
	// Note:  Windows 95 can only test for snapping against the root window
	//
	// In:
	//    pDock -> hWindow = Handle to dialog being moved
	//             hRoot   = Handle to application's main window
	//             thread  = Thread handle of process
	//    x,y    = Mouse cursor position

	void __fastcall PrepareMove(s32 x, s32 y);


	//**********************************************************************************************
	// Move Dialog Position
	//
	// Returns the position of a dialog based on the position of the mouse cursor.  If the dialog is
	// close enough to another window, the position will snap next to it.
	//
	// Call this function when your dialog receives the WM_MOVING message.
	//
	// PrepareMove() must be called first.  See above.
	//
	// In:
	//    pRect -> Rectangle to store new coordinates in
	//    x,y    = Mouse cursor position
	//
	// Out:
	//    true, if the dialog snapped to the root window

	b8 __fastcall MoveDialog(RECT *pRect, s32 x, s32 y);


	//**********************************************************************************************
	// Get Alpha of All Windows
	//
	// Gathers the transparency value (layered attribute) of all windows in a thread
	//
	// Notes: Only works in Windows 2000 or later.  Other versions return false.
	//        The first value in 'alpha' is not guaranteed to be the root window
	//        'hWindow' is only needed if you want to exclude a window from alpha retrieval
	//
	// In:
	//    pDock -> hWindow = Handle to current dialog (can be NULL)
	//             thread  = Thread handle of process
	//
	// Out:
	//    true, if function was successful

	u32 __fastcall GetWindowAlpha(u8 alpha[], u32 num);
};

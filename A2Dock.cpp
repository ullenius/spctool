/***************************************************************************************************
* Window Docking Library                                                                           *
*                                                        Copyright (C)2003-04 Alpha-II Productions *
***************************************************************************************************/

#define _WIN32_WINNT 0x0500

#include	"windows.h"
#include	"Types.h"
#include	"A2Dock.h"


#ifdef	__BORLANDC__
#define	ENUM_CAST	int(__stdcall*)()
#else
#define	ENUM_CAST	int(__stdcall*)(HWND,long)
#endif


//**************************************************************************************************
// Variables

//User32 functions -----------------------------
BOOL	(__stdcall *PGetWindowInfo)(HWND,PWINDOWINFO) = NULL;							//Win98
BOOL	(__stdcall *PEnumDisplayMonitors)(HDC,LPCRECT,MONITORENUMPROC,LPARAM) = NULL;	//Win98
void	(__stdcall *PGetLayeredWindowAttributes)(HWND,COLORREF*,BYTE*,DWORD*) = NULL;	//Win2k
BOOL	(__stdcall *PSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD) = NULL;		//Win2k


////////////////////////////////////////////////////////////////////////////////////////////////////
// Alpha-II Docking Class

//**************************************************************************************************
A2Dock::A2Dock()
{
	hDialog = NULL;
	hRoot = NULL;
	thread = NULL;
	windows = 0;
	monitors = 0;
}


//**************************************************************************************************
// Initialize Docking Library

b8 A2Dock::Init()
{
	HINSTANCE	hUser;


	//Connect to the user library --------------
	// These are functions that don't exist in all versions of Windows.  If we allow the
	// compiler to link to them at run-time, Winamp won't be able to load in_snes.dll under
	// Win95/98.

	hUser = GetModuleHandle("USER32");			//Get the handle to USER32.DLL
	if (!hUser) return 0;						//Hmm... something is seriously wrong

	PGetWindowInfo =
		(BOOL (__stdcall *)(HWND,PWINDOWINFO))GetProcAddress(hUser,"GetWindowInfo");
	PEnumDisplayMonitors =
		(BOOL (__stdcall *)(HDC,LPCRECT,MONITORENUMPROC,LPARAM))GetProcAddress(hUser,"EnumDisplayMonitors");
	PGetLayeredWindowAttributes =
		(void (__stdcall *)(HWND,COLORREF*,BYTE*,DWORD*))GetProcAddress(hUser,"GetLayeredWindowAttributes");
	PSetLayeredWindowAttributes =
		(BOOL (__stdcall *)(HWND,COLORREF,BYTE,DWORD))GetProcAddress(hUser,"SetLayeredWindowAttributes");

	if (!PGetWindowInfo) return 0;

	return 1;
}


//**************************************************************************************************

void __fastcall A2Dock::SetDialog(HWND hWnd)
{
	hDialog = hWnd;
}

void __fastcall A2Dock::SetRootWindow(HWND hWnd)
{
	if (hRoot != hWnd)
	{
		hRoot = hWnd;
		thread = GetWindowThreadProcessId(hWnd, NULL);
	}
}


//**************************************************************************************************
// Is Form Visible?

b8 __fastcall A2Dock::IsVisible()
{
	u32				i;
	TITLEBARINFO	tbi;


	//Get the location of the title bar --------
	tbi.cbSize = sizeof(TITLEBARINFO);
	GetTitleBarInfo(hDialog, &tbi);
	if (tbi.rgstate[0] & STATE_SYSTEM_INVISIBLE)	//If the dialog doesn't have a title bar,
	{												// fake it
		GetWindowRect(hDialog, &tbi.rcTitleBar);
		tbi.rcTitleBar.bottom = tbi.rcTitleBar.top + GetSystemMetrics(SM_CYCAPTION);
		tbi.rcTitleBar.left += GetSystemMetrics(SM_CXSMICON);
	}

	tbi.rcTitleBar.right -= GetSystemMetrics(SM_CXSIZE);	//Subtract the size of the close button

	//Get the dimensions of all display devices
	if (PEnumDisplayMonitors)
	{
		monitors = 0;
		PEnumDisplayMonitors(NULL, NULL, DesktopEnumProc, (u32)this);
	}
	else
	{
		monitors = 1;
		rect[0].top = 0;
		rect[0].left = 0;
		rect[0].bottom = GetSystemMetrics(SM_CYSCREEN);
		rect[0].right = GetSystemMetrics(SM_CXSCREEN);
	}

	//Make sure the title bar is partially visible on at least one device
	for (i=0; i<monitors; i++)
	{
		if (tbi.rcTitleBar.left < rect[i].right && tbi.rcTitleBar.right > rect[i].left &&
			tbi.rcTitleBar.bottom < rect[i].bottom && tbi.rcTitleBar.top > rect[i].top) return 1;
	}

	return 0;
}


//**************************************************************************************************
// Center Dialog

void __fastcall A2Dock::CenterDialog(RECT *pRect)
{
	s32		w,h,x,y;


	GetWindowRect(hDialog, pRect);
	h = GetSystemMetrics(SM_CYSCREEN);
	w = GetSystemMetrics(SM_CXSCREEN);

	y = pRect->bottom - pRect->top;
	if (h < y) y = h;

	x = pRect->right - pRect->left;
	if (w < x) x = w;

	pRect->top = (h - y) >> 1;
	pRect->bottom = pRect->top + y;
	pRect->left = (w - x) >> 1;
	pRect->right = pRect->left + x;
}


//**************************************************************************************************
// Is Form Docked to Window?

b8 __fastcall A2Dock::IsDocked()
{
	RECT	dlg,*pTest;
	u32		i;
	b8		sticky;


	//Check if docked against root window ------
	windows = 1;
	pTest = &rect[0];
	GetWindowRect(hRoot, pTest);				//Get rect of root window
	GetWindowRect(hDialog, &dlg);				//Get rect of dialog to test
	sticky = 0;

	if (( (dlg.left   == pTest->right  || dlg.right  == pTest->left) &&
		 ((dlg.top    >= pTest->top    && dlg.top    <  pTest->bottom) ||
		  (dlg.bottom >  pTest->top    && dlg.bottom <= pTest->bottom)) ) ||
		( (dlg.top    == pTest->bottom || dlg.bottom == pTest->top) &&
		 ((dlg.left   >= pTest->left   && dlg.left   <  pTest->right) ||
		  (dlg.right  >  pTest->left   && dlg.right  <= pTest->right)) )) sticky = 1;

	if (!sticky) return 0;

	//Check if docked to any other window ------
	if (PGetWindowInfo)
	{
		EnumThreadWindows(thread, (ENUM_CAST)&EnumThreadWndProc, (u32)this);

		for (i=1; i<windows; i++)
		{
			pTest = &rect[i];

			if (( (dlg.left   == pTest->right  || dlg.right  == pTest->left) &&
				 ((dlg.top    >= pTest->top    && dlg.top    <  pTest->bottom) ||
				  (dlg.bottom >  pTest->top    && dlg.bottom <= pTest->bottom)) ) ||
				( (dlg.top    == pTest->bottom || dlg.bottom == pTest->top) &&
				 ((dlg.left   >= pTest->left   && dlg.left   <  pTest->right) ||
				  (dlg.right  >  pTest->left   && dlg.right  <= pTest->right)) )) return 0;
		}
	}

	return 1;
}


//**************************************************************************************************
// Is Form Docked to Root Window?

//======================================================================
// Follow a Window Docking Path to the Root Window
//
// A recursive function that determines if a window is docked to the
// root window either directly or indirectly through other windows.
//
// In:
//    pTest -> Rectangle to test for docking
//    aRect -> Array of rectangles to test against
//    num    = Number of rectangles in array
//    idx    = Used for recursion, don't set
//
// Out:
//    true, if window is somehow connected to the main window

b8 DockingPath(RECT *pTest, RECT aRect[], u32 num, u32 idx = 0)
{
	RECT	*pRect;
	b8		sticky,c;


	//Check if docked to the main window -------
	pRect = &aRect[0];

	if ( (pTest->left   == pRect->right || pTest->right  == pRect->left) &&
		((pTest->top    >= pRect->top   && pTest->top    <= pRect->bottom) ||
		 (pTest->bottom >= pRect->top   && pTest->bottom <= pRect->bottom)) ) return 1;
	else
	if ( (pTest->top   == pRect->bottom || pTest->bottom == pRect->top) &&
		((pTest->left  >= pRect->left   && pTest->left   <= pRect->right) ||
		 (pTest->right >= pRect->left   && pTest->right  <= pRect->right)) ) return 1;

	//Check if docked to other windows ---------
	for (++idx; idx<num; idx++)
	{
		pRect = &aRect[idx];
		sticky = 0;

		if ( (pTest->left   == pRect->right || pTest->right  == pRect->left) &&
			((pTest->top    >= pRect->top   && pTest->top    <= pRect->bottom) ||
			 (pTest->bottom >= pRect->top   && pTest->bottom <= pRect->bottom)) ) sticky = 1;
		else
		if ( (pTest->top   == pRect->bottom || pTest->bottom == pRect->top) &&
			((pTest->left  >= pRect->left   && pTest->left   <= pRect->right) ||
			 (pTest->right >= pRect->left   && pTest->right  <= pRect->right)) ) sticky = 1;

		if (sticky)
		{
			c = DockingPath(pRect, aRect, num, idx);
			if (c) return 1;
		}
	}

	return 0;
}


b8 __fastcall A2Dock::IsDockedPath()
{
	RECT	dlg;


	GetWindowRect(hRoot, &rect[0]);				//Get rect of root window

	windows = 1;
	if (PGetWindowInfo)							//Get rects of remaining windows
		EnumThreadWindows(thread, (ENUM_CAST)&EnumThreadWndProc, (u32)this);

	GetWindowRect(hDialog, &dlg);				//Get rect of window to test

	return DockingPath(&dlg, &rect[0], windows);
}


//**************************************************************************************************
// Prepare to Move Dialog

void __fastcall A2Dock::PrepareMove(s32 x, s32 y)
{
	GetWindowRect(hDialog, &adj);
	adj.top -= y; 						 		//Relative position of mouse cursor to dialog's
	adj.left -= x;								// screen position
	adj.bottom -= y;
	adj.right -= x;

	windows = 0;
	if (hRoot)
	{
		GetWindowRect(hRoot, &rect[0]);			//Get coordinates of root window
		windows = 1;

		if (PGetWindowInfo)						//Get coordinates for all windows created by process
			EnumThreadWindows(thread, (ENUM_CAST)&EnumThreadWndProc, (u32)this);
	}

	monitors = windows;
	if (PEnumDisplayMonitors) PEnumDisplayMonitors(NULL, NULL, DesktopEnumProc, (u32)this);
}


//**************************************************************************************************
// Move Dialog

b8 __fastcall A2Dock::MoveDialog(RECT *pRect, s32 x, s32 y)
{
	RECT 	r;									//Rectangle used for docking test
	RECT	*pTest;								//-> rectangle to test against
	s32		snapTop,snapLeft;					//Coordinates after snapping to an edge
	s32		newTop,newLeft;						//New (final) coordinates of Control dialog
	s32		height,width;
	u32		sticky,snapped,i;


	sticky = 0;									//Assume Control dialog is not docked

	//Get dialog's logical position based on mouse cursor
	newTop	=	snapTop		=	r.top		=	y + adj.top;
	newLeft	=	snapLeft	=	r.left		=	x + adj.left;
								r.bottom	=	y + adj.bottom;
								r.right		=	x + adj.right;

	height = adj.bottom - adj.top;
	width = adj.right - adj.left;

	//==========================================
	//Test for docking against all windows
	//
	//Bits 0 and 1 of 'snapped' are used to flag if the control dialog has docked to the top/bottom
	//and left/right of the current window.  Bits 0 and 1 of 'sticky' are used if the dialog has
	//docked to any window.
	//
	//At the end of the function, 'sticky' will be 1 or 0, depending on if the control dialog is
	//docked to the root window.

	for (i=0; i<windows; i++)					//Iterate through all visible windows
	{
		pTest = &rect[i];						//-> current window's rectangle
		snapped = 0;

		//Test for docking on top or bottom ----
		//Before testing if the dialog is within snapping range on the top or bottom, make sure that
		//it isn't already docked to the top/bottom edge of another window and that some part of it
		//is inside the window on the left or right side.
		if (!(sticky & 1) &&
			r.left <= pTest->right && r.right >= pTest->left)
		{
			//If the bottom of the dialog is within ten pixels of the top edge of the window...
			if (abs(r.bottom - pTest->top) < 10)
			{
				snapTop = pTest->top - height;
				snapped |= 1;
			}
			else
			if (abs(r.top - pTest->bottom) < 10)	//top -> bottom
			{
				snapTop = pTest->bottom;
				snapped |= 1;
			}
		}

		//Test for docking on left or right ----
		if (!(sticky & 2) &&
			r.top <= pTest->bottom && r.bottom >= pTest->top)
		{
			if (abs(r.right - pTest->left) < 10)	//right -> left
			{
				snapLeft = pTest->left - width;
				snapped |= 2;
			}
			else
			if (abs(r.left - pTest->right) < 10)	//left -> right
			{
				snapLeft = pTest->right;
				snapped |= 2;
			}
		}

		//Test for alignment on top or bottom --
		//If the dialog just docked to the left/right edge of a window, and it isn't already docked
		//on the top/bottom edge of another window, then see if it's within range to be aligned with
		//the top/bottom edge of this window.  This code causes corners to snap together.
		if (!(sticky & 1) && snapped==2)
		{
			if (abs(r.top - pTest->top) < 10)		//top -> top
			{
				snapTop = pTest->top;
				snapped |= 1;
			}
			else
			if (abs(r.bottom - pTest->bottom) < 10)	//bottom -> bottom
			{
				snapTop = pTest->bottom - height;
				snapped |= 1;
			}
		}

		//Test for alignment on left or right --
		if (!(sticky & 2) && snapped==1)
		{
			if (abs(r.left - pTest->left) < 10)		//left -> left
			{
				snapLeft = pTest->left;
				snapped |= 2;
			}
			else
			if (abs(r.right - pTest->right) < 10)	//right -> right
			{
				snapLeft = pTest->right - width;
				snapped |= 2;
			}
		}

		//Verify new position is good ----------
		//The dialog should only stick to the outside of windows, not the inside.
		if (snapped)
		{
			if (!((( snapLeft          >= pTest->left &&  snapLeft          <  pTest->right )  ||
				   ((snapLeft + width) >  pTest->left && (snapLeft + width) <= pTest->right )) &&
				  (( snapTop           >= pTest->top  &&  snapTop           <  pTest->bottom)  ||
				   ((snapTop + height) >  pTest->top  && (snapTop + height) <= pTest->bottom))))
			{
				if (snapped & 1) newTop = snapTop;
				if (snapped & 2) newLeft = snapLeft;
				if (!i) sticky |= 4;			//Set bit 2 if docked to main window (see below)
				sticky |= snapped;
			}
		}
	}

	//==========================================
	//Test for snapping against monitor edges
	//
	//A little different from the above loop, but the same basic concept.

	for (; i<monitors; i++)
	{
		pTest = &rect[i];
		snapped = 0;

		//Test for docking on top or bottom ----
		if (!(sticky & 1) &&
			r.left <= pTest->right && r.right >= pTest->left)
		{
			if (abs(r.top - pTest->top) < 10)
			{
				snapTop = pTest->top;
				snapped |= 1;
			}
			else
			if (abs(r.bottom - pTest->bottom) < 10)
			{
				snapTop = pTest->bottom - height;
				snapped |= 1;
			}
		}

		//Test for docking on left or right ----
		if (!(sticky & 2) &&
			r.top <= pTest->bottom && r.bottom >= pTest->top)
		{
			if (abs(r.left - pTest->left) < 10)
			{
				snapLeft = pTest->left;
				snapped |= 2;
			}
			else
			if (abs(r.right - pTest->right) < 10)
			{
				snapLeft = pTest->right - width;
				snapped |= 2;
			}
		}

		if (snapped)
		{
			//Same as above test, but reversed to verify dialog is inside rectangle
			if (((( snapLeft          >= pTest->left &&  snapLeft          <  pTest->right )  ||
				  ((snapLeft + width) >  pTest->left && (snapLeft + width) <= pTest->right )) &&
				 (( snapTop           >= pTest->top  &&  snapTop           <  pTest->bottom)  ||
				  ((snapTop + height) >  pTest->top  && (snapTop + height) <= pTest->bottom))))
			{
				if (snapped & 1) newTop = snapTop;
				if (snapped & 2) newLeft = snapLeft;
				sticky |= snapped;
			}
		}
	}

	//Set current position ---------------------
	pRect->top = newTop;
	pRect->left = newLeft;
	pRect->bottom = newTop + height;
	pRect->right = newLeft + width;

	return (sticky >> 2) != 0;					//Shift off temp bits and save main window flag
}


//**************************************************************************************************
// Get Alpha of All Windows

u32 __fastcall A2Dock::GetWindowAlpha(u8 alpha[], u32 num)
{
	if (!PGetLayeredWindowAttributes) return 0;

	pAlpha = alpha;
	maxWin = num;
	windows = 0;
	monitors = 0;

	EnumThreadWindows(thread, (ENUM_CAST)&EnumThreadWndProcA, (u32)this);

	return windows;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Functions

//**************************************************************************************************
// Enumerate Windows Created by a Thread
//
// Called by EnumThreadWindows once for each window created by a thread.  Used to build a list of
// all visible windows associated with an application.

BOOL __stdcall A2Dock::EnumThreadWndProc(HWND hwnd, LPARAM usr)
{
	WINDOWINFO	winInfo;
	A2Dock		*p;


	p = (A2Dock*)usr;

	if (hwnd == p->hDialog || hwnd == p->hRoot)	//We don't need to store our own or the root
		return 1;								// window's coordinates

	if (p->windows < DOCK_MAXWIN)				//Make sure there's room in the array
	{
		winInfo.cbSize = sizeof(WINDOWINFO);
		PGetWindowInfo(hwnd, &winInfo);

		if (winInfo.dwStyle & WS_VISIBLE)		//If the window's visible, copy its coordinates
			memcpy(&p->rect[p->windows++], &winInfo.rcWindow, sizeof(RECT));

		return 1;
	}

	return 0;									//Tell Windows we're done accepting handles
}


BOOL __stdcall A2Dock::EnumThreadWndProcA(HWND hwnd, LPARAM usr)
{
	WINDOWINFO	winInfo;
	A2Dock		*p;
	DWORD		flags;
	u8			blend;


	p = (A2Dock*)usr;

	if (hwnd == p->hDialog) return 1;			//We don't need to store our own alpha

	if (p->windows < p->maxWin)					//Make sure there's room in the array
	{
		winInfo.cbSize = sizeof(WINDOWINFO);
		PGetWindowInfo(hwnd, &winInfo);

		if (winInfo.dwStyle & WS_VISIBLE)
		{
			p->pAlpha[p->windows] = 255;

			if (winInfo.dwExStyle & WS_EX_LAYERED)
			{
				PGetLayeredWindowAttributes(hwnd, NULL, &blend, &flags);

				if (flags & LWA_ALPHA) p->pAlpha[p->windows] = blend;
			}

			p->windows++;
		}

		return 1;
	}

	return 0;									//Tell Windows we're done accepting handles
}


//**************************************************************************************************
// Enumerate Monitors Comprising Desktop Display
//
// Called by EnumDisplayMonitors once for each monitor used by the desktop.

BOOL __stdcall A2Dock::DesktopEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT pRect, LPARAM usr)
{
	MONITORINFO	mi;
	A2Dock		*p = (A2Dock*)usr;

	if (p->monitors < DOCK_MAXWIN)				//Make sure there's room in the array
	{
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hmonitor, &mi);
		memcpy(&p->rect[p->monitors++], &mi.rcWork, sizeof(RECT));
		return 1;
	}

	return 0;									//Tell Windows we're done accepting handles
}

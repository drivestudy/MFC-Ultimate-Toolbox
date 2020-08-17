
// Version: 9.3

#ifndef _HOOKWND_H
#define _HOOKWND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"



// timer ID event. Every time when it happens we call CheckMousePos function to 
// check whether mouse id over the hooked window or not
#define ID_CHECKMOUSETIMER			1
// default delay
#ifndef DFLT_CHECKMOUSEDELAY
#define DFLT_CHECKMOUSEDELAY		100
#endif	// DFLT_CHECKMOUSEDELAY

//////////////////
// Generic class to hook messages on behalf of a CWnd.
// Once hooked, all messages go to COXHookWnd::WindowProc before going
// to the window. Specific subclasses can trap messages and do something.
// To use:
//
// * Derive a class from COXHookWnd.
//
// * Override COXHookWnd::WindowProc to handle messages. Make sure you call
//   COXHookWnd::WindowProc if you don't handle the message, or your window will
//   never get messages. If you write separate message handlers, you can call
//   Default() to pass the message to the window.
//
// * Instantiate your derived class somewhere and call HookWindow(pWnd)
//   to hook your window, AFTER it has been created.
//	 To unhook, call UnhookWindow().
//
class OX_CLASS_DECL COXHookWnd : public CObject 
{
protected:
	DECLARE_DYNAMIC(COXHookWnd);

	// the window hooked
	HWND m_hWndHooked;	
	// original window proc
	// we call it to handle all unhandled messages
	WNDPROC	m_pOldWndProc;		

	// This class is generic so we cannot rule out the situation
	// when more than one object of COXHookWnd derived class hooked 
	// the same window. We have to make sure that all these objects 
	// eventually will get chance to handle messages that wasn't 
	// handled by the other objects that hooked the window later.
	// Thus always call COXHookWnd::WindowProc in your WindowProc on
	// all messages you don't handle
	//
	// next in chain of hooks for this window
	COXHookWnd*	m_pNext;		

	// timer for checking mouse position (will be activated only after
	// InstallSpy() function is called)
	UINT_PTR m_nCheckMouseTimerID;
	// flag that is set to TRUE when mouse is over the hooked window and 
	// to FALSE otherwise. The value of this flag is valid only if 
	BOOL m_bMouseIsOver;

	// Override this to handle messages in specific handlers
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
	// call this at the end of handler functions
	LRESULT Default();				

	// check if mouse is over the hooked window or not and notifies the hooked
	// window if the state has changed (HWM_ENTERWINDOW and HWM_LEAVEWINDOW messages 
	// will be sent)
	void CheckMousePos();

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXHookWnd();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXHookWnd();


	// --- In  :	pRealWnd	-	pointer to created window that is 
	//								supposed to be hooked
	// --- Out : 
	// --- Returns:
	// --- Effect : Hook a window
	virtual void HookWindow(CWnd* pRealWnd);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Unhook a window. Automatic on WM_NCDESTROY
	virtual void UnhookWindow();

	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE if any window was hooked
	// --- Effect : 
	inline BOOL IsHooked() const { return (m_hWndHooked!=NULL); }

	
	// --- In  :
	// --- Out : 
	// --- Returns:	pointer to hooked window
	// --- Effect : 
	inline CWnd* GetHookedWnd() const { return CWnd::FromHandle(m_hWndHooked); }
	inline HWND GetHookedHwnd() const { return m_hWndHooked; }


	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect : installs spy timer that will notify the hooked window when mouse
	//				enter and leave the window
	virtual BOOL InstallSpy();

	// --- In  :
	// --- Out : 
	// --- Returns:	
	// --- Effect : removes spy timer that notifies the hooked window when mouse
	//				enter and leave the window(HWM_ENTERWINDOW and HWM_LEAVEWINDOW 
	//				messages will be sent)
	virtual void RemoveSpy();


	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE if mouse is currently over the window, and FALSE otherwise
	// --- Effect : retrieves the flag that specifies if mouse is over the 
	//				hooked window. Note that the return value is valid only if
	//				InstallSpy() function has been called before
	inline BOOL IsMouseOver() const { return m_bMouseIsOver; }


	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE is this object is set to spy for mouse enter and 
	//				mouse exit events
	// --- Effect : Retrieves the flag that specifies if COXHookWnd object
	//				will notify the hooked window if mouse enteres or leaves
	//				this window borders
	inline BOOL IsSpyInstalled() const { return (m_nCheckMouseTimerID!=0); }



	friend LRESULT CALLBACK HookWndProc(HWND, UINT, WPARAM, LPARAM);
	friend class COXHookWndMap;
};

//////////////////
// The message hook map is derived from CMapPtrToPtr, which associates
// a pointer with another pointer. It maps an HWND to a COXHookWnd, like
// the way MFC's internal maps map HWND's to CWnd's. The first hook
// attached to a window is stored in the map; all other hooks for that
// window are then chained via COXHookWnd::m_pNext.
//
class OX_CLASS_DECL COXHookWndMap : private CMapPtrToPtr 
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXHookWndMap() {}; 

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXHookWndMap();


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Trick to instantiate object only when it is really needed
	static COXHookWndMap& GetHookMap();


	// --- In  :	hwnd	-	handle of window
	//				pMsgHook-	pointer to COXHookWnd object
	// --- Out : 
	// --- Returns:
	// --- Effect : adds a pair of window and its hook into the map
	void Add(HWND hwnd, COXHookWnd* pMsgHook);

	
	// --- In  :	pMsgHook-	pointer to COXHookWnd object
	// --- Out : 
	// --- Returns:
	// --- Effect : removes specified hook from the map (remember that one COXHookWnd
	//				object could be used to hook only one CWnd object)
	void Remove(COXHookWnd* pMsgHook);

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : cleans up the map
	void RemoveAll(HWND hwnd);

	// --- In  :	hwnd	-	handle of window
	// --- Out : 
	// --- Returns:	pointer to corresponding COXHookWnd object,
	//				or NULL if it doesn't exist
	// --- Effect : looks for COXHookWnd object used to hook hwnd window
	COXHookWnd* Lookup(HWND hwnd) const;
};

#endif // #ifndef _HOOKWND_H

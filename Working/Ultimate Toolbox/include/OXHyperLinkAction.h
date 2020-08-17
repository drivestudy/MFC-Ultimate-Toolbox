// ==========================================================================
// 						Class Specification : 
//						COXHyperLinkAction
// ==========================================================================

// Header file : OXHyperLinkAction.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
#ifndef _HYPERLINKACTION_H
#define _HYPERLINKACTION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

/////////////////////////////////////////////////////////////////////////////////////////
//
// COXHyperLinkAction class is mostly just a wrapper around ShellExecute function.
// Here is the brief description of this function:
//
//		HINSTANCE ShellExecute(
//		    HWND hwnd, 
//		    LPCTSTR lpOperation,
//		    LPCTSTR lpFile, 
//		    LPCTSTR lpParameters, 
//		    LPCTSTR lpDirectory,
//		    INT nShowCmd
//			);	
//
//		Opens or prints a specified file. 
//
//		Returns a value greater than 32 if successful; or an error value that is less 
//		than or equal to 32 otherwise.
//
//		hwnd		-	Window handle to a parent window. This window receives any message 
//						boxes that an application produces.
//		lpOperation	-	Address of a null-terminated string that specifies the operation 
//						to perform. The following operation strings are valid: "open",
//						"print" and "explore"
//		lpFile		-	Address of a null-terminated string that specifies the file to 
//						open or print or the folder to open or explore.
//		lpParameters-	If the lpFile parameter specifies an executable file, 
//						lpParameters is an address to a null-terminated string that 
//						specifies the parameters to be passed to the application. 
//		lpDirectory -	Address of a null-terminated string that specifies the default 
//						directory. 
//		nShowCmd	-	If lpFile specifies an executable file, nShowCmd specifies how 
//						the application is to be shown when it is opened.
//
// In COXHyperLinkAction we provide three additional operation: "none", "email" and 
// "userdefined". Plus class provides full error notification functionality.
// Usually COXHyperLinkAction is used in conjunction with any commomn control 
// Below you will find description of all of COXHyperLinkAction operations and 
// examples of using them in conjunction with COXStaticHyperLink).
// 
// type of actions
// at the moment 6 type of actions are defined:
//
//		1)	None - use this type if you need functionality
//				this control provide but don't want to invoke any action
//				if user clicks on the control
//
//				Below you will find example of using such action
//
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// do nothing if user clicks
//					m_ctlStaticLink.SetAction(ID_HLACTION_NONE);
//
//
//		2)	Userdefined - use this type if you want to define your own 
//				action that should be taken when a hyperlink is activated.
//				In that case you have to set callback message ID and
//				handle to the recipient	window to the corresponding
//				COXHyperLinkAction object. In your application you 
//				have to define unique message ID (the best way to do 
//				that is to call RegisterWindowMessage function):
//
//					UINT nCallbackMsgID=RegisterWindowMessage(_T("HyperLinkCallbackMessage"));
//
//				and function to react on that message. Such function have 
//				to follow next prototype:
//
//					afx_msg LONG OnHyperLinkActivated(UINT wParam, LONG lParam);
//						wParam - not used
//						lParam - pointer to corresponding COXHyperLinkAction object
//						return value is 1 if your action succeeded or 0 otherwise
//						
//				Below you will find example of using userdefined action
//							
//				in your header file:
//					// let it be COXStaticHyperLink object
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// in MESSAGE_MAP section declare the function:
//					afx_msg LONG OnHyperLinkActivated(UINT wParam, LONG lParam);
//
//				in your *.cpp file:
//					static UINT g_nCallbackMsgID=RegisterWindowMessage(_T("HyperLinkCallbackMessage"));
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					ON_REGISTERED_MESSAGE(g_nCallbackMsgID,OnHyperLinkActivated)
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					m_ctlStaticLink.SetAction(ID_HLACTION_USERDEFINED,NULL,NULL,NULL,NULL,
//						g_nCallbackMsgID,GetSafeHwnd());
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					
//					LONG CYourClass::OnHyperLinkActivated(UINT wParam, LONG lParam)
//					{
//						UNREFERENCED_PARAMETER(wParam);
//
//						COXHyperLinkAction* phla=(COXHyperLinkAction*)lParam;
//						if(phla->GetAction()!=ID_HLACTION_USERDEFINED || 
//							(phla->GetRecipientWnd()!=GetSafeHwnd() && 
//							phla->GetRecipientWnd()!=HWND_BROADCAST))
//						{
//							return (LONG)0;
//						}
//
//						AfxMessageBox(_T("HyperLink had been successfully activated!"));
//
//						return (LONG)1;
//					}
//
//
//		3) Open - use this type if you want to open the file specified by the 
//				COXHyperLinkAction::m_sFile parameter. The file can be 
//				an executable file, shortcut or document file. It can also be a folder.
//				Use this action if you want to open the website specified by m_sFile.
//				If the m_sFile parameter specifies an executable file then 
//				COXHyperLinkActionlpFile::m_sParameters specifies the parameters 
//				to be passed to the application. If m_sFile specifies a document file, 
//				m_sParameters should be NULL. Use this action if you want to send 
//				an e-mail: you can do that setting m_sFile to "mailto:address@you.need"

//
//				Below you will find an example of using COXStaticHyperLink as link to
//				some website:
//
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// open website
//					m_ctlStaticLink.SetAction(ID_HLACTION_OPEN,
//						_T("www.dundas.com//develop//utoolbx//"));
//					
//					// open file
//					m_ctlStaticLink.SetAction(ID_HLACTION_OPEN,
//						_T("sol.exe"));
//
//					// sending e-mail
//					m_ctlStaticLink.SetAction(ID_HLACTION_OPEN,
//						_T("mailto:andreiz@dundas.com"));
//
//		4) Print - use this type if you want to print the file specified by the 
//				COXHyperLinkAction::m_sFile parameter. The file should be 
//				a document file. If the file is an executable file, the action opens 
//				the file, as if "Open" action had been specified. 
//
//				Below you will find an example of using COXStaticHyperLink as link to
//				print some document:
//
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// print the document
//					m_ctlStaticLink.SetAction(ID_HLACTION_PRINT,
//						_T("readme.txt"));
//
//			
//		5) Explore - use this type if you want to explore the folder specified by the 
//				COXHyperLinkAction::m_sFile parameter. 
//
//				Below you will find an example of using COXStaticHyperLink as link to
//				explore some folder:
//
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// explore the folder
//					m_ctlStaticLink.SetAction(ID_HLACTION_EXPLORE,
//						_T("C:\\Windows\\"));
//
//		6) Email - use this type if you want to send a message to a recipient 
//				specified by the COXHyperLinkAction::m_sFile parameter. 
//				Defined only for convinience purposes, eventually calls Open action
//
//				Below you will find an example of using COXStaticHyperLink as link to
//				send an email message:
//
//					COXStaticHyperLink m_StaticLink;
//					. . . . . . . . . . . . 
//					. . . . . . . . . . . . 
//					// explore the folder
//					m_ctlStaticLink.SetAction(ID_HLACTION_EMAIL,
//						_T("andreiz@dundas.com"));
//
//
// All COXHyperLinkAction properties could be set using next functions:
//
//		void Set(int nActionID = ID_HLACTION_OPEN, LPCTSTR sFile = NULL, 
//			LPCTSTR sParameters = NULL, LPCTSTR sDefaultDirectory = NULL, 
//			int nShowCmd = SW_SHOWNORMAL, UINT nCallbackMsgID=NULL,
//			HWND hWndRecipient=NULL);
//		// --- Effect : sets all COXHyperLinkAction properties using one function
//		void Set(COXHyperLinkAction& hla);
//		// --- Effect : sets all COXHyperLinkAction properties copying from another
//		//				COXHyperLinkAction object
//		void SetOwnerWnd(HWND hWndOwner)
//		void SetOwnerWnd(CWnd* pWnd)
//		// --- Effect : sets handle of owner window
//		void SetActionID(int nActionID)
//		// --- Effect : sets action ID
//		void SetFile(LPCTSTR sFile)
//		// --- Effect : sets depending on action:
//		//				file name to open or print;
//		//				folder name to explore;
//		//				e-mail address to use in new e-mail message window 
//		//				(use it as "mailto:address@you.need")
//		void SetParameters(LPCTSTR sParameters)
//		// --- Effect : if m_sFile specifies executable then sets the parameters 
//		//				to be passed to that executable
//		void SetDefaultDirectory(LPCTSTR sDefaultDirectory)
//		// --- Effect : if m_sFile specifies executable then sets default directory 
//		void SetShowCmd(int nShowCmd)
//		// --- Effect : if m_sFile specifies executable then sets flags that define
//		//				the way launched application would be displayed
//		void SetCallbackMsg(UINT nCallbackMsgID)
//		// --- Effect : if action is userdefined specifies callback message that will be 
//		//				sent to recipient window to notify that action have to be executed 
//		void SetRecipientWnd(HWND hWndRecipient)
//		void SetRecipientWnd(CWnd* pWnd) 
//		// --- Effect : if action is userdefined specifies recipient window that 
//		//				will be responsible	for executing action
//
//
// To execute COXHyperLinkAction you have to call next function
//
//		BOOL ExecuteIt(BOOL bErrorNotify=TRUE);
//
//		Executes action. If execution fails and bErrorNotify is TRUE
//		then corresponding error message will be displayed
//		using ErrorNotify function which is defined as virtual
//		just in case you would like more sophisticated error notification
//
//
// In result of execution of COXHyperLinkAction some application could be launched.
// To get handle to that application use next function
//
//		HINSTANCE GetLaunchedApp()
//
// To get last error that could happen in result of execution of COXHyperLinkAction
// call function
//
//		int GetLastError()
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#define ID_HLACTION_NONE						0x00000000
#define ID_HLACTION_USERDEFINED					0x00000001
#define ID_HLACTION_OPEN						0x00000002
#define ID_HLACTION_PRINT						0x00000003
#define ID_HLACTION_EXPLORE						0x00000004
#define ID_HLACTION_EMAIL						0x00000005

#define ID_HL_ACTIONNUMBER						0x00000006

// in result of execution of the action the COXHyperLinkAction can set next error codes:
//		ID_HLERROR_SUCCESS						- Action succedeed
//		ID_HLERROR_ACTIONUNKNOWN				- Unknown action was set
//
//	other error messages are the same as errors returned by ShellExecute function:
//		refer to online documentation on this function to get detail description

#define ID_HLERROR_SUCCESS						0xffffffff
#define ID_HLERROR_ACTIONUNKNOWN				0xfffffffe
#define ID_HLERROR_ACTIONFAILED					0xfffffffd


// array of all defined actions
static const int m_szAction[ID_HL_ACTIONNUMBER]=
{
	IDS_OX_HLINKACTIONNONE,		//"None"
	IDS_OX_HLINKACTIONUSER,		//"User defined"
	IDS_OX_HLINKACTIONOPEN,		//"Open"
	IDS_OX_HLINKACTIONPRINT,	//"Print"
	IDS_OX_HLINKACTIONEXPLORE,	//"Explore"
	IDS_OX_HLINKACTIONEMAIL		//"E-mail"
};

class OX_CLASS_DECL COXHyperLinkAction
{
protected:
	// handle to the window which is associated with that object
	HWND m_hWndOwner;

	// action's ID
	int m_nActionID;
	// depending on action defines:
	//		file name to open or print
	//		folder name to explore
	//		e-mail address to use in new e-mail message window 
	//			(use it as "mailto:address@you.need")
	CString m_sFile;
	// if m_sFile specifies executable then m_sParameters specifies the parameters 
	//		to be passed to that executable
	CString m_sParameters;
	// specifies the default directory
	CString m_sDefaultDirectory;
	//	If in result of action some application is started then 
	//	COXHyperLinkAction::nShowCmd specifies how that application is to be 
	//	shown when it is opened. This parameter can be one of the following values: 
	//		SW_HIDE				- Hides the window and activates another window. 
	//		SW_MAXIMIZE			- Maximizes the specified window. 
	//		SW_MINIMIZE			- Minimizes the specified window and activates the next 
	//			top-level window in the z-order. 
	//		SW_RESTORE			- Activates and displays the window. If the window is 
	//			minimized or maximized, Windows restores it to its original size and 
	//			position. An application should specify this flag when restoring 
	//			a minimized window. 
	//		SW_SHOW				- Activates the window and displays it in its current
	//			size and position. 
	//		SW_SHOWDEFAULT		- Sets the show state based on the SW_ flag specified in 
	//			the STARTUPINFO structure passed to the CreateProcess function by 
	//			the program that started the application. An application should 
	//			call ShowWindow with this flag to set the initial show state of its 
	//			main window. 
	//		SW_SHOWMAXIMIZED	- Activates the window and displays it as a maximized window. 
	//		SW_SHOWMINIMIZED	- Activates the window and displays it as a minimized window. 
	//		SW_SHOWMINNOACTIVE	- Displays the window as a minimized window. The active 
	//			window remains active. 
	//		SW_SHOWNA			- Displays the window in its current state. The active 
	//			window remains active. 
	//		SW_SHOWNOACTIVATE	- Displays a window in its most recent size and position. 
	//			The active window remains active. 
	//		SW_SHOWNORMAL		- Activates and displays a window. If the window is 
	//			minimized or maximized, Windows restores it to its original size and 
	//			position. An application should specify this flag when displaying 
	//			the window for the first time. 
	//
	//	If m_sFile specifies a document file, m_nShowCmd should be zero. 
	int m_nShowCmd;

	// if action is userdefined then we send m_nCallbackMsgID mesage to the 
	// window specified by m_hWndRecipient handle (it can be HWND_BROADCAST)
	UINT m_nCallbackMsgID;
	HWND m_hWndRecipient;

	// specifies result of the last execution of the action, can be:
	//	ID_HLERROR_SUCCESS						- Action succedeed
	//	ID_HLERROR_ACTIONUNKNOWN				- Unknown action was set
	//	ID_HLERROR_ACTIONFAILED					- Action failed
	int m_nError;
	// in result of execution we can launch new application, we save handle 
	// to that application in m_hLaunchedApp
	HINSTANCE m_hLaunchedApp;

public:
	COXHyperLinkAction() { Reset(); }
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	virtual ~COXHyperLinkAction() {}
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destroy the object

	void Reset();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : sets all COXHyperLinkAction properties to 
	//				their default value

	void Set(int nActionID = ID_HLACTION_OPEN, LPCTSTR sFile = NULL, 
		LPCTSTR sParameters = NULL, LPCTSTR sDefaultDirectory = NULL, 
		int nShowCmd = SW_SHOWNORMAL, UINT nCallbackMsgID=NULL,
		HWND hWndRecipient=NULL);
	// --- In  :	refer to description of protected members section
	// --- Out : 
	// --- Returns:
	// --- Effect : sets all COXHyperLinkAction properties using one function
	void Set(COXHyperLinkAction& hla);
	// --- In  :	hla	-	COXHyperLinkAction object
	// --- Out : 
	// --- Returns:
	// --- Effect : sets all COXHyperLinkAction properties copying from another
	//				COXHyperLinkAction object

    // assignment operator
    COXHyperLinkAction& operator=(const COXHyperLinkAction& hla);

	inline void SetOwnerWnd(HWND hWndOwner) { m_hWndOwner=hWndOwner; }
	// --- In  :	hWndOwner	-	handle of owner window
	// --- Out : 
	// --- Returns:
	// --- Effect : sets handle of owner window
	inline void SetOwnerWnd(CWnd* pWnd) { 
		ASSERT_VALID(pWnd);
		m_hWndOwner=pWnd->GetSafeHwnd(); }
	// --- In  :	pWnd	-	pointer to CWnd object
	// --- Out : 
	// --- Returns:
	// --- Effect : sets handle of owner window
	inline HWND GetOwnerWnd() { return m_hWndOwner; }
	// --- In  :
	// --- Out : 
	// --- Returns:	handle of owner window
	// --- Effect : 

	inline void SetActionID(int nActionID) { m_nActionID=nActionID; }
	// --- In  :	nActionID	-	action ID
	// --- Out : 
	// --- Returns:
	// --- Effect : sets action ID
	inline int GetActionID() { return m_nActionID; }
	// --- In  :
	// --- Out : 
	// --- Returns:	action ID
	// --- Effect : 

	inline void SetFile(LPCTSTR sFile) { m_sFile=sFile; }
	// --- In  :	sFile	-	string 
	// --- Out : 
	// --- Returns:
	// --- Effect : sets depending on action:
	//					file name to open or print;
	//					folder name to explore;
	//					e-mail address to use in new e-mail message window 
	//						(use it as "mailto:address@you.need")
	inline CString GetFile() { return m_sFile; }
	// --- In  :
	// --- Out : 
	// --- Returns:	depending on action:
	//					file name to open or print;
	//					folder name to explore;
	//					e-mail address to use in new e-mail message window 
	//						(use it as "mailto:address@you.need")
	// --- Effect :

	inline void SetParameters(LPCTSTR sParameters) { m_sParameters=sParameters; }
	// --- In  :	sParameters	-	string 
	// --- Out : 
	// --- Returns:
	// --- Effect : if m_sFile specifies executable then sets the parameters 
	//				to be passed to that executable
	inline CString GetParameters() { return m_sParameters; }
	// --- In  :
	// --- Out : 
	// --- Returns:	if m_sFile specifies executable then returns the parameters 
	//				to be passed to that executable
	// --- Effect : 

	inline void SetDefaultDirectory(LPCTSTR sDefaultDirectory) {	
		m_sDefaultDirectory=sDefaultDirectory; }
	// --- In  :	sDefaultDirectory	-	string 
	// --- Out : 
	// --- Returns:
	// --- Effect : if m_sFile specifies executable then sets default directory 
	inline CString GetDefaultDirectory() { return m_sDefaultDirectory; }
	// --- In  :	
	// --- Out : 
	// --- Returns:	if m_sFile specifies executable then returns default directory 
	// --- Effect : 

	inline void SetShowCmd(int nShowCmd) { m_nShowCmd=nShowCmd; }
	// --- In  :	nShowCmd	-	if in result of action some 
	//				application is started then it specifies how that application 
	//				is to be shown when it is opened. This parameter can be one 
	//				of the following values: 
	//		SW_HIDE				- Hides the window and activates another window. 
	//		SW_MAXIMIZE			- Maximizes the specified window. 
	//		SW_MINIMIZE			- Minimizes the specified window and activates the next 
	//			top-level window in the z-order. 
	//		SW_RESTORE			- Activates and displays the window. If the window is 
	//			minimized or maximized, Windows restores it to its original size and 
	//			position. An application should specify this flag when restoring 
	//			a minimized window. 
	//		SW_SHOW				- Activates the window and displays it in its current
	//			size and position. 
	//		SW_SHOWDEFAULT		- Sets the show state based on the SW_ flag specified in 
	//			the STARTUPINFO structure passed to the CreateProcess function by 
	//			the program that started the application. An application should 
	//			call ShowWindow with this flag to set the initial show state of its 
	//			main window. 
	//		SW_SHOWMAXIMIZED	- Activates the window and displays it as a maximized window. 
	//		SW_SHOWMINIMIZED	- Activates the window and displays it as a minimized window. 
	//		SW_SHOWMINNOACTIVE	- Displays the window as a minimized window. The active 
	//			window remains active. 
	//		SW_SHOWNA			- Displays the window in its current state. The active 
	//			window remains active. 
	//		SW_SHOWNOACTIVATE	- Displays a window in its most recent size and position. 
	//			The active window remains active. 
	//		SW_SHOWNORMAL		- Activates and displays a window. If the window is 
	//			minimized or maximized, Windows restores it to its original size and 
	//			position. An application should specify this flag when displaying 
	//			the window for the first time. 
	//
	//				If m_sFile specifies a document file, nShowCmd should be zero. 
	// --- Out : 
	// --- Returns:
	// --- Effect : if m_sFile specifies executable then sets flags that define
	//				the way launched application would be displayed
	inline int GetShowCmd() { return m_nShowCmd; }
	// --- In  :	
	// --- Out : 
	// --- Returns:	if m_sFile specifies executable then returns flags that define
	//				the way launched application would be displayed
	// --- Effect : 

	inline void SetCallbackMsg(UINT nCallbackMsgID) { m_nCallbackMsgID=nCallbackMsgID; }
	// --- In  :	nCallbackMsgID	-	callback message ID 
	// --- Out : 
	// --- Returns:
	// --- Effect : if action is userdefined specifies callback message that will be 
	//				sent to recipient window to notify that action have to be executed 
	inline UINT GetCallbackMsg() { return m_nCallbackMsgID; }
	// --- In  :
	// --- Out : 
	// --- Returns:	if action is userdefined returns callback message that will be 
	//				sent to recipient window to notify that action have to be executed 
	// --- Effect : 

	inline void SetRecipientWnd(HWND hWndRecipient) { m_hWndRecipient=hWndRecipient; }
	// --- In  :	hWndRecipient	-	handle to a window 
	// --- Out : 
	// --- Returns:
	// --- Effect : if action is userdefined specifies recipient window that 
	//				will be responsible	for executing action
	inline void SetRecipientWnd(CWnd* pWnd) { 
		ASSERT_VALID(pWnd);
		m_hWndRecipient=pWnd->GetSafeHwnd(); }
	// --- In  :	pWnd	-	pointer to a CWnd object 
	// --- Out : 
	// --- Returns:
	// --- Effect : if action is userdefined specifies recipient window that
	//				will be responsible	for executing action
	inline HWND GetRecipientWnd() { return m_hWndRecipient; }
	// --- In  :	
	// --- Out : 
	// --- Returns:	if action is userdefined returns recipient window that 
	//				will be responsible	for executing action
	// --- Effect : 

	// Executes action. If execution fails and bErrorNotify is TRUE
	// then corresponding error message will be displayed
	// using ErrorNotify function which is defined as virtual
	// just in case you would like more sophisticated error notification
	//
	BOOL ExecuteIt(BOOL bErrorNotify=TRUE);
	// --- In  :	bErrorNotify	-	if TRUE and error happens in result of 
	//									execution of the action then
	//									corresponding error message will be displayed 
	// --- Out : 
	// --- Returns:
	// --- Effect : executes action
	virtual void ErrorNotify();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : notifies about errors happen in result of execution of an action

	inline HINSTANCE GetLaunchedApp() { return m_hLaunchedApp; }
	// --- In  :
	// --- Out : 
	// --- Returns:	returns the handle to launched application if any was started
	// --- Effect : 
	inline int GetLastError() { return m_nError; }
	// --- In  :
	// --- Out : 
	// --- Returns:	returns the last error 
	// --- Effect : 
};


#endif //_HYPERLINKACTION_H

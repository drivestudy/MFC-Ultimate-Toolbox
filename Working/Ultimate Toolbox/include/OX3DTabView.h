// ==========================================================================
//								Class Specification : 
//					COX3DTabViewContainer & COX3DTabViewDropTarget
// ==========================================================================

// Header file : OX3DTabView.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

/*

For most applications it's not enough to use only one window to provide its all output. 
There are different solutions for this problem like splitters or docking windows. 
But they usually have common inconvenience: all windows are shown at the same time.
They take a precious screen space while could be rarely used. 

COXShortcutBar control can be used in order to show a number of child windows while
keeping active (fully displayed) only one at a time. But COXShortcutBar was primarily 
designed to show icons and have a set of notifications that make sense only while
using such controls as treeview or listview.

Very good example of how the problem could be resolved can be found in Developer Studio
IDE. For instance "Output" window (with "Build", "Debug", "Find in Files..." panes) or 
"Result List" window (with "Search", "Lookup", "See Also" and "History" panes). We call 
them TabViews.

TabViews can be a good alternative for splitter window when you need to have more than 
one view per document. Also TabViews can be used within docking window and used as a 
container for associated windows that usually implemented as dialog bars.

We already have in the library COXTabViewContainer class that implements the TabViews
paradigm. In terms of graphical representation this class duplicates the functionality
that can be found in such products as Microsoft Visual Studio IDE (Output window) or
Microsoft Excel (multiple sheets in the same book). Tab buttons are positioned 
at the bottom of the window on the same line as the horizontal scroll bar. 

Here we introduce new implementation of the TabViews. The paradigm remains the same 
but we've changed the way windows represented in the container. We used standard
Tab control and displayed a tab button for each associated window (page). When a
user clicks a button the corresponding page is activated and displayed. Tab buttons
can be positioned at any side of the container window by means of applying 
corresponding Tab control styles (refer to Create() function for details). 
 
The new class is called COX3DTabViewContainer. It is derived from the standard
CTabCtrl and implements all functionality needed to support tab views. 

COX3DTabViewContainer is easy to use. If you previously worked with 
COXTabViewContainer class then you are already know ho to use this class because
we decide to implement it as close as possible to the existing COXTabViewContainer 
class. If you haven't used the COXTabViewContainer class yet then the closest
comparison would be splitter windows. 

Here is the list of steps that should be taken in order to deploy TabViews in 
your application:

	First Case:		COX3DTabViewContainer will be used as a container for document view(s).

		1)	Embed a COX3DTabViewContainer member variable in the parent frame (main frame
			window for SDI application, MDIChild window for MDI application).

		2)	Override the parent frame's CFrameWnd::OnCreateClient member function.

		3)	From within the overridden OnCreateClient, call the Create member function 
			of COX3DTabViewContainer. In this function you have to specify the parent 
			window and optionally you can specify the initial rectangle, window styles
			and window ID. This function is the place where you can specify Tab control
			styles that defines the way tab buttons are positioned and displayed.

		4)	If you plan to assign images to TabView pages then you have to create 
			and load image list with images and associate it with 
			COX3DTabViewContainer object using CTabCtrl::SetImageList() function.
		
		5)	After COX3DTabViewContainer window was successfully created you can populate
			it with window objects using AddPage() or InsertPage() functions. If you are
			inserting view object you have to specify runtime class and context 
			information in order to keep the document/view architecture in place. 
			If you are adding a window object that is not a document view then you 
			have to create it before adding it to COX3DTabViewContainer window. 
			In AddPage() or InsertPage() functions you can specify text that will be 
			used as page title in the corresponding tab button. Also you can specify
			the index of image in the tab control image list that should be 
			displayed in the tab button.

	Example:

	BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
	{
		// TODO: Add your specialized code here and/or call the base class

		UNREFERENCED_PARAMETER(lpcs);

		if(!m_TabViewContainer.Create(this))
			return FALSE;

		VERIFY(m_ilTabView.Create(IDB_IL_TABVIEWS,16,0,RGB(255,0,255)));
		m_TabViewContainer.SetImageList(&m_ilTabView);

		if(!m_TabViewContainer.AddPage(pContext->m_pNewViewClass,
			pContext,_T("Primary View"),0))
			return FALSE;
		if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CMyView2),
			pContext,_T("View2"),1))
			return FALSE;
		
		m_TabViewContainer.SetActivePageIndex(0);
	
		return TRUE;
	}




	Second Case:	COX3DTabViewContainer will be used as a container for windows within
					control bar.

		1)	Create your own CControlBar-derived class (you can use our 
			COXSizeControlBar as parent class if you need sizable docking windows).
			Let's call it CMyControlBar.

		2)	Embed a COX3DTabViewContainer member variable in this class.

		3)	Override  CMyControlBar::OnCreate member function.

		4)	From within the overridden OnCreate(), call the Create() member function 
			of COX3DTabViewContainer. In this function you have to specify the parent 
			window and optionally you can specify the initial rectangle, window styles
			and window ID. This function is the place where you can specify Tab control
			styles that defines the way tab buttons are positioned and displayed.

		5)	If you plan to assign images to TabView pages then you have to create 
			and load image list with images and associate it with 
			COX3DTabViewContainer object using CTabCtrl::SetImageList() function.
		
		6)	After COX3DTabViewContainer window was successfully created you can populate
			it with window objects using AddPage or InsertPage function. You have to 
			create window object before adding it to COX3DTabViewContainer. In AddPage or 
			InsertPage functions you can specify text that will be used as page title 
			in tab button. Also you can specify the index of image in the tab 
			control image list that should be displayed in the tab button.

		7)	Override  CMyControlBar::OnSize member function and resize in it 
			COX3DTabViewContainer object as appropriate


	Example:

	int CMyControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		if (COXSizeControlBar::OnCreate(lpCreateStruct) == -1)
			return -1;

		if(!m_TabViewContainer.Create(this))
			return -1;

		VERIFY(m_ilTabView.Create(IDB_IL_TABVIEWS,16,0,RGB(255,0,255)));
		m_TabViewContainer.SetImageList(&m_ilTabView);

		// edit control
		if(!edit.Create(WS_CHILD|ES_MULTILINE|ES_AUTOHSCROLL|
			ES_AUTOVSCROLL|WS_HSCROLL|WS_VSCROLL,CRect(0,0,0,0),
			&m_TabViewContainer,1))
			return -1;
		m_TabViewContainer.AddPage(&edit,_T("Edit"),0);

		// list box
		if(!listBox.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL,
			CRect(0,0,0,0),&m_TabViewContainer,2))
			return -1;
		m_TabViewContainer.AddPage(&listBox,_T("ListBox"),1);

		// list control
		if(!listCtrl.Create(WS_CHILD|LVS_REPORT,
			CRect(0,0,0,0),&m_TabViewContainer,3))
			return -1;
		m_TabViewContainer.AddPage(&listCtrl,_T("List"),2);

		// tree control
		if(!treeCtrl.Create(WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,
			CRect(0,0,0,0),&m_TabViewContainer,4))
			return -1;
		m_TabViewContainer.AddPage(&treeCtrl,_T("Tree"),3);

		m_TabViewContainer.SetActivePageIndex(0);
	
		return 0;					
	}


Note, that any child window can be used as COX3DTabViewContainer page.


The steps that should be taken in order to implement COX3DTabViewContainer in CControlBar
derived window can be applied in general case too. We just decided to show it using 
CControlBar derived window because we feel like it's going to be used as parent window
for COX3DTabViewContainer in most cases.

Above we described the process of creating and populating of COX3DTabViewContainer 
object. In most cases that would be all the code you need. For those who need to change 
dynamically the contents of COX3DTabViewContainer object we provide a set of the 
following functions.

In order to remove any page at run time you have to use DeletePage() function.

To set/retrieve page title that is displayed in corresponding tab button use 
GetPageTitle() and SetPageTitle() functions.

To set/retrive active page index call GetActivePageIndex() and SetActivepageIndex() 
functions.

Refer to the class reference for list and description of all public functions.


Also take look at the following sample that can be found in .\samples\gui 
subdirectory of your ultimate Toolbox directory:

  3DTabViews		-	text editor with three panes: text editor, hex viewer, 
						list view with statistics on number of unique symbols 
						found in text.




Dependencies:

#include "OX3DTabView.h"


Source code files:

"OX3DTabView.h"		-	COX3DTabViewContainer implementation


*/

#ifndef _OX3DTABVIEW_H__
#define _OX3DTABVIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXSkinnedSpinCtrl.h"
#include "OXSkinnedTabCtrl.h"

#ifndef __AFXOLE_H__
#include <afxole.h>
#define __AFXOLE_H__
#endif


#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

const int ID_3DTABVIEW_OFFSET_EXTERNAL=0;
const int ID_3DTABVIEW_OFFSET_INTERNAL=0;
const DWORD DEFAULT_3DTABCTRLSTYLE=TCS_MULTILINE|TCS_BOTTOM|TCS_HOTTRACK|
	TCS_RIGHTJUSTIFY|TCS_FOCUSNEVER|WS_VISIBLE|WS_CHILD;

// default extended style for a page
#ifndef OX3DTABVIEWCONTAINER_EX_STYLE
#define OX3DTABVIEWCONTAINER_EX_STYLE		WS_EX_CLIENTEDGE
#endif

/////////////////////////////////////////////////////////////////////////////
// COX3DTabViewContainerDropTarget drop target for COX3DTabViewContainer, to faciliate 
// auto-selection on drag over.
class OX_CLASS_DECL COX3DTabViewDropTarget : public COleDropTarget
{
public:
	COX3DTabViewDropTarget() : m_nOldItem(-1) {};
	virtual ~COX3DTabViewDropTarget() {};

protected:
	int m_nOldItem;

protected:
	// Change tab on drag over handler
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave( CWnd* pWnd );
};

//////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COX3DTabViewContainer window

class COXTabSkin;

class OX_CLASS_DECL COX3DTabViewContainer : public COXSkinnedTabCtrl
{
	friend class COXTabSkinXP;
	friend class COXTabSkin2003;	

	DECLARE_DYNAMIC(COX3DTabViewContainer)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COX3DTabViewContainer();

// Attributes
public:

protected:
	// drop target object for OLE dragging support (when any object is
	// dragged over tab items the corresponding page window will be
	// activated)
	COX3DTabViewDropTarget m_dropTarget;
	
	// if TRUE then when any object is dragged over tab items the 
	// corresponding page window will be activated
	BOOL m_bAcceptDraggedObject;

	// internal array of unique IDs
	CArray<DWORD,DWORD> m_arrUniqueIDs;

	// There is only one tab
	BOOL m_bOneTabMode;

	// offset from the borders of the client area of the parent window
	// where the tab control will be displayed
	DWORD m_dwOffsetExternal;

	// offset from the borders of the active page window and display rectangle
	// of the tab control 
	DWORD m_dwOffsetInternal;

	// index of the current active page
	int m_nActivePageIndex;

	// internal flag that specifies whether current background painting happens for 
	// non-client or client area
	BOOL m_bDrawingNCBackground;

	// rectangle for displaying a page in
	CRect m_rectPage;

// Operations
public:

	// --- In  :	lpszClassName	-	ignored
	//				lpszWindowName	-	ignored
	//				dwStyle			-	The 3DTabView container's style.
	//				rect			-	window rectangle
	//				pParentWnd		-	Pointer to the window that is the 
	//									3DTabView container's parent.
	//				nID				-	The 3DTabView container's ID.
	//				pContext		-	ignored
	// --- Out : 
	// --- Returns:	TRUE if 3DTabView container was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the 3DTabView container. Implemented in order to 
	//				support dynamic creation of the object.
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext=NULL);


	// --- In  :	pParentWnd	-	Pointer to the window that is the 
	//								3DTabView container's parent.
	//				rect		-	window rectangle
	//				dwStyle		-	The 3DTabView container's style. The same like
	//								for CTabCtrl.
	//				nID			-	The 3DTabView container's ID.
	// --- Out : 
	// --- Returns:	TRUE if 3DTabView container was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the 3DTabView container
	virtual BOOL Create(CWnd* pParentWnd, CRect rect=CRect(0,0,0,0), 
		DWORD dwStyle=DEFAULT_3DTABCTRLSTYLE, UINT nID=AFX_IDW_PANE_FIRST);


	// --- In  :	pClass		-	pointer to runtime class information of
	//								the new window to be added as new page
	//				pContext	-	pointer to context info (refer to the
	//								description of CCreateContext class)
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	//				nImage		-	index of the image in the image list 
	//								associated with the container that will be
	//								set to the corresponding button
	//				dwExStyle	-	extended style to be applied to the page
	//				bActivate	-	if TRUE then added page will be activated
	// --- Out : 
	// --- Returns: TRUE if new page was successfully added, or FALSE otherwise
	// --- Effect : Adds new page to 3DTabView container. Use this version of
	//				function if you have to add CView derived class which is
	//				part of document/view architecture of your application
	BOOL AddPage(
		CRuntimeClass* pClass, CCreateContext* pContext, LPCTSTR lpszTitle=NULL, 
		int nImage=-1, DWORD dwExStyle=WS_EX_CLIENTEDGE, BOOL bActivate=TRUE) 
	{
		return InsertPage(
			GetPageCount(),pClass,pContext,lpszTitle,nImage,dwExStyle,bActivate);
	}

	BOOL AddPage(CWnd* pWnd, UINT nTitleID, int nImage=-1, 
		DWORD dwExStyle=WS_EX_CLIENTEDGE, BOOL bActivate=TRUE) 
	{ 
	   CString sCaption;
	   if(!sCaption.LoadString(nTitleID))
	   {
		  return FALSE;
	   }
	   return InsertPage(
		   GetPageCount(),pWnd,(LPCTSTR)sCaption,nImage,dwExStyle,bActivate); 
	}

	// --- In  :	pWnd		-	pointer to created window to be added 
	//								as new page
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	//				nImage		-	index of the image in the image list 
	//								associated with the container that will be
	//								set to the corresponding button
	//				dwExStyle	-	extended style to be applied to the page
	//				bActivate	-	if TRUE then added page will be activated
	// --- Out : 
	// --- Returns: TRUE if new page was successfully added, or FALSE otherwise
	// --- Effect : Adds new page to 3DTabView container. Use this version of
	//				function if you have to add any generic window into the 
	//				3DTabView container.
	BOOL AddPage(CWnd* pWnd, LPCTSTR lpszTitle=NULL, int nImage=-1, 
		DWORD dwExStyle=WS_EX_CLIENTEDGE, BOOL bActivate=TRUE) 
	{ 
		return InsertPage(GetPageCount(),pWnd,lpszTitle,nImage,dwExStyle,bActivate); 
	}

	// --- In  :	nIndex		-	zero-based index of the new page
	//				pClass		-	pointer to runtime class information of
	//								the new window to be added as new page
	//				pContext	-	pointer to context info (refer to the
	//								description of CCreateContext class)
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	//				nImage		-	index of the image in the image list 
	//								associated with the container that will be
	//								set to the corresponding button
	//				dwExStyle	-	extended style to be applied to the page
	//				bActivate	-	if TRUE then inserted page will be activated
	// --- Out : 
	// --- Returns: TRUE if new page was successfully inserted, or FALSE otherwise
	// --- Effect : Inserts new page to 3DTabView container. Use this version of
	//				function if you have to insert CView derived class which is
	//				part of document/view architecture of your application
	virtual BOOL InsertPage(int nIndex, CRuntimeClass* pClass, 
		CCreateContext* pContext, LPCTSTR lpszTitle=NULL, int nImage=-1, 
		DWORD dwExStyle=WS_EX_CLIENTEDGE, BOOL bActivate=TRUE);

	// --- In  :	nIndex		-	zero-based index of the new page
	//				pWnd		-	pointer to created window to be inserted 
	//								as new page
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	//				nImage		-	index of the image in the image list 
	//								associated with the container that will be
	//								set to the corresponding button
	//				dwExStyle	-	extended style to be applied to the page
	//				bActivate	-	if TRUE then inserted page will be activated
	// --- Out : 
	// --- Returns: TRUE if new page was successfully inserted, or FALSE otherwise
	// --- Effect : Inserts new page to 3DTabView container. Use this version of
	//				function if you have to add any generic window into the 
	//				3DTabView container.
	virtual BOOL InsertPage(int nIndex, CWnd* pWnd, LPCTSTR lpszTitle=NULL, 
		int nImage=-1, DWORD dwExStyle=WS_EX_CLIENTEDGE, BOOL bActivate=TRUE);


	// --- In  :	pWnd		-	pointer to the page to be deleted
	//				bDestroy	-	flag that specifies if window has to be 
	//								destroyed
	// --- Out : 
	// --- Returns: TRUE if the specified page was successfully deleted, 
	//				or FALSE otherwise
	// --- Effect : Deletes existing page from 3DTabView container.
	virtual BOOL DeletePage(const CWnd* pWnd, BOOL bDestroy=TRUE); 
	
	// --- In  :	nIndex		-	zero-based index of the page to be deleted
	//				bDestroy	-	flag that specifies if window has to be 
	//								destroyed
	// --- Out : 
	// --- Returns: TRUE if the specified page was successfully deleted, 
	//				or FALSE otherwise
	// --- Effect : Deletes existing page from 3DTabView container.
	virtual BOOL DeletePage(int nIndex, BOOL bDestroy=TRUE);


	// --- In  :	nIndex		-	zero-based index of the page to be retrieved
	// --- Out : 
	// --- Returns: pointer to the corresponding page, or NULL if out of range
	//				index was specified
	// --- Effect : Retrieves pointer to the page with specified index
	inline CWnd* GetPage(int nIndex) const 
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			TCITEM tci;
			tci.mask=TCIF_PARAM;
			GetItem(nIndex,&tci);
			return (CWnd*)tci.lParam;
		}
		return NULL;
	}


	// --- In  :	pWnd		-	pointer to the page which title
	//								is to be retrieved
	// --- Out : 
	// --- Returns: title of the corresponding page
	// --- Effect : Retrieves title of the specified page
	inline CString GetPageTitle(const CWnd* pWnd) const 
	{
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
		{
			return _T("");
		}
		
		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
		{
			return _T("");
		}
		
		return GetPageTitle(nIndex);
	}

	// --- In  :	nIndex		-	zero-based index of the page which title
	//								is to be retrieved
	// --- Out : 
	// --- Returns: title of the corresponding page
	// --- Effect : Retrieves title of the page with specified index
	inline CString GetPageTitle(int nIndex) const 
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			TCHAR szBuffer[1024];
			TCITEM tci;
			tci.mask = TCIF_TEXT;
			tci.cchTextMax = 1024;
			tci.pszText = szBuffer;
			GetItem(nIndex, &tci);
			return tci.pszText;
		}
		return _T("");
	}


	// --- In  :	pWnd		-	pointer to the page which title
	//								is to be set
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the title of the specified page
	inline BOOL SetPageTitle(const CWnd* pWnd, LPCTSTR lpszTitle) 
	{
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
		{
			return FALSE;
		}
		
		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
		{
			return FALSE;
		}

		return SetPageTitle(nIndex,lpszTitle);
	}

	// --- In  :	nIndex		-	zero-based index of the page which title
	//								is to be set
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets title of the page with specified index
	inline BOOL SetPageTitle(int nIndex, LPCTSTR lpszTitle) 
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			TCITEM tci;
			tci.mask=TCIF_TEXT;
			tci.pszText=(LPTSTR)lpszTitle;
			return SetItem(nIndex,&tci);
		}
		return FALSE;
	}


	// --- In  :	pWnd		-	pointer to the page which image index
	//								is to be retrieved
	// --- Out : 
	// --- Returns: image index of the corresponding page
	// --- Effect : Retrieves the image index of the specified page
	inline int GetPageImageIndex(const CWnd* pWnd) const 
	{
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
		{
			return -1;
		}
		
		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
		{
			return -1;
		}

		return GetPageImageIndex(nIndex);
	}

	// --- In  :	nIndex		-	zero-based index of the page which image index
	//								is to be retrieved
	// --- Out : 
	// --- Returns: image index of the corresponding page
	// --- Effect : Retrieves the image index of the specified page
	inline int GetPageImageIndex(int nIndex) const 
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			TCITEM tci;
			tci.mask=TCIF_IMAGE;
			GetItem(nIndex,&tci);
			return tci.iImage;
		}
		return -1;
	}


	// --- In  :	pWnd		-	pointer to the page which title
	//								is to be set
	//				nImage		-	image index in the image list associated with 
	//								the control that will be used as page image in
	//								corresponding tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the image index of the specified page
	inline BOOL SetPageImageIndex(const CWnd* pWnd, int nImage) 
	{
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
		{
			return FALSE;
		}

		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
		{
			return FALSE;
		}

		return SetPageImageIndex(nIndex,nImage);
	}

	// --- In  :	nIndex		-	zero-based index of the page which title
	//								is to be set
	//				nImage		-	image index in the image list associated with 
	//								the control that will be used as page image in
	//								corresponding tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the image index of the specified page
	inline BOOL SetPageImageIndex(int nIndex, int nImage) 
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			TCITEM tci;
			tci.mask=TCIF_IMAGE;
			tci.iImage=nImage;
			return SetItem(nIndex,&tci);
		}
		return FALSE;
	}


	// --- In  :	
	// --- Out : 
	// --- Returns: number of pages in the 3DTabView container
	// --- Effect : Retrieves the number of pages in the 3DTabView container
	inline int GetPageCount() const { return GetItemCount(); }


	// --- In  :	pTestWnd	-	pointer to the window to be tested
	//								as 3DTabView container's page
	//				hTestWnd	-	handle of the window to be tested
	//								as 3DTabView container's page
	// --- Out :	nIndex		-	reference to the variable where zero-based 
	//								index of the found page will be saved
	// --- Returns: TRUE if specified window is 3DTabView container's page,
	//				or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is 3DTabView container's page and if it is TRUE then
	//				save the index of found page into nIndex
	inline BOOL FindPage(const CWnd* pTestWnd, int& nIndex) const 
	{
		ASSERT(pTestWnd!=NULL);
		if(pTestWnd!=NULL)
		{
			return FindPage(pTestWnd->m_hWnd,nIndex);
		}
		return FALSE;
	}
	
	inline BOOL FindPage(HWND hTestWnd, int& nIndex) const 
	{
		BOOL bResult=FALSE;
		for(nIndex=0; nIndex<GetPageCount(); nIndex++)
		{
			CWnd* pWnd=GetPage(nIndex);
			ASSERT(pWnd!=NULL);
			if(pWnd->GetSafeHwnd()==hTestWnd)
			{
				bResult=TRUE;
				break;
			}
		}
		return bResult;
	}


	// --- In  :	pTestWnd	-	pointer to the window to be tested
	//								as 3DTabView container's page
	//				hTestWnd	-	handle of the window to be tested
	//								as 3DTabView container's page
	// --- Out :	
	// --- Returns: TRUE if specified window is 3DTabView container's page,
	//				or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is 3DTabView container's page
	inline BOOL IsPage(HWND hTestWnd) const 
	{
		int nIndex=-1;
		return FindPage(hTestWnd,nIndex);
	}

	inline BOOL IsPage(const CWnd* pTestWnd) const 
	{
		int nIndex=-1;
		return FindPage(pTestWnd,nIndex);
	}


	// --- In  :	pTestWnd	-	pointer to the window to be tested as 
	//								currently active 3DTabView container's page
	//				hTestWnd	-	handle of the window to be tested as 
	//								currently active 3DTabView container's page
	// --- Out :	
	// --- Returns: TRUE if specified window is currently active 3DTabView 
	//				container's page, or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is currently active 3DTabView container's page
	inline BOOL IsActivePage(HWND hTestWnd) const 
	{
		int nIndex=-1;
		if(FindPage(hTestWnd,nIndex) && GetActivePageIndex()==nIndex)
		{
			return TRUE;
		}
		return FALSE;
	}
	
	inline BOOL IsActivePage(const CWnd* pTestWnd) const 
	{
		int nIndex=-1;
		if(FindPage(pTestWnd,nIndex) && GetActivePageIndex()==nIndex)
		{
			return TRUE;
		}
		return FALSE;
	}


	// --- In  :	
	// --- Out :	
	// --- Returns: index of currently active 3DTabView container's page
	// --- Effect : Retrieves the index of currently active 3DTabView 
	//				container's page
	inline int GetActivePageIndex() const { return m_nActivePageIndex; }

	// --- In  :	
	// --- Out :	
	// --- Returns: pointer to currently active 3DTabView container's page
	// --- Effect : Retrieves the pointer to currently active 3DTabView 
	//				container's page
	inline CWnd* GetActivePage() const 
	{ 
		int nActivePageIndex=GetActivePageIndex();
		if(nActivePageIndex>=0 && nActivePageIndex<GetPageCount())
		{
			return GetPage(nActivePageIndex);
		}
		return NULL;
	}


	// --- In  :	pWnd		-	pointer to the page to be set as active
	// --- Out : 
	// --- Returns: TRUE is specified page was successfully set as active
	// --- Effect : Sets the specified page as active one
	inline BOOL SetActivePage(const CWnd* pWnd) 
	{
		ASSERT(pWnd!=NULL);
		if(pWnd!=NULL)
		{
			int nIndex=-1;
			if(FindPage(pWnd,nIndex))
			{
				return SetActivePageIndex(nIndex);
			}
		}
		return FALSE;
	}

	// --- In  :	nIndex		-	index of the page to be set as active
	// --- Out : 
	// --- Returns: TRUE is page with specified index was successfully set 
	//				as active
	// --- Effect : Sets the page with specified index as active one
	virtual BOOL SetActivePageIndex(int nIndex);


	// --- In  :	dwOffset		-	offset in points from the parent window
	//									client area where the tab control will be 
	//									displayed
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the tab control offset from parent borders
	inline void SetOffsetExternal(DWORD dwOffset) 
	{ 
		m_dwOffsetExternal=dwOffset; 
		if(::IsWindow(GetSafeHwnd()))
		{
			SetWindowPos(NULL,0,0,0,0,
				SWP_NOMOVE|SWP_DRAWFRAME|SWP_NOSIZE|SWP_NOZORDER);
			UpdatePages();
		}
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	offset in points from the parent window client area 
	//				where the tab control will be displayed
	// --- Effect : Retrievs the tab control offset from parent borders
	inline DWORD GetOffsetExternal() const { return m_dwOffsetExternal; }


	// --- In  :	dwOffset		-	offset in points from the display area
	//									of the tab control and active page window
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the page window offset from the tab control 
	//				display rectangle
	inline void SetOffsetInternal(DWORD dwOffset) 
	{ 
		m_dwOffsetInternal=dwOffset; 
		if(::IsWindow(GetSafeHwnd()))
		{
			SetWindowPos(NULL,0,0,0,0,
				SWP_NOMOVE|SWP_DRAWFRAME|SWP_NOSIZE|SWP_NOZORDER);
			UpdatePages();
		}
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	offset in points from the display area
	//				of the tab control and active page window
	// --- Effect : Retrievs the page window offset from the tab control 
	//				display rectangle
	inline DWORD GetOffsetInternal() const { return m_dwOffsetInternal; }


	// --- In  :	bAccept		-	if TRUE then when any object is dragged 
	//								over tab items the corresponding page 
	//								window will be activated
	// --- Out : 
	// --- Returns:	
	// --- Effect : Set/Remove drag object over support for the tab control.
	inline void AcceptDraggedObject(BOOL bAccept=TRUE) 
	{
		m_bAcceptDraggedObject=bAccept;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if when any object is dragged over tab items the 
	//				corresponding page window will be activated
	// --- Effect : Retrieves the flag that specifies whether the control 
	//				activates the corresponding page window when an object 
	//				is dragged over tab control items.
	inline BOOL IsAcceptingDraggedObject() const { return m_bAcceptDraggedObject; }


	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Calculates the rectangle that specifies the coordinates of 
	//				active page window
	void RecalcPageRect();

	// --- In  :	
	// --- Out : 
	// --- Returns:	rectangle that specifies the coordinates of active page window
	// --- Effect : Retrieves the rectangle that specifies the coordinates of 
	//				active page window
	inline CRect GetPageRect() const { return m_rectPage; }


protected:
	// retrieves unique ID for newly added page
	virtual DWORD GetUniqueId();

	// updates the size of page
	virtual void UpdatePages();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COX3DTabViewContainer)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	virtual ~COX3DTabViewContainer();

	// Generated message map functions
protected:
	//{{AFX_MSG(COX3DTabViewContainer)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    friend class COX3DTabViewDropTarget;
};

////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // _OX3DTABVIEW_H__

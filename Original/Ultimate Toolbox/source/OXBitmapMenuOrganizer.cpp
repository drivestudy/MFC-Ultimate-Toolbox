// ==========================================================================
//				Class Implementation : COXBitmapMenuOrganizer
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXBitmapMenuOrganizer.h"

#include "afxpriv.h"
#include "afxole.h"

#ifndef __OXMFCIMPL_H__
#if _MFC_VER >= 0x0700
#if _MFC_VER >= 1400
#include <afxtempl.h>
#endif
#include <..\src\mfc\afximpl.h>
#else
#include <..\src\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif


#ifndef __OX_OLEIMPL2_H__
#ifdef MAP_LOGHIM_TO_PIX
#undef MAP_LOGHIM_TO_PIX
#endif
#ifdef MAP_PIX_TO_LOGHIM
#undef MAP_PIX_TO_LOGHIM
#endif

#if _MFC_VER < 0x0700
	#include <..\src\oleimpl2.h>
#else
	#include <..\src\mfc\oleimpl2.h>
#endif

#define __OX_OLEIMPL2_H__
#endif

#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#ifndef TB_GETIMAGELIST
#define TB_GETIMAGELIST			(WM_USER + 49)
#endif // TB_GETIMAGELIST

#ifndef TB_GETHOTIMAGELIST
#define TB_GETHOTIMAGELIST		(WM_USER + 53)
#endif // TB_GETHOTIMAGELIST

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// --- A map of all the frame windows that have been subclassed and that are associated with
//     a menu organizer object.  This object will handle the windows messages first
CMap<HWND, HWND, COXBitmapMenuOrganizer*, COXBitmapMenuOrganizer*> 
COXBitmapMenuOrganizer::m_allMenuOrganizers;

// handle of the old mouse hook procedure
HHOOK COXBitmapMenuOrganizer::g_pfnOldMouseHookProc=NULL;
// handle of the old keyboard hook procedure
HHOOK COXBitmapMenuOrganizer::g_pfnOldKeyboardHookProc=NULL;
///////////////////////////////////////////



// Member functions ---------------------------------------------------------
// public:

COXBitmapMenuOrganizer::COXBitmapMenuOrganizer()
	:
	m_pFrameWnd(NULL),
	m_hWnd(NULL),
	m_pfnSuper(NULL),
	m_bInCutomizationMode(FALSE),
	m_hWndCustomizeOrganizer(NULL),
	m_bShowOnlyRecentlyUsed(TRUE),
	m_bForceToDisplayHiddenItems(FALSE),
	m_bMainMenuIsActive(FALSE),
	m_nActivePopupMenuCount(0),
	m_pDeferedBM(NULL)
{
	Init();
}

BOOL COXBitmapMenuOrganizer::Init(UINT cx, UINT cy, UINT nCheckID, UINT nBulletID)
{
	CDC memDC;
	VERIFY(memDC.CreateCompatibleDC(NULL));

	CRect rcCheckMark;
	ASSERT((!cx && !cy) || (cx && cy));
	if(!cx && !cy)
	{
		rcCheckMark=CRect(0,0,
			::GetSystemMetrics(SM_CXMENUCHECK),::GetSystemMetrics(SM_CYMENUCHECK));
		// Convert from device to logical points
		memDC.DPtoLP(&rcCheckMark);	
	}
	else
	{
		rcCheckMark=CRect(0, 0, cx, cy);
	}
	
	if(m_DefaultImgList.m_hImageList!=NULL)
	{
		m_DefaultImgList.DeleteImageList();
	}
	// The default image list will contain a checkmark and a bulletmark
	VERIFY(m_DefaultImgList.Create(rcCheckMark.right,rcCheckMark.bottom,TRUE,2,0)); 

	CBitmap bmTemp;
	VERIFY(bmTemp.CreateCompatibleBitmap(&memDC,rcCheckMark.right,rcCheckMark.bottom));
	
	// Create a checkmark bitmap
	
	ASSERT((nCheckID==NULL && nBulletID==NULL) || (nCheckID!=NULL && nCheckID!=NULL));

	CBitmap* pbmTempOld;
	if(!nCheckID)
	{
		pbmTempOld=memDC.SelectObject(&bmTemp);
		memDC.DrawFrameControl(&rcCheckMark, DFC_MENU, DFCS_MENUCHECK);
		memDC.SelectObject(pbmTempOld);
		m_DefaultImgList.Add(&bmTemp, RGB(255,255,255));
	}
	else
	{
		CBitmap bmp;
		VERIFY(bmp.LoadBitmap(nCheckID));
		m_DefaultImgList.Add(&bmp,  RGB(255,255,255));
	}

	// Create a bulletmark bitmap
	if(!nBulletID)
	{
		pbmTempOld=memDC.SelectObject(&bmTemp);
		memDC.DrawFrameControl(&rcCheckMark, DFC_MENU, DFCS_MENUBULLET);
		memDC.SelectObject(pbmTempOld);
		m_DefaultImgList.Add(&bmTemp, RGB(255,255,255));
	}
	else
	{
		CBitmap bmp;
		VERIFY(bmp.LoadBitmap(nBulletID));
		m_DefaultImgList.Add(&bmp,  RGB(255,255,255));
	}

	// Clean up
	memDC.DeleteDC();

	return TRUE;
}

void COXBitmapMenuOrganizer::Empty(BOOL bCopyText/*=TRUE*/)
{
	CString sKey;
	CImageList* pImageList;
	POSITION pos=m_mapStringToImageList.GetStartPosition();
	// Remove all the elements from m_mapStringToImageList and deletes them
	while(pos!=NULL)
	{
		m_mapStringToImageList.GetNextAssoc(pos,sKey,pImageList);
		if(pImageList!=&m_DefaultImgList)
		{
			delete pImageList;
			pImageList = NULL;
		}
	}
	m_mapStringToImageList.RemoveAll();

	DWORD wKey;
	COXImageInfo* pImageInfo;
	pos=m_mapCommandToImageInfo.GetStartPosition();
	// Remove all the elements from m_mapCommandToImageInfo
	while(pos != NULL)
	{
		m_mapCommandToImageInfo.GetNextAssoc(pos,wKey,pImageInfo);
		delete pImageInfo;
		pImageInfo = NULL;
	}
	m_mapCommandToImageInfo.RemoveAll();

	// Deletes all the instantiated COXBitmapMenus
	while(!m_BitmapMenuList.IsEmpty())
	{

		COXBitmapMenu* pBitmapMenu=m_BitmapMenuList.RemoveHead();
		TRACE(_T("\nEmpty: Found bitmapmenu %x - Line: %d, %s"), pBitmapMenu, __LINE__, __FILE__);
		RestoreBitmapMenu(pBitmapMenu,bCopyText);

		//delete pBitmapMenu;
	}
}

COXBitmapMenuOrganizer::~COXBitmapMenuOrganizer()
{
	Empty(FALSE);

	// delete deferred from RestoreBitmapMenu - 9.3 patch
	if(m_pDeferedBM != NULL) {
		delete m_pDeferedBM;
	}

	// ... Detach a possible attached frame window
	DetachFrameWnd();

	for(int nIndex=PtrToInt(m_arrCreatedPopupMenus.GetSize())-1; nIndex>=0; nIndex--)
	{
		::DestroyMenu(m_arrCreatedPopupMenus[nIndex]);
	}
	m_arrCreatedPopupMenus.RemoveAll();

	ASSERT(g_pfnOldMouseHookProc==NULL);
	ASSERT(g_pfnOldKeyboardHookProc==NULL);

}

BOOL COXBitmapMenuOrganizer::AttachFrameWnd(CFrameWnd* pFrameWnd)
{
	if((m_pFrameWnd==NULL) && (pFrameWnd != NULL))
	{
		m_pFrameWnd=pFrameWnd;
		VERIFY(SubclassFrameWindow(pFrameWnd));
		return TRUE;
	}
	else
	{
		TRACE0("COXBitmapMenuOrganizer::AttachFrameWnd : Failed because already attached or frame wnd is NULL\n");
		return FALSE;
	}
}

CFrameWnd* COXBitmapMenuOrganizer::DetachFrameWnd()
{
	CFrameWnd* pFrameWnd=m_pFrameWnd;
	if(pFrameWnd != NULL)
	{
		UnsubclassFrameWindow();
	}
	m_pFrameWnd=NULL;
	Empty();
	return pFrameWnd;
}

BOOL COXBitmapMenuOrganizer::SetMenuImage(UINT nCommandID, LPCTSTR pszBitmapID, 
										  int nBitmapIndex, int nCx)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::SetMenuImage : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	CString sUResourse=GetUniqueResourceString(pszBitmapID);
	CImageList* pImageList;
	//Doesn't add if already the resource is asociated with a ImageList
	if(!m_mapStringToImageList.Lookup(sUResourse,pImageList))
	{
		pImageList=new CImageList;
		m_mapStringToImageList[sUResourse]=pImageList;
	}
	else
	{
		VERIFY(pImageList->DeleteImageList());
	}
	pImageList->Create(pszBitmapID,nCx,0,RGB(192,192,192)); 
	ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		pImageInfo=new COXImageInfo(pImageList,nBitmapIndex);
		m_mapCommandToImageInfo[nCommandID]=pImageInfo;
	}
	else
	{
		pImageInfo->SetImageList(pImageList);
	}
	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SetMenuImage(UINT nCommandID, HICON hIcon, 
										  int nCx, int nCy)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::SetMenuImage : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	CString sUResourse=GetUniqueResourceString((UINT_PTR)hIcon);
	CImageList* pImageList;
	//Doesn't add if already the resource is asociated with a ImageList
	if(!m_mapStringToImageList.Lookup(sUResourse,pImageList))
	{
		pImageList=new CImageList;
		m_mapStringToImageList[sUResourse]=pImageList;
	}
	else
	{
		VERIFY(pImageList->DeleteImageList());
	}
	VERIFY(pImageList->Create(nCx,nCy,ILC_COLOR24|ILC_MASK,0,0)); 
	VERIFY(pImageList->Add(hIcon)!=-1);
	ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		pImageInfo=new COXImageInfo(pImageList,0);
		m_mapCommandToImageInfo[nCommandID]=pImageInfo;
	}
	else
	{
		pImageInfo->SetImageList(pImageList);
	}

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SetMenuImage(UINT nCommandID, CBitmap* pbmImage, 
										  COLORREF crMask, int nCx, int nCy)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::SetMenuImage : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	CString sUResourse=GetUniqueResourceString((UINT_PTR)pbmImage);
	CImageList* pImageList;
	//Doesn't add if already the resource is asociated with a ImageList
	if(!m_mapStringToImageList.Lookup(sUResourse,pImageList))
	{
		pImageList=new CImageList;
		m_mapStringToImageList[sUResourse]=pImageList;
	}
	else
	{
		VERIFY(pImageList->DeleteImageList());
	}
	VERIFY(pImageList->Create(nCx,nCy,ILC_COLOR24|ILC_MASK,0,0)); 
	VERIFY(pImageList->Add(pbmImage,crMask)!=-1);
	ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		pImageInfo=new COXImageInfo(pImageList,0);
		m_mapCommandToImageInfo[nCommandID]=pImageInfo;
	}
	else
	{
		pImageInfo->SetImageList(pImageList);
	}

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SetMenuHotImage(UINT nCommandID, LPCTSTR pszBitmapID, 
											 int nBitmapIndex, int nCx)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : Must attach a frame window before calling this function\n"));
		return FALSE;
	}

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : normal image must be set for the item before calling this function\n"));
		return FALSE;
	}
	ASSERT(pImageInfo!=NULL);
	CImageList* pExistingImageList=pImageInfo->GetImageList();
	ASSERT(pExistingImageList!=NULL);

	CImageList imageList;
	if(!imageList.Create(pszBitmapID,nCx,0,RGB(192,192,192)))
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : failed to load the bitmap\n"));
		return FALSE;
	}
	ASSERT((HIMAGELIST)imageList!=NULL);

	HICON hIcon=imageList.ExtractIcon(nBitmapIndex);
	ASSERT(hIcon!=NULL);

	int nIndex=pExistingImageList->Add(hIcon);
	VERIFY(::DestroyIcon(hIcon));
	if(nIndex==-1)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : failed to add bitmap to menu item's image list\n"));
		return FALSE;
	}

	pImageInfo->SetHotIndex(nIndex);

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SetMenuHotImage(UINT nCommandID, HICON hIcon)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : Must attach a frame window before calling this function\n"));
		return FALSE;
	}

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : normal image must be set for the item before calling this function\n"));
		return FALSE;
	}
	ASSERT(pImageInfo!=NULL);
	CImageList* pExistingImageList=pImageInfo->GetImageList();
	ASSERT(pExistingImageList!=NULL);

	ASSERT(hIcon!=NULL);

	int nIndex=pExistingImageList->Add(hIcon);
	if(nIndex==-1)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : failed to add bitmap to menu item's image list\n"));
		return FALSE;
	}

	pImageInfo->SetHotIndex(nIndex);

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SetMenuHotImage(UINT nCommandID, CBitmap* pbmImage, 
											 COLORREF crMask)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : Must attach a frame window before calling this function\n"));
		return FALSE;
	}

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : normal image must be set for the item before calling this function\n"));
		return FALSE;
	}
	ASSERT(pImageInfo!=NULL);
	CImageList* pExistingImageList=pImageInfo->GetImageList();
	ASSERT(pExistingImageList!=NULL);

	ASSERT(pbmImage!=NULL);

	int nIndex=pExistingImageList->Add(pbmImage,crMask);
	if(nIndex==-1)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotImage : failed to add bitmap to menu item's image list\n"));
		return FALSE;
	}

	pImageInfo->SetHotIndex(nIndex);

	return TRUE;
}



BOOL COXBitmapMenuOrganizer::SetMenuBitmap(UINT nCommandID, LPCTSTR pszBitmapID)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::SetMenuBitmap : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	CBitmap Bitmap;
	//to get the width of the bitmap
	if(!Bitmap.LoadBitmap(pszBitmapID)) 
		return FALSE;
	BITMAP BitmapInfo;
	Bitmap.GetBitmap(&BitmapInfo);
	Bitmap.DeleteObject();
	return SetMenuImage(nCommandID, pszBitmapID, 0, BitmapInfo.bmWidth);
}

BOOL COXBitmapMenuOrganizer::SetMenuBitmap(UINT nCommandID, UINT nBitmapID)
{
	return SetMenuBitmap(nCommandID,MAKEINTRESOURCE(nBitmapID));
}

BOOL COXBitmapMenuOrganizer::SetMenuHotBitmap(UINT nCommandID, LPCTSTR pszBitmapID)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::SetMenuBitmap : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	CBitmap Bitmap;
	//to get the width of the bitmap
	if(!Bitmap.LoadBitmap(pszBitmapID)) 
		return FALSE;
	BITMAP BitmapInfo;
	Bitmap.GetBitmap(&BitmapInfo);
	Bitmap.DeleteObject();
	return SetMenuHotImage(nCommandID, pszBitmapID, 0, BitmapInfo.bmWidth);
}

BOOL COXBitmapMenuOrganizer::SetMenuHotBitmap(UINT nCommandID, UINT nBitmapID)
{
	return SetMenuHotBitmap(nCommandID,MAKEINTRESOURCE(nBitmapID));
}

BOOL COXBitmapMenuOrganizer::SetMenuIcon(UINT nCommandID, LPCTSTR pszIconID)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuIcon : Must attach a frame window before calling this function\n"));
		return FALSE;
	}

	HICON hIcon;
	//Loads Icon
	hIcon=AfxGetApp()->LoadIcon(pszIconID);
	if(hIcon==NULL)
		return FALSE;
		
	CImageList* pImageList;
	CString sUResource=GetUniqueResourceString(pszIconID,FALSE);
	//if the Icon resource is not already added
	if(!m_mapStringToImageList.Lookup(sUResource,pImageList))
	{
		pImageList=new CImageList;
		//To differentiate between Icon and bitmap add "Icon" at the end
		m_mapStringToImageList[sUResource]=pImageList;
	}
	else
	{
		VERIFY(pImageList->DeleteImageList());
	}
	VERIFY(pImageList->Create(GetSystemMetrics(SM_CXICON),
		GetSystemMetrics(SM_CYICON),FALSE,1,0)); 
	VERIFY(pImageList->Add(hIcon)!=-1);
	ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		pImageInfo=new COXImageInfo(pImageList,0);
		m_mapCommandToImageInfo[nCommandID]=pImageInfo;
	}
	else
	{
		pImageInfo->SetImageList(pImageList);
	}

	return TRUE;
}
	
BOOL COXBitmapMenuOrganizer::SetMenuIcon(UINT nCommandID, UINT nIconID)
{
	return SetMenuIcon(nCommandID,MAKEINTRESOURCE(nIconID));
}

BOOL COXBitmapMenuOrganizer::SetMenuHotIcon(UINT nCommandID, LPCTSTR pszIconID)
{
	if(m_pFrameWnd==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotIcon : Must attach a frame window before calling this function\n"));
		return FALSE;
	}

	COXImageInfo* pImageInfo=NULL;
	if(!m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo))
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotIcon : normal image must be set for the item before calling this function\n"));
		return FALSE;
	}
	ASSERT(pImageInfo!=NULL);
	CImageList* pExistingImageList=pImageInfo->GetImageList();
	ASSERT(pExistingImageList!=NULL);

	HICON hIcon;
	//Loads Icon
	hIcon=AfxGetApp()->LoadIcon(pszIconID);
	if(hIcon==NULL)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotIcon : failed to load the icon\n"));
		return FALSE;
	}
		
	int nIndex=pExistingImageList->Add(hIcon);
	if(nIndex==-1)
	{
		TRACE(_T("COXBitmapMenuOrganizer::SetMenuHotIcon : failed to add the icon to menu item's image list\n"));
		return FALSE;
	}

	pImageInfo->SetHotIndex(nIndex);

	return TRUE;
}
	

BOOL COXBitmapMenuOrganizer::SetMenuHotIcon(UINT nCommandID, UINT nIconID)
{
	return SetMenuHotIcon(nCommandID,MAKEINTRESOURCE(nIconID));
}


COXImageInfo* COXBitmapMenuOrganizer::GetMenuItemImageInfo(UINT nCommandID) const
{
	COXImageInfo* pImageInfo=NULL;
	m_mapCommandToImageInfo.Lookup(nCommandID,pImageInfo);
	return pImageInfo;
}


BOOL COXBitmapMenuOrganizer::AutoSetMenuImage()
{
	if(m_pFrameWnd==NULL)
	{
		TRACE0("COXBitmapMenuOrganizer::AutoSetMenuImage : Must attach a frame window before calling this function\n");
		return FALSE;
	}

	// Empty previous settings
	Empty();

	CToolBar* pToolBar=NULL;
	UINT nID=0;
	UINT nStyle=0;
	int iImage=0;
	int iHotImage=0;

	int nListIndex=0;
	CString sListIndex;
	// Iterate all the control bars and use only the toolbars
	POSITION pos=m_pFrameWnd->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		pToolBar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)m_pFrameWnd->m_listControlBars.GetNext(pos));
		//If its a CToolBar
		if(pToolBar!=NULL)
		{
			// Get the original imagelist of the toolbar
			HIMAGELIST hOrigImageList=
				(HIMAGELIST)pToolBar->SendMessage(TB_GETIMAGELIST);
			if(hOrigImageList!=NULL)
			{
				CImageList origImageList;
				CImageList* pOrigImageList=
					CImageList::FromHandlePermanent(hOrigImageList);
				if(pOrigImageList==NULL)
				{
					VERIFY(origImageList.Attach(hOrigImageList));
					pOrigImageList=&origImageList;
				}
				ASSERT(pOrigImageList!=NULL);

				int nOrigImageCount=pOrigImageList->GetImageCount();
				ASSERT(0<nOrigImageCount);

				// Get the hot imagelist of the toolbar
				HIMAGELIST hHotImageList=
					(HIMAGELIST)pToolBar->SendMessage(TB_GETHOTIMAGELIST);
				CImageList hotImageList;
				CImageList* pHotImageList=NULL;
				int nHotImageCount=0;
				if(hHotImageList!=NULL)
				{
					pHotImageList=CImageList::FromHandlePermanent(hHotImageList);
					if(pHotImageList==NULL)
					{
						VERIFY(hotImageList.Attach(hHotImageList));
						pHotImageList=&hotImageList;
					}
					ASSERT(pHotImageList!=NULL);
					nHotImageCount=pHotImageList->GetImageCount();
					ASSERT(nHotImageCount==nOrigImageCount);
				}

				CImageList* pNewImageList=new CImageList;
				CRect buttonRect;
				pToolBar->GetItemRect(0, buttonRect);

				int nButtonIndex=0;
#if _MFC_VER > 0x0421
				VERIFY(pNewImageList->Create(pOrigImageList));
#else
				IMAGEINFO imageInfo;
				VERIFY(pOrigImageList->GetImageInfo(0,&imageInfo));
				VERIFY(pNewImageList->Create(imageInfo.rcImage.right-imageInfo.rcImage.left,
					imageInfo.rcImage.bottom-imageInfo.rcImage.top,
					ILC_COLOR24|ILC_MASK,0,0));
				for(nButtonIndex=0; nButtonIndex<nOrigImageCount; nButtonIndex++)
				{
					HICON hIcon=pOrigImageList->ExtractIcon(nButtonIndex);
					ASSERT(hIcon!=NULL);
					VERIFY(pNewImageList->Add(hIcon)!=-1);
					VERIFY(::DestroyIcon(hIcon));
				}
#endif
				for(nButtonIndex=0; nButtonIndex<nHotImageCount; nButtonIndex++)
				{
					HICON hIcon=pHotImageList->ExtractIcon(nButtonIndex);
					ASSERT(hIcon!=NULL);
					VERIFY(pNewImageList->Add(hIcon)!=-1);
					VERIFY(::DestroyIcon(hIcon));
				}

				ASSERT(pNewImageList->GetImageCount()==nHotImageCount+nOrigImageCount);

				// Close everything
				if((HIMAGELIST)hotImageList!=NULL)
					hotImageList.Detach();
				if((HIMAGELIST)origImageList!=NULL)
					origImageList.Detach();

				nListIndex++;
				sListIndex.Format(_T("%i"), nListIndex);
				m_mapStringToImageList[sListIndex]=pNewImageList;

				int nButtonCount=pToolBar->GetToolBarCtrl().GetButtonCount();
				// Iterate all buttons on the toolbar
				for(nButtonIndex=0; nButtonIndex < nButtonCount; nButtonIndex++)
				{
					nID=0;
					nStyle=0;
					iImage=0;
					iHotImage=-1;
					pToolBar->GetButtonInfo(nButtonIndex, nID, nStyle, iImage);
					if(nHotImageCount>0)
					{
						iHotImage=iImage+nOrigImageCount;
					}
					//If the Item is not a button
					if(nStyle&TBBS_SEPARATOR)
						continue;

					//Sets the mapping
					COXImageInfo* pImageInfo=NULL;
					if(!m_mapCommandToImageInfo.Lookup(nID,pImageInfo))
					{
						pImageInfo=new COXImageInfo(
							pNewImageList,iImage,FALSE,iHotImage);
						// ... Set new value
						m_mapCommandToImageInfo[nID]=pImageInfo;
					}
					else
					{
						ASSERT(pImageInfo!=NULL);
						pImageInfo->SetImageList(pNewImageList);
						pImageInfo->SetIndex(iImage);
						pImageInfo->SetHotIndex(iHotImage);
						pImageInfo->SetDefaultImgList(FALSE);
					}
				}
			}
		}
	}

	return TRUE;
}

void COXBitmapMenuOrganizer::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, 
											 BOOL bSysMenu)
{
	ASSERT(pPopupMenu->GetSafeHmenu()!=NULL);	

	// Replace the attached CMenu by a new COXBitmapMenu object
	// if necessary (if not done yet)
	CMenu* pMenu=pPopupMenu;

	COXBitmapMenu* pBitmapMenu=DYNAMIC_DOWNCAST(COXBitmapMenu, pMenu);
	//If a COXBitmapMenu is not already attached to the SubMenu
	BOOL bNewBitmapMenu=FALSE;
	if(pBitmapMenu==NULL)
	{
		pBitmapMenu=new COXBitmapMenu();
		
		// Detaches the CMenu
		HMENU hMenu=pMenu->Detach();
		
		//Attaches the COXBitmapMenu object to the menu handle
		pBitmapMenu->Attach(hMenu);
		m_BitmapMenuList.AddTail(pBitmapMenu);
		pPopupMenu=pBitmapMenu;

		bNewBitmapMenu=TRUE;
	}

#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
	BOOL bUpdateCmdUI=TRUE;
	if(m_pFrameWnd->m_pNotifyHook!=NULL)
	{
		if(m_pFrameWnd->m_pNotifyHook->m_pActiveItem!=NULL)
		{
			bUpdateCmdUI=FALSE;
		}
	}
	if(bUpdateCmdUI)
	{
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421

		// Before we begin iterating the menuitems to make them ownerdrawn, be sure
		// the CmdUI mechanism doesn't want to add or change some menuitems 
		// (E.g. MRU list)
		pBitmapMenu->OnUpdateCmdUI(CWnd::FromHandle(m_hWnd), nIndex, bSysMenu);

#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
	}
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421

	
	ConvertBitmapMenu(pBitmapMenu,bNewBitmapMenu);

	pBitmapMenu->
		SetCutomizationMode(IsInCustomizationMode(),m_hWndCustomizeOrganizer);
}


LRESULT COXBitmapMenuOrganizer::OnMenuChar(UINT nChar, UINT /* nFlags */, 
										   CMenu* pMenu)
{
	//to check wether its a COXBitmapMenu
	COXBitmapMenu* pBitmapMenu=DYNAMIC_DOWNCAST(COXBitmapMenu, pMenu);
	int nLoWord =0;
	int nHiWord =0;
	//if its a COXBitmapMenu
	if(pBitmapMenu != NULL)
	{
		int nIndex=0;
		//sets the index of the menu item for the corresponding char
		if(pBitmapMenu->m_KeyAccessMap.Lookup((TCHAR)nChar,nIndex))
		{
			nHiWord=2;
			nLoWord=nIndex;
		}
	}
	return MAKELRESULT(nLoWord,nHiWord);
}

// Measure item implementation relies on unique control/menu IDs
BOOL COXBitmapMenuOrganizer::OnMeasureItem(int /*nIDCtl*/, 
										   LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// ... Handle the cases we can handle
	BOOL bHandled=TRUE;
	CWnd* pWnd=CWnd::FromHandle(m_hWnd);
	ASSERT(pWnd != NULL);
	if(lpMeasureItemStruct->CtlType==ODT_MENU)
	{
		ASSERT(lpMeasureItemStruct->CtlID==0);
		CMenu* pMenu;

		_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
		if(pThreadState->m_hTrackingWindow==m_hWnd)
		{
			// start from popup
			pMenu=CMenu::FromHandle(pThreadState->m_hTrackingMenu);
		}
		else
		{
			// start from menubar
			pMenu=pWnd->GetMenu();
		}

		pMenu=OwnFindPopupMenuFromID(pMenu, lpMeasureItemStruct->itemID);
		if(pMenu==NULL)
		{
			if(m_arrPopupMenusTrace.GetSize()>0 && m_nActivePopupMenuCount>0)
			{
				OXPOPUPMENUTRACE popupMenu=
					m_arrPopupMenusTrace[m_nActivePopupMenuCount-1];
				pMenu=CMenu::FromHandlePermanent(popupMenu.m_hPopupMenu);
			}
		}
		if(pMenu!=NULL)
		{
			pMenu->MeasureItem(lpMeasureItemStruct);
		}
		else
		{
			TRACE0("COXBitmapMenuOrganizer::OnMeasureItem: Unknown Menu : passing to Default()\n");
			bHandled=FALSE;
		}
	}
	else
	{
		CWnd* pChild=pWnd->GetDescendantWindow(lpMeasureItemStruct->CtlID, TRUE);
		if(pChild != NULL && pChild->SendChildNotifyLastMsg())
			return bHandled;     // eaten by child
	}
	return bHandled;
}

CString COXBitmapMenuOrganizer::GetUniqueResourceString(UINT_PTR uResourceID,
														BOOL bBitmapIcon)
{
	CString sRet;
	CString sNum;
	sNum.Format(_T("%u"), uResourceID);
	sNum.ReleaseBuffer();
	sRet=CString((TCHAR)CHAR_MAX)+sNum+ 
		CString(bBitmapIcon ? (TCHAR)CHAR_MAX : (TCHAR)CHAR_MIN);
	return sRet;
}

CString COXBitmapMenuOrganizer::GetUniqueResourceString(LPCTSTR pszResource,
														BOOL bBitmapIcon)
{
	if(HIWORD((DWORD_PTR)pszResource)==0)
	{
		return GetUniqueResourceString(LOWORD((DWORD_PTR)pszResource),bBitmapIcon);
	}
	else
		return CString(pszResource)+
			CString(bBitmapIcon ? (TCHAR)CHAR_MAX : (TCHAR)CHAR_MIN);
}

BOOL COXBitmapMenuOrganizer::SubclassFrameWindow(CFrameWnd* pFrameWnd)
	// --- In  : pFrameWnd : The frame  window 
	// --- Out : 
	// --- Returns : Whether it was successful or not
	// --- Effect : This function subclasses the frame window 
{
	ASSERT(pFrameWnd != NULL);
	ASSERT(pFrameWnd->GetSafeHwnd() != NULL);
	ASSERT_VALID(pFrameWnd);

	if(m_pfnSuper != NULL)
	{
		// Already subclasses, check that hWnd and window proc is correct
		if((m_hWnd != pFrameWnd->GetSafeHwnd()) || 
		     ((WNDPROC)::GetWindowLongPtr(pFrameWnd->GetSafeHwnd(),GWL_WNDPROC)!= 
			 GlobalMenuOrganizerProc))
		{
			TRACE0("COXBitmapMenuOrganizer::SubclassFrameWindow : Failed because already subclassed by other window proc\n");
			return FALSE;
		}
		return TRUE;
	}

	ASSERT(m_hWnd==NULL);
	ASSERT(m_pfnSuper==NULL);
#ifdef _DEBUG
	COXBitmapMenuOrganizer* pDummyLayoutManager=NULL;
	// ... Should not yet be in list of subclassed frame windows
	ASSERT(!m_allMenuOrganizers.Lookup(m_hWnd, pDummyLayoutManager));
#endif // _DEBUG

	// ... Subclass window (Windows way, not MFC : because may already be subclessed by MFC)
	m_hWnd=pFrameWnd->GetSafeHwnd();
	m_pfnSuper=(WNDPROC)::GetWindowLongPtr(pFrameWnd->GetSafeHwnd(), GWL_WNDPROC);
	::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)GlobalMenuOrganizerProc);

	// ... Store in the global map
	m_allMenuOrganizers.SetAt(m_hWnd, this);

	ASSERT_VALID(this);
	return (m_hWnd != NULL);;
}

void COXBitmapMenuOrganizer::UnsubclassFrameWindow()
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : This function unsubclasses the window 
	//				It removes this object from the map
	//				When it is the last in the list it restores the original
	//				windows procedure
{
	ASSERT_VALID(this);

	if(m_hWnd != NULL)
	{
		// Put back original window procedure
		ASSERT(m_pfnSuper != NULL);
		ASSERT(m_pfnSuper != GlobalMenuOrganizerProc);
		// ... GlobalLayoutManagerProc is not used anymore : set WNDPROC back to original value
		::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)m_pfnSuper);
		// ... Remove use from global map
		m_allMenuOrganizers.RemoveKey(m_hWnd);
		m_hWnd=NULL;
		m_pfnSuper=NULL;
	}

	ASSERT(m_hWnd==NULL);
	ASSERT(m_pfnSuper==NULL);

	ASSERT_VALID(this);
}

LRESULT CALLBACK COXBitmapMenuOrganizer::GlobalMenuOrganizerProc(HWND hWnd, 
																 UINT uMsg, 
																 WPARAM wParam, 
																 LPARAM lParam)
	// --- In  : hWnd : 
	//			 uMsg : 
	//			 wParam : 
	//			 lParam :
	// --- Out : 
	// --- Returns : The result of the message
	// --- Effect : This is the global windows procedure of all the menu organizers
	//              objects that have subclasses a window
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	COXBitmapMenuOrganizer* pBitmapMenuOrganizer=NULL;

	VERIFY(m_allMenuOrganizers.Lookup(hWnd, pBitmapMenuOrganizer));
	ASSERT_VALID(pBitmapMenuOrganizer);
	return pBitmapMenuOrganizer->MenuOrganizerProc(hWnd, uMsg, wParam, lParam);
}


LRESULT COXBitmapMenuOrganizer::MenuOrganizerProc(HWND hWnd, UINT uMsg, 
												  WPARAM wParam, LPARAM lParam)
	// --- In  : hWnd : 
	//			 uMsg : 
	//			 wParam : 
	//			 lParam :
	// --- Out : 
	// --- Returns: The result of the message
	// --- Effect : This is the member function called by the windows procedure of the 
	//				COXBitmapMenuOrganizer object
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT_VALID(this);
	ASSERT(hWnd==m_hWnd);

	// Handling before base class
	BOOL bHandled=FALSE;
	LRESULT result=0;

	// Let the original message procedure handle it first of all
	ASSERT(m_pfnSuper != NULL);
	ASSERT(m_pfnSuper != GlobalMenuOrganizerProc);
	if(uMsg!=WM_MEASUREITEM)
	{
		result=CallWindowProc(m_pfnSuper,hWnd, uMsg, wParam, lParam);
	}

	if(!::IsWindow(hWnd))
	{
		return result;
	}

	switch (uMsg)
	{
	case WM_MENUCHAR:
		{
			CMenu* pMenu=CMenu::FromHandle((HMENU)lParam);
			int nChar=towlower(LOWORD(wParam));
			int nFlags=HIWORD(wParam);
			result=OnMenuChar(nChar, nFlags, pMenu);
			bHandled=TRUE;
			break;
		}
	
	case WM_INITMENUPOPUP:
		{
			CMenu* pPopupMenu=CMenu::FromHandle((HMENU)wParam);
			UINT nIndex=(UINT) LOWORD(lParam); 
			BOOL bSysMenu=(BOOL) HIWORD(lParam);

			UpdateActivePopupMenuCount();
			int nTraceSize=PtrToInt(m_arrPopupMenusTrace.GetSize());
			if(nTraceSize>m_nActivePopupMenuCount)
			{
				for(int nMenuIndex=nTraceSize-1; 
					nMenuIndex>=m_nActivePopupMenuCount; 
					nMenuIndex--)
				{
					// update map of all popup menu windows
					if(nMenuIndex==m_nActivePopupMenuCount)
					{
						// reuse window object
						HWND hPopupMenuWnd=NULL;
						if(m_mapPopupMenuWindows.Lookup(
							m_arrPopupMenusTrace[nMenuIndex].m_hPopupMenu,
							hPopupMenuWnd))
						{
							m_mapPopupMenuWindows.SetAt((HMENU)wParam,hPopupMenuWnd);
						}
					}
					m_mapPopupMenuWindows.RemoveKey(
						m_arrPopupMenusTrace[nMenuIndex].m_hPopupMenu);

					m_arrPopupMenusTrace.RemoveAt(nMenuIndex);
				}
			}
			m_nActivePopupMenuCount++;

			// Don't make system menu's owner-drawn.  We can't retrieve them
			// in OnMeasureitem.
			if(bSysMenu)
			{
				TRACE(_T("COXBitmapMenuOrganizer::MenuOrganizerProc: System Menu skipping Ownerdraw\n"));
				bHandled=FALSE;
				break;
			}

			OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

			OXPOPUPMENUTRACE pmt;
			pmt.m_hPopupMenu=(HMENU)wParam;
			m_arrPopupMenusTrace.Add(pmt);
			////////////////////////////////

			// the default msg proc further only deals with CmdUI and that is already
			// done in OnUpdateCmdUI, so halt further processing
			bHandled=TRUE;
			break;
		}

	case WM_MEASUREITEM:
		{
			int nIDCtl=(int)wParam;
			LPMEASUREITEMSTRUCT lpMeasureItemStruct=
				(LPMEASUREITEMSTRUCT)(LPTSTR)lParam;
			result=OnMeasureItem(nIDCtl,lpMeasureItemStruct);
			bHandled=TRUE;
			break;
		}

	case WM_COMMAND:
		{
			if(HIWORD(wParam)==0)
			{
				int nCmdID=LOWORD(wParam);

				if(nCmdID==ID_OX_SHOWALLITEMS)
				{
					m_bForceToDisplayHiddenItems=TRUE;

					// current menu
					UpdateActivePopupMenuCount();
					ASSERT(m_nActivePopupMenuCount>0);
					OXPOPUPMENUTRACE pmt=
						m_arrPopupMenusTrace[m_nActivePopupMenuCount-1];

					// handle these event differently for menu bar and standard menu
					// in the case the top level popup menu is being displayed.
					// In the case GetMenu() returns NULL we assume that we deal 
					// with menu bar
					BOOL bMenuBar=(m_pFrameWnd->GetMenu()==NULL);
					BOOL bSpecialCase=(bMenuBar && m_nActivePopupMenuCount==1);

					if(bSpecialCase)
					{
						m_pFrameWnd->LockWindowUpdate();
					}

					HWND hWndSendRepopulateMsg=NULL;
					if(!bSpecialCase)
					{
						if(m_nActivePopupMenuCount>1)
						{
							VERIFY(m_mapPopupMenuWindows.Lookup(
								m_arrPopupMenusTrace[m_nActivePopupMenuCount-2].
								m_hPopupMenu,hWndSendRepopulateMsg));
						}
						else
						{
							hWndSendRepopulateMsg=m_pFrameWnd->GetSafeHwnd();
						}
						ASSERT(hWndSendRepopulateMsg!=NULL);
					}

					// hide current menu
					HWND hWnd=NULL;
					VERIFY(m_mapPopupMenuWindows.Lookup(pmt.m_hPopupMenu,hWnd));
					ASSERT(hWnd!=NULL);
					if(bSpecialCase || bMenuBar)
					{
						::SendMessage(hWnd,WM_KEYDOWN,VK_ESCAPE,0x00010001);
					}
					else
					{
						::PostMessage(hWnd,WM_KEYDOWN,VK_ESCAPE,0x00010001);
					}

					if(bSpecialCase)
					{
						m_pFrameWnd->UnlockWindowUpdate();
					}
						
					// repopulate menus
					POSITION pos=m_BitmapMenuList.GetHeadPosition();
					while(pos!=NULL)
					{
						COXBitmapMenu* pBitmapMenu=m_BitmapMenuList.GetNext(pos);
						ASSERT(pBitmapMenu!=NULL);
						HWND hWnd=NULL;
						if(!::IsMenu(pBitmapMenu->m_hMenu) || 
							!m_mapPopupMenuWindows.Lookup(
							pBitmapMenu->GetSafeHmenu(),hWnd) || !::IsWindow(hWnd) ||
							pBitmapMenu->GetSafeHmenu()==pmt.m_hPopupMenu)
						{
							// restore the menu
							RestoreBitmapMenu(pBitmapMenu,TRUE);
						}
					}

					if(!bSpecialCase)
					{
						if(bMenuBar)
						{
							// display the menu again
							::SendMessage(hWndSendRepopulateMsg,
								WM_KEYDOWN,VK_RETURN,0x001c0001);
						}
						else
						{
							// display the menu again
							::PostMessage(hWndSendRepopulateMsg,
								WM_KEYDOWN,VK_RETURN,0x001c0001);
						}
					}
				}
				else if(nCmdID>0 && IsShowOnlyRecentlyUsedItems() && 
					m_arrPopupMenusTrace.GetSize()>0)
				{
					BOOL bMarkMenuAsChanged=FALSE;
					if(!IsRecentlyUsed(nCmdID))
					{
						AddToRecentlyUsed(nCmdID);
						bMarkMenuAsChanged=TRUE;
					}
					for(int nIndex=1; 
						nIndex<m_arrPopupMenusTrace.GetSize(); nIndex++)
					{
						HMENU hPopupMenu=m_arrPopupMenusTrace[nIndex].m_hPopupMenu;
						if(!IsSubmenuRecentlyUsed(hPopupMenu))
						{
							AddSubmenuToRecentlyUsed(hPopupMenu);
							bMarkMenuAsChanged=TRUE;
						}
					}

					if(bMarkMenuAsChanged && IsAttached())
					{
						m_pFrameWnd->SendMessage(WM_OX_MENUCHANGED);
					}
				}

				bHandled=TRUE;
			}
			break;
		}

	case WM_ENTERMENULOOP:
		{
			if(wParam==0)
			{
				m_bMainMenuIsActive=TRUE;
			}
			
			bHandled=TRUE;

			break;
		}

	case WM_EXITMENULOOP:
		{
			if(wParam==0)
			{
				if(m_bForceToDisplayHiddenItems)
				{
					// repopulate menus
					m_bForceToDisplayHiddenItems=FALSE;
					if(IsShowOnlyRecentlyUsedItems())
					{
						POSITION pos=m_BitmapMenuList.GetHeadPosition();
						while(pos!=NULL)
						{
							COXBitmapMenu* pBitmapMenu=m_BitmapMenuList.GetNext(pos);
							RestoreBitmapMenu(pBitmapMenu,TRUE);
							
							TRACE(_T("\n %x menu should be removed by RestoreBitmapMenu"), pBitmapMenu);
							//delete pBitmapMenu;
						}
					}
				}

				m_bMainMenuIsActive=FALSE;

				m_nActivePopupMenuCount=0;
			}

			// unhook mouse messages
			if(g_pfnOldMouseHookProc!=NULL)
			{
				VERIFY(::UnhookWindowsHookEx(g_pfnOldMouseHookProc));
				g_pfnOldMouseHookProc=NULL;
			}

			// unhook keyboard messages
			if(g_pfnOldKeyboardHookProc!=NULL)
			{
				VERIFY(::UnhookWindowsHookEx(g_pfnOldKeyboardHookProc));
				g_pfnOldKeyboardHookProc=NULL;
			}
			
			bHandled=TRUE;

			break;
		}

	case WM_MENUSELECT:
		{
			if(lParam!=NULL && m_arrPopupMenusTrace.GetSize()>0)
			{
				OXPOPUPMENUTRACE& pmt=
					m_arrPopupMenusTrace[m_arrPopupMenusTrace.GetSize()-1];
				if(pmt.m_hPopupMenu==(HMENU)lParam)
				{
					// save Top/Left coordinates of the menu window
					CWnd* pPopupWnd=CWnd::GetDesktopWindow()->GetTopWindow();
					ASSERT(pPopupWnd!=NULL);
					CRect rectPopupWnd;
					pPopupWnd->GetWindowRect(rectPopupWnd);
					pmt.m_ptTopLeft=rectPopupWnd.TopLeft();
					////////////////////////////////
					m_mapPopupMenuWindows.
						SetAt((HMENU)lParam,pPopupWnd->GetSafeHwnd());
					if(IsShowOnlyRecentlyUsedItems())
					{
						// setup hooks for mouse and keyboard messages
						if(g_pfnOldMouseHookProc==NULL)
						{
							g_pfnOldMouseHookProc=::SetWindowsHookEx(WH_MOUSE,
								MenuMouseHookProc,NULL,::GetCurrentThreadId());
						}
						if(g_pfnOldKeyboardHookProc==NULL)
						{
							g_pfnOldKeyboardHookProc=::SetWindowsHookEx(WH_KEYBOARD,
								MenuKeyboardHookProc,NULL,::GetCurrentThreadId());
						}
						//////////////////////////////////////////////
					}
				}
			}
/*
			UINT nFlags=HIWORD(wParam);
			if((nFlags&MF_HILITE)!=0 && !(nFlags==0xffff && lParam==NULL) &&
				LOWORD(wParam)!=-1)
			{
				while(m_nActivePopupMenuCount>0 && m_arrPopupMenusTrace[
					m_nActivePopupMenuCount-1].m_hPopupMenu!=(HMENU)lParam)
				{
					m_nActivePopupMenuCount--;
				}
			}
*/
			break;
		}

	case WM_OX_SAVEMENUSTATE:
		{
			// save info about recently used submenu items
			HMENU hMenu=(HMENU)wParam;
			CString sKey=(LPCTSTR)lParam;
			if(hMenu!=NULL)
			{
				VERIFY(SaveRUSubmenusState(hMenu,
					sKey+_T("_RecentlyUsedSubmenuItems")));
			}
		}

	case WM_OX_LOADMENUSTATE:
		{
			// load info about recently used submenu items
			HMENU hMenu=(HMENU)wParam;
			CString sKey=(LPCTSTR)lParam;
			if(hMenu!=NULL)
			{
				VERIFY(LoadRUSubmenusState(hMenu,
					sKey+_T("_RecentlyUsedSubmenuItems")));
			}
		}

	default:
			// Do nothing special
			;
	}

	return result;
}


void COXBitmapMenuOrganizer::UpdateActivePopupMenuCount()
{
	int nTraceLength=PtrToInt(m_arrPopupMenusTrace.GetSize());

	if(nTraceLength==0)
	{
		m_nActivePopupMenuCount=0;
		return;
	}
	
	int nIndex=0;
	for(nIndex=0; nIndex<nTraceLength; nIndex++)
	{
		HWND hWnd=NULL;
		if(!m_mapPopupMenuWindows.
			Lookup(m_arrPopupMenusTrace[nIndex].m_hPopupMenu,hWnd))
		{
			break;
		}
	
		if(!::IsWindow(hWnd) || !::IsWindowVisible(hWnd))
		{
			break;
		}
	}

	if(nIndex<m_nActivePopupMenuCount)
	{
		m_nActivePopupMenuCount=nIndex;
	}
}


CMenu* COXBitmapMenuOrganizer::OwnFindPopupMenuFromID(CMenu* pMenu, UINT nID)
{
	ASSERT_VALID(pMenu);

	// walk through all items, looking for ID match
	UINT_PTR nItems=pMenu->GetMenuItemCount();
	UINT_PTR nItemID=0;
	for (int iItem=0; iItem < (int)nItems; iItem++)
	{
		// Always check the menu item itself first
		nItemID=pMenu->GetMenuItemID(iItem);
		if(nItemID==(UINT)-1)
		{
			// ... If it is a cascade menu, use its handle instead of its ID (which is always -1)
			//     So appearently Windows passes the HMENU instead of the ID for cascade manus
			//     in MEASUREITEMSTRUCT when sending WM_MEASUREITEM
			nItemID=(UINT_PTR)pMenu->GetSubMenu(iItem)->GetSafeHmenu();
		}

		if(nItemID==nID || nID==-1)
		{
			// ... it is an item inside our popup
			pMenu=CMenu::FromHandlePermanent(pMenu->m_hMenu);
			return pMenu;
		}

		// Then check child menus
		CMenu* pPopup=pMenu->GetSubMenu(iItem);
		if(pPopup != NULL)
		{
			// ... Recurse to child popup
			pPopup=OwnFindPopupMenuFromID(pPopup, nID);
			// ... Check popups on this popup
			if(pPopup != NULL)
				return pPopup;
		}
	}
	// ... Not found
	return NULL;
}

void COXBitmapMenuOrganizer::ConvertBitmapMenu(COXBitmapMenu* pBitmapMenu,
											   BOOL bNewBitmapMenu/*=TRUE*/)
{
	ASSERT(pBitmapMenu!=NULL);

	UINT uItemID;
	COXImageInfo* pImageInfo;
	int nCount=pBitmapMenu->GetMenuItemCount();

	int nAmpIndex;
	MENUITEMINFO miiGet, miiPut;
	CString sText;

	COXArrSavedMenuItem arrSavedMenuItems;
	BOOL bPrevSepBeforeHidden=TRUE;

	//Make all the menu items OwnerDrawn
	int nIndex=0;
	for(nIndex=0; nIndex < nCount; nIndex++)
	{
		// Even cascade menu item should be made owner drawn
		//  so that the text of these items is aligned properly

		::ZeroMemory(&miiGet, sizeof(miiGet));
		miiGet.cbSize=sizeof(miiGet);
		miiGet.fMask=MIIM_TYPE|MIIM_ID|MIIM_STATE|MIIM_DATA|
			MIIM_SUBMENU|MIIM_CHECKMARKS;
		miiGet.fType=MFT_STRING;
		miiGet.dwTypeData = NULL;
		::GetMenuItemInfo(pBitmapMenu->GetSafeHmenu(),nIndex,TRUE,&miiGet);
		++miiGet.cch;
		miiGet.dwTypeData=sText.GetBuffer(miiGet.cch + 1);
		// Gets the item info
		miiGet.dwTypeData[0]=_T('\0');
		::GetMenuItemInfo(pBitmapMenu->GetSafeHmenu(),nIndex,TRUE,&miiGet);
		sText.ReleaseBuffer();
		
#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
		if(miiGet.fType & MFT_SEPARATOR)
		{
			if(m_pFrameWnd->m_pNotifyHook!=NULL)
			{
				if(m_pFrameWnd->m_pNotifyHook->m_pActiveItem!=NULL)
				{
//					continue;
				}
			}
		}
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421

		if(bNewBitmapMenu && !m_bForceToDisplayHiddenItems && 
			!IsInCustomizationMode() && IsShowOnlyRecentlyUsedItems() && 
			(!IsRecentlyUsed(miiGet.wID) || 
			!IsSubmenuRecentlyUsed(miiGet.hSubMenu) ||
			((miiGet.fType & MFT_SEPARATOR) && bPrevSepBeforeHidden)))
		{
			OXSAVEDMENUITEM smi;
			smi.nIndex=nIndex;
			smi.miInfo=miiGet;
			if(miiGet.hSubMenu!=NULL)
			{
				smi.miInfo.hSubMenu=CopyPopupMenu(miiGet.hSubMenu);
			}
			smi.sText=sText;
			ASSERT(smi.miInfo.wID!=-1 || ::IsMenu(smi.miInfo.hSubMenu));
			arrSavedMenuItems.Add(smi);

			if(!(miiGet.fType & MFT_SEPARATOR) && !bPrevSepBeforeHidden && nIndex>0)
			{
				if(pBitmapMenu->GetMenuItemID(nIndex-1)==0)
				{
					bPrevSepBeforeHidden=TRUE;
				}
			}
		}
		else
		{
			BOOL bConvertToOwnerdraw=((miiGet.fType & MFT_OWNERDRAW)==0);

			uItemID=((miiGet.hSubMenu==NULL) ? miiGet.wID : (UINT)-1);

			//Makes the item owner-drawn and sets the ItemInfo to the ItemData
			// which in turn will be used in COXBitmapMenu while drawing
			::ZeroMemory(&miiPut, sizeof(miiPut));
			miiPut.cbSize=sizeof(miiPut);
			miiPut.fMask=(bConvertToOwnerdraw ? MIIM_TYPE : 0) | MIIM_DATA | MIIM_ID;
			miiPut.fType=(miiGet.fType & ~MFT_STRING) | MFT_OWNERDRAW;

			switch(miiGet.wID)
			{
				// Restore
			case 61728:
				miiPut.wID = ID_OXBITMAPMENU_RESTORE;
				break;
				// Minimize
			case 61472:
				miiPut.wID = ID_OXBITMAPMENU_MINIMIZE;
				break;
				// Close
			case 61536:
				miiPut.wID = ID_OXBITMAPMENU_CLOSE;
				break;
			default:
				miiPut.wID = miiGet.wID;
			}			
	
			// Gets the ImageInfo for the command if exist
			if(!m_mapCommandToImageInfo.Lookup(uItemID,pImageInfo))
			{
				CImageList* pImageList;
				CString sUResource=GetUniqueResourceString(uItemID,FALSE);
				
				// if the Normal menuitem is not already added
				if(!m_mapStringToImageList.Lookup(sUResource, pImageList))
				{
					m_mapStringToImageList[sUResource]=pImageList=&m_DefaultImgList;
				}

				ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));
				pImageInfo=new COXImageInfo(pImageList, 0, TRUE);
				//sets the mapping between MenuItem ID and Image Info
				m_mapCommandToImageInfo[uItemID]=pImageInfo;
			}			

			COXItemInfo* pItemInfo=NULL;
			if(miiGet.dwItemData != 0 && !bNewBitmapMenu)
			{
				pItemInfo=(COXItemInfo*)miiGet.dwItemData;
				miiPut.dwItemData=(DWORD_PTR)pItemInfo;
				if(miiGet.fType==MFT_STRING)
				{
					pItemInfo->SetText(sText);
				}
			}
			else
			{		

				pItemInfo=new COXItemInfo(pImageInfo, sText);
				TRACE(_T("\nCreated COXItemInfo at %x - Line: %d, %s"), pItemInfo, __LINE__, __FILE__);
				miiPut.dwItemData=(DWORD_PTR)pItemInfo;
				pBitmapMenu->AddItemInfo(pItemInfo);
			}			

			// Put the menuitem with the new data back in its place
			::SetMenuItemInfo(pBitmapMenu->GetSafeHmenu(),nIndex,TRUE,&miiPut);

			// A mapping is maintained for keyboard access chars of menu items and 
			// the commands IDs.
			nAmpIndex=sText.Find('&'); 
			TCHAR LoCase=_T('\0');
			if(nAmpIndex!=-1)
			{
				LoCase=(TCHAR)towlower(sText[nAmpIndex+1]);
				(pBitmapMenu->m_KeyAccessMap)[LoCase]=nIndex;
			}

			bPrevSepBeforeHidden=FALSE;			
		}
	}	

	if(bNewBitmapMenu)
	{
		if(arrSavedMenuItems.GetSize()>0)
		{			
			// Gets the ImageInfo for the command if exist
			if(!m_mapCommandToImageInfo.Lookup(ID_OX_SHOWALLITEMS,pImageInfo))
			{
				CString sUResource=GetUniqueResourceString(ID_OX_SHOWALLITEMS,FALSE);
				CImageList* pImageList;
				m_mapStringToImageList[sUResource]=pImageList=&m_DefaultImgList;
				ASSERT(AfxIsValidAddress(pImageList, sizeof(CImageList)));
				pImageInfo=new COXImageInfo(pImageList, 0, TRUE);
				//sets the mapping between MenuItem ID and Image Info
				m_mapCommandToImageInfo[ID_OX_SHOWALLITEMS]=pImageInfo;
			}			

			COXItemInfo* pItemInfo=new COXItemInfo(pImageInfo,_T(""));
			pBitmapMenu->AddItemInfo(pItemInfo);

			// add expansion item
			pBitmapMenu->
				AppendMenu(MF_OWNERDRAW,ID_OX_SHOWALLITEMS,(LPCTSTR)pItemInfo);
		}		

		ASSERT(!m_mapSavedMenuItems.RemoveKey(pBitmapMenu));
		m_mapSavedMenuItems.SetAt(pBitmapMenu,arrSavedMenuItems);
		
		for(nIndex= PtrToInt(arrSavedMenuItems.GetSize())-1; nIndex>=0; nIndex--)
		{
			OXSAVEDMENUITEM smi=arrSavedMenuItems[nIndex];
			VERIFY(pBitmapMenu->DeleteMenu(smi.nIndex,MF_BYPOSITION));
		}
	}


	//Calculates the various internal extents
	pBitmapMenu->CalcExtents();
}


void COXBitmapMenuOrganizer::RestoreBitmapMenu(COXBitmapMenu* pBitmapMenu, 
											   BOOL bCopyText/*=TRUE*/)
{
	BOOL bDeferDeletion = FALSE;

	// remove it from the list if it hasn't been removed yet
	POSITION pos=m_BitmapMenuList.GetHeadPosition();
	while(pos!=NULL)
	{
		if(m_BitmapMenuList.GetAt(pos)==pBitmapMenu)
		{
			TRACE(_T("\nRemoving bitmapmenu at %x - Line: %d, %s"), pBitmapMenu, __LINE__, __FILE__);
			m_BitmapMenuList.RemoveAt(pos);
			break;
		}
		m_BitmapMenuList.GetNext(pos);
	}

	// remove expansion item
	if(::IsMenu(pBitmapMenu->m_hMenu) && pBitmapMenu->GetMenuItemCount()>0)
	{
		if(pBitmapMenu->GetMenuItemID(pBitmapMenu->GetMenuItemCount()-1)==
			ID_OX_SHOWALLITEMS)
		{
			bDeferDeletion = TRUE;
			pBitmapMenu->DeleteMenu(pBitmapMenu->GetMenuItemCount()-1,MF_BYPOSITION);
		}
	}

	MENUITEMINFO miiGet, miiPut;
	CString sText;

	if(bCopyText && ::IsMenu(pBitmapMenu->m_hMenu))
	{
		int nCount=pBitmapMenu->GetMenuItemCount();

		// Put the strings of the menu's back in, the ownerdraw state has emptied them.
		for(int nIndex=0; nIndex < nCount; nIndex++)
		{
			::ZeroMemory(&miiGet, sizeof(miiGet));
			miiGet.cbSize=sizeof(miiGet);
			miiGet.fMask=MIIM_TYPE | MIIM_DATA;
			// Gets the item info
			::GetMenuItemInfo(pBitmapMenu->GetSafeHmenu(), nIndex, TRUE, &miiGet);

			// all item must have been converted to ownerdraw
			// only the items that has been added programmatically 
			// and hasn't been displayed yet won't have ownerdraw style
			if((miiGet.fType&MFT_OWNERDRAW)!=0)
			{
				COXItemInfo* pItemInfo=(COXItemInfo*)miiGet.dwItemData;
				if(pItemInfo!=NULL)
				{
					sText=pItemInfo->GetText();

					// Put the text text back into the menu
					::ZeroMemory(&miiPut, sizeof(miiPut));
					miiPut.cbSize=sizeof(miiPut);
					miiPut.fMask=MIIM_TYPE|MIIM_DATA;
					if((miiGet.fType&MFT_SEPARATOR)==0)
					{
						miiPut.fType=(miiGet.fType&~MFT_OWNERDRAW)|MFT_STRING;
					}
					else
					{
						miiPut.fType=(miiGet.fType&~MFT_OWNERDRAW);
					}
					miiPut.cch=sText.GetLength();
					miiPut.dwTypeData=sText.GetBuffer(miiPut.cch);
					miiPut.dwItemData=0;
					// Put the menuitem with the new data back in its place
					::SetMenuItemInfo(pBitmapMenu->GetSafeHmenu(),nIndex,TRUE,&miiPut);
					sText.ReleaseBuffer();
				}
			}
		}
	}

	COXArrSavedMenuItem arrSavedMenuItems;
	VERIFY(m_mapSavedMenuItems.Lookup(pBitmapMenu,arrSavedMenuItems));
	if(::IsMenu(pBitmapMenu->m_hMenu))
	{
		for(int nIndex=0; nIndex<arrSavedMenuItems.GetSize(); nIndex++)
		{
			OXSAVEDMENUITEM smi=arrSavedMenuItems[nIndex];
			smi.miInfo.dwTypeData=(LPTSTR)((LPCTSTR)smi.sText);
			VERIFY(::InsertMenuItem(pBitmapMenu->GetSafeHmenu(),
				smi.nIndex,TRUE,&smi.miInfo));
		}
	}
	VERIFY(m_mapSavedMenuItems.RemoveKey(pBitmapMenu));


	//TRACE(_T("\npBitmapMenu at %0x\n"),pBitmapMenu);

	//  - 9.3 patch - defer deletion of bitmap till restored bitmap displayed 
	if(!bDeferDeletion) {
		pBitmapMenu->Detach();
		delete pBitmapMenu;
		pBitmapMenu = NULL;
	}
	else {
		if( m_pDeferedBM != NULL) {
			delete m_pDeferedBM;
		}
		pBitmapMenu->Detach();
		m_pDeferedBM = pBitmapMenu;
	}
}


void COXBitmapMenuOrganizer::ShowOnlyRecentlyUsedItems(BOOL bShowOnlyRecentlyUsed)
{
	if(bShowOnlyRecentlyUsed!=m_bShowOnlyRecentlyUsed)
	{	
		m_bShowOnlyRecentlyUsed=bShowOnlyRecentlyUsed;

		POSITION pos=m_BitmapMenuList.GetHeadPosition();
		while(pos!=NULL)
		{
			COXBitmapMenu* pBitmapMenu=m_BitmapMenuList.GetNext(pos);
			COXBitmapMenuPopupWnd* pPopupWnd=pBitmapMenu->GetPopupWnd();
			if(pPopupWnd!=NULL && ::IsWindow(pPopupWnd->GetSafeHwnd()))
			{
				pPopupWnd->OnMenuChanged();
			}
		}
	}
}


BOOL COXBitmapMenuOrganizer::ExcludeFromRecentlyUsed(int nCmdID)
{
	ASSERT(nCmdID>=0);
	
	if(nCmdID==0)
	{
		// separators cannot be hidden
		return FALSE;
	}
	
	if(!IsRecentlyUsed(nCmdID))
	{
		// not a recently used item
		return FALSE;
	}

	m_mapHiddenCommands.SetAt(nCmdID,0);
	
	return TRUE;
}


BOOL COXBitmapMenuOrganizer::ExcludeFromRecentlyUsed(CMenu* pMenu, int nItemIndex)
{
	ASSERT(pMenu!=NULL);
	ASSERT(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount());
	
	int nCmdID=(int)pMenu->GetMenuItemID(nItemIndex);
	if(nCmdID==-1)
	{
		// submenu
		ASSERT(pMenu->GetSubMenu(nItemIndex)!=NULL);
		return ExcludeSubmenuFromRecentlyUsed(
			pMenu->GetSubMenu(nItemIndex)->GetSafeHmenu());
	}
	else
	{
		return ExcludeFromRecentlyUsed(nCmdID);
	}
}


BOOL COXBitmapMenuOrganizer::ExcludeSubmenuFromRecentlyUsed(HMENU hPopupMenu)
{
	ASSERT(hPopupMenu!=NULL);
	
	if(!IsSubmenuRecentlyUsed(hPopupMenu))
	{
		// not a recently used submenu
		return FALSE;
	}

	m_mapHiddenSubmenus.SetAt(hPopupMenu,0);
	
	return TRUE;
}



BOOL COXBitmapMenuOrganizer::AddToRecentlyUsed(int nCmdID)
{
	ASSERT(nCmdID>0);

	if(!m_mapHiddenCommands.RemoveKey(nCmdID))
	{
		// this command is already marked as recently used
		return FALSE;
	}

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::AddToRecentlyUsed(CMenu* pMenu, int nItemIndex)
{
	ASSERT(pMenu!=NULL);
	ASSERT(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount());

	int nCmdID=(int)pMenu->GetMenuItemID(nItemIndex);
	if(nCmdID==-1)
	{
		// submenu
		ASSERT(pMenu->GetSubMenu(nItemIndex)!=NULL);
		return AddSubmenuToRecentlyUsed(pMenu->GetSubMenu(nItemIndex)->GetSafeHmenu());
	}
	else
	{
		return AddToRecentlyUsed(nCmdID);
	}
}


BOOL COXBitmapMenuOrganizer::AddSubmenuToRecentlyUsed(HMENU hPopupMenu)
{
	ASSERT(hPopupMenu!=NULL);

	if(!m_mapHiddenSubmenus.RemoveKey(hPopupMenu))
	{
		// this submenu is already marked as recently used
		return FALSE;
	}

	return TRUE;
}



BOOL COXBitmapMenuOrganizer::IsRecentlyUsed(int nCmdID) const 
{ 
	int nDummy=-1;
	return (!m_mapHiddenCommands.Lookup(nCmdID,nDummy)); 
}


BOOL COXBitmapMenuOrganizer::IsRecentlyUsed(CMenu* pMenu, int nItemIndex) const 
{ 
	if (nItemIndex == -1)
		return TRUE;
	ASSERT(pMenu!=NULL);
	ASSERT(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount());

	int nCmdID=(int)pMenu->GetMenuItemID(nItemIndex);
	if(nCmdID==-1)
	{
		// submenu
		ASSERT(pMenu->GetSubMenu(nItemIndex)!=NULL);
		return IsSubmenuRecentlyUsed(pMenu->GetSubMenu(nItemIndex)->GetSafeHmenu());
	}
	else
	{
		return IsRecentlyUsed(nCmdID);
	}
}


BOOL COXBitmapMenuOrganizer::IsSubmenuRecentlyUsed(HMENU hPopupMenu) const 
{ 
	int nDummy=-1;
	return (!m_mapHiddenSubmenus.Lookup(hPopupMenu,nDummy)); 
}



void COXBitmapMenuOrganizer::SetCutomizationMode(BOOL bInCutomizationMode,
												 HWND hWndCustomizeOrganizer/*=NULL*/) 
{ 
	if(m_bInCutomizationMode!=bInCutomizationMode)
	{
		m_bInCutomizationMode=bInCutomizationMode;
		if(m_bInCutomizationMode)
		{
			if(IsShowOnlyRecentlyUsedItems() && !m_bForceToDisplayHiddenItems)
			{
				POSITION pos=m_BitmapMenuList.GetHeadPosition();
				while(pos!=NULL)
				{
					COXBitmapMenu* pBitmapMenu=m_BitmapMenuList.GetNext(pos);
					RestoreBitmapMenu(pBitmapMenu,TRUE);
//					delete pBitmapMenu;
				}
			}
		}
	}
	m_hWndCustomizeOrganizer=hWndCustomizeOrganizer;
}


BOOL COXBitmapMenuOrganizer::SaveRUCommandsState(LPCTSTR lpszSubKey)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
	{
		sProfileName=_T("RecentlyUsedMenuItems");
	}
	CString sValueName=_T("RUContents");
	
	CMemFile memFile;
	CArchive ar(&memFile,CArchive::store);
	VERIFY(SaveRUCommandsContents(ar));
	ar.Close();
	int nBufferLength=(int)memFile.GetLength();
	ASSERT(nBufferLength>0);
	BYTE* pBuffer=memFile.Detach();

	pApp->WriteProfileBinary(sProfileName,sValueName,pBuffer,nBufferLength);
	::free((void*)pBuffer);

	sValueName=_T("RUItemsDataSize");
	pApp->WriteProfileInt(sProfileName,sValueName,nBufferLength);

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXBitmapMenuOrganizer::LoadRUCommandsState(LPCTSTR lpszSubKey)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
	{
		sProfileName=_T("RecentlyUsedMenuItems");
	}
	CString sValueName=_T("RUItemsDataSize");
	int nSavedBufferLength=
		pApp->GetProfileInt(sProfileName,sValueName,-1);
	sValueName=_T("RUContents");
	if(nSavedBufferLength!=-1)
	{
		ASSERT(nSavedBufferLength>0);
		UINT nBufferLength=0;
		BYTE* pBuffer=NULL;
		if(pApp->GetProfileBinary(sProfileName,sValueName,&pBuffer,&nBufferLength))
		{
			ASSERT(nBufferLength>0);
			ASSERT(pBuffer!=NULL);
			CMemFile memFile(pBuffer,nBufferLength);
			CArchive ar(&memFile,CArchive::load);
			VERIFY(LoadRUCommandsContents(ar));
			ar.Close();
			memFile.Detach();
			delete[] pBuffer;
		}
	}
	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXBitmapMenuOrganizer::SaveRUCommandsContents(CArchive& ar)
{
	int nCommandsCount=PtrToInt(m_mapHiddenCommands.GetCount());
	ar<<nCommandsCount;
	POSITION pos=m_mapHiddenCommands.GetStartPosition();
	while(pos!=NULL)
	{
		int nCmdID=-1;
		int nIndex=-1;
		m_mapHiddenCommands.GetNextAssoc(pos,nCmdID,nIndex);
		ASSERT(nCmdID>0);
		ar<<nCmdID;
	}

	return TRUE;
}

BOOL COXBitmapMenuOrganizer::LoadRUCommandsContents(CArchive& ar)
{
	// remove all items from the list of hidden commands
	m_mapHiddenCommands.RemoveAll();

	DWORD dwCommandsCount=0;
	ar>>dwCommandsCount;
	for(int nIndex=0; nIndex<(int)dwCommandsCount; nIndex++)
	{
		DWORD dwCmdID=0;
		ar>>dwCmdID;
		ASSERT(dwCmdID>0);
		ExcludeFromRecentlyUsed((int)dwCmdID);
	}	

	return TRUE;
}


BOOL COXBitmapMenuOrganizer::SaveRUSubmenusState(HMENU hMenu, LPCTSTR lpszSubKey)
{
#ifndef _MAC
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
	{
		sProfileName=_T("RecentlyUsedSubmenuItems");
	}
	CString sValueName=_T("RUContents");
	
	CMemFile memFile;
	CArchive ar(&memFile,CArchive::store);
	VERIFY(SaveRUSubmenusContents(hMenu,ar));
	ar.Close();
	int nBufferLength=(int)memFile.GetLength();
	ASSERT(nBufferLength>0);
	BYTE* pBuffer=memFile.Detach();

	pApp->WriteProfileBinary(sProfileName,sValueName,pBuffer,nBufferLength);
	::free((void*)pBuffer);

	sValueName=_T("RUItemsDataSize");
	pApp->WriteProfileInt(sProfileName,sValueName,nBufferLength);

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXBitmapMenuOrganizer::LoadRUSubmenusState(HMENU hMenu, LPCTSTR lpszSubKey)
{
#ifndef _MAC
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
	{
		sProfileName=_T("RecentlyUsedSubmenuItems");
	}
	CString sValueName=_T("RUItemsDataSize");
	int nSavedBufferLength=
		pApp->GetProfileInt(sProfileName,sValueName,-1);
	sValueName=_T("RUContents");
	if(nSavedBufferLength!=-1)
	{
		ASSERT(nSavedBufferLength>0);
		UINT nBufferLength=0;
		BYTE* pBuffer=NULL;
		if(pApp->GetProfileBinary(sProfileName,sValueName,&pBuffer,&nBufferLength))
		{
			ASSERT(nBufferLength>0);
			ASSERT(pBuffer!=NULL);
			CMemFile memFile(pBuffer,nBufferLength);
			CArchive ar(&memFile,CArchive::load);
			VERIFY(LoadRUSubmenusContents(hMenu,ar));
			ar.Close();
			memFile.Detach();
			delete[] pBuffer;
		}
	}
	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXBitmapMenuOrganizer::SaveRUSubmenusContents(HMENU hMenu, CArchive& ar)
{
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	int nSubmenusCount=PtrToInt(m_mapHiddenSubmenus.GetCount());
	ar<<nSubmenusCount;
	POSITION pos=m_mapHiddenSubmenus.GetStartPosition();
	while(pos!=NULL)
	{
		HMENU hPopupMenu=NULL;
		int nIndex=-1;
		m_mapHiddenSubmenus.GetNextAssoc(pos,hPopupMenu,nIndex);
		ASSERT(hPopupMenu!=NULL);
		if(::IsMenu(hPopupMenu))
		{
			CArray<int,int> arrPath;
			if(FindPopupMenuPath(hPopupMenu,hMenu,arrPath))
			{
				for(int nIndex=0; nIndex<arrPath.GetSize(); nIndex++)
				{
					ar<<arrPath[nIndex];
				}
			}
		}
		// end of path
		ar<<-1;
	}

	return TRUE;
}

BOOL COXBitmapMenuOrganizer::LoadRUSubmenusContents(HMENU hMenu, CArchive& ar)
{
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	// remove all items from the list of hidden submenus
	m_mapHiddenSubmenus.RemoveAll();

	DWORD dwSubmenusCount=0;
	ar>>dwSubmenusCount;
	for(int nIndex=0; nIndex<(int)dwSubmenusCount; nIndex++)
	{
		DWORD dwItemIndex;
		CArray<int,int> arrPath;
		do
		{
			ar>>dwItemIndex;
			if(dwItemIndex!=-1)
			{
				arrPath.Add(dwItemIndex);
			}
		} while(dwItemIndex!=-1);

		HMENU hPopupMenu=NULL;
		if(arrPath.GetSize()>0)
		{
			HMENU hParentMenu=hMenu;
			for(int nPathIndex=0; nPathIndex<arrPath.GetSize(); nPathIndex++)
			{
				hParentMenu=GetSubMenu(hParentMenu,arrPath[nPathIndex]);
				if(hParentMenu==NULL)
				{
					TRACE(_T("COXBitmapMenuOrganizer::LoadRUSubmenusContents: failed to load RU submenu\n"));
					break;
				}
			}
			hPopupMenu=hParentMenu;
		}

		if(hPopupMenu!=NULL)
		{
			ExcludeSubmenuFromRecentlyUsed(hPopupMenu);
		}
	}	

	return TRUE;
}


COXBitmapMenuOrganizer* COXBitmapMenuOrganizer::FindOrganizer(HWND hWnd)
{
	COXBitmapMenuOrganizer* pOrganizer=NULL;
	if(!m_allMenuOrganizers.Lookup(hWnd,pOrganizer))
	{
		return NULL;
	}
	else
	{
		return pOrganizer;
	}
}


BOOL COXBitmapMenuOrganizer::IsTopLevelPopupMenu(HMENU hPopupMenu)
{
	ASSERT(hPopupMenu!=NULL);
	ASSERT(::IsMenu(hPopupMenu));
	ASSERT(IsAttached());

	CMenu* pMenu=m_pFrameWnd->GetMenu();
	if(pMenu==NULL)
		return FALSE;

	BOOL bFound=FALSE;
	for(int nIndex=0; nIndex<(int)pMenu->GetMenuItemCount(); nIndex++)
	{
		CMenu* pTopLevelPopupMenu=pMenu->GetSubMenu(nIndex);
		if(pTopLevelPopupMenu!=NULL && 
			pTopLevelPopupMenu->GetSafeHmenu()==hPopupMenu)
		{
			bFound=TRUE;
			break;
		}
	}

	return bFound;
}


HMENU COXBitmapMenuOrganizer::CopyPopupMenu(HMENU hMenuDest)
{
	HMENU hMenu=::CreatePopupMenu();
	ASSERT(hMenu!=NULL);
	m_arrCreatedPopupMenus.Add(hMenu);
	if(!IsSubmenuRecentlyUsed(hMenuDest))
	{
		AddSubmenuToRecentlyUsed(hMenuDest);
		ExcludeSubmenuFromRecentlyUsed(hMenu);
	}

	MENUITEMINFO miInfo;
	CString sText;

	for(int nIndex=0; nIndex<::GetMenuItemCount(hMenuDest); nIndex++)
	{
		// Even cascade menu item should be made owner drawn
		//  so that the text of these items is align properly

		::ZeroMemory(&miInfo,sizeof(miInfo));
		miInfo.cbSize=sizeof(miInfo);
		miInfo.fMask=MIIM_TYPE|MIIM_ID|MIIM_STATE|MIIM_DATA|
			MIIM_SUBMENU|MIIM_CHECKMARKS;
		miInfo.fType=MFT_STRING;
		miInfo.cch=300;
		miInfo.dwTypeData=sText.GetBuffer(miInfo.cch);
		// Gets the item info
		miInfo.dwTypeData[0]=_T('\0');
		::GetMenuItemInfo(hMenuDest,nIndex,TRUE,&miInfo);
		if(miInfo.hSubMenu!=NULL)
		{
			miInfo.hSubMenu=CopyPopupMenu(miInfo.hSubMenu);
		}
		VERIFY(::InsertMenuItem(hMenu,nIndex,TRUE,&miInfo));
		sText.ReleaseBuffer();
	}

	return hMenu;
}


BOOL COXBitmapMenuOrganizer::FindPopupMenuPath(HMENU hPopupMenu, 
											   HMENU hParentMenu, 
											   CArray<int,int>& arrPath)
{
	ASSERT(hPopupMenu!=NULL);
	ASSERT(::IsMenu(hPopupMenu));
	ASSERT(hParentMenu!=NULL);
	ASSERT(::IsMenu(hParentMenu));

	int nItemCount=::GetMenuItemCount(hParentMenu);
	for(int nIndex=0; nIndex<nItemCount; nIndex++)
	{
		HMENU hSubmenu=::GetSubMenu(hParentMenu,nIndex);
		if(hSubmenu==hPopupMenu || 
			(hSubmenu!=NULL && FindPopupMenuPath(hPopupMenu,hSubmenu,arrPath)))
		{
			arrPath.InsertAt(0,nIndex);
			return TRUE;
		}
	}

	return FALSE;
}


LRESULT CALLBACK COXBitmapMenuOrganizer::
MenuMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(g_pfnOldMouseHookProc!=NULL);

	if(nCode<0)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldMouseHookProc,nCode,wParam,lParam);
	}

	if(wParam!=WM_LBUTTONDOWN && wParam!=WM_RBUTTONDOWN && 
		wParam!=WM_LBUTTONUP && wParam!=WM_RBUTTONUP && 
		wParam!=WM_LBUTTONDBLCLK && wParam!=WM_RBUTTONDBLCLK)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldMouseHookProc,nCode,wParam,lParam);
	}

	COXBitmapMenuOrganizer* pBMOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pBMOrganizer!=NULL);
	ASSERT(pBMOrganizer->IsShowOnlyRecentlyUsedItems());

	DWORD dwMessagePos=::GetMessagePos();
	POINTS points=MAKEPOINTS(dwMessagePos);
	CPoint ptLast(points.x,points.y);
	CWnd* pWnd=CWnd::WindowFromPoint(ptLast);
	ASSERT(pWnd!=NULL);
	// find if pWnd is on of the windows that host popup menus
	POSITION pos=pBMOrganizer->m_mapPopupMenuWindows.GetStartPosition();
	while(pos!=NULL)
	{
		HMENU hMenu=NULL;
		HWND hWnd=NULL;
		pBMOrganizer->m_mapPopupMenuWindows.GetNextAssoc(pos,hMenu,hWnd);
		ASSERT(hMenu!=NULL);
		ASSERT(hWnd!=NULL);
		if(pWnd->GetSafeHwnd()==hWnd)
		{
			ASSERT(::IsMenu(hMenu));
			// find the menu in the trace array
			BOOL bFound=FALSE;
			int nIndex=0;
			for(nIndex=0; 
				nIndex<pBMOrganizer->m_arrPopupMenusTrace.GetSize(); nIndex++)
			{
				if(pBMOrganizer->m_arrPopupMenusTrace[nIndex].m_hPopupMenu==hMenu)
				{
					bFound=TRUE;
					break;
				}
			}
			ASSERT(bFound);
			if(pBMOrganizer->m_nActivePopupMenuCount>nIndex+1)
			{
				BOOL bFoundPopupMenuToClose=FALSE;
				for(int nIndexPopupWnd=nIndex+1; 
					nIndexPopupWnd<pBMOrganizer->m_arrPopupMenusTrace.GetSize(); 
					nIndexPopupWnd++)
				{
					// try to find and close the remaining popup menus
					HWND hWnd=NULL;
					if(pBMOrganizer->m_mapPopupMenuWindows.Lookup(pBMOrganizer->
						m_arrPopupMenusTrace[nIndexPopupWnd].m_hPopupMenu,hWnd))
					{
						if(::IsWindow(hWnd))
						{
							::SendMessage(hWnd,WM_KEYDOWN,VK_ESCAPE,0x00010001);
						}
						bFoundPopupMenuToClose=TRUE;
					}
				}
				if(bFoundPopupMenuToClose)
				{
					pBMOrganizer->m_nActivePopupMenuCount=nIndex+1;
				}
			}

			// check if "Show All items" menu item has been clicked
			int nMenuItemIndex=::MenuItemFromPoint(hWnd,hMenu,ptLast);
			if(nMenuItemIndex!=-1)
			{
				int nCmdID=::GetMenuItemID(hMenu,nMenuItemIndex);
				switch(nCmdID)
				{
				case ID_OX_SHOWALLITEMS:
					{
						if(wParam!=WM_LBUTTONDOWN && wParam!=WM_RBUTTONDOWN)
						{
							pBMOrganizer->m_pFrameWnd->PostMessage(WM_COMMAND,
								MAKEWPARAM(ID_OX_SHOWALLITEMS,0));
						}
						return 1;
					}
				}
			}
		}
	}

    return ::CallNextHookEx(g_pfnOldMouseHookProc,nCode,wParam,lParam);
}


LRESULT CALLBACK COXBitmapMenuOrganizer::
MenuKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif
	ASSERT(g_pfnOldKeyboardHookProc!=NULL);

	if(nCode<0)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldKeyboardHookProc,nCode,wParam,lParam);
	}

	if(wParam!=VK_RETURN)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldKeyboardHookProc,nCode,wParam,lParam);
	}
	
	COXBitmapMenuOrganizer* pBMOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pBMOrganizer!=NULL);
	ASSERT(pBMOrganizer->IsShowOnlyRecentlyUsedItems());

	CWnd* pWnd=CWnd::GetDesktopWindow()->GetTopWindow();
	ASSERT(pWnd!=NULL);
	// find if pWnd is on of the windows that host popup menus
	POSITION pos=pBMOrganizer->m_mapPopupMenuWindows.GetStartPosition();
	while(pos!=NULL)
	{
		HMENU hMenu=NULL;
		HWND hWnd=NULL;
		pBMOrganizer->m_mapPopupMenuWindows.GetNextAssoc(pos,hMenu,hWnd);
		ASSERT(hMenu!=NULL);
		ASSERT(hWnd!=NULL);
		if(pWnd->GetSafeHwnd()==hWnd)
		{
			MENUITEMINFO miInfo={ sizeof(MENUITEMINFO) };
			miInfo.fMask=MIIM_ID|MIIM_STATE;
			VERIFY(::GetMenuItemInfo(hMenu,::GetMenuItemCount(hMenu)-1,TRUE,&miInfo));
			if(miInfo.wID==ID_OX_SHOWALLITEMS && (miInfo.fState&MFS_HILITE))
			{
				if((lParam&0xe0000000)==0)
				{
					pBMOrganizer->m_pFrameWnd->PostMessage(WM_COMMAND,
						MAKEWPARAM(ID_OX_SHOWALLITEMS,0));
				}
				
				return 1;
			}
		}
	}

    return ::CallNextHookEx(g_pfnOldKeyboardHookProc,nCode,wParam,lParam);
}



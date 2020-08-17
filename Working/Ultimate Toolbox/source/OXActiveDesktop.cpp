// ==========================================================================
// 						Class Implementation : COXActiveDesktop
// ==========================================================================

// Source file OXActiveDesktop.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXActiveDesktop.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

LPWSTR GetWcharString(LPCTSTR pszSource)
{
	//This function converts LPCTSTR string
	//to LPWSTR string. 
	//NOTE: If the function returns a value other than
	//not NULL the caller is responsible for
	//freeing the memory by operator delete[]
	
	if (!pszSource)
		return NULL;

	LPWSTR lpszwSource=new WCHAR[lstrlen(pszSource)+1];
	
	//zero-length string
	if (!lstrlen(pszSource))
	{
		*lpszwSource=NULL;
		return lpszwSource;
	}

	//non zero-length string
#ifdef _UNICODE
	//just copy string
	UTBStr::tcscpy(lpszwSource, lstrlen(pszSource)+1, pszSource);
#else
	int nRet=::MultiByteToWideChar(CP_ACP,NULL,
		pszSource,-1,lpszwSource,lstrlen(pszSource)+1);
	if (!nRet)
	{
		TRACE2("GetWcharString() : error %d while converting string %s\n",
			GetLastError(),pszSource);
		delete []lpszwSource;
		return NULL;
	}
#endif

	return lpszwSource;
}

LPTSTR GetTcharString(LPWSTR pszwSource)
{

	//This function converts LPWSTR string
	//to LPTSTR string. 
	//NOTE: If the function returns a value other than
	//not NULL the caller is responsible for
	//freeing the memory by operator delete[]
	
	if (!pszwSource)
		return NULL;

	LPTSTR lpszSource=new TCHAR[lstrlenW(pszwSource)+1];

	//zero-length string
	if (!lstrlenW(pszwSource))
	{
		*lpszSource=NULL;
		return lpszSource;
	}
	

#ifdef _UNICODE
	UTBStr::tcscpy(lpszSource, lstrlenW(pszwSource)+1, pszwSource);
#else
	int nRet=::WideCharToMultiByte(CP_ACP,NULL,
		pszwSource,-1,lpszSource,lstrlenW(pszwSource)+1, NULL, NULL);
	if (!nRet)
	{
		TRACE1("GetWcharString() : error %d while converting a string\n",
			GetLastError());
		delete []lpszSource;
		return NULL;
	}
#endif
	return lpszSource;

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXActiveDesktop::COXActiveDesktop(HWND hWnd) :
m_pActiveDesktop(NULL) , m_nError(NULL), m_hWnd(hWnd)
{
}

COXActiveDesktop::COXActiveDesktop(const COXActiveDesktop& Src)
{
	m_pActiveDesktop=NULL;
	*this=Src;
}

const COXActiveDesktop& COXActiveDesktop::operator=(const COXActiveDesktop& Src)
{
	m_hWnd=Src.m_hWnd;
	if (m_pActiveDesktop)
		m_pActiveDesktop->Release();
	m_pActiveDesktop=Src.m_pActiveDesktop;
	m_pActiveDesktop->AddRef();
	m_nError=Src.m_nError;

	return *this;
}


COXActiveDesktop::~COXActiveDesktop()
{
	if (m_pActiveDesktop)
		m_pActiveDesktop->Release();

}

BOOL COXActiveDesktop::Init()
{
	//This function must be called first. If this
	//function failed all next calls to other functions 
	//will fail

	ASSERT(m_pActiveDesktop==NULL);
	
	m_nError=NULL;

	HRESULT hRslt=::CoCreateInstance(CLSID_ActiveDesktop,
		NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&m_pActiveDesktop);

	OX_ACTDESK_CHECK_ERROR(FALSE);
	
	return TRUE;
}



BOOL COXActiveDesktop::AddItem(LPCOMPONENT pComp, LPCTSTR pszSource, 
	BOOL bDisplayDialog, BOOL bApply)
{

	//This function inserts the item in ActiveDesktop
	
	ASSERT(m_pActiveDesktop);
	ASSERT(pComp);

	m_nError=NULL;

	if (pszSource)//IActiveDesktop::AddUrl
	{
		
		ASSERT(lstrlen(pszSource));
		ASSERT(m_hWnd);

		DWORD dwFlags=bDisplayDialog?NULL:ADDURL_SILENT;

		LPWSTR lpszSource=GetWcharString(pszSource);
		if (!lpszSource)
			return FALSE;

		HRESULT hRslt=m_pActiveDesktop->AddUrl(m_hWnd,
			lpszSource, pComp, dwFlags);
		delete []lpszSource;

		OX_ACTDESK_CHECK_ERROR(FALSE)
		
		if (bApply)
			return ApplyChanges();

		return TRUE;
	}
	else
		if (bDisplayDialog)//IActiveDesktop::AddDesktopItemWithUI
		{
			ASSERT(m_hWnd);
			HRESULT hRslt=m_pActiveDesktop->AddDesktopItemWithUI(m_hWnd,
				pComp, NULL);

		OX_ACTDESK_CHECK_ERROR(FALSE)

		if (bApply)
			return ApplyChanges();

		return TRUE;
		}
		else//IActiveDesktop::AddDesktopItem
		{
			HRESULT hRslt=m_pActiveDesktop->AddDesktopItem(pComp,NULL);
			OX_ACTDESK_CHECK_ERROR(FALSE)
		
			if (bApply)
				return ApplyChanges();

			return TRUE;
		}

}

BOOL COXActiveDesktop::CreateHTML(LPCTSTR pszFileName, 
								  LPCOMPONENT pComp)
{
	//This function generates HTML file from the supplied
	//component

	ASSERT(m_pActiveDesktop);
	ASSERT(pComp);
	ASSERT(pszFileName);
	ASSERT(lstrlen(pszFileName));
	m_nError=NULL;

	LPWSTR lpszSource=GetWcharString(pszFileName);
	if (!lpszSource)
		return FALSE;

	HRESULT hRslt=m_pActiveDesktop->GenerateDesktopItemHtml(lpszSource,
		pComp,NULL);
	delete []lpszSource;

	OX_ACTDESK_CHECK_ERROR(FALSE)
	
	return TRUE;
}

LPCOMPONENT COXActiveDesktop::GetComponent(LPCTSTR pszSource)
{
	//This function returns component by the source
	//if succeed. Otherwise NULL will be returned.
	//NOTE: If this function returns non-NULL the
	//caller is responsible for freeing memory by operator delete.

	ASSERT(m_pActiveDesktop);
	ASSERT(pszSource);
	ASSERT(lstrlen(pszSource));
	m_nError=NULL;

	LPWSTR lpszSource=GetWcharString(pszSource);
	if (!lpszSource)
		return NULL;

	LPCOMPONENT pComp=new COMPONENT;
	::ZeroMemory(pComp,sizeof(COMPONENT));
	pComp->dwSize=sizeof(COMPONENT);

	HRESULT hRslt=m_pActiveDesktop->GetDesktopItemBySource(lpszSource,
		pComp,NULL);

	if (FAILED(hRslt))
		delete pComp; 
	
	OX_ACTDESK_CHECK_ERROR(NULL)
	
	return pComp;
}

LPCOMPONENT COXActiveDesktop::GetComponent(DWORD dwID)
{
	//This function returns component by the ID
	//if succeed. Otherwise NULL will be returned.
	//NOTE: If this function returns non-NULL the
	//caller is responsible for freeing memory by operator delete.

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	LPCOMPONENT pComp=new COMPONENT;
	::ZeroMemory(pComp,sizeof(COMPONENT));
	pComp->dwSize=sizeof(COMPONENT);

	HRESULT hRslt=m_pActiveDesktop->GetDesktopItemByID(dwID, pComp, NULL);
	
	if (FAILED(hRslt))
		delete pComp; 

	OX_ACTDESK_CHECK_ERROR(NULL)
	
	return pComp;

}

LPCOMPONENT COXActiveDesktop::GetComponent(int nComponent)
{
	//This function returns component by number
	//if succeed. Otherwise NULL will be returned.
	//NOTE: If this function returns non-NULL the
	//caller is responsible for freeing memory by operator delete.

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	LPCOMPONENT pComp=new COMPONENT;
	::ZeroMemory(pComp,sizeof(COMPONENT));
	pComp->dwSize=sizeof(COMPONENT);

	HRESULT hRslt=m_pActiveDesktop->GetDesktopItem(nComponent,
		pComp,NULL);

	if (FAILED(hRslt))
		delete pComp; 

	OX_ACTDESK_CHECK_ERROR(NULL)
	
	return pComp;

}

int COXActiveDesktop::GetItemCount()
{
	//This function returns number of items in 
	//the Active Desktop
	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	int nRet=0;
	HRESULT hRslt=m_pActiveDesktop->GetDesktopItemCount(&nRet,NULL);
	
	OX_ACTDESK_CHECK_ERROR(-1)

	return nRet;
}

BOOL COXActiveDesktop::IsDesktopEnabled(BOOL* pEnable)
{
	//This function returns TRUE if succeed,
	//FALSE otherwise. If the function succeeed,
	//on out pEnable will have enable state of
	//the Active Desktop

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;
	*pEnable=FALSE;

	COMPONENTSOPT Opt;
	Opt.dwSize=sizeof(Opt);
	HRESULT hRslt=m_pActiveDesktop->GetDesktopItemOptions(&Opt,
		NULL);

	OX_ACTDESK_CHECK_ERROR(FALSE)
	
	*pEnable=Opt.fActiveDesktop;
	return TRUE;
}

BOOL COXActiveDesktop::IsComponentsEnabled(BOOL* pEnable)
{
	//This function returns TRUE if succeed,
	//FALSE otherwise. If the function succeeed,
	//on out pEnable will have enable state of
	//the Active Desktop components

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	COMPONENTSOPT Opt;
	Opt.dwSize=sizeof(Opt);
	HRESULT hRslt=m_pActiveDesktop->GetDesktopItemOptions(&Opt,
		NULL);
	OX_ACTDESK_CHECK_ERROR(FALSE)
	*pEnable=Opt.fEnableComponents;
	return TRUE;

}

BOOL COXActiveDesktop::GetPattern(CString& sPattern)
{
	//This function on out in sPattern will have 
	//decimal representation of the Active Desktop
	//pattern. If no pattern has been specified, 
	//string "None" will be returned. 
	//If function failed it returnes FALSE, if 
	//succeed - TRUE

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;
	sPattern=_T("");

	WCHAR* szPattern=new WCHAR[_MAX_PATH];
	::ZeroMemory(szPattern,_MAX_PATH*sizeof(TCHAR));

	HRESULT hRslt=m_pActiveDesktop->GetPattern(szPattern,_MAX_PATH,NULL);
	if (FAILED(hRslt))
	{
		delete []szPattern;
		m_nError=HRESULT_CODE(hRslt);
		return FALSE;
	}
	
	ASSERT(lstrlenW(szPattern)<_MAX_PATH);

	LPTSTR lpszPattern=GetTcharString(szPattern);
	if (lpszPattern)
	{
		sPattern=lpszPattern;
	}
	delete []szPattern;
	if (lpszPattern)
		delete []lpszPattern;

	return TRUE;
}

BOOL COXActiveDesktop::GetWallpaper(CString& sWallpaper)
{
	//The function returns TRUE if succeed,
	//FALSE otherwise. If the function succeed
	//sWallpaper will have filename of the wallpaper

	
	ASSERT(m_pActiveDesktop);
	m_nError=NULL;
	sWallpaper=_T("");

	WCHAR* szWallpaper=new WCHAR[_MAX_PATH];
	::ZeroMemory(szWallpaper,_MAX_PATH*sizeof(TCHAR));

	HRESULT hRslt=m_pActiveDesktop->GetWallpaper(szWallpaper, _MAX_PATH,
		NULL);
	if (FAILED(hRslt))
	{
		delete []szWallpaper;
		m_nError=HRESULT_CODE(hRslt);
		return FALSE;
	}
	LPTSTR lpszWallpaper=GetTcharString(szWallpaper);
	if (lpszWallpaper)
	{
		sWallpaper=lpszWallpaper;
		delete []lpszWallpaper;
	}
	delete []szWallpaper;

	return TRUE;
}

int COXActiveDesktop::GetWallpaperStyle()
{
	//This function returns styles of the wallpaper
	//that maybe any of WPSTYLE_CENTER, WPSTYLE_TILE,
	//WPSTYLE_STRETCH, WPSTYLE_MAX on success or -1 if failed
    
	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	WALLPAPEROPT Opt;
	::ZeroMemory(&Opt, sizeof(Opt));
	Opt.dwSize=sizeof(Opt);

	HRESULT hRslt=m_pActiveDesktop->GetWallpaperOptions(&Opt,NULL);
	
	OX_ACTDESK_CHECK_ERROR(-1)
	
	return Opt.dwStyle; 
}

BOOL COXActiveDesktop::ModifyItem(LPCOMPONENT pComp, DWORD dwFlags, BOOL bApply)
{
	//This function modifies supplied item
	//and returns TRUE on succeed or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	ASSERT(pComp);
	m_nError=NULL;


	HRESULT hRslt=m_pActiveDesktop->ModifyDesktopItem(pComp,dwFlags);

	OX_ACTDESK_CHECK_ERROR(FALSE)
	
	if (bApply)
		return ApplyChanges();
	else
		return TRUE;
}

BOOL COXActiveDesktop::RemoveItem(LPCOMPONENT pComp, BOOL bApply)
{
	//This function removes supplied item
	//and returns TRUE on succes or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	ASSERT(pComp);
	m_nError=NULL;

	HRESULT hRslt=m_pActiveDesktop->RemoveDesktopItem(pComp,NULL);
	OX_ACTDESK_CHECK_ERROR(FALSE)

	if (bApply)
		return ApplyChanges();
	else
		return TRUE;
}

BOOL COXActiveDesktop::SetComponentsEnable(BOOL bEnable)
{
	//This function enables/disables components
	//on the ActiveDesktop and returns TRUE if succeed
	//or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	COMPONENTSOPT Opt;
	::ZeroMemory(&Opt, sizeof(Opt));
	Opt.dwSize=sizeof(Opt);

	Opt.fEnableComponents=bEnable;
	if (!IsDesktopEnabled(&Opt.fActiveDesktop))
		return FALSE;
	HRESULT hRslt=m_pActiveDesktop->SetDesktopItemOptions(&Opt,NULL);

	OX_ACTDESK_CHECK_ERROR(FALSE)
	return TRUE;
}

BOOL COXActiveDesktop::SetDesktopEnable(BOOL bEnable)
{
	//This function enables/disables ActiveDesktop 
	//and returns TRUE if succeed or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	COMPONENTSOPT Opt;
	::ZeroMemory(&Opt, sizeof(Opt));
	Opt.dwSize=sizeof(Opt);

	Opt.fActiveDesktop=bEnable;
	if (!IsComponentsEnabled(&Opt.fEnableComponents))
		return FALSE;
	HRESULT hRslt=m_pActiveDesktop->SetDesktopItemOptions(&Opt,NULL);

	OX_ACTDESK_CHECK_ERROR(FALSE)
	return TRUE;
}

BOOL COXActiveDesktop::SetPattern(CString sPattern)
{
	//This function sets new pattern
	//on the Active Desktop. Returns TRUE
	//if succeed or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	LPWSTR lpszwPattern=NULL;
	if (!sPattern.IsEmpty())
	{
		lpszwPattern=GetWcharString((LPCTSTR) sPattern);
	}

	HRESULT hRslt=m_pActiveDesktop->SetPattern(lpszwPattern, NULL);
	if (lpszwPattern)
		delete []lpszwPattern;

	OX_ACTDESK_CHECK_ERROR(FALSE)
	return TRUE;
}

BOOL COXActiveDesktop::SetWallpaper(CString sWallpaper)
{
	//This function sets new pattern
	//on the Active Desktop. Returns TRUE
	//if succeed or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	LPWSTR lpszwWallpaper=NULL;
	if (!sWallpaper.IsEmpty())
	{
		lpszwWallpaper=GetWcharString((LPCTSTR) sWallpaper);
	}

	HRESULT hRslt=m_pActiveDesktop->SetWallpaper(lpszwWallpaper, NULL);
	if (lpszwWallpaper)
		delete []lpszwWallpaper;

	OX_ACTDESK_CHECK_ERROR(FALSE)
	return TRUE;
}

BOOL COXActiveDesktop::SetWallpaperStyle(DWORD dwStyle)
{
	//This function sets styles of the wallpaper
	//that maybe any of WPSTYLE_CENTER, WPSTYLE_TILE,
	//WPSTYLE_STRETCH, WPSTYLE_MAX.
	//Returns TRUE on success or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;

	WALLPAPEROPT Opt;
	::ZeroMemory(&Opt, sizeof(Opt));
	Opt.dwSize=sizeof(Opt);
	Opt.dwStyle=dwStyle;
	HRESULT hRslt=m_pActiveDesktop->SetWallpaperOptions(&Opt,NULL);

	OX_ACTDESK_CHECK_ERROR(FALSE)
	return TRUE;
}


BOOL COXActiveDesktop::ApplyChanges(DWORD dwFlags)
{
	//This function applies changes
	//to the Active Desktop. The dwFlags
	//variable may contain any of AD_APPLY_SAVE,
    //AD_APPLY_HTMLGEN, AD_APPLY_REFRESH,
	//AD_APPLY_ALL or AD_APPLY_FORCE. The function returns TRUE
	//on success or FALSE otherwise

	ASSERT(m_pActiveDesktop);
	m_nError=NULL;
	    
	HRESULT hRslt=m_pActiveDesktop->ApplyChanges(dwFlags);
	
	OX_ACTDESK_CHECK_ERROR(FALSE);

	return TRUE;

}


BOOL COXActiveDesktop::GetPattern(BYTE* pBuffer)
{
	//This function returns BYTE representation of the 
	//pattern. Buffer has to be allocated prior to 
	//call to this function and his size must be at least
	//8 bytes.

	ASSERT(pBuffer);
	CString sPattern;
	if (!GetPattern(sPattern))
		return FALSE;
	
	::ZeroMemory(pBuffer,8);

	if (sPattern.Find(_T("None"))!=-1)
		return TRUE;

	for (int i=0; i<8; i++)
	{
		int nFind=sPattern.Find(_T(" "));
		int n=_ttoi((LPCTSTR) sPattern);
		*(pBuffer+i)=(BYTE) n;
		if (nFind!=-1)
			sPattern=sPattern.Right(sPattern.GetLength()-nFind);
		sPattern.TrimLeft();
	}
	return TRUE;
}

BOOL COXActiveDesktop::SetPattern(BYTE *pBuffer)
{
	//This function sets the pattern for 
	//the Active Desktop. Pattern represented by
	//pBuffer and must be of 8 bytes.

	ASSERT(pBuffer);

	CString sPattern;
	TCHAR szBuffer[8];

	for (int i=0; i<8; i++)
	{
		UTBStr::itot(*(pBuffer+i), (LPTSTR) &szBuffer, 8, 10);
		sPattern+=(LPCTSTR) &szBuffer;
		sPattern+=_T(" ");
	}
	sPattern.TrimRight();

	return SetPattern(sPattern);
}

BOOL COXActiveDesktop::Refresh()
{
	ASSERT(m_pActiveDesktop);
	m_pActiveDesktop->Release();
	m_pActiveDesktop=NULL;
	return Init();
}

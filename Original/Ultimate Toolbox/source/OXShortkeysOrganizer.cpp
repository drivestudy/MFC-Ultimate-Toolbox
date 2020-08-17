// ==========================================================================
//			Class Implementation: COXShortkeysOrganizer
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
#include "OXShortkeysOrganizer.h"
#include "OXRegistryValFile.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXShortkeysOrganizer::COXShortkeysOrganizer() :
	m_pFrameWnd(NULL)
{
}

COXShortkeysOrganizer::~COXShortkeysOrganizer()
{
	m_arrAccelTables.RemoveAll();
	m_arrRemovedAccelTables.RemoveAll();
	for(int nIndex=0; nIndex<m_arrCreatedAccel.GetSize(); nIndex++)
	{
		VERIFY(::DestroyAcceleratorTable(m_arrCreatedAccel[nIndex]));
	}
	m_arrCreatedAccel.RemoveAll();
}

//////////////////////////////////////////////////////////////////////

BOOL COXShortkeysOrganizer::Attach(CFrameWnd* pFrameWnd, 
								   BOOL bRetrieveAccelerators/*=FALSE*/)
{
	ASSERT(pFrameWnd!=NULL);

	if(IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::Attach: already has attached frame window. Call Detach to detach the existing ine before\n"));
		return FALSE;
	}

	ASSERT(m_arrAccelTables.GetSize()==0);
	ASSERT(m_arrRemovedAccelTables.GetSize()==0);

	m_pFrameWnd=pFrameWnd;

	if(bRetrieveAccelerators)
		return RetrieveAccelerators();
	else
		return TRUE;
}


BOOL COXShortkeysOrganizer::Detach()
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::Detach: there is no attached frame window. Makes no sense to call this function\n"));
		return TRUE;
	}

	m_pFrameWnd=NULL;
	m_arrAccelTables.RemoveAll();
	m_arrRemovedAccelTables.RemoveAll();

	return TRUE;
}


BOOL COXShortkeysOrganizer::AddAccelerator(BYTE fVirt, WORD key, WORD cmd, 
										   CMultiDocTemplate* pDocTemplate,
										   BOOL bIgnoreConflict/*=FALSE*/)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::AddAccelerator: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return FALSE;
	}

	WORD cmdExisting=FindAccelerator(fVirt,key,pDocTemplate);
	if(cmdExisting!=0)
	{
		if(!bIgnoreConflict)
		{
			TRACE(_T("COXShortkeysOrganizer::AddAccelerator: the shortkey already exists\n"));
			return FALSE;
		}
		else
		{
			VERIFY(RemoveAccelerator(fVirt,key,cmdExisting,pDocTemplate));
		}
	}

	ACCEL accel;
	accel.fVirt=fVirt;
	accel.key=key;
	accel.cmd=cmd;
	pArrAccels->Add(accel);

	return TRUE;
}

BOOL COXShortkeysOrganizer::AddAccelerator(ACCEL* pAccel, 
										   CMultiDocTemplate* pDocTemplate,
										   BOOL bIgnoreConflict/*=FALSE*/)
{
	ASSERT(pAccel!=NULL);
	return AddAccelerator(pAccel->fVirt,pAccel->key,pAccel->cmd,
		pDocTemplate,bIgnoreConflict);
}

BOOL COXShortkeysOrganizer::RemoveAccelerator(BYTE fVirt, WORD key, WORD cmd,
											  CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::RemoveAccelerator: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return FALSE;
	}

	COXArrAccelerators* pArrRemovedAccels=FindRemovedAcceleratorTable(pDocTemplate);
	if(pArrRemovedAccels==NULL)
	{
		OXACCELTABLE accelTable;
		accelTable.m_docTemplate=pDocTemplate;
		m_arrRemovedAccelTables.Add(accelTable);
		pArrRemovedAccels=FindRemovedAcceleratorTable(pDocTemplate);
	}
	ASSERT(pArrRemovedAccels!=NULL);

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if((accel.fVirt&~FNOINVERT)==(fVirt&~FNOINVERT) && 
			accel.key==key && accel.cmd==cmd)
		{
			pArrAccels->RemoveAt(nAccelIndex);
			nAccelIndex--;
			// save the removed accelerator 
			pArrRemovedAccels->Add(accel);
			break;
		}
	}

	return TRUE;
}

BOOL COXShortkeysOrganizer::RemoveAccelerator(ACCEL* pAccel,
											  CMultiDocTemplate* pDocTemplate)
{
	ASSERT(pAccel!=NULL);
	return RemoveAccelerator(pAccel->fVirt,pAccel->key,pAccel->cmd,pDocTemplate);
}

BOOL COXShortkeysOrganizer::RemoveAllAccelerators(WORD cmd, 
												  CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::RemoveAllAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return FALSE;
	}

	COXArrAccelerators* pArrRemovedAccels=FindRemovedAcceleratorTable(pDocTemplate);
	if(pArrRemovedAccels==NULL)
	{
		OXACCELTABLE accelTable;
		accelTable.m_docTemplate=pDocTemplate;
		m_arrRemovedAccelTables.Add(accelTable);
		pArrRemovedAccels=FindRemovedAcceleratorTable(pDocTemplate);
	}
	ASSERT(pArrRemovedAccels!=NULL);

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if(accel.cmd==cmd)
		{
			pArrAccels->RemoveAt(nAccelIndex);
			nAccelIndex--;
			// save the removed accelerator 
			pArrRemovedAccels->Add(accel);
		}
	}

	return TRUE;
}

WORD COXShortkeysOrganizer::FindAccelerator(BYTE fVirt, WORD key, 
											CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAccelerator: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return 0;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return 0;
	}

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if((accel.fVirt&~FNOINVERT)==(fVirt&~FNOINVERT) && accel.key==key)
		{
			return accel.cmd;
		}
	}

	return 0;
}

COXArrAccelerators* COXShortkeysOrganizer::
FindAcceleratorTable(CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAcceleratorTable: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return NULL;
	}

	for(int nIndex=0; nIndex<m_arrAccelTables.GetSize(); nIndex++)
	{
		if(m_arrAccelTables[nIndex].m_docTemplate==pDocTemplate)
		{
			return &m_arrAccelTables[nIndex].m_accelerators;
		}
	}

	TRACE(_T("COXShortkeysOrganizer::FindAcceleratorTable: specified CMultiDocTemplate object hasn't been identified as valid host of an accelerator table\n"));
	return NULL;
}

BOOL COXShortkeysOrganizer::FindAllAccelerators(WORD cmd, 
												CMultiDocTemplate* pDocTemplate,
												COXArrAccelerators& arrAccels)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAllAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return FALSE;
	}

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if(accel.cmd==cmd)
		{
			arrAccels.Add(accel);
		}
	}

	return TRUE;
}

BOOL COXShortkeysOrganizer::FindAllAccelerators(BYTE fVirt, WORD key, 
												COXArrAccelerators& arrAccels)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAllAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	for(int nIndex=0; nIndex<m_arrAccelTables.GetSize(); nIndex++)
	{
		COXArrAccelerators* pArrAccels=&m_arrAccelTables[nIndex].m_accelerators;
		for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
		{
			ACCEL accel=pArrAccels->GetAt(nAccelIndex);
			if((accel.fVirt&~FNOINVERT)==(fVirt&~FNOINVERT) && accel.key==key)
			{
				arrAccels.Add(accel);
			}
		}
	}

	return TRUE;
}


WORD COXShortkeysOrganizer::FindRemovedAccelerator(BYTE fVirt, WORD key, 
												   CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindRemovedAccelerator: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return 0;
	}

	COXArrAccelerators* pArrAccels=FindRemovedAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return 0;
	}

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if((accel.fVirt&~FNOINVERT)==(fVirt&~FNOINVERT) && accel.key==key)
		{
			return accel.cmd;
		}
	}

	return 0;
}

COXArrAccelerators* COXShortkeysOrganizer::
FindRemovedAcceleratorTable(CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindRemovedAcceleratorTable: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return NULL;
	}

	for(int nIndex=0; nIndex<m_arrRemovedAccelTables.GetSize(); nIndex++)
	{
		if(m_arrRemovedAccelTables[nIndex].m_docTemplate==pDocTemplate)
		{
			return &m_arrRemovedAccelTables[nIndex].m_accelerators;
		}
	}

	TRACE(_T("COXShortkeysOrganizer::FindRemovedAcceleratorTable: specified CMultiDocTemplate object hasn't been identified as valid host of an accelerator table\n"));
	return NULL;
}

BOOL COXShortkeysOrganizer::FindAllRemovedAccelerators(WORD cmd, 
													   CMultiDocTemplate* pDocTemplate,
													   COXArrAccelerators& arrAccels)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAllRemovedAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindRemovedAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		return FALSE;
	}

	for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
	{
		ACCEL accel=pArrAccels->GetAt(nAccelIndex);
		if(accel.cmd==cmd)
		{
			arrAccels.Add(accel);
		}
	}

	return TRUE;
}

BOOL COXShortkeysOrganizer::FindAllRemovedAccelerators(BYTE fVirt, WORD key, 
													   COXArrAccelerators& arrAccels)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::FindAllRemovedAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	for(int nIndex=0; nIndex<m_arrRemovedAccelTables.GetSize(); nIndex++)
	{
		COXArrAccelerators* pArrAccels=&m_arrRemovedAccelTables[nIndex].m_accelerators;
		for(int nAccelIndex=0; nAccelIndex<pArrAccels->GetSize(); nAccelIndex++)
		{
			ACCEL accel=pArrAccels->GetAt(nAccelIndex);
			if((accel.fVirt&~FNOINVERT)==(fVirt&~FNOINVERT) && accel.key==key)
			{
				arrAccels.Add(accel);
			}
		}
	}

	return TRUE;
}


BOOL COXShortkeysOrganizer::RetrieveAccelerators()
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::RetrieveAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	ASSERT(m_pFrameWnd!=NULL);
	ASSERT(m_pFrameWnd->GetSafeHwnd());

	m_arrAccelTables.RemoveAll();

	if(m_pFrameWnd->m_hAccelTable!=NULL)
	{
		VERIFY(AddAccelTable(m_pFrameWnd->m_hAccelTable,NULL));
	}

	// if the attached frame window is an MDI mainframe window then we also 
	// have to retrieve accelerators associated with CMultiDocTemplate objects
	if(AfxGetMainWnd()==m_pFrameWnd && 
		m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);
		POSITION pos=pApp->GetFirstDocTemplatePosition();
		while(pos!=NULL)
		{
			CMultiDocTemplate* pDocTemplate=
				(CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
			ASSERT(pDocTemplate!=NULL);
			if(pDocTemplate->m_hAccelTable!=NULL)
			{
				VERIFY(AddAccelTable(pDocTemplate->m_hAccelTable,pDocTemplate));
			}
		}
	}

	return TRUE;
}


BOOL COXShortkeysOrganizer::SetAccelerators()
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::SetAccelerators: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	ASSERT(m_pFrameWnd!=NULL);
	ASSERT(m_pFrameWnd->GetSafeHwnd());

	// first of all build a table for main window
	HACCEL hNewMainAccelTable=BuildAccelTable(NULL);
	m_pFrameWnd->m_hAccelTable=hNewMainAccelTable;


	// if the attached frame window is an MDI mainframe window then we also 
	// have to build accelerators associated with CMultiDocTemplate objects
	if(AfxGetMainWnd()==m_pFrameWnd && 
		m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);
		POSITION pos=pApp->GetFirstDocTemplatePosition();
		while(pos!=NULL)
		{
			CMultiDocTemplate* pDocTemplate=
				(CMultiDocTemplate*)pApp->GetNextDocTemplate(pos);
			ASSERT(pDocTemplate!=NULL);
			HACCEL hNewAccelTable=BuildAccelTable(pDocTemplate);
			HACCEL hOldAccelTable=pDocTemplate->m_hAccelTable;
			pDocTemplate->m_hAccelTable=hNewAccelTable;

			// change accelerator table for all MDIChild windows that host 
			// documents of this template
			POSITION posDoc=pDocTemplate->GetFirstDocPosition();
			while(posDoc!=NULL)
			{
				CDocument* pDoc=pDocTemplate->GetNextDoc(posDoc);
				ASSERT_VALID(pDoc);

				// go through all views
				POSITION posView=pDoc->GetFirstViewPosition();
				while(posView!=NULL)
				{
					CView* pView=pDoc->GetNextView(posView);
					ASSERT_VALID(pView);

					CFrameWnd* pFrame=pView->GetParentFrame();
					ASSERT_VALID(pFrame);

					if(pFrame->m_hAccelTable==hOldAccelTable)
					{
						pFrame->m_hAccelTable=hNewAccelTable;
					}
				}
			}
		}
	}

	// clean up information about removed accelerators
	m_arrRemovedAccelTables.RemoveAll();

	return TRUE;
}


BOOL COXShortkeysOrganizer::
SaveState(LPCTSTR lpszProfileName/*=_T("ShortkeysOrganizerState")*/)
{
#ifndef _MAC
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	CString m_sProfileName=lpszProfileName;
	ASSERT(!m_sProfileName.IsEmpty());

	ASSERT(m_pFrameWnd!=NULL);
	ASSERT(m_pFrameWnd->GetSafeHwnd());

	CWinApp* pApp=AfxGetApp();

	// make sure you called CWinApp::SetRegistryKey() functions before
	if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: CWinApp::SetRegistryKey() must be called in InitInstance()\n"));
		return FALSE;
	}
	// we use default registry key assigned to your application by MFC
	HKEY hSecKey=pApp->GetSectionKey(_T(""));
	if (hSecKey==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: unable to get section key\n"));
		return FALSE;
	}
	
	BOOL bResult=TRUE;
	try
	{
		COXRegistryValFile regAccelState(hSecKey,m_sProfileName,
			_T("AcceleratorsState"));
		CArchive ar(&regAccelState, CArchive::store);
		Serialize(ar);
		ar.Close();
	}
	catch(CException* e)
	{
		UNREFERENCED_PARAMETER(e);
		bResult=FALSE;
	}

	::RegCloseKey(hSecKey);

	return bResult;
#else
	return FALSE;
#endif
}

BOOL COXShortkeysOrganizer::
LoadState(LPCTSTR lpszProfileName/*=_T("ShortkeysOrganizerState")*/,
		  BOOL bApply/*=TRUE*/)
{
#ifndef _MAC
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	CString m_sProfileName=lpszProfileName;
	ASSERT(!m_sProfileName.IsEmpty());

	ASSERT(m_pFrameWnd!=NULL);
	ASSERT(m_pFrameWnd->GetSafeHwnd());

	CWinApp* pApp=AfxGetApp();

	// make sure you called CWinApp::SetRegistryKey() functions before
	if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: CWinApp::SetRegistryKey() must be called in InitInstance()\n"));
		return FALSE;
	}
	// we use default registry key assigned to your application by MFC
	HKEY hSecKey=pApp->GetSectionKey(_T(""));
	if (hSecKey==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::SaveState: unable to get section key\n"));
		return FALSE;
	}
	
	BOOL bResult=TRUE;
	try
	{
		COXRegistryValFile regAccelState(hSecKey,m_sProfileName,
			_T("AcceleratorsState"));
		if(regAccelState.GetLength()>0)
		{
			CArchive ar(&regAccelState, CArchive::load);
			Serialize(ar);
			ar.Close();
		}
	}
	catch(CException* e)
	{
		UNREFERENCED_PARAMETER(e);
		bResult=FALSE;
	}

	::RegCloseKey(hSecKey);

	if(bResult && bApply)
		VERIFY(SetAccelerators());

	return bResult;
#else
	return FALSE;
#endif
}


void COXShortkeysOrganizer::Serialize(CArchive& ar)
{
    if(ar.IsStoring())
    {
        ar<<(DWORD)m_arrAccelTables.GetSize();
		for(int nIndex=0; nIndex<m_arrAccelTables.GetSize(); nIndex++)
		{
			SerializeAccelTable(ar,&m_arrAccelTables[nIndex]);
		}
	}
	else
	{
		DWORD dwTableCount=0;
		ar>>dwTableCount;
		ASSERT(m_arrAccelTables.GetSize()==(int)dwTableCount);
		if(m_arrAccelTables.GetSize()!=(int)dwTableCount)
		{
			TRACE(_T("COXShortkeysOrganizer::Serialize: failed to read the serialized data\n"));
			return;
		}
		for(int nIndex=0; nIndex<(int)dwTableCount; nIndex++)
		{
			SerializeAccelTable(ar,&m_arrAccelTables[nIndex]);
		}
	}                 
} 


CString COXShortkeysOrganizer::GetAccelKeyString(ACCEL* pAccel)
{
	ASSERT(pAccel!=NULL);

	CString sKey(_T(""));
	if(pAccel->fVirt&FSHIFT)
		sKey+=_T("Shift+");
	if(pAccel->fVirt&FCONTROL)
		sKey+=_T("Ctrl+");
	if(pAccel->fVirt&FALT)
		sKey+=_T("Alt+");
	if(pAccel->fVirt&FVIRTKEY)
	{
		TCHAR szKeyName[10];
		LPARAM lParam=MAKELPARAM(0,::MapVirtualKey(pAccel->key,0)) | 
			(pAccel->key<0x0030 ? 0x01000000 : 0);
		if(pAccel->key!=0xbf)
		{
			::GetKeyNameText(PtrToLong(lParam),szKeyName,10);
		}
		else
		{
			lstrcpy(szKeyName,_T("Slash"));
		}
		sKey+=szKeyName;
	}
	else
	{
		sKey+=(TCHAR)pAccel->key;
	}

	return sKey;
}


BOOL COXShortkeysOrganizer::GetAccelKeyFromString(LPCTSTR lpszString, ACCEL* pAccel)
{
	ASSERT(lpszString!=NULL);
	ASSERT(pAccel!=NULL);

	CString sKey=lpszString;
	sKey.TrimLeft();
	sKey.TrimRight();
	if(sKey.IsEmpty())
		return FALSE;

	pAccel->fVirt=FVIRTKEY;
	pAccel->cmd=0;
	int nPlusPos=sKey.Find(_T("+"));

	while(nPlusPos!=-1)
	{
		CString sSystemKey=sKey.Left(nPlusPos);
		sKey=sKey.Mid(nPlusPos+1);
		if(sSystemKey.CompareNoCase(_T("Shift"))==0)
			pAccel->fVirt|=FSHIFT;
		else if(sSystemKey.CompareNoCase(_T("Ctrl"))==0)
			pAccel->fVirt|=FCONTROL;
		else if(sSystemKey.CompareNoCase(_T("Alt"))==0)
			pAccel->fVirt|=FALT;

		if(sKey.GetLength()==1)
			break;
		else
			nPlusPos=sKey.Find(_T("+"));
		if(nPlusPos==sKey.GetLength()-1)
			break;
	}

	if(sKey.GetLength()==1)
	{
		if(sKey.GetAt(0)==_T('/'))
		{
			pAccel->key=VK_DIVIDE;
		}
		else
		{
			SHORT vkKey=::VkKeyScan(sKey.GetAt(0));;
			pAccel->key=LOBYTE(vkKey);
		}
	}
	else if(sKey.CompareNoCase(_T("Slash"))==0)
	{
		pAccel->key=0xbf;
	}
	else if(sKey.CompareNoCase(_T("Enter"))==0)
	{
		pAccel->key=VK_RETURN;
	}
	else if(sKey.CompareNoCase(_T("End"))==0)
	{
		pAccel->key=VK_END;
	}
	else if(sKey.CompareNoCase(_T("Insert"))==0)
	{
		pAccel->key=VK_INSERT;
	}
	else if(sKey.CompareNoCase(_T("Del"))==0)
	{
		pAccel->key=VK_DELETE;
	}
	else if(sKey.CompareNoCase(_T("Home"))==0)
	{
		pAccel->key=VK_HOME;
	}
	else if(sKey.CompareNoCase(_T("Page Up"))==0)
	{
		pAccel->key=VK_PRIOR;
	}
	else if(sKey.CompareNoCase(_T("Page Down"))==0)
	{
		pAccel->key=VK_NEXT;
	}
	else if(sKey.CompareNoCase(_T("Esc"))==0)
	{
		pAccel->key=VK_ESCAPE;
	}
	else if(sKey.CompareNoCase(_T("Up"))==0)
	{
		pAccel->key=VK_UP;
	}
	else if(sKey.CompareNoCase(_T("Down"))==0)
	{
		pAccel->key=VK_DOWN;
	}
	else if(sKey.CompareNoCase(_T("Right"))==0)
	{
		pAccel->key=VK_RIGHT;
	}
	else if(sKey.CompareNoCase(_T("Left"))==0)
	{
		pAccel->key=VK_LEFT;
	}
	else if(sKey.CompareNoCase(_T("F1"))==0)
	{
		pAccel->key=VK_F1;
	}
	else if(sKey.CompareNoCase(_T("F2"))==0)
	{
		pAccel->key=VK_F2;
	}
	else if(sKey.CompareNoCase(_T("F3"))==0)
	{
		pAccel->key=VK_F3;
	}
	else if(sKey.CompareNoCase(_T("F4"))==0)
	{
		pAccel->key=VK_F4;
	}
	else if(sKey.CompareNoCase(_T("F5"))==0)
	{
		pAccel->key=VK_F5;
	}
	else if(sKey.CompareNoCase(_T("F6"))==0)
	{
		pAccel->key=VK_F6;
	}
	else if(sKey.CompareNoCase(_T("F7"))==0)
	{
		pAccel->key=VK_F7;
	}
	else if(sKey.CompareNoCase(_T("F8"))==0)
	{
		pAccel->key=VK_F8;
	}
	else if(sKey.CompareNoCase(_T("F9"))==0)
	{
		pAccel->key=VK_F9;
	}
	else if(sKey.CompareNoCase(_T("F10"))==0)
	{
		pAccel->key=VK_F10;
	}
	else if(sKey.CompareNoCase(_T("F11"))==0)
	{
		pAccel->key=VK_F11;
	}
	else if(sKey.CompareNoCase(_T("F12"))==0)
	{
		pAccel->key=VK_F12;
	}
	else if(sKey.CompareNoCase(_T("Num 0"))==0)
	{
		pAccel->key=VK_NUMPAD0;
	}
	else if(sKey.CompareNoCase(_T("Num 1"))==0)
	{
		pAccel->key=VK_NUMPAD1;
	}
	else if(sKey.CompareNoCase(_T("Num 2"))==0)
	{
		pAccel->key=VK_NUMPAD2;
	}
	else if(sKey.CompareNoCase(_T("Num 3"))==0)
	{
		pAccel->key=VK_NUMPAD3;
	}
	else if(sKey.CompareNoCase(_T("Num 4"))==0)
	{
		pAccel->key=VK_NUMPAD4;
	}
	else if(sKey.CompareNoCase(_T("Num 5"))==0)
	{
		pAccel->key=VK_NUMPAD5;
	}
	else if(sKey.CompareNoCase(_T("Num 6"))==0)
	{
		pAccel->key=VK_NUMPAD6;
	}
	else if(sKey.CompareNoCase(_T("Num 7"))==0)
	{
		pAccel->key=VK_NUMPAD7;
	}
	else if(sKey.CompareNoCase(_T("Num 8"))==0)
	{
		pAccel->key=VK_NUMPAD8;
	}
	else if(sKey.CompareNoCase(_T("Num 9"))==0)
	{
		pAccel->key=VK_NUMPAD9;
	}
	else if(sKey.CompareNoCase(_T("Num /"))==0)
	{
		pAccel->key=VK_DIVIDE;
	}
	else if(sKey.CompareNoCase(_T("Num *"))==0)
	{
		pAccel->key=VK_MULTIPLY;
	}
	else if(sKey.CompareNoCase(_T("Num -"))==0)
	{
		pAccel->key=VK_SUBTRACT;
	}
	else if(sKey.CompareNoCase(_T("Num +"))==0)
	{
		pAccel->key=VK_ADD;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL COXShortkeysOrganizer::AddAccelTable(HACCEL hAccelTable, 
										  CMultiDocTemplate* pDocTemplate)
{
	ASSERT(hAccelTable!=NULL);

	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::AddAccelTable: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return FALSE;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels!=NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::AddAccelTable: accelerator table for specified CMultiDocTemplate object has already been added\n"));
		return FALSE;
	}

	OXACCELTABLE accelTable;
	accelTable.m_docTemplate=pDocTemplate;

	int nAccelCount=::CopyAcceleratorTable(hAccelTable,NULL,0);
	if(nAccelCount!=0)
	{
		ACCEL* pAccel=new ACCEL[nAccelCount];
		if (pAccel==NULL)
		{
			TRACE(_T("COXShortkeysOrganizer::AddAccelTable: failed to copy an accelerator table\n"));
			return FALSE;
		}
		VERIFY(::CopyAcceleratorTable(hAccelTable,pAccel,nAccelCount)==nAccelCount);

		for(int nIndex=0; nIndex<nAccelCount; nIndex++)
		{
			accelTable.m_accelerators.Add(pAccel[nIndex]);
		}

		delete[] pAccel;
	}

	m_arrAccelTables.Add(accelTable);

	return TRUE;
}


HACCEL COXShortkeysOrganizer::BuildAccelTable(CMultiDocTemplate* pDocTemplate)
{
	if(!IsAttached())
	{
		TRACE(_T("COXShortkeysOrganizer::BuildAccelTable: there is no attached frame window. You have to attach frame window before calling this function\n"));
		return NULL;
	}

	COXArrAccelerators* pArrAccels=FindAcceleratorTable(pDocTemplate);
	if(pArrAccels==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::BuildAccelTable: accelerator table for specified CMultiDocTemplate object doesn't exist\n"));
		return NULL;
	}

	ACCEL* pAccelTable=new ACCEL[pArrAccels->GetSize()];
	if(pAccelTable==NULL)
	{
		TRACE(_T("COXShortkeysOrganizer::BuildAccelTable: failed to allocate memory for new accelerator table\n"));
		return NULL;
	}
	for(int nIndex=0; nIndex<pArrAccels->GetSize(); nIndex++)
	{
		pAccelTable[nIndex].fVirt=pArrAccels->GetAt(nIndex).fVirt;
		pAccelTable[nIndex].key=pArrAccels->GetAt(nIndex).key;
		pAccelTable[nIndex].cmd=pArrAccels->GetAt(nIndex).cmd;
	}

    HACCEL hNewTable=::CreateAcceleratorTable(pAccelTable,PtrToInt(pArrAccels->GetSize()));
	m_arrCreatedAccel.Add(hNewTable);

	delete[] pAccelTable;

	return hNewTable;
}


void COXShortkeysOrganizer::SerializeAccelTable(CArchive& ar, 
												OXACCELTABLE* pAccelTable)
{
	ASSERT(pAccelTable!=NULL);

    if(ar.IsStoring())
    {
		ar<<(DWORD)pAccelTable->m_accelerators.GetSize();
		for(int nIndex=0; nIndex<pAccelTable->m_accelerators.GetSize(); nIndex++)
		{
			ACCEL accel=pAccelTable->m_accelerators[nIndex];
			ar<<accel.fVirt;
			ar<<accel.key;
			ar<<accel.cmd;
		}
	}
	else
	{
		DWORD dwAccelCount;
		ar>>dwAccelCount;
		pAccelTable->m_accelerators.SetSize(dwAccelCount);
		for(int nIndex=0; nIndex<(int)dwAccelCount; nIndex++)
		{
			ACCEL accel;
			ar>>accel.fVirt;
			ar>>accel.key;
			ar>>accel.cmd;
			pAccelTable->m_accelerators.SetAt(nIndex,accel);
		}
	}                 
}


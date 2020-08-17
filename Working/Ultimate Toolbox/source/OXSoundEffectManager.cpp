// ==========================================================================
// 							   Class Implementation : 
//					COXSoundEffectManager & COXSoundEffectOrganizer
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OXSoundEffectManager.h"

#ifndef OXSE_NO_SAVESTATE
#include "OXRegistryValFile.h"
#endif // OXSE_NO_SAVESTATE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(COXSoundEffectManager, COXHookWnd);

// Constructor
COXSoundEffectManager::COXSoundEffectManager(COXSoundEffectOrganizer* pOrganizer/*=NULL*/)
{
	m_pOrganizer=pOrganizer;
	Reset();
}

// Destructor
COXSoundEffectManager::~COXSoundEffectManager()
{
	if(IsHooked())
		UnregisterAllEffects();
	// delete all the structures we allocated
	LPSOUNDEFFECT pSoundEffect;
	UINT nMsgID;
	POSITION pos=m_allSoundPlayEvents.GetStartPosition();
	while(pos!=NULL)
	{
		m_allSoundPlayEvents.GetNextAssoc(pos,nMsgID,pSoundEffect);
		ASSERT(nMsgID!=SNDEVENT_NONE);
		ASSERT(pSoundEffect!=NULL);
		delete pSoundEffect;
	}
	m_allSoundPlayEvents.RemoveAll();

	m_allSoundStopEvents.RemoveAll();
}

// Attach ListCtrl handler.
BOOL COXSoundEffectManager::Attach(CWnd* pWnd)
{
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	Reset();

	HookWindow(pWnd);

	// call this function in oder to get HWM_MOUSEENTER and HWM_MOUSELEAVE messages
	InstallSpy();

	return TRUE;
}

// Detach the handler. Called by default when hooked window is about
// to be destroyed.
void COXSoundEffectManager::Detach()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	Reset();
	
	UnhookWindow();
}

////////////////////////////////////////////////////////////////////////////
// Message handler handles sound events and sound-related messages
//
LRESULT COXSoundEffectManager::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	BOOL bHandled=FALSE;

	// playing of the sound has been finished
	if(msg==WM_OX_SOUNDPLAYBACKCOMPLETE)
	{
		ASSERT(!bHandled);

		// start playing next event if there is any in the queue
		if(m_nNextEvent!=SNDEVENT_NONE)
		{
			m_nCurrentEvent=m_nNextEvent;
			m_nNextEvent=SNDEVENT_NONE;
			StartPlaying();
		}
		else
			Reset();

		bHandled=TRUE;
	}

	// playing sound in the loop
	if(msg==WM_OX_SOUNDPLAYLOOPING && m_nCurrentEvent!=SNDEVENT_NONE)
	{
		TRACE(_T("WM_OX_SOUNDPLAYLOOPING\n"));
		ASSERT(!bHandled);
		
		LPSOUNDEFFECT pSoundEffect=GetSoundEffect(m_nCurrentEvent);
		ASSERT(pSoundEffect);
		ASSERT(pSoundEffect->bLoop);

		if(pSoundEffect->nLoopCount!=-1)
		{
			m_nLoopCount++;
			ASSERT(pSoundEffect->nLoopCount==0 || 
				pSoundEffect->nLoopCount>=m_nLoopCount);
			// stop playing sound if the specified number of repeated playings
			// has elapsed
			if(pSoundEffect->nLoopCount<=m_nLoopCount)
				StopPlaying();
		}

		bHandled=TRUE;
	}

	UINT nOrder;
	// check if the current message is one of the stop event
	if(m_allSoundStopEvents.Lookup(msg,nOrder))
	{
		ASSERT(!bHandled);
		StopPlaying();
		bHandled=TRUE;
	}

	LPSOUNDEFFECT pSoundEffect;
	// check if the current message is one of the playing event
	if(m_allSoundPlayEvents.Lookup(msg,pSoundEffect))
	{
		ASSERT(!bHandled);
	
		ASSERT(pSoundEffect);
		BOOL bAmIPlaying=IsPlaying();
		BOOL bOtherPlaying=FALSE;
		if(!bAmIPlaying && m_pOrganizer!=NULL)
			bOtherPlaying=m_pOrganizer->IsPlaying();
		BOOL bPlaying=bAmIPlaying|bOtherPlaying;

		if(!bPlaying || pSoundEffect->bForceToStart)
		{
			if(bAmIPlaying)
			{
				StopPlaying();
				m_nNextEvent=msg;
			}
			else
			{
				if(bOtherPlaying && m_pOrganizer!=NULL)
					m_pOrganizer->StopPlaying();
				m_nCurrentEvent=msg;
				StartPlaying();
			}
		}

		bHandled=TRUE;
	}

	// I don't handle it: pass along
	return COXHookWnd::WindowProc(msg, wp, lp);
}

void COXSoundEffectManager::UnregisterAllPlayEvents()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	StopPlaying();

	// delete all start playing events
	LPSOUNDEFFECT pSoundEffect;
	UINT nMsgID;
	POSITION pos=m_allSoundPlayEvents.GetStartPosition();
	while(pos!=NULL)
	{
		m_allSoundPlayEvents.GetNextAssoc(pos,nMsgID,pSoundEffect);
		ASSERT(nMsgID!=SNDEVENT_NONE);
		ASSERT(pSoundEffect!=NULL);
		delete pSoundEffect;
	}
	m_allSoundPlayEvents.RemoveAll();
}

void COXSoundEffectManager::UnregisterAllStopEvents()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	// delete all stop playing events
	m_allSoundStopEvents.RemoveAll();
}

void COXSoundEffectManager::UnregisterAllEffects()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	UnregisterAllPlayEvents();
	UnregisterAllStopEvents();
}

BOOL COXSoundEffectManager::RegisterPlayEvent(UINT nMsgIDtoStartPlaying, 
											  LPCTSTR pszFileName, 
											  BOOL bLoop/*=FALSE*/, 
											  int nLoopCount/*=-1*/, 
											  BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(pszFileName!=NULL);

	ASSERT(nMsgIDtoStartPlaying!=SNDEVENT_NONE);

	// create sound object
	COXSound* pSound=CreateSound();

	if(pSound==NULL)
	{
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to create COXSound object\n"));
		return FALSE;
	}

	// try to open the specified file
	if(!pSound->Open(pszFileName))
	{
		delete pSound;
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to open specified sound file\n"));
		return FALSE;
	}

	// register it
	return RegisterSoundEffect(nMsgIDtoStartPlaying,pSound,
		bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectManager::RegisterPlayEvent(UINT nMsgIDtoStartPlaying, 
											  UINT nSoundResource, 
											  HINSTANCE hResInstance/*=AfxGetResourceHandle()*/, 
											  BOOL bLoop/*=FALSE*/, 
											  int nLoopCount/*=-1*/, 
											  BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStartPlaying!=SNDEVENT_NONE);

	COXSound* pSound=CreateSound();
	if(pSound==NULL)
	{
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to create COXSound object\n"));
		return FALSE;
	}

	if(!pSound->Open(nSoundResource,hResInstance))
	{
		delete pSound;
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to open specified sound file\n"));
		return FALSE;
	}

	return RegisterSoundEffect(nMsgIDtoStartPlaying,pSound,
		bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectManager::RegisterPlayEvent(UINT nMsgIDtoStartPlaying, 
											  CFile* pOpenedFile, 
											  BOOL bLoop/*=FALSE*/, 
											  int nLoopCount/*=-1*/, 
											  BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStartPlaying!=SNDEVENT_NONE);
	ASSERT(pOpenedFile!=NULL);

	COXSound* pSound=CreateSound();
	if(pSound==NULL)
	{
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to create COXSound object\n"));
		return FALSE;
	}

	if(!pSound->Open(pOpenedFile))
	{
		delete pSound;
		TRACE(_T("COXSoundEffectManager::RegisterPlayEvent: failed to open specified sound file\n"));
		return FALSE;
	}

	return RegisterSoundEffect(nMsgIDtoStartPlaying,pSound,
		bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectManager::UnregisterPlayEvent(UINT nMsgIDtoStartPlaying)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStartPlaying!=SNDEVENT_NONE);

	if(nMsgIDtoStartPlaying==m_nCurrentEvent)
	{
		ASSERT(IsPlaying());
		VERIFY(StopPlaying());
	}

	LPSOUNDEFFECT pSoundEffect;
	if(m_allSoundPlayEvents.Lookup(nMsgIDtoStartPlaying,pSoundEffect))
	{
		ASSERT(pSoundEffect);
		delete pSoundEffect;
		VERIFY(m_allSoundPlayEvents.RemoveKey(nMsgIDtoStartPlaying));

		return TRUE;
	}
	else
	{
		TRACE(_T("COXSoundEffectManager::UnregisterPlayEvent: event %d wasn't registered\n"),nMsgIDtoStartPlaying);
		return FALSE;
	}
}

BOOL COXSoundEffectManager::IsRegisteredPlayEvent(UINT nMsgIDtoStartPlaying)
{
	LPSOUNDEFFECT pSoundEffect;
	return (m_allSoundPlayEvents.Lookup(nMsgIDtoStartPlaying,pSoundEffect));
}

UINT COXSoundEffectManager::GetNextPlayEvent(POSITION& rNextPosition)
{
	ASSERT(rNextPosition!=NULL);

	LPSOUNDEFFECT pSoundEffect;
	UINT nMsgID;
	m_allSoundPlayEvents.GetNextAssoc(rNextPosition,nMsgID,pSoundEffect);

	ASSERT(nMsgID!=SNDEVENT_NONE);
	ASSERT(pSoundEffect!=NULL);

	return nMsgID;
}

BOOL COXSoundEffectManager::RegisterStopEvent(UINT nMsgIDtoStopPlaying)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStopPlaying!=SNDEVENT_NONE);

	LPSOUNDEFFECT pSoundEffect;
	if(m_allSoundPlayEvents.Lookup(nMsgIDtoStopPlaying,pSoundEffect))
	{
		TRACE(_T("COXSoundEffectManager::RegisterStopEvent: event already registered as Start Event\n"));
		return FALSE;
	}

	UnregisterStopEvent(nMsgIDtoStopPlaying);
	m_allSoundStopEvents.SetAt(nMsgIDtoStopPlaying,PtrToUint(m_allSoundPlayEvents.GetCount()));

	return TRUE;
}

BOOL COXSoundEffectManager::UnregisterStopEvent(UINT nMsgIDtoStopPlaying)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStopPlaying!=SNDEVENT_NONE);

	UINT nOrder;
	if(m_allSoundStopEvents.Lookup(nMsgIDtoStopPlaying,nOrder))
	{
		VERIFY(m_allSoundStopEvents.RemoveKey(nMsgIDtoStopPlaying));
		return TRUE;
	}
	else
	{
		TRACE(_T("COXSoundEffectManager::UnregisterStopEvent: event %d wasn't registered\n"),nMsgIDtoStopPlaying);
		return FALSE;
	}
}

BOOL COXSoundEffectManager::IsRegisteredStopEvent(UINT nMsgIDtoStopPlaying)
{
	UINT nOrder;
	return (m_allSoundStopEvents.Lookup(nMsgIDtoStopPlaying,nOrder));
}

UINT COXSoundEffectManager::GetNextStopEvent(POSITION& rNextPosition)
{
	ASSERT(rNextPosition!=NULL);

	UINT nOrder;
	UINT nMsgID;
	m_allSoundStopEvents.GetNextAssoc(rNextPosition,nMsgID,nOrder);

	ASSERT(nMsgID!=SNDEVENT_NONE);

	return nMsgID;
}

BOOL COXSoundEffectManager::IsPlaying()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	if(m_nCurrentEvent==SNDEVENT_NONE)
		return FALSE;
	else
	{
		LPSOUNDEFFECT pSoundEffect=GetSoundEffect(m_nCurrentEvent);
		ASSERT(pSoundEffect);
		return pSoundEffect->pSound->IsPlaying();
	}
}

BOOL COXSoundEffectManager::StopPlaying()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	if(!IsPlaying())
	{
		TRACE(_T("COXSoundEffectManager::StopPlaying: there is nothing to stop\n"));
		return TRUE;
	}
	else
	{
		LPSOUNDEFFECT pSoundEffect=GetSoundEffect(m_nCurrentEvent);
		ASSERT(pSoundEffect);
		ASSERT(pSoundEffect->pSound->IsPlaying());

		pSoundEffect->pSound->Stop();
		Reset();

		return TRUE;
	}
}

COXSound* COXSoundEffectManager::CreateSound()
{
	COXSound* pSound=NULL;
	try
	{
		pSound=new COXSound;
	}
	catch (CMemoryException* pException)
	{
		UNREFERENCED_PARAMETER(pException);
		TRACE(_T("COXSoundEffectManager::CreateSound: failed to allocate memory for COXSound object\n"));
	}

	return pSound;
}

BOOL COXSoundEffectManager::RegisterSoundEffect(UINT nMsgIDtoStartPlaying, 
												COXSound* pSound, 
												BOOL bLoop/*=FALSE*/, 
												int nLoopCount/*=-1*/, 
												BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	ASSERT(nMsgIDtoStartPlaying!=SNDEVENT_NONE);
	ASSERT(pSound);

	UINT nOrder;
	if(m_allSoundStopEvents.Lookup(nMsgIDtoStartPlaying,nOrder))
	{
		delete pSound;
		TRACE(_T("COXSoundEffectManager::RegisterSoundObject: event already registered as Stop Event\n"));
		return FALSE;
	}

	UnregisterPlayEvent(nMsgIDtoStartPlaying);

	LPSOUNDEFFECT pSoundEffect=new SOUNDEFFECT;
	pSoundEffect->pSound=pSound;
	pSoundEffect->bLoop=bLoop;
	pSoundEffect->nLoopCount=nLoopCount;
	pSoundEffect->bForceToStart=bForceToStart;

	m_allSoundPlayEvents.SetAt(nMsgIDtoStartPlaying,pSoundEffect);

	return TRUE;
}

BOOL COXSoundEffectManager::StartPlaying()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	if(m_nCurrentEvent==SNDEVENT_NONE)
	{
		TRACE(_T("COXSoundEffectManager::StartPlaying: there is no an active event to start playing\n"));
		return FALSE;
	}

	LPSOUNDEFFECT pSoundEffect=GetSoundEffect(m_nCurrentEvent);
	ASSERT(pSoundEffect);

	pSoundEffect->pSound->SetCallbackWnd(GetHookedWnd());
	if(!pSoundEffect->pSound->Play(pSoundEffect->bLoop))
	{
		TRACE(_T("COXSoundEffectManager::StartPlaying: failed to start playing current sound effect\n"));
		m_nCurrentEvent=SNDEVENT_NONE;
		return FALSE;
	}
	else
		return TRUE;
}

LPSOUNDEFFECT COXSoundEffectManager::GetSoundEffect(UINT nMsgIDtoStartPlaying)
{
	LPSOUNDEFFECT pSoundEffect;
	if(m_allSoundPlayEvents.Lookup(nMsgIDtoStartPlaying,pSoundEffect))
	{
		ASSERT(pSoundEffect);
		ASSERT(pSoundEffect->pSound);
		return pSoundEffect;
	}
	else
		return NULL;
}

void COXSoundEffectManager::Reset()
{
	m_nCurrentEvent=SNDEVENT_NONE;
	m_nLoopCount=0;
	m_nNextEvent=SNDEVENT_NONE;
}

//////////////////////////////////////
#ifndef OXSE_NO_SAVESTATE
//////////////////////////////////////
BOOL COXSoundEffectManager::SaveState(LPCTSTR lpszProfileName)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();

	// make sure you called CWinApp::SetRegistryKey() functions before
	if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
	{
		TRACE(_T("COXSoundEffectManager::SaveState: SetRegistryKey() haven't been called\n"));
		return FALSE;
	}
	// we use default registry key assigned to your application by MFC
	HKEY hSecKey=pApp->GetSectionKey(_T(""));
	if (hSecKey==NULL)
	{
		TRACE(_T("COXSoundEffectManager::SaveState: unable to get section key\n"));
		return FALSE;
	}

	BOOL bResult=TRUE;
	try
	{
		COXRegistryValFile reg(hSecKey, lpszProfileName, _T("SoundEffects"));
		CArchive ar(&reg, CArchive::store);
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

BOOL COXSoundEffectManager::LoadState(LPCTSTR lpszProfileName)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();

	// make sure you called CWinApp::SetRegistryKey() functions before
	if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
	{
		TRACE(_T("COXSoundEffectManager::LoadState: SetRegistryKey() haven't been called\n"));
		return FALSE;
	}
	// we use default registry key assigned to your application by MFC
	HKEY hSecKey=pApp->GetSectionKey(_T(""));
	if (hSecKey==NULL)
	{
		TRACE(_T("COXSoundEffectManager::SaveState: unable to get section key\n"));
		return FALSE;
	}
	
	BOOL bResult=TRUE;
	try
	{
		COXRegistryValFile reg(hSecKey, lpszProfileName, _T("SoundEffects"));
		if(reg.GetLength()>0)
		{
			CArchive ar(&reg, CArchive::load);
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

	return bResult;
#else
	return FALSE;
#endif
}
//////////////////////////////////////
#endif // OXSE_NO_SAVESTATE
//////////////////////////////////////

void COXSoundEffectManager::Serialize(CArchive& ar)
{
	ASSERT(IsHooked());

    // Only CObject-derived objects and six data-type
    // primitives are serializable. However, you
    // can cast any data type to a serializable data type,
    // and then you can serialize your data. The serializable
    // data types are

    // BYTE:    8 bits unsigned
    // WORD:    16 bits unsigned
    // LONG:    32 bits unsigned
    // DWORD:   32 bits unsigned
    // float    32 bits
    // double   64 bits, IEEE standard
    
    if (ar.IsStoring())
    {
		DWORD nPlayEventCount= PtrToLong(m_allSoundPlayEvents.GetCount());
        ar << nPlayEventCount;
		LPSOUNDEFFECT pSoundEffect;
		UINT nMsgID;
		POSITION pos=m_allSoundPlayEvents.GetStartPosition();
		while(pos!=NULL)
		{
			m_allSoundPlayEvents.GetNextAssoc(pos,nMsgID,pSoundEffect);
			ASSERT(nMsgID!=SNDEVENT_NONE);
			ASSERT(pSoundEffect);
			ASSERT(pSoundEffect->pSound);

	        ar << (DWORD)nMsgID;
			pSoundEffect->Serialize(ar);
			ASSERT(pSoundEffect->pSound);
			pSoundEffect->pSound->GetSoundSourceInfo()->Serialize(ar);
		}

		DWORD nStopEventCount= PtrToInt(m_allSoundStopEvents.GetCount());
        ar << nStopEventCount;
		UINT nOrder;
		pos=m_allSoundStopEvents.GetStartPosition();
		while(pos!=NULL)
		{
			m_allSoundStopEvents.GetNextAssoc(pos,nMsgID,nOrder);
			ASSERT(nMsgID!=SNDEVENT_NONE);

	        ar << (DWORD)nMsgID;
		}
    }
    else
    {
		UnregisterAllEffects();

		DWORD nPlayEventCount;
        ar >> nPlayEventCount;
		DWORD nMsgID;
		DWORD nIndex=0;
		for(nIndex=0; nIndex<nPlayEventCount; nIndex++)
		{
	        ar >> nMsgID;
			ASSERT(nMsgID!=SNDEVENT_NONE);
			SOUNDEFFECT soundEffect;
			soundEffect.Serialize(ar);
			SOUNDSOURCEINFO ssInfo;
			ssInfo.Serialize(ar);
			if(ssInfo.source!=SNDSRC_CFILE && ssInfo.source!=SNDSRC_UNKNOWN)
			{
				switch(ssInfo.source)
				{
					case SNDSRC_FILE:
						RegisterPlayEvent((UINT)nMsgID,ssInfo.sFileName,
							soundEffect.bLoop,soundEffect.nLoopCount,
							soundEffect.bForceToStart);
						break;
					case SNDSRC_INTRESOURCE:
						RegisterPlayEvent((UINT)nMsgID,ssInfo.nResourceID,
							ssInfo.hInstance,soundEffect.bLoop,
							soundEffect.nLoopCount,soundEffect.bForceToStart);
						break;
					default:
						ASSERT(FALSE);
				}
			}
		}

		DWORD nStopEventCount;
        ar >> nStopEventCount;
		for(nIndex=0; nIndex<nStopEventCount; nIndex++)
		{
	        ar >> nMsgID;
			ASSERT(nMsgID!=SNDEVENT_NONE);
			RegisterStopEvent((UINT)nMsgID);
		}

    }                 
}

///////////////////////////////////////////////////////////////

// Constructor
COXSoundEffectOrganizer::COXSoundEffectOrganizer()
{
}

// Destructor
COXSoundEffectOrganizer::~COXSoundEffectOrganizer()
{
	// delete all COXSoundEffectManager objects we allocated
	COXSoundEffectManager* pManager;
	CWnd* pWnd;
	POSITION pos=m_allSoundEffectManagers.GetStartPosition();
	while(pos!=NULL)
	{
		m_allSoundEffectManagers.GetNextAssoc(pos,pWnd,pManager);
		ASSERT(pWnd!=NULL);
		ASSERT(pManager!=NULL);
		delete pManager;
	}
	m_allSoundEffectManagers.RemoveAll();
}

void COXSoundEffectOrganizer::UnregisterAllPlayEvents(CWnd* pWnd/*=NULL*/)
{
	COXSoundEffectManager* pManager;
	if(pWnd==NULL)
	{
		POSITION pos=m_allSoundEffectManagers.GetStartPosition();
		while(pos!=NULL)
		{
			m_allSoundEffectManagers.GetNextAssoc(pos,pWnd,pManager);
			ASSERT(pWnd!=NULL);
			ASSERT(pManager!=NULL);
			pManager->UnregisterAllPlayEvents();
		}
	}
	else
	{
		pManager=GetManager(pWnd);
		ASSERT(pManager!=NULL);
		pManager->UnregisterAllPlayEvents();
	}
}

void COXSoundEffectOrganizer::UnregisterAllStopEvents(CWnd* pWnd/*=NULL*/)
{
	COXSoundEffectManager* pManager;
	if(pWnd==NULL)
	{
		POSITION pos=m_allSoundEffectManagers.GetStartPosition();
		while(pos!=NULL)
		{
			m_allSoundEffectManagers.GetNextAssoc(pos,pWnd,pManager);
			ASSERT(pWnd!=NULL);
			ASSERT(pManager!=NULL);
			pManager->UnregisterAllStopEvents();
		}
	}
	else
	{
		pManager=GetManager(pWnd);
		ASSERT(pManager!=NULL);
		pManager->UnregisterAllStopEvents();
	}
}

void COXSoundEffectOrganizer::UnregisterAllEffects(CWnd* pWnd/*=NULL*/)
{
	UnregisterAllPlayEvents(pWnd);
	UnregisterAllStopEvents(pWnd);
}

BOOL COXSoundEffectOrganizer::RegisterPlayEvent(CWnd* pWnd, UINT nMsgIDtoStartPlaying, 
												LPCTSTR pszFileName, 
												BOOL bLoop/*=FALSE*/, 
												int nLoopCount/*=-1*/, 
												BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	ASSERT(pszFileName!=NULL);

	COXSoundEffectManager* pManager=GetManager(pWnd);
	if(pManager==NULL)
		pManager=CreateManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->RegisterPlayEvent(nMsgIDtoStartPlaying,pszFileName,
		bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectOrganizer::RegisterPlayEvent(CWnd* pWnd, UINT nMsgIDtoStartPlaying, 
												UINT nSoundResource, 
												HINSTANCE hResInstance/*=AfxGetResourceHandle()*/, 
												BOOL bLoop/*=FALSE*/, 
												int nLoopCount/*=-1*/, 
												BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	if(pManager==NULL)
		pManager=CreateManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->RegisterPlayEvent(nMsgIDtoStartPlaying,nSoundResource,
		hResInstance,bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectOrganizer::RegisterPlayEvent(CWnd* pWnd, UINT nMsgIDtoStartPlaying, 
												CFile* pOpenedFile, 
												BOOL bLoop/*=FALSE*/, 
												int nLoopCount/*=-1*/, 
												BOOL bForceToStart/*=TRUE*/)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	if(pManager==NULL)
		pManager=CreateManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->RegisterPlayEvent(nMsgIDtoStartPlaying,pOpenedFile,
		bLoop,nLoopCount,bForceToStart);
}

BOOL COXSoundEffectOrganizer::UnregisterPlayEvent(CWnd* pWnd, 
												  UINT nMsgIDtoStartPlaying)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->UnregisterPlayEvent(nMsgIDtoStartPlaying);
}

COXSoundEffectManager* COXSoundEffectOrganizer::GetManager(CWnd* pWnd)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=NULL;
	if(!m_allSoundEffectManagers.Lookup(pWnd,pManager))
		return NULL;
	else
		return pManager;
}

CWnd* COXSoundEffectOrganizer::GetNextRegisteredWindow(POSITION& rNextPosition)
{
	ASSERT(rNextPosition!=NULL);

	COXSoundEffectManager* pManager;
	CWnd* pWnd;
	m_allSoundEffectManagers.GetNextAssoc(rNextPosition,pWnd,pManager);

	ASSERT(pWnd!=NULL);
	ASSERT(pManager!=NULL);

	return pWnd;
}

BOOL COXSoundEffectOrganizer::RegisterStopEvent(CWnd* pWnd, UINT nMsgIDtoStopPlaying)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	if(pManager==NULL)
		pManager=CreateManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->RegisterStopEvent(nMsgIDtoStopPlaying);
}

BOOL COXSoundEffectOrganizer::UnregisterStopEvent(CWnd* pWnd, 
												  UINT nMsgIDtoStopPlaying)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->UnregisterStopEvent(nMsgIDtoStopPlaying);
}

BOOL COXSoundEffectOrganizer::IsPlaying(CWnd* pWnd/*=NULL*/)
{
	COXSoundEffectManager* pManager;
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
		pManager=GetManager(pWnd);
		ASSERT(pManager);
		return pManager->IsPlaying();
	}
	else
	{
		BOOL bResult=FALSE;
		POSITION pos=m_allSoundEffectManagers.GetStartPosition();
		while(pos!=NULL)
		{
			m_allSoundEffectManagers.GetNextAssoc(pos,pWnd,pManager);
			ASSERT(pWnd!=NULL);
			ASSERT(pManager!=NULL);
			if(pManager->IsPlaying())
			{
				bResult=TRUE;
				break;
			}
		}

		return bResult;
	}
}

BOOL COXSoundEffectOrganizer::StopPlaying(CWnd* pWnd/*=NULL*/)
{
	if(!IsPlaying(pWnd))
	{
		TRACE(_T("COXSoundEffectOrganizer::StopPlaying: there is nothing to stop\n"));
		return TRUE;
	}
	else
	{
		COXSoundEffectManager* pManager;
		if(pWnd!=NULL)
		{
			ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
			pManager=GetManager(pWnd);
			ASSERT(pManager);
			ASSERT(pManager->IsPlaying());
			return pManager->StopPlaying();
		}
		else
		{
			BOOL bResult=TRUE;
			POSITION pos=m_allSoundEffectManagers.GetStartPosition();
			while(pos!=NULL)
			{
				m_allSoundEffectManagers.GetNextAssoc(pos,pWnd,pManager);
				ASSERT(pWnd!=NULL);
				ASSERT(pManager!=NULL);
				bResult&=pManager->StopPlaying();
			}

			return bResult;
		}
	}
}

#ifndef OXSE_NO_SAVESTATE
	// Save to and load from registry all registered start and stop playing events
	// for specified window. These functions require using COXRegistryValFile class. 
	// If you are not going to use them then you can include in your stdafx.h 
	// file next code:
	//	
	//		#define OXSE_NO_SAVESTATE
	//
	// In this case you don't need to include into your project references 
	// to source code for COXRegistryValFile class.
	//
BOOL COXSoundEffectOrganizer::SaveState(CWnd* pWnd, LPCTSTR lpszProfileName)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->SaveState(lpszProfileName);
}

BOOL COXSoundEffectOrganizer::LoadState(CWnd* pWnd, LPCTSTR lpszProfileName)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=GetManager(pWnd);
	if(pManager==NULL)
		pManager=CreateManager(pWnd);
	ASSERT(pManager!=NULL);

	return pManager->LoadState(lpszProfileName);
}
#endif // OXCP_NO_SAVESTATE

COXSoundEffectManager* COXSoundEffectOrganizer::CreateManager(CWnd* pWnd)
{
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	COXSoundEffectManager* pManager=NULL;
	try
	{
		pManager=new COXSoundEffectManager(this);
	}
	catch (CMemoryException* pException)
	{
		UNREFERENCED_PARAMETER(pException);
		TRACE(_T("COXSoundEffectOrganizer::CreateManager: failed to allocate memory for COXSoundEffectManager object\n"));
		return NULL;
	}

	ASSERT(pManager!=NULL);
	if(!pManager->Attach(pWnd))
	{
		TRACE(_T("COXSoundEffectOrganizer::CreateManager: failed to attach specified window\n"));
		delete pManager;
		return NULL;
	}

	m_allSoundEffectManagers.SetAt(pWnd,pManager);
	return pManager;
}


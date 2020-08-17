// ==========================================================================
// 							Class Specification : COXUndo
// ==========================================================================

// Header file : COXUndo.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	YES	Abstract class (does not have any objects)
//	NO	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO  Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//		This class is an extension of the CUndo class described in the
//		May 97 Windows Tech Journal article "Undo Voodoo" by Keith Rule.

// Remark:
//		This implementation is somewhat unusual in that both the definition
//      and the implimentation are found in this file. This is intentional.
//      One of the goals of this class is to make the addition of undo/redo
//      require as few changes to an existing project as possible. Keeping 
//      the definition and implimentation in a single file allows this feature
//      to be added to an existing project simply by including this file in
//      stdafx.h.

// Prerequisites (necessary conditions):
//		none

/////////////////////////////////////////////////////////////////////////////
#ifndef __COXUNDO_H__
#define __COXUNDO_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXTEMPL_H__
#include <AfxTempl.h>
#define __AFXTEMPL_H__
#endif


// ==========================================================================
// class : COXUndoState
// Desciption :         
//		Holds undo-state. The undo-state consists of the CMemFile that contains
//      the serialized CDocument data, and any associated text string description.

// Remark:
//		This class is not intended to be used outside of the COXUndo class.


class OX_CLASS_DECL COXUndoState {
// Data members -------------------------------------------------------------
public:
	CMemFile*	m_pFile;
	CString		m_sDesc;

// Member functions ---------------------------------------------------------
	COXUndoState(CMemFile* pFile = NULL, LPCTSTR pszDesc = NULL) 
		: 
		m_pFile(pFile), 
		m_sDesc(pszDesc) 
		{
			;
		}
	virtual ~COXUndoState() 
		{
			if (m_pFile != NULL) 
				delete m_pFile;
		}
};

class OX_CLASS_DECL COXUndo {
// Data members -------------------------------------------------------------
public:
protected:
	CList<COXUndoState*, COXUndoState*>	m_undolist;		
	CList<COXUndoState*, COXUndoState*>	m_redolist;		
	long	m_growsize;		
	long	m_undoLevels;	
	long	m_chkptLevel;
	CString	m_undoLabel;
	CString	m_redoLabel;
	CString	m_undoAccLabel;
	CString	m_redoAccLabel;
	BOOL	m_bUseDesc;

private:

// Member functions ---------------------------------------------------------
public:

	// User accessable members
	COXUndo(long undolevels = 4, long growsize = 32768, BOOL bUseDesc = FALSE);
	// --- In      : undolevels : number of undo/redo levels allowed
	//				 growsize : passed to CMemFile as growsize
	//				 bUseDesc : when FALSE menu resource strings used when menu updated,
	//                          when TRUE menu labels are used with undo/redo state 
	//                          descriptor string
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	BOOL CanUndo();		
	// --- In      : 
	// --- Out     : 
	// --- Returns : Returns true if any undo states are available
	// --- Effect  : 

	BOOL CanRedo();		
	// --- In      : 
	// --- Out     : 
	// --- Returns : Returns true if any redo states are available
	// --- Effect  : 

	void Undo();		
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Restores next undo state from the undo list 

	void Redo();					
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Restores next redo state from the redo list

	void CheckPoint(LPCTSTR pszDescription = NULL);
	// --- In      : pszDescription : optional state description string
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Save the current state on the undo list, 

	void EnableCheckPoint();
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Enables CheckPoint() member after a DisableCheckPoint() call.

	void DisableCheckPoint();
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Disables CheckPoint() until matching EnableCheckPoint() call.

	void OnUpdateUndo(CCmdUI* pCmdUI);
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Updates undo menu item

	void OnUpdateRedo(CCmdUI* pCmdUI);
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Updates redo menu item

	void UndoMenuLabel(LPTSTR undoLabel, LPTSTR undoAccLabel = _T(""));
	// --- In      : undoLabel : label to place on undo menu
	//               undoAccLabel : accelerator label to place on undo menu
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Changes undo menu and undo accelerator labels. These 
	//               values are used by OnUpdateUndo().

	void RedoMenuLabel(LPTSTR undoLabel, LPTSTR undoAccLabel = _T(""));
	// --- In      : redoLabel : label to place on redo menu
	//               redoAccLabel : accelerator label to place on redo menu 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Changes redo menu and redo accelerator labels. These
	//               values are used by OnUpdateRedo().

	// Here are the hooks into the CDocument class
	virtual void Serialize(CArchive& ar) = 0;
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : 

	virtual void DeleteContents() = 0;
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : 

	virtual ~COXUndo();			
	// --- In      : 
	// --- Out     : 
	// --- Returns :
	// --- Effect  : // Destructor -- Cleans up any remain storage

protected:
	void AddUndo(COXUndoState* pState);
	void AddRedo(COXUndoState* pState); 
	void Load(CMemFile* pFile);
	void Store(CMemFile* pFile);
	void ClearRedoList();

private:
};

// ==========================================================================
// 							Class Implementation : COXUndo
// ==========================================================================


// Member functions ---------------------------------------------------------
// public:

inline COXUndo::COXUndo(long undolevels /* = 4 */, long growsize /* = 32768 */, 
	BOOL bUseDesc /* = FALSE */)
	:	
	m_growsize(growsize), m_undoLevels(undolevels),
	m_chkptLevel(0), 
	m_bUseDesc(bUseDesc)
	{
	VERIFY(m_undoLabel.LoadString(IDS_OX_UNDO_LABEL));
	VERIFY(m_undoAccLabel.LoadString(IDS_OX_UNDO_ACCLABEL));
	VERIFY(m_redoLabel.LoadString(IDS_OX_UNDO_REDO));
	VERIFY(m_redoAccLabel.LoadString(IDS_OX_UNDO_REDOACCLABEL));
	} 

inline BOOL COXUndo::CanUndo() 
	{
	return (1 < m_undolist.GetCount()); 
	}

inline BOOL COXUndo::CanRedo() 
	{
	return (0 < m_redolist.GetCount()); 
	}

inline void COXUndo::CheckPoint(LPCTSTR pszDescription /* = NULL */) 
	{
	if (m_chkptLevel <= 0) 
		{
		CMemFile* pFile = new CMemFile(m_growsize);
		COXUndoState* pState = new COXUndoState(pFile, pszDescription);
		Store(pState->m_pFile);
		AddUndo(pState);
		ClearRedoList();
		}
	}

inline void COXUndo::EnableCheckPoint()
	{
	if (m_chkptLevel > 0) 
		m_chkptLevel--;
	}


inline void COXUndo::DisableCheckPoint()
	{
	m_chkptLevel++;
	}

inline void COXUndo::Undo() 
	{
	if (CanUndo()) 
		{
		COXUndoState *pState = m_undolist.GetHead();
		m_undolist.RemoveHead();
		AddRedo(pState);
		pState = m_undolist.GetHead();
		Load(pState->m_pFile);
		}
	}

inline void COXUndo::Redo() 
	{
	if (CanRedo()) 
		{
		COXUndoState *pState = m_redolist.GetHead() ;
		m_redolist.RemoveHead();
		AddUndo(pState);
		Load(pState->m_pFile);
		}
	}

inline void COXUndo::UndoMenuLabel(LPTSTR undoLabel, LPTSTR undoAccLabel)
	{
	m_undoLabel = undoLabel;
	m_undoAccLabel = undoAccLabel;
	}

inline void COXUndo::RedoMenuLabel(LPTSTR redoLabel, LPTSTR redoAccLabel)
	{
	m_redoLabel = redoLabel;
	m_redoAccLabel = redoAccLabel;
	}

inline void COXUndo::OnUpdateUndo(CCmdUI* pCmdUI)
	{
	pCmdUI->Enable(CanUndo());
	if (m_bUseDesc) 
		{
		if (CanUndo()) 
			{
			COXUndoState *pState = m_undolist.GetHead();
			pCmdUI->SetText(m_undoLabel + _T(" ") + pState->m_sDesc + _T("\t") + m_undoAccLabel);
			} 
		else 
			{
			pCmdUI->SetText(m_undoLabel + _T("\t") + m_undoAccLabel);
			}
		}
	}

inline void COXUndo::OnUpdateRedo(CCmdUI* pCmdUI)
	{
	pCmdUI->Enable(CanRedo());
	if (m_bUseDesc) 
		{
		if (CanRedo()) 
			{
			COXUndoState *pState = m_redolist.GetHead();
			pCmdUI->SetText(m_redoLabel + _T(" ") + pState->m_sDesc + _T("\t") + m_redoAccLabel);
			} 
		else 
			{
			pCmdUI->SetText(m_redoLabel + _T("\t") + m_redoAccLabel);
			}
		}
	}

inline COXUndo::~COXUndo() 
	{
	POSITION pos = m_undolist.GetHeadPosition(); 
	COXUndoState* pState = NULL;
	while(pos) 
		{
		pState = m_undolist.GetNext(pos);
		// ... pState may be NULL
		delete pState;
		}
	m_undolist.RemoveAll();	

	ClearRedoList();
	}

// protected:
inline void COXUndo::AddUndo(COXUndoState* pState) 
	{
	if (m_undolist.GetCount() > m_undoLevels) 
		{
		COXUndoState* pTmp = m_undolist.RemoveTail();
		// ... pTmp may be NULL
		delete pTmp;
		}
	m_undolist.AddHead(pState);
	}

inline void COXUndo::AddRedo(COXUndoState* pState) 
	{
	m_redolist.AddHead(pState);
	}

inline void COXUndo::Load(CMemFile* pFile) 
	{
	DeleteContents(); 
	pFile->SeekToBegin();
	CArchive ar(pFile, CArchive::load);
	Serialize(ar); 
	ar.Close();
	}

inline void COXUndo::Store(CMemFile* pFile) 
	{
	pFile->SeekToBegin();
	CArchive ar(pFile, CArchive::store);
	Serialize(ar); 
	ar.Close();
	}

inline void COXUndo::ClearRedoList()
	{
	POSITION pos = m_redolist.GetHeadPosition(); 
	COXUndoState* pState = NULL;
	while (pos) 
		{
		pState = m_redolist.GetNext(pos);
		delete pState;
		}
	m_redolist.RemoveAll();	
	}


#endif // __COXUNDO_H__

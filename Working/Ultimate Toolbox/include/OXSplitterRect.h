// ===================================================================================
// 					Class Specification : COXSplitterRect
// ===================================================================================

// Header file : OXSplitterRect.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is � 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
//	simple representation of splitter rectangles

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SPLITTERRECT_H__
#define __SPLITTERRECT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#define SPLITTER_VERT   1
#define SPLITTER_HORZ   2


class OX_CLASS_DECL COXSplitterRect : public CObject
{
// Data members -------------------------------------------------------------
public:

    CRect   m_rect;					// rectangle
    TCHAR   m_type;					// Vertical or Horizontal
    int     m_nPos;					// position at which it was inserted
									// ie points to pane immediately following it.

// Member functions ---------------------------------------------------------
public:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state
	COXSplitterRect(int type, const RECT& rect);

	// --- In  : pDC : The DC where the Splitter will be drawn
	// --- Out : 
	// --- Returns :
	// --- Effect : draws a splitter UI
	void Draw(CDC* pDC);

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
	virtual ~COXSplitterRect();

protected:

private :

};

#endif  // __SPLITTERRECT_H__

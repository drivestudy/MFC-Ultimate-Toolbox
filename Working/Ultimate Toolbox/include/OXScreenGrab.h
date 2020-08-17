// ==========================================================================
// 					Class Specification : COXScreenGrabber
// ==========================================================================

// Header file : oxscreengrab.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
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
//	This class can be used to grab a portion of the screen, a window or the whole
//	desktop

// Remark:
//		***

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSCREENGRAB_H__
#define __OXSCREENGRAB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "oxdib.h"			// file header for the DIB member


class OX_CLASS_DECL COXScreenGrabber : public CObject
{
DECLARE_DYNAMIC(COXScreenGrabber)
// Data members -------------------------------------------------------------
public:

protected:
	COXDIB GrabDIB;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXScreenGrabber();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	BOOL GrabFullWindow(CWnd* pWnd = NULL);
	// --- In  :  pWnd : the window to grab. If NULL the active window will be grabbed
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Grabs the entire window area
	//				If bActive and pWnd are both specified, the grab
	//				will be performed for the pWnd.

	BOOL GrabClientWindow(CWnd* pWnd = NULL);
	// --- In  : pWnd : the window which client area to grab.
	//					If NULL the active window will be grabbed
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Grabs the window client area
	//				If bActive and pWnd are both specified, the grab
	//				will be performed for the pWnd.

	BOOL GrabRectangle(CRect SrcRect);
	// --- In  :  SrcRect : the rect in screen coordinates
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Grabs a rectangular area from the screen

	BOOL GrabTracker(CWnd* pWndFromStartPoint, CPoint StartPoint, CWnd* pWndForRect);
	// --- In  :  pWndFromStartPoint : the window whose client coordinate system defines
	//								   the coordinates of pStartpoint
	//			  StartPoint : start point of tracking rect in clientcoordinates of
	//						   pWndFromStartPoint
	//			  pWndForRect : the window that contains the tracking rectangle
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Grabs a rectangular area starting with a point in the window
	//				pWndFromPoint and lying in the window pWndForRect.  To indicate
	//				the grabbing a visual tracker is drawn.  This function will
	//				normally be called from within a WM_LBUTTONDOWN message handler

	COXDIB* GetGrabDIB();
	// --- In  :  
	// --- Out : 
	// --- Returns : returns the grabbed DIB
	// --- Effect : 

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXScreenGrabber();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:
	BOOL PrepareWindow(BOOL bClient, CRect& ScrRect, CWnd* pWnd);

private:
                   
};

#endif	// __OXSCREENGRAB_H__

// ==========================================================================

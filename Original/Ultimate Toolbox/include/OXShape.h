// ==========================================================================
// 					Class Specification : COXShape
// ==========================================================================

// Header file : OXShape.h

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
//	NO	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	Defines base c++ class to encapsulate the SetWindowsRgn() Windows 95 and
//	Windows NT 3.51 API.  The methods provided use the MFC CRgn class to create
//	and manipulate GDI region objects then call SetWindowsRgn() to set 	the window's
//	visible region.

// Remark:
//	COXShape must be combined with an object that contains a window handle (HWND).
//	This window handle must be returned through the method GetHWND() that is
//	implemented in the combined class.
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SHAPE_H__
#define __SHAPE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxtempl.h>


// Maximum line segments that CalcLines() can create.
// The default of 100 should be more than enough.
#define MAX_SHAPELINES	100

class OX_CLASS_DECL COXShape
{
// Data members -------------------------------------------------------------
public:

protected:
	// The points of the Region
	CArray<CPoint, CPoint> m_ShapePointsArray;

private:
	// The visible window region
	CRgn	m_rgnWnd;

	// The window handle (retrieved by GetHWND())
 	HWND	m_hShapeWnd;

// Member functions ---------------------------------------------------------
public:
	COXShape();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	const CArray<CPoint, CPoint>* GetShapePoints() const; 
	// --- In  :
	// --- Out : 
	// --- Returns : the points the shape was constructed with
	// --- Effect : 

	virtual HWND GetHWND() = 0;
	// --- In  :
	// --- Out : 
	// --- Returns : a window handle
	// --- Effect : Returns window handle (must be implemented in combined class)

	void SetPolyShape(LPPOINT lpPoints, int nPoints);
	// --- In  : lpPoints : array of points
	//			 nPoints : number of points in the array lpPoints
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to the specified polygon

	void SetRoundRectShape(LPRECT lpRect = NULL, LPPOINT lpptCorners = NULL);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the shape
	//			 lpptCorners : pointer to a point.
	//							x must specify the width of the ellipse used to create the rounded corners.
	//							y must specify the height of the ellipse used to create the rounded corners.
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to a round cornered rectangle

	void SetEllipseShape(LPRECT lpRect = NULL);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the ellipse shape
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to a circle

	void SetStarShape(LPRECT lpRect = NULL, double dbNumberOfPoints = 5.0f, 
							double dbInnerRadius = 2.0f, double dbRotation = 0.0f);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the star shape
	//			 dbNumberOfPoints : the number of points
	//			 dbInnerRadius : depth of each point
	//			 dbRotation : rotation of each point
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to a star

	void SetPentagonShape(LPRECT lpRect = NULL, double dbRotation = 0.0f);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the pentagon shape
	//			 dbRotation : rotation of each point
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to a pentagon

	void SetHexagonShape(LPRECT lpRect = NULL, double dbRotation = 0.0f);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the hexagon shape
	//			 dbRotation : rotation of each point
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to a hexagon

	void SetTriangleShape(LPRECT lpRect = NULL, double dbRotation = 0.0f);
	// --- In  : lpRect : pointer to the rect containing ccordinates for the triangle shape
	//			 dbRotation : rotation of each point
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the window's shape to an equilateral triangle

	virtual ~COXShape();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:

private:
	// Multi-purpose routine to create line-segment based shapes
	void CalcLines(RECT& rect, double dbNumberOfPoints, 
						double dbInnerRadius, double dbRotation);

	// Routine to get the client rectangle or passed in argument
	CRect& GetShapeRect(LPRECT lpRect);

};

/////////////////////////////////////////////////////////////////////////////

// Helper macro to declare shape custom control class
#define DECLARE_SHAPECLASS(theClass) \
	private:\
    static BOOL m_bRegistered; \
	static BOOL Register(); \
	WNDPROC* theClass::GetSuperWndProcAddr()\
		{\
		static WNDPROC NEAR pfnSuper = NULL;\
		return &pfnSuper;\
		}

// Helper macro to implement shape custom control class
#define IMPLEMENT_SHAPECLASS(theClass) \
	extern "C" \
	{\
	LRESULT CALLBACK Proc##theClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);\
	}\
	BOOL theClass::m_bRegistered = Register();\
    BOOL theClass::Register() \
		{ \
		WNDCLASS wc; \
		wc.style = CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW; \
		wc.lpfnWndProc = Proc##theClass; \
		wc.cbClsExtra = 0; \
		wc.cbWndExtra = 0; \
		wc.hInstance = NULL; \
		wc.hIcon = NULL; \
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW); \
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); \
		wc.lpszMenuName = NULL; \
		wc.lpszClassName = _T(#theClass); \
		if (!::RegisterClass(&wc)) \
			{ \
			ASSERT(FALSE); \
			return FALSE; \
			} \
		return TRUE;\
		} \
	LRESULT CALLBACK Proc##theClass(HWND hWnd, UINT uiMsg, \
					WPARAM wParam, LPARAM lParam)\
		{\
		switch(uiMsg) \
			{\
			case WM_NCCREATE: \
					{\
					theClass *pClass = new theClass;\
					ASSERT(pClass);\
					BOOL bOkay = pClass->SubclassWindow(hWnd);\
					ASSERT(bOkay);\
					return bOkay;\
					} \
				break;\
			default:\
				return ::DefWindowProc(hWnd, uiMsg, wParam, lParam);\
			}\
		}

#endif
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// 					Class Implementation : COXShape
// ==========================================================================

// Source file : OXShape.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "oxshape.h" 

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor
COXShape::COXShape()
{
}

// Destructor
COXShape::~COXShape()
{
}

const CArray<CPoint, CPoint>* COXShape::GetShapePoints() const
{
	return &m_ShapePointsArray;
}

// Private method to get the size of the visible region
CRect& COXShape::GetShapeRect(LPRECT lpRect)
	{
	static CRect rect;
	
	if(lpRect == NULL)
		::GetClientRect(m_hShapeWnd, &rect);
	else
		rect = *lpRect;
	
	return rect;
	}

// Multi-purpose method to calculate shapes based on line segments.
// This method is used to generate the various line based shapes
// in COXShape such as stars, hexagons and triangles.  It can easily be
// extended by experimenting with various combinations fo the point,
// radius and rotation parameters.
//
// (thanks to Michael Hatton for this method)
void COXShape::CalcLines(RECT& rect, double dbNumberOfPoints, 
						 double dbInnerRadius, double dbRotation)
	{
#define PI 3.14159
	
	short x = 0;
	short y = 0;
	
	// Declare line segment work area
	POINT aptKey[MAX_SHAPELINES];
	
	// Normalize the rotation factor
	dbRotation += 1.5f;
	
	// Initialize work variables
	int xCenter = rect.right / 2;
	int yCenter = rect.bottom / 2;
	double dbRadius = xCenter;
	double dbRadians = PI / dbNumberOfPoints;
	
	// Iterate through each inner and outer points of the shape
	for(int nSide = 0; nSide < (int)(2.0f * dbNumberOfPoints); nSide++ )
		{
		// Calculate the horizontal (x) and vertical (y) location
		x = (short)(cos(nSide * dbRadians + dbRotation) * dbRadius);
		y = (short)(sin(nSide * dbRadians + dbRotation) * dbRadius);
		
		// If this is an inner point then base it off of the inner radius
		if(nSide % 2)
			{
			x = (short) (x / dbInnerRadius);
			y = (short) (y / dbInnerRadius);
			}
		
		// Save the point information
		aptKey[nSide].x = xCenter + x;
		aptKey[nSide].y = yCenter - y;
		}
	
	// Finished calculating the shape, lets set the visible region to it
	SetPolyShape(aptKey, (int)(2.0f * dbNumberOfPoints));
	}

// Sets the visible region a polygon
void COXShape::SetPolyShape(LPPOINT lpPoints, int nPoints)
	{
	// Do a quick sanity check
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	ASSERT(lpPoints != NULL);
	ASSERT(nPoints != 0);
	ASSERT(AfxIsValidAddress(lpPoints, nPoints * sizeof(POINT), FALSE)); 
	
	// Create a polygon region
	SetWindowRgn(m_hShapeWnd, NULL, FALSE);
	m_rgnWnd.DeleteObject();
	m_rgnWnd.CreatePolygonRgn(lpPoints, nPoints, ALTERNATE);

	// copy the points of the region to the points array
	m_ShapePointsArray.RemoveAll();
	m_ShapePointsArray.SetSize(nPoints);
	for (int i = 0; i < nPoints; i++)
		m_ShapePointsArray.SetAt(i, CPoint(lpPoints[i]));
	
	// Set the window's visible region to match it, then repaint it
	SetWindowRgn(m_hShapeWnd, m_rgnWnd, TRUE);
	}

// Sets the visible region to a rounded rectangle
void COXShape::SetRoundRectShape(LPRECT lpRect, LPPOINT lpptCorners)
	{
	// Do a quick sanity check
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	// Get the shape's rectangle
	CRect rect = GetShapeRect(lpRect);
	
	// If NULL was passed for the rounded corner size then just use 0
	// this will make a normal rectangle
	POINT ptCorners;
	if(lpptCorners == NULL)
		ptCorners.x = ptCorners.y = 0;
	else ptCorners = *lpptCorners;
	
	// Create a rounded rectangle region
	SetWindowRgn(m_hShapeWnd, NULL, FALSE);
	m_rgnWnd.DeleteObject();
	m_rgnWnd.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom,
		ptCorners.x,ptCorners.y);
	
	// Set the window's visible region to match it, the repaint it
	SetWindowRgn(m_hShapeWnd, m_rgnWnd, TRUE);
	}

// Sets the visible region to an ellipse
void COXShape::SetEllipseShape(LPRECT lpRect)
	{
	// Do a quick sanity check
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	CRect rect = GetShapeRect(lpRect);
	
	// Create a rounded rectangle region
	SetWindowRgn(m_hShapeWnd, NULL, FALSE);
	m_rgnWnd.DeleteObject();
	m_rgnWnd.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	
	// Set the window's visible region to match it, the repaint it
	SetWindowRgn(m_hShapeWnd, m_rgnWnd, TRUE);
	}

// Sets the visible region to a star -- the number of points, depth
// of each point and rotation can be specified in floating point
// numbers
void COXShape::SetStarShape(LPRECT lpRect, double dbNumberOfPoints, 
							double dbInnerRadius, double dbRotation)
	{
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	// Get the shape's rectangle
	CRect rect = GetShapeRect(lpRect);
	
	CalcLines(rect, dbNumberOfPoints, dbInnerRadius, dbRotation);
	}

// Sets the visible region to a pentagon -- the rotation can be specified
void COXShape::SetPentagonShape(LPRECT lpRect, double dbRotation)
	{
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	// Get the shape's rectangle
	CRect rect = GetShapeRect(lpRect);
	
	CalcLines(rect, 2.5f, 1.0f, dbRotation);
	}

// Sets the visible region to a hexagon -- the rotation can be specified
void COXShape::SetHexagonShape(LPRECT lpRect, double dbRotation)
	{
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	// Get the shape's rectangle
	CRect rect = GetShapeRect(lpRect);
	
	CalcLines(rect, 4.0f, 1.0f, dbRotation);
	}

// Sets the visible region to a triangle - the rotation can be specified
void COXShape::SetTriangleShape(LPRECT lpRect, double dbRotation)
	{
	m_hShapeWnd = GetHWND();
	ASSERT(m_hShapeWnd != NULL);
	
	// Get the shape's rectangle
	CRect rect = GetShapeRect(lpRect);
	
	CalcLines(rect, 1.5f, 1.0f, dbRotation);
	}


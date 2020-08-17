// ==========================================================================
// 					Class Specification : COXIteratorRegistryItem
// ==========================================================================

// Header file : OXIteratorRegistryItem.h

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
//	YES	Derived from COXRegistryItem

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class can be used to iterate a values and subkeys.
//		- You can start by specifying the start key
//		   (e.g. \Software\Dundas\).  You can assign this to a 
//         COXIteratorRegistryItem object.
//		- Now you can iterate by using Start(), Next() several times and End().
//		  Instead of using Next you can also use operator++.
//		  During iteration the COXIteratorRegistryItem will change value until
//		   the end is reached (and the object will be empty)
//		- When starting you can specify whether values and/or subkeys
//		   should be part of the result.  You can also specify whether to search recursively

// Example :
//		COXIteratorRegistryItem iterItem("\\Software\\Dundas\\");
//		iterItem.Start(TRUE, FALSE, FALSE, TRUE);
//		while(!iterItem.IsEmpty())
//			{
//			TRACE1("Reg value : %s\n", (LPCTSTR)iterItem);
//			++iterItem;
//			}
//		iterItem.End();


// Remark:
//	When using the copy constructor or assignment operator, note that the search 
//	 specific data is NOT copied, only the data from the base class COXUNC.
//	This is because handles cannot be duplicated

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXITERATORREGISTRYITEM_H__
#define __OXITERATORREGISTRYITEM_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXRegistryItem.h"

#ifndef __AFXTEMPL_H__
#error Make sure <AfxTempl.h> is added to your StdAfx.h
#endif


class OX_CLASS_DECL COXIteratorRegistryItem : public COXRegistryItem
{
DECLARE_DYNAMIC(COXIteratorRegistryItem);

// Data members -------------------------------------------------------------
public:

protected:
	struct COXSearchData
		{
		COXRegistryItem m_regItem;
		int m_nValueIndex;
		int m_nSubkeyIndex;
		int m_nRecursiveSubkeyIndex;

		COXSearchData()
			:
			m_regItem(),
			m_nValueIndex(-1),
			m_nSubkeyIndex(-1),
			m_nRecursiveSubkeyIndex(-1)
			{
			}
		};
	CList<COXSearchData*, COXSearchData*> m_searchStack;

	BOOL m_bIncludeValues;
	BOOL m_bIncludeSubkeys;
	BOOL m_bReversed;
	BOOL m_bRecursive;
	BOOL m_bDepthFirst;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXIteratorRegistryItem(LPCTSTR pszFullRegistryItem = NULL);
	// --- In  : pszFullRegistryItem : Initial full registry item
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	COXIteratorRegistryItem(const COXRegistryItem& registryItem);
	COXIteratorRegistryItem(const COXIteratorRegistryItem& registryItem);
	// --- In  : registryItem : source object
	// --- Out : 
	// --- Returns :
	// --- Effect : Copy constructor
	//				Note that possible open handles are not copied

	COXIteratorRegistryItem& operator=(const COXRegistryItem& registryItem);
	COXIteratorRegistryItem& operator=(const COXIteratorRegistryItem& registryItem);
	// --- In  : registryItem : source object
	// --- Out : 
	// --- Returns :
	// --- Effect : Assignment operator
	//				Note that possible open handles are not copied

	void Empty();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clears the entire objects (and closes a possible open key)

	BOOL Start(BOOL bIncludeValues = TRUE, BOOL bIncludeSubkeys = FALSE, 
		BOOL bReversed = FALSE, BOOL bRecursive = FALSE, BOOL bDepthFirst = FALSE);
	// --- In  : bIncludeValues : Whether to include values in the search result
	//			 bIncludeSubkeys : Whether to include subkeys in the search result
	//			 bReversed : Whether high indexed items should be handled before
	//				low indexed ones (TRUE) or not (FALSE)
	//			 bRecursive : Whether subkeys should be searched recursively
	//			 bDepthFirst : Whether a recursive search should use depth-first (TRUE)
	//						   or breadth-first (FALSE) traversal
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Starts the iteration.  This object will get the new value
	//				When this object is empty, the end of the iteration has been reached

	BOOL Next();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Iterate to the next registry item.  This object will get the new value
	//				When this object is empty, the end of the iteration has been reached

	void End();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Ends a possible ongoing iteration
	//				This will free used resources

	COXIteratorRegistryItem& operator++();       
	COXIteratorRegistryItem operator++(int);     
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Prefix/Postfix  increment operator (same as Next())
	//				Iterate to the next registry item.  This object will get the new value
	//				When this object is empty, the end of the iteration has been reached

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXIteratorRegistryItem();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	COXSearchData* AddNewSearchData(const COXRegistryItem& registryItem,
		LPCTSTR pszSubkeyName = NULL, BOOL bDepthFirst = FALSE);

private:
                   
};

#endif // __OXITERATORREGISTRYITEM_H__
// ==========================================================================

// =============================================================================
// 							Class Specification : COXUserTool
// =============================================================================
//
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
//
// /////////////////////////////////////////////////////////////////////////////
//
// Properties:
//
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject
//	NO	Is a Cwnd.
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)
//	YES	Persistent objects (saveable on disk)
//	NO	Uses exceptions
//
////////////////////////////////////////////////////////////////////////////////
//
// Desciption :
//		This class provides a way to implement a special menu in you apps where
//		users can insert their  own tools.  Those tools can then be executed from within
//		your app.  (Similar to the Tools menu in the  Visual C++ IDE's).
//
// Remark:
//
// Prerequisites (necessary conditions):
//
// Limited edition:
//
/////////////////////////////////////////////////////////////////////////////

#if !defined( _USERTOOL_H_ )
#define _USERTOOL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXUserTool : public CObject
{
DECLARE_SERIAL( COXUserTool );

// Data members -------------------------------------------------------------
public:

protected:
 	CString m_strMenuText;	
	CString m_strCommand;	
	CString m_strArgs;		
	CString m_strDirectory;	
	UINT m_uiShowFlag;		

private:

// Member functions ---------------------------------------------------------
public:
	COXUserTool();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state
	COXUserTool( const COXUserTool& rhs );


	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Read or write to archive
	virtual void Serialize( CArchive& archive );


	// --- In  : pReplacements : list of arguments to replace (see Effect for details)
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : This method allows execution of the given tool with
	//				a list of string replacements to be performed.  The
	//				replacements are applied to the command, argument, and
	//				initial directory.  To replace some string XXX with YYY, 
	//				add "XXX=YYY" to the string list.
	BOOL Execute( CStringList* pReplacements ) const;


	// --- In  : rhs : Usertool to assign from
	// --- Out : 
	// --- Returns :
	// --- Effect : Assigment operator
	COXUserTool& operator=( const COXUserTool& rhs );


	// --- In  : 
	// --- Out : 
	// --- Returns : duplicated object
	// --- Effect : Duplicator of object
	COXUserTool* Clone() const;


	// Basic access functions for all members.
	//

	LPCTSTR GetMenuText() const
		{ return m_strMenuText; }

	LPCTSTR SetMenuText( LPCTSTR str )
		{ return m_strMenuText = str; }

	LPCTSTR GetCommand() const
		{ return m_strCommand; }

	LPCTSTR SetCommand( LPCTSTR str )
		{ return m_strCommand = str; }

	LPCTSTR GetArgs() const
		{ return m_strArgs; }

	LPCTSTR SetArgs( LPCTSTR str )
		{ return m_strArgs = str; }

	LPCTSTR GetDirectory() const
		{ return m_strDirectory; }

	LPCTSTR SetDirectory( LPCTSTR str )
		{ return m_strDirectory = str; }

	UINT GetShowFlag() const
		{ return m_uiShowFlag; }

	UINT SetShowFlag( UINT i )
		{ return m_uiShowFlag = i; }

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
	virtual ~COXUserTool();

protected:
	BOOL ParseReplacement( LPCTSTR p, CString& strKey, 
						   CString& strData ) const;

private:
};

// Helper functions for integrating user tools into a given application.
//

// --- In  : pcszFilename : FileName
// --- Out : a : will contain all read in COXUserTool's
// --- Returns : Succeeded or not
// --- Effect : Reads in a diskfile and stores all Usertools in an Object array
OX_API_DECL BOOL ReadUserToolFile( LPCTSTR pcszFilename, CObArray& a );


// --- In  : pcszFilename : FileName
//			 a : Contains allocated COXUserTool's
// --- Out : 
// --- Returns : Succeeded or not
// --- Effect : Writes the contents of the object array(the usertools) to a diskfile
OX_API_DECL BOOL WriteUserToolFile( LPCTSTR pcszFilename, CObArray& a );


// --- In  : pMenu : the Menu where the Usertool menuitems will be located
//			 uiFirstID : the ID after which all UserTools in a will be appended
//			 a : Contains allocated COXUserTool's
// --- Out : 
// --- Returns : Succeeded or not
// --- Effect : Appends new Usertools to an existing Menu
OX_API_DECL BOOL AppendUserTools( CMenu* pMenu, UINT uiFirstID, const CObArray& a );


// --- In  : a : Contains allocated COXUserTool's
// --- Out : 
// --- Returns :
// --- Effect : Deletes all allocated COXUserTool's in a and removes the items in the array.
OX_API_DECL void EmptyUserToolArray( CObArray& a );

#endif	// _USERTOOL_H_
// ==========================================================================

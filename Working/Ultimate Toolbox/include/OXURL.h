// ==========================================================================
// 					Class Specification : COXURL
// ==========================================================================

// Header file : OXURL.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	An URL is a physical address of objects (file etc.) which are retrievable 
//	 using protocols deployed on the Internet.
//	An url has the follwing format :
//		protocol://server:port/path
//	which is divided into the follwong parts :
//		Protocol : protocol://
//		Port : port
//		UNC : server/path
//	The port with its leading colon is optional (protocol://server/path)
//	The URL may be optionaly prefixed by the string "URL:" (4 characters).

// Remark:
//	A balance has been made between usability and exact implementation of the URL
//	 specification.  Certain parts which are valid in a URL are not supported
//	 by this COXURL class.  Some of them are :
//		- URL protocols which do not use a double slash "//", e.g. "news:"
//		- URL that uses protocol specific extensions e.g. FTP uses a 
//		  login username and password, type-code etc.
//		- URL that uses suffixes such as fragment ID ("#") or search part ("?")
//		- Relative (partial) URL
//	Some of these items made be supported in futur releases of the COXURL class

//	The optional URL prefix "URL:" is allowed but is automatically removed

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXURL_H__
#define __OXURL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXUNC.h"


class OX_CLASS_DECL COXURL
{
// Data members -------------------------------------------------------------
protected:
	// ... The full URL string must be the first data member
	CString m_sURL;

public:
	static const UINT m_nUnknownPort;
	// --- The value of the port when it is not specified

protected:
	BOOL	m_bMainBuilt;

	CString m_sProtocol;
	UINT	m_nPort;
	COXUNC	m_UNC;
	BOOL	m_bPartsBuilt;

	static const LPCTSTR m_pszURLPrePrefix;
	static const LPCTSTR m_pszProtocolSuffix;
	static const LPCTSTR m_pszFileProtocol;
	static const LPCTSTR m_pszPortPrefix;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXURL(LPCTSTR pszURL = NULL);
	COXURL(LPCTSTR pszProtocol, int nPort, LPCTSTR pszUNC);
	COXURL(LPCTSTR pszProtocol, int nPort, COXUNC UNC);
	// --- In  : pszURL : Full URL specification
	//			 pszProtocol : Protocol specification
	//			 nPort : The TCP/IP port (or m_nUnknownPort)
	//			 pszUNC / UNC : UNC specification
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of the object


	COXURL(const COXURL& URL);
	// --- In  : URL : Source URL
	// --- Out : 
	// --- Returns :
	// --- Effect : Copy constructor

	COXURL& operator=(const COXURL& URL);
	COXURL& operator=(LPCTSTR pszURL);
	// --- In  : URL : Source URL
	// --- Out : 
	// --- Returns : This object
	// --- Effect : Assignment operator

	operator const CString() const;
	const CString  Protocol() const;
	const UINT Port() const;
	const COXUNC UNC() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The complete URL or the requested part
	// --- Effect : Retrieves the requested value (cannot be changed)

	operator CString&();
	CString& Protocol();
	UINT& Port();
	COXUNC& UNC();
	// --- In  : 
	// --- Out : 
	// --- Returns : A reference to the complete URL or the requested part
	// --- Effect : Retrieves the requested value 
	//				You can change the value by assigning a new value
	//				Do not store this reference for later use !

	void Empty();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clears the contents

	COXURL StandardForm(BOOL bMakeLower = TRUE) const;
	// --- In  : 
	// --- Out : 
	// --- Returns : A standard form of this URL
	//				 The standard form uses all backslashes
	//				 All characters are converted to lower case letters
	//				 The standard form is usefull for comparisons
	// --- Effect : 

#ifdef _DEBUG
#endif

	virtual ~COXURL();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void BuildMain();
	void BuildParts();
	void AdjustMain();
	void AdjustParts();
	static void Trim(CString& sText);
	void SetMainBuilt();
	void SetPartsBuilt();
	void DestroyMain();
	void DestroyParts();
	BOOL IsMainBuilt() const;
	BOOL ArePartsBuilt() const;

private:
};

// Include inline functions
#include "OXURL.inl"

#endif // __OXURL_H__
// ==========================================================================

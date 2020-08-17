// ==========================================================================
// 					Class Specification : COXScript
// ==========================================================================

// Header file : oxscript.h

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
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	base class for handling CGI scripts called from a WEB server
//   

// Remark:
//		***
// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXScript : public CObject
{
// Data members -------------------------------------------------------------
public:

protected:
	TCHAR InputBuffer[4096];

private:

// Member functions ---------------------------------------------------------
public:
	COXScript();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

    int DoMain(int argc, char** argv);
	// --- In  : argc : number of commandline arguments
	//			 argv : the commandline arguments
	// --- Out : 
	// --- Returns : exit code of the program
	// --- Effect : Executes the main loop

    // common attributes to get
    LPCTSTR GetPathInfo();
	// --- In  :
	// --- Out : 
	// --- Returns : the path
	// --- Effect : get path part

    // not so common attributes to get
    LPCTSTR GetScriptName();
	// --- In  :
	// --- Out : 
	// --- Returns : the name of the script
	// --- Effect : get script name

    LPCTSTR GetAccepted();  
	// --- In  :
	// --- Out : 
	// --- Returns : list of accepted formats (HTTP_ACCEPT)
	// --- Effect : 

    LPCTSTR GetRemoteAddress();
	// --- In  :
	// --- Out : 
	// --- Returns : the IP number of the remote computer
	// --- Effect : get remote IP number

    // Client Info
    LPCTSTR GetClientInfo(LPCTSTR pszPart);
  	// --- In  : pszPart
	// --- Out : 
	// --- Returns : the client info
	// --- Effect : get the client info
    // 				such as ACCEPT or USER_AGENT or REFERER

    // Server info
    LPCTSTR GetServerName();
	// --- In  :
	// --- Out : 
	// --- Returns : the name of the server the CGI is running on
	// --- Effect : get name of the server

    int GetServerPort();
	// --- In  :
	// --- Out : 
	// --- Returns : the port the server is using
	// --- Effect : get the port

    LPCTSTR GetServerProtocol();
 	// --- In  :
	// --- Out : 
	// --- Returns : the protocol the server is using
	// --- Effect : get protocol

   LPCTSTR GetServerSoftware();
	// --- In  :
	// --- Out : 
	// --- Returns : the CGI software the server is using
	// --- Effect : get CGI software

   	virtual ~COXScript();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
    
protected:
    virtual void OnGet(LPCTSTR pszParam) = 0;
    virtual void OnPost(LPCTSTR pszType, LPCTSTR pszData) = 0;
	virtual void OnRunCommandLine(int argc, char** argv) = 0;

	static BOOL Extract(CString& strRet, LPCTSTR pszData, int iArg, LPCTSTR pszLabel);
	static BOOL ExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep);
	static BOOL FindAndReplace(LPCTSTR pszcFind, LPCTSTR pszcReplace, LPCTSTR pszcSource, LPTSTR& pszDestination);
	
	// The following function does not do UUDecoding.  It has been named improperly.  However,
	// to save legacy code that uses this function we will not rename it but simply provide this
	// information to users who may wish to change it.
	//
	// What this function does is simply parsing out the query sent by the browser to the CGI
	// script
	//
	// ex: Jeff%20Scott%20Webmaster
		//  would be converted to 
		//  Jeff Scott Webmaster  
		//  through this function
	// A recommended function name is 
	//               ParseQuery(LPCTSTR pszcSource, LPTSTR& pszDestination);
	// This is a little more representative of its function since it is parseing out the 
	// query that was sent from the user's browser to the script.
	static BOOL UUDecode(LPCTSTR pszcSource, LPTSTR& pszDestination);


};

#endif	// __SCRIPT_H__
// ==========================================================================


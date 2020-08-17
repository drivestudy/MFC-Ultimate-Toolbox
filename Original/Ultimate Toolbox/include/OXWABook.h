// //////////////////////////////////////////////////////////////////////////
// OXWABook.h: interface for the COXWABook class.
// //////////////////////////////////////////////////////////////////////////

// Version: 9.3

// Desciption :         
//
// This class incapsulated internet address book managment.
// The internet address book is address book that we usually use 
// with Outlook or Outlook Express programms as well as another
// mailer programms. This address book represented in Windows 
// by wab32.dll. The dll exposed it functionality with COM interfaces,
// so there is nothing to worry about implementation of this dll.
// Actually we can have access to 3 interfaces for sure:
// IAddrBook, IWABObject, IABContainer (implements personal address book)
// These interfaces are not described in details, but some information 
// we can get is in MSDN in shell API refernce. Consider to read
// descriptions of MAPI implementation of the functions because
// it provides more info and is more clearly.
//
// The class allows browse for users, select users in predefined
// edit boxes, create, delete and edit users and users' properties.
//
// To use the class You first of all must call function Init() and 
// make sure it is succeed. Otherwise you will not be able to work with the class.
// The COXWABook class uses 2 helper classes. 
// One of them is COXMailUser class that incapsulated user properties and operations
// with these properties. Another one, COXPropValue has been created for
// simplify use of SPropValue structure.
//
// To use this class make sure you have added "Edkguid.lib"
// in Project->Settings->Link->Object/Library modules
//	
/////////////////////////////////////////////////////////////////////////////


#if !defined(_OXWABOOK_H__)
#define _OXWABOOK_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mapiguid.h>
#include <wab.h>
#include "oxDllext.h"
 
#define OX_REGISTRY_WAB_LOCATION _T("SOFTWARE\\Microsoft\\WAB\\DLLPath")
#define OX_WAB_NAME _T("wab32.dll")
#define OX_WAB_OPEN "WABOpen"

typedef struct sOXWABOptions
{

	ULONG ulType1;
	ULONG ulType2;
	ULONG ulType3;
	CString sType1;
	CString sType2;
	CString sType3;
	CString sCaption;
	CString sNewEntry;
	CString sDest;

	sOXWABOptions(): ulType1(MAPI_TO),
		ulType2(MAPI_CC), ulType3(MAPI_BCC){}

}tOXWABOptions;

//////////////////////////////////////////////////////////////////////
// class COXPropValue
//////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXPropValue : public  SPropValue
{
public:

//functions
	
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Constructs the object
	COXPropValue();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Copy constructor
	COXPropValue(const COXPropValue& src);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Copy constructor
	COXPropValue(const SPropValue& src);

	// --- In  :
	// --- Out : 
	// --- Returns: string represents of the Value member of the class 
	//              if the type of the property is string, 
	//				otherwise returns empty string
	// --- Effect: Returns string representing the Value member
	CString& GetStringProperty();  

	// --- In  :	nTag - tag to be checked
	// --- Out : 
	// --- Returns: Name of the property tag if found
	// --- Effect: Returns name of the property tag.
	static CString GetPropertyNameFromTag(int nTag);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Assign operator
	const COXPropValue& operator=(const COXPropValue& src);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Assign operator
	const COXPropValue& operator=(const SPropValue& src);

	// --- In  :
	// --- Out : 
	// --- Returns: Property type
	// --- Effect: Returns type of the property
	inline short GetPropertyType() const{return (short) PROP_TYPE(ulPropTag);}
	
	// --- In  :
	// --- Out : 
	// --- Returns: Name of the property
	// --- Effect: Returns property name
	inline CString GetPropertyName(){return GetPropertyNameFromTag(ulPropTag);}

	~COXPropValue();

protected:

//functions
	void Clear();

//members
	CString		m_sValue;
	
};

//////////////////////////////////////////////////////////////////////
// COXMailUser
//////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXMailUser : public CObject
{
public:

//functions

	// --- In  : pWAB - pointer to the IWABObject object
	//			 pMailUser - pointer to the IMailUser object
	// --- Out : 
	// --- Returns:
	// --- Effect: Constructs the object
	COXMailUser(IWABObject* pWAB, IMailUser* pMailUser);


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Copy constructor
	COXMailUser(const COXMailUser& user);


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Assign operator
	const COXMailUser& operator= (const COXMailUser& user);

	// --- In  :
	// --- Out : sName - Display Name property
	// --- Returns: 
	// --- Effect: Returns value of PR_DISPLAY_NAME property
	void GetName(CString& sName);

	// --- In  :
	// --- Out : sEmail - EMail property
	// --- Returns: 
	// --- Effect: Returns value of PR_EMAIL_ADDRESS property
	void GetEmail(CString& sEmail );

	// --- In  :
	// --- Out : pulCount - pointer to number of the bytes in
	//						ENTRYID
	//			 ppEntry - pointer to pointer to ENTRYID
	// --- Returns: Returns ENTRYID for the object. 
	//				This value is readonly and you do not have to change it
	// --- Effect: Returns unique ENTRYID in the address book for the object
	BOOL GetID(ULONG* pulCount, ENTRYID** ppEntry);
	
	// --- In  :
	// --- Out : 
	// --- Returns: Last error
	// --- Effect: Returns last error. Note: Some functions
	//				set error to zero before make calls to 
	//				functions of the exposed COM interfaces.
	//				Therefore, the error will have only last
	//				result of call to a function.
	inline int GetLastError() const {return m_nError;}
	
	// --- In  :	pPropTagArray - pointer to array of the tag properties
	//								to be retrieved.If NULL, all properties to be retrieved
	//				 
	// --- Out :	pulNumber - pointer to number of properties has been retrieved.
	//							You can use specify NULL if you do not need number of 
	//							the properties to be retrieved
	// --- Returns:	array of COXPropValue objects, where number of objects will be 
	//				equal to number of elements in the property tag array requested
	//				or number of the objects is in pulNumber on out.
	//				If the function fails, return NULL. Call GetLastError()
	//				in this case to check error.
	// --- Effect: Call this function to get one or more properties.
	//				NOTE: the caller are responsable for freeing memory 
	//				by operator delete.
	COXPropValue* GetProperties(SPropTagArray* pPropTagArray=NULL, ULONG* pulNumber=NULL);

	
	// --- In  :
	// --- Out : 
	// --- Returns: array of property tags that currently has 
	//				the object.
	// --- Effect: Returns array of property tags. The pointer is temporary
	//				and cannot be saved. To use this properties in the future,
	//				make copy of it. You do not have to free this memory
	const SPropTagArray* GetPropertiesList();
	
	// --- In  :	nCount - number of the properties to save
	//				pProperties - pointer to SPropValue array 
	// --- Out : 
	// --- Returns: The function returns NULL on succeed or pointer to 
	//				array of SPropProblem structures, describing error.
	//				
	// --- Effect: Call this function to save properties of the object.
	//				If the function returns not NULL, this pointer is temporary
	//				and cannot be saved. You do not have to free this memory.
	const SPropProblemArray* SaveProperties(UINT nCount,SPropValue* pProperties);
	
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Constructs the object
	const SPropProblemArray* DeleteProperties(SPropTagArray* pPropTagArray);
	
	// --- In  :	pPropTagArray - pointer to the array of the properties
	//								to be deleted
	// --- Out : 
	// --- Returns: The function returns NULL on succeed or pointer to 
	//				array of SPropProblem structures, describing error.
	//				
	// --- Effect: Deletes properties. 
	//				If the function returns not NULL, this pointer is temporary
	//				and cannot be saved. You do not have to free this memory.
	const SPropTagArray* RegisterPropertyName(UINT nCount,
		MAPINAMEID* pPropNames, ULONG nFlags=MAPI_CREATE);

	virtual ~COXMailUser();

protected:


//members
	UINT m_nEntryCount;
	ENTRYID* m_pEntry;
	SPropProblemArray*	m_pProblems;
	IMailUser*			m_pUser;
	int					m_nError;
	SPropTagArray*		m_pTags;
	IWABObject*			m_pWAB;
};

//////////////////////////////////////////////////////////////////////
// COXWABook
//////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXWABook : public CObject  
{ 
public:

//functions

	// --- In  : hWnd - HWND of the window will be used as parent for
	//					some dialogs. Some functions requere this member 
	//					to be valid and not equal NULL.
	//			 lpszBookPath - pointer to the wab32.dll	
	// --- Out : 
	// --- Returns:
	// --- Effect: Constructs the object
	COXWABook(HWND hWnd=NULL, LPCTSTR lpszBookPath=_T(""));

	// --- In  : sName - display name (value of the propertry 
	//					 of the PR_DISPLAY_NAME) of the user to found
	//					 in the address book
	//			 bDisplayDialog - set it to TRUE if you want show dialog to resolve names
	//							  (if more than one user will be found with the same pattern),
	//							  otherwise set it to FALSE. If you specified FALSE and more than one
	//							  user has been found, the function will return error
	//			 lpszTitle - title of the dialog box to resolve user names.
	//			 ulFlags - flags will be applied for the user if found.
	//						May be:
	//						MAPI_BEST_ACCESS - Specifies to open with best available access rights
	//						MAPI_MODIFY - Requests read/write access.
	// --- Out : 
	// --- Returns: pointer to COXMailUser object on success, NULL otherwise
	// --- Effect: Call this function to find user by name. NOTE: The caller are responsible 
	//			   for freeing returned object if not NULL by operator delete
	COXMailUser* FindUserByName(CString &sName, BOOL bDisplayDialog=TRUE, LPTSTR lpszTitle=NULL,
		ULONG ulFlags=MAPI_BEST_ACCESS);

	// --- In  : sEMail - email (value of the propertry 
	//					 of the PR_EMAIL_ADDRESS) of the user to found
	//					 in the address book
	//			 bDisplayDialog - set it to TRUE if you want show dialog to resolve names
	//							  (if more than one user will be found with the same pattern),
	//							  otherwise set it to FALSE. If you specified FALSE and more than one
	//							  user has been found, the function will return error
	//			 lpszTitle - title of the dialog box to resolve user names.
	//			 ulFlags - flags will be applied for the user if found.
	//						May be:
	//						MAPI_BEST_ACCESS - Specifies to open with best available access rights
	//						MAPI_MODIFY - Requests read/write access.
	// --- Out : 
	// --- Returns: pointer to COXMailUser object on success, NULL otherwise
	// --- Effect: Call this function to find user by email. NOTE: The caller are responsible 
	//			   for freeing returned object if not NULL by operator delete
	COXMailUser* FindUserByEMail(CString& sEMail, BOOL bDisplayDialog=TRUE, LPTSTR lpszTitle=NULL,
		ULONG ulFlags=MAPI_BEST_ACCESS);

	// --- In  : pUser - pointer to COXMailUser object to edit
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect: Call this function to edit user's properties
	BOOL EditUser(COXMailUser* pUser);

	// --- In  : pUser - pointer to the user object to delete
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect: Deletes user from address book. This function does not 
	//				delete actually COXMailUser object. Since it will be 
	//				invalid on success you have to delete this object on success.
	BOOL DeleteUser(COXMailUser* pUser);

	// --- In  :
	// --- Out : 
	// --- Returns: New user successfully has been created or NULL if failed
	// --- Effect: Creates new user in address book
	COXMailUser* CreateUser();

	// --- In  :	ulFlags - flags to be used to work with PAB.
	//						  For description see FindUserByName() function
	// --- Out : 
	// --- Returns: pointer to personal address book object on success,
	//				NULL if failed.
	// --- Effect: 	Call this function to get pointer to the PAB.
	//				Once you have finished to work, call Release()
	//				method of IABContainer
	IABContainer* GetPAB(ULONG ulFlags);

	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to IAddrBook interface on success, NULL if failed.
	// --- Effect: Call this function to get pointer to IAddrBook interface
	inline IAddrBook* GetIAddrBook() {return m_pIAddrBook;}

	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to IWABObject interface on success, NULL if failed.
	// --- Effect: Call this function to get pointer to IWABObject interface
	inline IWABObject* GetWAB() {return m_pIWABObject;}


	// --- In  :	ulCount - number of bytes in ENTRYID
	//				pEntry - pointer to ENTRYID of the user to delete
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect: Call this function to delete user from address book
	BOOL DeleteUser(ULONG ulCount, ENTRYID* pEntry);

	// --- In  : lpszLDAPURL - string representing LDAP query
	//			 ulFlags - only one flag may be set on:
	//				WABOBJECT_LDAPURL_RETURN_MAILUSER - it means the data
	//			    will be retrieved in COXMailUser object, otherwise dialog
	//				may be shown.
	// --- Out : 
	// --- Returns: Pointer to COXMailUser object on success, FALSE otherwise
	// --- Effect: Call this function to make search. If the function returns not NULL,
	//				the caller are responsible for deleting the returned object.
	COXMailUser* SearchLDAP(LPTSTR  lpszLDAPURL, ULONG ulFlags=NULL);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Shows standart dialog to find mail users.
	void ShowFindDialog();

	// --- In  : pProperty - property to find a user by
	//			 bDisplayDialog - set it to TRUE if you want show dialog to resolve names
	//							  (if more than one user will be found with the same pattern),
	//							  otherwise set it to FALSE. If you specified FALSE and more than one
	//							  user has been found, the function will return error
	//			 lpszTitle - title of the dialog box to resolve user names.
	// --- Out :	pulCount - number of bytes in ENTRYID
	//				ppEntry - pointer to ENTRYID of the user that has been found on success
	//						 or NULL if failed.
	// --- Returns: TRUE on success, FALSE if failed.
	// --- Effect: Call this function to find a user by suppled property.
	//				Note:  The caller is responsible for deleting the ENTRYID 
	//				by operator delete[]
	BOOL FindUser(SPropValue* pProperty, ULONG *pulCount, ENTRYID **ppEntry,
							  BOOL bDisplayDialog=TRUE, LPTSTR lpszTitle=NULL);

	// --- In  :	ulCount - number of bytes in ENTRYID
	//				pEntry - pointer to ENTRYID of the user 
	//				ulFlags - flags to be used to work with the object.
	//						  For description see FindUserByName() function
	// --- Out : 
	// --- Returns: pointer to COXMailUser object on success, otherwise NULL
	// --- Effect: Create COXMailUser object from supplied ENTRYID of the user.
	//				Note:  The caller is responsible for deleting the ENTRYID 
	//				by operator delete
	COXMailUser* GetUserFromID(UINT ulCount, ENTRYID *pEntry, UINT nFlags=MAPI_BEST_ACCESS);

	// --- In  :	ulCount - number of bytes in ENTRYID
	//				pEntry - pointer to ENTRYID of the user 
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect: Call this function to edit the user properties
	BOOL EditUser(UINT ulCount, ENTRYID* pEntry);

	// --- In  :
	// --- Out : 
	// --- Returns: Last error
	// --- Effect: Call this function to get last error
	inline UINT GetLastError() {return m_nError;}

	// --- In  :
	// --- Out :	ulCount - number of bytes in ENTRYID
	//				pEntry - pointer to pointer to ENTRYID of the user that has been created on success
	//						 or NULL if failed.
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect: Call this function to create a user interactively
	//				The caller is reponsible for deleting ENTRYID by
	//				operator delete[]
	BOOL CreateUser(ULONG* pulCount,ENTRYID** ppEntry);

	// --- In  :
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise.
	// --- Effect: The first function must be called before doing anything else.
	//			   In order to provide further work this function must succeed.
	BOOL Init();

	// --- In  : pOptions - pointer to tOXWABOptions structure
	//						the members of the structure:	
	//						ULONG ulType1 - type of the first edit box, usualy MAPI_TO
	//						ULONG ulType2 - type of the second edit box, usualy MAPI_CC
	//						ULONG ulType3 - type of the third edit box, usualy MAPI_BCC
	//						CString sType1 - name of the first box, usually "To"
	//						CString sType2 - name of the second box, usually "Cc"
	//						CString sType3 - name of the third box, usually "Bcc"
	//						CString sCaption - title of the dialog
	//						CString sNewEntry - title of the new entry
	//						CString sDest - title of the destination
	//			ulFlags - Bitmask of flags associated with various address dialog box options. 
	//					  The following flags can be set: 
	//				AB_RESOLVE - Causes all names to be resolved after 
	//					the address book dialog box is closed. 
	//					The Resolve Name dialog box is displayed if 
	//					there are ambiguous entries in the recipient list. 
	//				AB_SELECTONLY - Disables the creation of custom recipient addresses 
	//					and direct type-in entries for a recipient list. 
	//					This flag is used only if the dialog box is modal.
	//				ADDRESS_ONE - Indicates that the user of the dialog box 
	//					can select exactly one message recipient, 
	//					instead of a number of recipients from 
	//					a recipient list. This flag is valid only when 
	//					cDestFields is zero. This flag is used only if 
	//					the dialog box is modal. 
	//				DIALOG_MODAL - Causes a modal dialog box to be displayed. 
	//					The client must set either this flag or DIALOG_SDI, 
	//					but not both.
	//				DIALOG_SDI - Causes a modeless dialog box to be displayed. 
	//					This call returns immediately.
	//			lpfnABSDI - Address of an Internet Explorer Address Book 
	//					function based on the ACCELERATEABSDI (see MAPI documentation) prototype.
	//			lpfnDismiss - Address of a function based on the DISMISSMODELESS 
	//					(see MAPI documentation) prototype
	//			lpfnDismissCtxt - Address of the context information to be passed 
	//					to the DISMISSMODELESS function pointed to 
	//					by the lpfnDismiss member
	//			ulFocus - Field in the address book dialog box 
	//					that should have the initial focus 
	//					when the dialog box appears
	// --- Out : phWndOut - HWND of the dialog box if DIALOG_SDI flag 
	//					has been specified.
	// --- Returns: Pointer to ADRLIST structure, contains list of selected users
	//				on success, NULL otherwise. If the edit boxes have been specified
	//				the PR_RECIPIENT_TYPE properties of the user defines type 
	//				of the edit boxes the user has bben selected to.
	// --- Effect: Call this function to browse the address book and, probably,
	//				select the users to edit boxes.
	ADRLIST* SelectUser(tOXWABOptions* pOptions=NULL, ULONG ulFlags=DIALOG_MODAL | AB_RESOLVE,
		LPFNABSDI lpfnABSDI=NULL, LPFNDISMISS lpfnDismiss=NULL, 
		void* lpfnDismissCtxt=NULL, ULONG ulFocus=NULL, HWND* phWndOut=NULL);

	// --- In  : hWnd - HWND of the window will be used as parent for some dialogs
	// --- Out : 
	// --- Returns:
	// --- Effect: Sets the parent window for address book dialog boxes.
	inline void SetParent(HWND hWnd) {m_hWnd=hWnd;}

	// --- In  :
	// --- Out : 
	// --- Returns: HWND of the window is used as parent for some dialogs
	// --- Effect: Gets the parent window for address book dialog boxes.
	inline HWND GetParent() const {return m_hWnd;}

	virtual ~COXWABook();

protected:

//functions	
	CString GetWabLocation();

//members
	UINT			m_nError;
	CString			m_sBookPath;
	HWND			m_hWnd; 
	IWABObject*		m_pIWABObject;
	IAddrBook*		m_pIAddrBook;
	HRESULT			(PASCAL* m_lpfnWabOpen) (LPADRBOOK   * lppAdrBook,
						LPWABOBJECT* lppWABObject, LPWAB_PARAM lpWP, 
						DWORD Reserved2);
	HINSTANCE		m_hWab;
};

#endif // !defined(_OXWABOOK_H__)

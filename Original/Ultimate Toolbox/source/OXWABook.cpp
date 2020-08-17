// OXWABook.cpp: implementation of the COXWABook class.
//
//////////////////////////////////////////////////////////////////////
// Version: 9.3


#include "stdafx.h"
#include "OXWABook.h"
#include "OXMainRes.h"
#include "UTBStrOp.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// class COXPropValue
//////////////////////////////////////////////////////////////////////
const GUID IID_IMailUser={0x0002030A, 0000, 0000,
{0xC0, 0, 0, 0, 0, 0, 0, 0x46}};

COXPropValue::COXPropValue()
{
	::ZeroMemory(&Value, sizeof(Value));
	ulPropTag=NULL;
	dwAlignPad=NULL;
}

COXPropValue::COXPropValue(const COXPropValue& src)
{
	*this=src;
}

COXPropValue::COXPropValue(const SPropValue& src)
{
	*this=src;
}

COXPropValue::~COXPropValue()
{
	Clear();
}

void COXPropValue::Clear()
{
	//just huge switch to clear the memory
	//depending on the type of thus property
	switch(PROP_TYPE(ulPropTag))
	{
	case PT_SHORT:
		{
			Value.i=NULL;
		}
		break;
	case PT_BOOLEAN:
		{
			Value.b=NULL;
		}
		break;
	case PT_LONG:
		{
			Value.ul=NULL;
		}
		break;
	case PT_NULL:
		{
			Value.x=NULL;
		}
		break;		
	case PT_FLOAT:
		{
			Value.flt=NULL;
		}
		break;
	case PT_DOUBLE:
		{
			Value.dbl=NULL;
		}
	case PT_APPTIME:
		{
			Value.at=NULL;
		}
		break;		
	case PT_CURRENCY:
		{
			Value.cur.Hi=NULL;
			Value.cur.Lo=NULL;
		}
		break;		
	case PT_SYSTIME:
		{
			Value.ft.dwHighDateTime=NULL;
			Value.ft.dwLowDateTime=NULL;
		}
		break;		
	case PT_STRING8:
		{
			delete []Value.lpszA;
			Value.lpszA=NULL;
		}
		break;		
	case PT_BINARY:
		{
			delete []Value.bin.lpb;
			Value.bin.cb=NULL;
			Value.bin.lpb=NULL;
		}
		break;		
	case PT_UNICODE:
		{
			delete []Value.lpszW;
			Value.lpszW=NULL;
		}
		break;		
	case PT_CLSID:
		{
			delete Value.lpguid;
			Value.lpguid=NULL;
		}
		break;		
	case PT_I8:
		{
			Value.li.QuadPart=NULL;
		}
		break;		
	case PT_MV_I2:
		{
			delete []Value.MVi.lpi;
			Value.MVi.lpi=NULL;
			Value.MVi.cValues=NULL;
		}
		break;		
	case PT_MV_LONG:
		{
			delete []Value.MVl.lpl;
			Value.MVl.lpl=NULL;
			Value.MVl.cValues=NULL;
		}
		break;		
	case PT_MV_R4:
		{
			delete []Value.MVflt.lpflt;
			Value.MVflt.cValues=NULL;
			Value.MVflt.lpflt=NULL;
		}
		break;		
	case PT_MV_DOUBLE:
		{
			delete []Value.MVdbl.lpdbl;
			Value.MVdbl.cValues=NULL;
			Value.MVdbl.lpdbl=NULL;
		}
		break;		
	case PT_MV_CURRENCY:
		{
			delete []Value.MVcur.lpcur;
			Value.MVcur.cValues=NULL;
			Value.MVcur.lpcur=NULL;
		}
		break;		
	case PT_MV_APPTIME:
		{
			delete []Value.MVat.lpat;
			Value.MVat.cValues=NULL;
			Value.MVat.lpat=NULL;
		}
		break;		
	case PT_MV_SYSTIME:
		{
			delete []Value.MVft.lpft;
			Value.MVft.cValues=NULL;
			Value.MVft.lpft=NULL;
		}
		break;		
	case PT_MV_BINARY:
		{
			for (UINT n=0;n<Value.MVbin.cValues;n++)
			{
				Value.MVbin.lpbin[n].cb=NULL;
				delete []Value.MVbin.lpbin[n].lpb;
				Value.MVbin.lpbin[n].lpb=NULL;
			}
			Value.MVbin.cValues=NULL;
			delete []Value.MVbin.lpbin;
			Value.MVbin.lpbin=NULL;
		}
		break;		
	case PT_MV_STRING8:
		{

			for (UINT n=0;n<Value.MVszA.cValues;n++)
			{
				delete [] *(Value.MVszA.lppszA+n);
			}
			delete [] Value.MVszA.lppszA;
			
			Value.MVszA.cValues=NULL;
			Value.MVszA.lppszA=NULL;

		}
		break;		
	case PT_MV_UNICODE:
		{
			for (UINT n=0;n<Value.MVszW.cValues;n++)
			{
				delete [] (*Value.MVszW.lppszW+n);
			}
			delete [] Value.MVszW.lppszW;
			Value.MVszW.cValues=NULL;
			Value.MVszW.lppszW=NULL;
		}
		break;		
	case PT_MV_CLSID:
		{
			Value.MVguid.cValues=NULL;
			delete []Value.MVguid.lpguid;
			Value.MVguid.lpguid=NULL;
		}
		break;		
	case PT_MV_I8:
		{
			Value.MVli.cValues=NULL;
			delete []Value.MVli.lpli;
			Value.MVli.lpli=NULL;
		}
		break;		
	case PT_ERROR:
		{
			Value.x=NULL;
		}
		break;		
	default:
		;//unknown type
	}

	ulPropTag=NULL;
	dwAlignPad=NULL;

}

const COXPropValue& COXPropValue::operator=(const COXPropValue& src)
{
	return *this = ((SPropValue&) src);
}

const COXPropValue& COXPropValue::operator=(const SPropValue& src)
{

	Clear();

	ulPropTag=src.ulPropTag;
	dwAlignPad=src.dwAlignPad;

	//another huge switch to allocate memory
	//and copy the data
	switch(PROP_TYPE(src.ulPropTag))
	{
	case PT_SHORT:
		{
			Value.i=src.Value.i;
		}
		break;
	case PT_BOOLEAN:
		{
			Value.b=src.Value.b;
		}
		break;
	case PT_LONG:
		{
			Value.ul=src.Value.ul;
		}
		break;
	case PT_NULL:
		{
			Value.x=src.Value.x;
		}
		break;		
	case PT_FLOAT:
		{
			Value.flt=src.Value.flt;
		}
		break;
	case PT_DOUBLE:
		{
			Value.dbl=src.Value.dbl;
		}
	case PT_APPTIME:
		{
			Value.at=src.Value.at;
		}
		break;		
	case PT_CURRENCY:
		{
			Value.cur.Hi =src.Value.cur.Hi;
			Value.cur.Lo=src.Value.cur.Lo;
		}
		break;		
	case PT_SYSTIME:
		{
			Value.ft.dwHighDateTime=src.Value.ft.dwHighDateTime;
			Value.ft.dwLowDateTime=src.Value.ft.dwLowDateTime;
		}
		break;		
	case PT_STRING8:
		{
			Value.lpszA=new char[strlen(src.Value.lpszA)+1];
			UTBStr::strcpy(Value.lpszA,strlen(src.Value.lpszA)+1, src.Value.lpszA);
		}
		break;		
	case PT_BINARY:
		{
			Value.bin.lpb=new BYTE[src.Value.bin.cb];
			Value.bin.cb=src.Value.bin.cb;
			CopyMemory(Value.bin.lpb, src.Value.bin.lpb, Value.bin.cb);
		}
		break;		
	case PT_UNICODE:
		{
			Value.lpszW=new WCHAR[lstrlenW(src.Value.lpszW)+1];
			lstrcpyW(Value.lpszW,src.Value.lpszW);
		}
		break;		
	case PT_CLSID:
		{
			Value.lpguid=new GUID;
			::CopyMemory(Value.lpguid, src.Value.lpguid,
				sizeof(GUID));
		}
		break;		
	case PT_I8:
		{
			::CopyMemory(&Value.li,&src.Value.li,sizeof(LARGE_INTEGER));
		}
		break;		
	case PT_MV_I2:
		{
			Value.MVi.cValues=src.Value.MVi.cValues;
			Value.MVi.lpi=new short[Value.MVi.cValues];
			::CopyMemory(Value.MVi.lpi,src.Value.MVi.lpi,
				Value.MVi.cValues*sizeof(short));
		}
		break;		
	case PT_MV_LONG:
		{
			Value.MVl.cValues=src.Value.MVl.cValues;
			Value.MVl.lpl=new long[Value.MVl.cValues];
			::CopyMemory(Value.MVl.lpl,src.Value.MVl.lpl,
				Value.MVl.cValues*sizeof(long));
		}
		break;		
	case PT_MV_R4:
		{
			Value.MVflt.cValues=src.Value.MVflt.cValues;
			Value.MVflt.lpflt=new float[Value.MVflt.cValues];
			::CopyMemory(Value.MVflt.lpflt,src.Value.MVflt.lpflt,
				Value.MVflt.cValues*sizeof(float));
		}
		break;		
	case PT_MV_DOUBLE:
		{
			Value.MVdbl.cValues=src.Value.MVdbl.cValues;
			Value.MVdbl.lpdbl=new double[Value.MVdbl.cValues];
			::CopyMemory(Value.MVdbl.lpdbl,src.Value.MVdbl.lpdbl,
				Value.MVdbl.cValues*sizeof(double));
		}
		break;		
	case PT_MV_CURRENCY:
		{
			Value.MVcur.cValues=src.Value.MVcur.cValues;
			Value.MVcur.lpcur=new CURRENCY[Value.MVcur.cValues];
			::CopyMemory(Value.MVcur.lpcur,src.Value.MVcur.lpcur,
				Value.MVcur.cValues*sizeof(CURRENCY));
		}
		break;		
	case PT_MV_APPTIME:
		{
			Value.MVat.cValues=src.Value.MVat.cValues;
			Value.MVat.lpat=new double[Value.MVat.cValues];
			::CopyMemory(Value.MVat.lpat,src.Value.MVat.lpat,
				Value.MVat.cValues*sizeof(double));
		}
		break;		
	case PT_MV_SYSTIME:
		{
			Value.MVft.cValues=src.Value.MVft.cValues;
			Value.MVft.lpft=new FILETIME[Value.MVft.cValues];
			::CopyMemory(Value.MVft.lpft,src.Value.MVft.lpft,
				Value.MVft.cValues*sizeof(FILETIME));
		}
		break;		
	case PT_MV_BINARY:
		{
			Value.MVbin.cValues=src.Value.MVbin.cValues;
			Value.MVbin.lpbin=new SBinary[Value.MVbin.cValues];
			for (UINT n=0;n<Value.MVbin.cValues;n++)
			{
				Value.MVbin.lpbin[n].cb=src.Value.MVbin.lpbin[n].cb;
				Value.MVbin.lpbin[n].lpb=new BYTE[Value.MVbin.lpbin[n].cb];
				::CopyMemory(Value.MVbin.lpbin[n].lpb,
					src.Value.MVbin.lpbin[n].lpb,
					Value.MVbin.lpbin[n].cb);
			}
		}
		break;		
	case PT_MV_STRING8:
		{
			Value.MVszA.cValues=src.Value.MVszA.cValues;
			Value.MVszA.lppszA=new LPSTR[Value.MVszA.cValues];
			for (UINT n=0;n<Value.MVszA.cValues;n++)
			{
				*(Value.MVszA.lppszA+n)= new char[strlen(
					src.Value.MVszA.lppszA[n])+1];
				UTBStr::strcpy(Value.MVszA.lppszA[n],strlen(
					src.Value.MVszA.lppszA[n])+1, src.Value.MVszA.lppszA[n]);

			}
		}
		break;		
	case PT_MV_UNICODE:
		{
			Value.MVszW.cValues=src.Value.MVszW.cValues;
			Value.MVszW.lppszW=new LPWSTR[Value.MVszW.cValues];
			for (UINT n=0;n<Value.MVszW.cValues;n++)
			{
				Value.MVszW.lppszW[n]= new WCHAR[lstrlenW(
					src.Value.MVszW.lppszW[n])+1];
				lstrcpyW(Value.MVszW.lppszW[n],src.Value.MVszW.lppszW[n]);

			}
		}
		break;		
	case PT_MV_CLSID:
		{
			Value.MVguid.cValues=src.Value.MVguid.cValues;
			Value.MVguid.lpguid=new GUID[Value.MVguid.cValues];
			::CopyMemory(Value.MVguid.lpguid,src.Value.MVguid.lpguid,
				Value.MVguid.cValues*sizeof(GUID));
		}
		break;		
	case PT_MV_I8:
		{
			Value.MVli.cValues=src.Value.MVli.cValues;
			Value.MVli.lpli=new LARGE_INTEGER[Value.MVli.cValues];
			::CopyMemory(Value.MVli.lpli,src.Value.MVli.lpli,
				Value.MVli.cValues*sizeof(LARGE_INTEGER));
		}
		break;		
	case PT_ERROR:
		{
			Value.x=src.Value.x;
		}
		break;		
	default:
		ASSERT(FALSE);//unknown type
	}

	
	return *this;
}



CString COXPropValue::GetPropertyNameFromTag(int nTag)
{
	//just huge switch
	CString sText;
	switch (PROP_ID(nTag))
	{
		case PROP_ID(PR_ROWID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ROWID ));//"Rowid"
			break;
		case PROP_ID(PR_DISPLAY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DISPLAYNAME ));//"Display name"
			break;
		case PROP_ID(PR_ADDRTYPE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ADDRTYPE ));//"Addrtype"
			break;
		case PROP_ID(PR_EMAIL_ADDRESS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_EMAILADDRESS ));//"Email address"
			break;
		case PROP_ID(PR_COMMENT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_COMMENT ));//"Comment"
			break;
		case PROP_ID(PR_DEPTH):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DEPTH ));//"Depth"
			break;
		case PROP_ID(PR_PROVIDER_DISPLAY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PROVIDERDISPLAY ));//"Provider display"
			break;
		case PROP_ID(PR_CREATION_TIME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CREATIONTIME ));//"Creation time"
			break;
		case PROP_ID(PR_LAST_MODIFICATION_TIME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MODIFICATIONTIME ));//"Last modification time"
			break;
		case PROP_ID(PR_RESOURCE_FLAGS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_RESOURCEFLAGS ));//"Resource flags"
			break;
		case PROP_ID(PR_PROVIDER_DLL_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PROVIDERDLL ));//"Provider dll name"
			break;
		case PROP_ID(PR_SEARCH_KEY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SEARCHKEY ));//"Search key"
			break;
		case PROP_ID(PR_PROVIDER_UID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PROVIDERUID ));//"Provider uid"
			break;
		case PROP_ID(PR_PROVIDER_ORDINAL):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PROVIDERORIGINAL ));//"Provider ordinal"
			break;
		case PROP_ID(PR_CONTAINER_FLAGS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTAINERFLAGS ));//"Container flags"
			break;
		case PROP_ID(PR_FOLDER_TYPE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_FOLDERTYPE ));//"Folder type"
			break;
		case PROP_ID(PR_CONTENT_COUNT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTENTCOUNT ));//"Content count"
			break;
		case PROP_ID(PR_CONTENT_UNREAD):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTENTUNREAD ));//"Content unread"
			break;
		case PROP_ID(PR_CREATE_TEMPLATES):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CREATETEMPLATES ));//"Create templates"
			break;
		case PROP_ID(PR_DETAILS_TABLE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DETAILSTABLE ));//"Details table"
			break;
		case PROP_ID(PR_SEARCH):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SEARCH ));//"Search"
			break;
		case PROP_ID(PR_SELECTABLE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SELECTABLE ));//"Selectable"
			break;
		case PROP_ID(PR_SUBFOLDERS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SUBFOLDERS ));//"Subfolders"
			break;
		case PROP_ID(PR_STATUS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_STATUS ));//"Status"
			break;
		case PROP_ID(PR_ANR):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ANR ));//"Anr"
			break;
		case PROP_ID(PR_CONTENTS_SORT_ORDER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTENTSSORT ));//"Contents sort order"
			break;
		case PROP_ID(PR_CONTAINER_HIERARCHY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTAINERHIERARCHY ));//"Container hierarchy"
			break;
		case PROP_ID(PR_CONTAINER_CONTENTS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTAINERCONTENTS ));//"Container contents"
			break;
		case PROP_ID(PR_FOLDER_ASSOCIATED_CONTENTS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_FOLDERASSOSIATED ));//"Folder associated contents"
			break;
		case PROP_ID(PR_DEF_CREATE_DL):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DEFCREATEDL ));//"Def create dl"
			break;
		case PROP_ID(PR_DEF_CREATE_MAILUSER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DEFCREATEMAILUSER ));//"Def create mailuser"
			break;
		case PROP_ID(PR_CONTAINER_CLASS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTAINERCLASS ));//"Container class"
			break;
		case PROP_ID(PR_CONTAINER_MODIFY_VERSION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTAINERMODIFY ));//"Container modify version"
			break;
		case PROP_ID(PR_AB_PROVIDER_ID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ABPROVIDERID ));//"Ab provider id"
			break;
		case PROP_ID(PR_DEFAULT_VIEW_ENTRYID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DEFAULTVIEWENTRYID ));//"Default view entryid"
			break;
		case PROP_ID(PR_ASSOC_CONTENT_COUNT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ASSOCCONTENTCOUNT ));//"Assoc content count"
			break;
		case PROP_ID(PR_DISPLAY_TYPE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DISPLAYTYPE ));//"Display type"
			break;
		case PROP_ID(PR_TEMPLATEID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_TEMPLATEID ));//"Templateid"
			break;
		case PROP_ID(PR_PRIMARY_CAPABILITY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PRIMARYCAPABILITY ));//"Primary capability"
			break;
		case PROP_ID(PR_7BIT_DISPLAY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_7BITDISPLAYNAME ));//"7bit display name"
			break;
		case PROP_ID(PR_ACCOUNT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ACCOUNT ));//"Account"
			break;
		case PROP_ID(PR_ALTERNATE_RECIPIENT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ALTERNATERECIPIENT ));//"Alternate recipient"
			break;
		case PROP_ID(PR_CALLBACK_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CALLBACKPHONENUMBER ));//"Callback telephone number"
			break;
		case PROP_ID(PR_CONVERSION_PROHIBITED):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONVERSIONPROHIBITED ));//"Conversion prohibited"
			break;
		case PROP_ID(PR_DISCLOSE_RECIPIENTS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DISCLOSERECIPIENTS ));//"Disclose recipients"
			break;
		case PROP_ID(PR_GENERATION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_GENERATION ));//"Generation"
			break;
		case PROP_ID(PR_GIVEN_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_GIVENNAME ));//"Given name"
			break;
		case PROP_ID(PR_GOVERNMENT_ID_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_GOVERMENTIDNUMBER ));//"Government id number"
			break;
		case PROP_ID(PR_BUSINESS_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_BUSINESSPHONENUMBER ));//"Business telephone number"
			break;
		case PROP_ID(PR_HOME_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEPHONENUMBER ));//"Home telephone number"
			break;
		case PROP_ID(PR_INITIALS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_INITIALS ));//"Initials"
			break;
		case PROP_ID(PR_KEYWORD):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_KEYWORD ));//"Keyword"
			break;
		case PROP_ID(PR_LANGUAGE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_LANGUAGE ));//"Language"
			break;
		case PROP_ID(PR_LOCATION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_LOCATION ));//"Location"
			break;
		case PROP_ID(PR_MAIL_PERMISSION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MAILPERMISSION ));//"Mail permission"
			break;
		case PROP_ID(PR_MHS_COMMON_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MHSCOMMONNAME ));//"Mhs common name"
			break;
		case PROP_ID(PR_ORGANIZATIONAL_ID_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ORGANIZATIONALID ));//"Organizational id number"
			break;
		case PROP_ID(PR_SURNAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SURNAME ));//"Surname"
			break;
		case PROP_ID(PR_ORIGINAL_ENTRYID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ORIGINALENTRYID ));//"Original entryid"
			break;
		case PROP_ID(PR_ORIGINAL_DISPLAY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ORIGINALDISPLAYNAME ));//"Original display name"
			break;
		case PROP_ID(PR_ORIGINAL_SEARCH_KEY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ORIGINALSEARCHKEY ));//"Original search key"
			break;
		case PROP_ID(PR_POSTAL_ADDRESS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_POSTALADDRESS ));//"Postal address"
			break;
		case PROP_ID(PR_COMPANY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_COMPANYNAME ));//"Company name"
			break;
		case PROP_ID(PR_TITLE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_TITLE ));//"Title"
			break;
		case PROP_ID(PR_DEPARTMENT_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DEPARTMENTNAME ));//"Department name"
			break;
		case PROP_ID(PR_OFFICE_LOCATION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OFFICELOCATION ));//"Office location"
			break;
		case PROP_ID(PR_PRIMARY_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PHONENUMBER1 ));//"Primary telephone number"
			break;
		case PROP_ID(PR_BUSINESS2_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_BUSINESSPHONE2 ));//"Business2 telephone number"
			break;
		case PROP_ID(PR_MOBILE_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MOBILEPHONENUMBER ));//"Mobile telephone number"
			break;
		case PROP_ID(PR_RADIO_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_RADIOPHONENUMBER ));//"Radio telephone number"
			break;
		case PROP_ID(PR_CAR_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CARPHONENUMBER ));//"Car telephone number"
			break;
		case PROP_ID(PR_OTHER_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERPHONENUMBER ));//"Other telephone number"
			break;
		case PROP_ID(PR_TRANSMITABLE_DISPLAY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_TRANSDISPNAME ));//"Transmitable display name"
			break;
		case PROP_ID(PR_PAGER_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PAGERPHONENUMBER ));//"Pager telephone number"
			break;
		case PROP_ID(PR_USER_CERTIFICATE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_USERCERTIFICATE ));//"User certificate"
			break;
		case PROP_ID(PR_PRIMARY_FAX_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_FAXNUMBER1 ));//"Primary fax number"
			break;
		case PROP_ID(PR_BUSINESS_FAX_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_BUSINESSFAXNUMBER ));//"Business fax number"
			break;
		case PROP_ID(PR_HOME_FAX_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEFAXNUMBER ));//"Home fax number"
			break;
		case PROP_ID(PR_COUNTRY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_COUNTRY ));//"Country"
			break;
		case PROP_ID(PR_LOCALITY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_LOCALITY ));//"Locality"
			break;
		case PROP_ID(PR_STATE_OR_PROVINCE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_STATEPROVINCE ));//"State or province"
			break;
		case PROP_ID(PR_STREET_ADDRESS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_STREETADDRESS ));//"Street address"
			break;
		case PROP_ID(PR_POSTAL_CODE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_POSTALCODE ));//"Postal code"
			break;
		case PROP_ID(PR_POST_OFFICE_BOX):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_POSTOFFICEBOX ));//"Post office box"
			break;
		case PROP_ID(PR_TELEX_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_TELEXNUMBER ));//"Telex number"
			break;
		case PROP_ID(PR_ISDN_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ISDNNUMBER ));//"Isdn number"
			break;
		case PROP_ID(PR_ASSISTANT_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ASSISTANCEPHONENUMBER ));//"Assistant telephone number"
			break;
		case PROP_ID(PR_HOME2_TELEPHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEPHONENUMBER2 ));//"Home2 telephone number"
			break;
		case PROP_ID(PR_ASSISTANT):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_ASSISTANT ));//"Assistant"
			break;
		case PROP_ID(PR_SEND_RICH_INFO):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SENDRICHINFO ));//"Send rich info"
			break;
		case PROP_ID(PR_WEDDING_ANNIVERSARY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_WEDDINGANNIVERSARY ));//"Wedding anniversary"
			break;
		case PROP_ID(PR_BIRTHDAY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_BIRTHDAY ));//"Birthday"
			break;
		case PROP_ID(PR_HOBBIES):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOBBIES ));//"Hobbies"
			break;
		case PROP_ID(PR_MIDDLE_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MIDDLENAME ));//"Middle name"
			break;
		case PROP_ID(PR_DISPLAY_NAME_PREFIX):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_DISPLAYNAMEPREFIX ));//"Display name prefix"
			break;
		case PROP_ID(PR_PROFESSION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PROFESSION ));//"Profession"
			break;
		case PROP_ID(PR_PREFERRED_BY_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PREFFEREDBYNAME ));//"Preferred by name"
			break;
		case PROP_ID(PR_SPOUSE_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_SPOUSENAME ));//"Spouse name"
			break;
		case PROP_ID(PR_COMPUTER_NETWORK_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_COMPUTERNETWORKNAME ));//"Computer network name"
			break;
		case PROP_ID(PR_CUSTOMER_ID):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CUSTOMERID ));//"Customer id"
			break;
		case PROP_ID(PR_TTYTDD_PHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_TTYTDPHONENUMBER ));//"Ttytdd phone number"
			break;
		case PROP_ID(PR_FTP_SITE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_FTPSITE ));//"Ftp site"
			break;
		case PROP_ID(PR_GENDER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_GENDER ));//"Gender"
			break;
		case PROP_ID(PR_MANAGER_NAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_MANAGERNAME ));//"Manager name"
			break;
		case PROP_ID(PR_NICKNAME):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_NICKNAME ));//"Nickname"
			break;
		case PROP_ID(PR_PERSONAL_HOME_PAGE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_PERSONALHOMEPAGE ));//"Personal home page"
			break;
		case PROP_ID(PR_BUSINESS_HOME_PAGE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_BUSINESSHOMEPAGE ));//"Business home page"
			break;
		case PROP_ID(PR_CONTACT_VERSION):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTACTVERSION ));//"Contact version"
			break;
		case PROP_ID(PR_CONTACT_ENTRYIDS):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTRACTENTRYIDS ));//"Contact entryids"
			break;
		case PROP_ID(PR_CONTACT_ADDRTYPES):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTACTADDRTYPE ));//"Contact addrtypes"
			break;
		case PROP_ID(PR_CONTACT_DEFAULT_ADDRESS_INDEX):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTACTDEFAULTADDRESSINDEX ));//"Contact default address index"
			break;
		case PROP_ID(PR_CONTACT_EMAIL_ADDRESSES):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CONTACTEMAILADDRESS ));//"Contact email addresses"
			break;
		case PROP_ID(PR_COMPANY_MAIN_PHONE_NUMBER):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_COMPANYMAINPHONENUMBER ));//"Company main phone number"
			break;
		case PROP_ID(PR_CHILDRENS_NAMES):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_CHILDRENSNAMES ));//"Childrens names"
			break;
		case PROP_ID(PR_HOME_ADDRESS_CITY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSCITY ));//"Home address city"
			break;
		case PROP_ID(PR_HOME_ADDRESS_COUNTRY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSCOUNTRY ));//"Home address country"
			break;
		case PROP_ID(PR_HOME_ADDRESS_POSTAL_CODE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSPOSTALCODE ));//"Home address postal code"
			break;
		case PROP_ID(PR_HOME_ADDRESS_STATE_OR_PROVINCE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSSTATE ));//"Home address state or province"
			break;
		case PROP_ID(PR_HOME_ADDRESS_STREET):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSSTREET ));//"Home address street"
			break;
		case PROP_ID(PR_HOME_ADDRESS_POST_OFFICE_BOX):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_HOMEADDRESSPOSTOFFICE ));//"Home address post office box"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_CITY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSCITY ));//"Other address city"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_COUNTRY):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSCOUNTRY ));//"Other address country"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_POSTAL_CODE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSPOSTALCODE ));//"Other address postal code"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_STATE_OR_PROVINCE):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSSTATE ));//"Other address state or province"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_STREET):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSSTREET ));//"Other address street"
			break;
		case PROP_ID(PR_OTHER_ADDRESS_POST_OFFICE_BOX):
			VERIFY(sText.LoadString(IDS_OX_WABOOK_OTHERADDRESSPOSTOFFICE ));//"Other address post office box"
			break;
	}
	return sText;
}

CString& COXPropValue::GetStringProperty()
{ 
	//returns empty string
	//if type of the property is not string


	m_sValue=_T("");

	if (PROP_TYPE(ulPropTag)==PT_STRING8)
	{
		m_sValue=Value.lpszA;
	}
	else
		if (PROP_TYPE(ulPropTag)==PT_UNICODE)
		{

			m_sValue=Value.lpszW;
		}

		return m_sValue;
}


//////////////////////////////////////////////////////////////////////
// COXMailUser
//////////////////////////////////////////////////////////////////////

COXMailUser::COXMailUser(IWABObject* pWAB, IMailUser* pMailUser) :
	m_pWAB(pWAB), m_pUser(pMailUser), m_nError(NULL),
	m_pTags(NULL), m_pProblems(NULL), m_nEntryCount(NULL), m_pEntry(NULL)
{
	ASSERT(m_pWAB);
	ASSERT(m_pUser);
	m_pUser->AddRef();
	m_pWAB->AddRef();
}

COXMailUser::COXMailUser(const COXMailUser &user) : 
 m_pWAB(user.m_pWAB), m_pUser(user.m_pUser), m_nError(NULL),
 m_pTags(NULL), m_pProblems(NULL)

{

	ASSERT(m_pWAB); 
	ASSERT(m_pUser);
	m_pWAB->AddRef(); 
	m_pUser->AddRef();
	m_nEntryCount=user.m_nEntryCount;
	if (m_nEntryCount)
	{
		HRESULT hRslt=m_pWAB->AllocateBuffer(m_nEntryCount,
			(void**) &m_pEntry);
		ASSERT(hRslt==S_OK);
		::CopyMemory((void*) m_pEntry, (void*) user.m_pEntry,
			m_nEntryCount);
	}
	else
		m_pEntry=NULL;
}

const COXMailUser& COXMailUser::operator= (const COXMailUser& user)
{
	if (m_pWAB)
		m_pWAB->Release();
	if (m_pUser)
		m_pUser->Release();
	m_pWAB=user.m_pWAB;
	m_pUser=user.m_pUser;
	ASSERT(m_pWAB);
	ASSERT(m_pUser);
	m_pWAB->AddRef();
	m_pUser->AddRef();
	if (m_pEntry)
	{
		ASSERT(m_pWAB);
		m_pWAB->FreeBuffer(m_pEntry);
	}
	if (m_nEntryCount)
	{
		HRESULT hRslt=m_pWAB->AllocateBuffer(m_nEntryCount,
			(void**) &m_pEntry);
		ASSERT(hRslt==S_OK);

		::CopyMemory((void*) m_pEntry, (void*) user.m_pEntry,
			m_nEntryCount);
	}
	else
		m_pEntry=NULL;
	return *this;
}


COXMailUser::~COXMailUser()
{
	if (m_pEntry)
	{
		ASSERT(m_pWAB);
		m_pWAB->FreeBuffer(m_pEntry);
	}
	if (m_pTags)
		delete []m_pTags;
	if (m_pProblems)
		delete []m_pProblems;
	if (m_pUser)
		m_pUser->Release();
	if (m_pWAB)
		m_pWAB->Release();
}


COXPropValue* COXMailUser::GetProperties(SPropTagArray* pPropTagArray,
		ULONG* pulNumber)
{

	//this function returns pointer to array of 
	//COXPropValue objects. Number of the elements in the array
	//on out will be in pulNumber. If you do not want the number,
	//(for example if you call this function just to retrieve one property)
	//pulNumber can be equal NULL. 
	//if pPropTagArray is equal NULL, all properties will be retrieved

	//NOTE: if the function returns not NULL, you are responsible for
	//freeing memory by delete []

	ASSERT(m_pUser);
	ASSERT(m_pWAB);

	m_nError=NULL;
#ifdef _DEBUG
	if (pPropTagArray)
		ASSERT(pPropTagArray->cValues!=NULL);
#endif

	ULONG ulFlags;
	SPropValue* pLocalValue=NULL;
	ULONG nNumber=0;

#ifdef _UNICODE
	ulFlags=MAPI_UNICODE;
#else
	ulFlags=NULL;
#endif

	HRESULT hRslt=m_pUser->GetProps(pPropTagArray,ulFlags,
		&nNumber,&pLocalValue);

	if (FAILED(hRslt))
	{
		if (pulNumber)
			*pulNumber=NULL;
		if (pLocalValue)
			m_pWAB->FreeBuffer(pLocalValue);
		m_nError=HRESULT_CODE(hRslt);
		return NULL;
	}


	ASSERT(nNumber);
	ASSERT(pLocalValue);

	if (pulNumber)
		*pulNumber=nNumber;

	COXPropValue* pProps=new COXPropValue[nNumber];
	for (UINT n=0;n<nNumber;n++)
		*(pProps+n)=*(pLocalValue+n);

	hRslt=m_pWAB->FreeBuffer(pLocalValue);

	return pProps;

}

	
const SPropProblemArray* COXMailUser::SaveProperties(UINT nCount,SPropValue* pProperties)
{
	//This function saves properties for the mail user.
	//In case of some problems, problem array will be
	//returned. Pointer to this array is temporary and could not
	//be saved. To manage by this array you have to copy it.
	//Next time this function will be called, the array may be freed
	// and allocated new one. The array is read only and you do
	//not have to free the memory occuppied by this array.
	ASSERT(m_pWAB);
	ASSERT(m_pUser);
	ASSERT(pProperties);
	ASSERT(nCount);

	m_nError=NULL;
	SPropProblemArray* pProblems=NULL;

	//set properties
	HRESULT hRslt=m_pUser->SetProps(nCount,pProperties,
		&pProblems);
	if (hRslt==S_OK)
	{
		m_nError=HRESULT_CODE(hRslt);
		if (pProblems)
			m_pWAB->FreeBuffer(pProblems);
		return NULL;
	}

	//execution okay, but we have some problems with properties
	if (pProblems)
	{
		if (m_pProblems)
			delete []m_pProblems;

		m_pProblems=(SPropProblemArray*) new long[pProblems->cProblem*3+1];
		::CopyMemory(m_pProblems,pProblems,
			sizeof(long)*(pProblems->cProblem*3+1));
		m_pWAB->FreeBuffer(pProblems);
		return m_pProblems;
	}

	//no problem - it is ok
	//save it now
	hRslt=m_pUser->SaveChanges(KEEP_OPEN_READWRITE);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
	}
	return NULL;

}
	
const SPropTagArray* COXMailUser::GetPropertiesList()
{
	//This function returns pointer to readonly list of 
	//the properties. The pointer is temporary and couldnot
	//be saved. You do not have to free this memory.

	ASSERT(m_pUser);
	ASSERT(m_pWAB);

	m_nError=NULL;
	SPropTagArray* pTags;

	HRESULT hRslt=m_pUser->GetPropList(NULL, &pTags);
	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		if (pTags)
			m_pWAB->FreeBuffer(pTags);
		return NULL;
	}
	if (m_pTags)
		delete []m_pTags;
	
	ASSERT(pTags);
	ASSERT(pTags->cValues);

	DWORD dwSize=pTags->cValues+1;
	m_pTags=(SPropTagArray*) new long[dwSize]; 
	::ZeroMemory(m_pTags,sizeof(long)*dwSize);
	::CopyMemory(m_pTags,pTags,sizeof(ULONG)*dwSize);

	hRslt=m_pWAB->FreeBuffer(pTags);
	return m_pTags;
	
}
	

const SPropProblemArray* COXMailUser::DeleteProperties(SPropTagArray* pPropTagArray)
{

	//This function deletes properties for the mail user.
	//In case of some problems, problem array will be
	//returned. Pointer to this array is temporary and could not
	//be saved. To manage by this array you have to copy it.
	//Next time this function will be called, the array may be freed
	// and allocated new one. The array is read only and you do
	//not have to free the memory occuppied by this array.
	ASSERT(m_pWAB);
	ASSERT(m_pUser);
	ASSERT(pPropTagArray);
	ASSERT(pPropTagArray->cValues);

	SPropProblemArray* pProblems=NULL;

	m_nError=NULL;
	HRESULT hRslt=m_pUser->DeleteProps(pPropTagArray, &pProblems);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		if (pProblems)
			m_pWAB->FreeBuffer(pProblems);
		return FALSE;
	}

	if (!pProblems)
		return NULL;

	if (m_pProblems)
		delete []m_pProblems;

	m_pProblems=(SPropProblemArray*) new long[pProblems->cProblem*3+1];

	::CopyMemory(m_pProblems,pProblems,sizeof(long)*(pProblems->cProblem*3+1));
	m_pWAB->FreeBuffer(pProblems);
	return m_pProblems;
}
	

const SPropTagArray* COXMailUser::RegisterPropertyName(UINT nCount,
		MAPINAMEID* pPropNames, ULONG nFlags)
{
	//the function registers new propety name
	//and returns  pointer to propertytags for the registered
	//properties. This array is readonly and 
	//and does not have to be freed  
	ASSERT(m_pUser);
	ASSERT(m_pWAB);
	ASSERT(pPropNames);

	m_nError=NULL;
	SPropTagArray* pTags=NULL;

	HRESULT hRslt=m_pUser->GetIDsFromNames(nCount,&pPropNames,nFlags,
		&pTags);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		if (pTags)
			m_pWAB->FreeBuffer(pTags);
		return NULL;
	}

	ASSERT(pTags);

	if (m_pTags)
		delete []m_pTags;
	m_pTags=(SPropTagArray*) new long[1+pTags->cValues];
	::CopyMemory(m_pTags,pTags,sizeof(long)*(1+pTags->cValues));
	m_pWAB->FreeBuffer(pTags);

	return NULL;
}


BOOL COXMailUser::GetID(ULONG *pCount, ENTRYID **ppEntry)
{
	//The functions returns ENTRYID of the user.
	//This value is readonly and you do not have to change it

	ASSERT(m_pWAB);
	ASSERT(pCount);
	ASSERT(ppEntry);
	*pCount=NULL;
	*ppEntry=NULL;

	if (m_pEntry)
	{
		*ppEntry=m_pEntry;
		*pCount=m_nEntryCount;
		return TRUE;
	}

	//the ENTRYID did not retrieved yet.
	//get it now

	SPropTagArray tagAr;
	tagAr.cValues=1;
	tagAr.aulPropTag[0]=PR_ENTRYID;
	const COXPropValue* pProp=GetProperties(&tagAr);
	
	BOOL bRet=FALSE;

	if (pProp)
	{
		HRESULT hRslt=m_pWAB->AllocateBuffer(pProp->Value.bin.cb,
			(void**) &m_pEntry);
		if (hRslt==S_OK)
		{
			*pCount=m_nEntryCount=pProp->Value.bin.cb;
			::CopyMemory((void*) m_pEntry, (void*) pProp->Value.bin.lpb,
				*pCount);
			*ppEntry=m_pEntry;
			bRet=TRUE;
		}
		else
			m_pEntry=NULL;

		delete []pProp;
	}
	return bRet;
}

void COXMailUser::GetEmail(CString& sEmail)
{
	//easy function to get email property
	//of the user

	sEmail=_T("");
	SPropTagArray tagAr;
	tagAr.cValues=1;
	tagAr.aulPropTag[0]=PR_EMAIL_ADDRESS;
	COXPropValue* pProp=GetProperties(&tagAr);
	if (pProp)
	{
		sEmail=*pProp->GetStringProperty();
		delete []pProp;
	}
}

void COXMailUser::GetName(CString& sName)
{

	//retrieves Display name property
	SPropTagArray tagAr;
	tagAr.cValues=1;
	tagAr.aulPropTag[0]=PR_DISPLAY_NAME;
	COXPropValue* pProp=GetProperties(&tagAr);
	
	if (pProp)
	{
		sName=*pProp->GetStringProperty();
		delete []pProp;
	}
}

//////////////////////////////////////////////////////////////////////
// COXWABook
//////////////////////////////////////////////////////////////////////

COXWABook::COXWABook(HWND hWnd, LPCTSTR lpszBookPath)
: m_hWab(NULL), m_hWnd(hWnd), m_sBookPath(lpszBookPath),
 m_lpfnWabOpen(NULL), m_pIAddrBook(NULL),m_pIWABObject(NULL),
 m_nError(NULL)
{
	
}

COXWABook::~COXWABook()
{
	if (m_pIWABObject)
		m_pIWABObject->Release();
	if (m_pIAddrBook)
		m_pIAddrBook->Release();
	if (m_hWab)
		::FreeLibrary(m_hWab);

}


BOOL COXWABook::Init()
{
	m_nError=NULL;

	//call this function once
	if (m_hWab)
	{
		return FALSE;
	}
	
	//get location of the library
	CString sPath=GetWabLocation();
	if (sPath.IsEmpty())
		return FALSE;


	//try to load the library
	m_hWab=::LoadLibrary((LPCTSTR) sPath);
	if (!m_hWab)
	{
		TRACE2("COXWABook::Init() Line %d error %d\r\n",
			__LINE__,GetLastError());
		return FALSE;
	}
	
	//find WabOpen function
	m_lpfnWabOpen=(long (__stdcall *)(struct IAddrBook ** ,struct IWABObject ** ,
		struct _tagWAB_PARAM *,unsigned long)) ::GetProcAddress(m_hWab, OX_WAB_OPEN);

	if (!m_lpfnWabOpen)
	{
		
		TRACE2("COXWABook::Init() Line %d error %d\r\n",
			__LINE__,GetLastError());
		return FALSE;
	}

	WAB_PARAM stWabParam;

	stWabParam.cbSize=sizeof(WAB_PARAM);
	stWabParam.hwnd=m_hWnd;

#if (defined _UNICODE) && ((defined WABIMPORTPARAM) || (_MFC_VER >= 0x0700))
	char szBuffer[_MAX_PATH];
#endif // _UNICODE

	if (m_sBookPath.IsEmpty())
	{
		stWabParam.szFileName=NULL;
	}
	else
	{
#if (defined _UNICODE) && ((defined WABIMPORTPARAM) || (_MFC_VER >= 0x0700))
		::WideCharToMultiByte(CP_ACP, WC_SEPCHARS, m_sBookPath, m_sBookPath.GetLength(),
			szBuffer, _MAX_PATH, NULL, NULL);
		stWabParam.szFileName = szBuffer;
#else
		stWabParam.szFileName = (TCHAR*)(LPCTSTR) m_sBookPath;
#endif
	}

	stWabParam.ulFlags=NULL;

	//get IAddrBook interface and IWABObject
	HRESULT hRslt=m_lpfnWabOpen(
		&m_pIAddrBook,&m_pIWABObject,&stWabParam,NULL);

	//check for results
	if (FAILED(hRslt))
	{
		TRACE2("COXWABook::Init() Line %d error %d\r\n",
			__LINE__,GetLastError());
		return FALSE;
	}
	
	//make sure we have 2 interfaces
	ASSERT(m_pIAddrBook);
	ASSERT(m_pIWABObject);

//	m_pIAddrBook->AddRef();
//	m_pIWABObject->AddRef();
	
	return TRUE;
}

CString COXWABook::GetWabLocation()
{
	HKEY hKey;
	CString sPath;
	TCHAR szBuffer[_MAX_PATH];
	DWORD dwSize=_MAX_PATH-1;

	if (ERROR_SUCCESS==(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		OX_REGISTRY_WAB_LOCATION,NULL,KEY_READ,&hKey)))
	{
		//wab32.dll found
		DWORD dwType=REG_EXPAND_SZ;

		
		HRESULT hRslt=::RegQueryValueEx(hKey,NULL,
			NULL,&dwType,(LPBYTE) &szBuffer,&dwSize);
		::RegCloseKey(hKey);

		//get path
		if (hRslt==ERROR_SUCCESS)
		{
			sPath=(LPCTSTR) szBuffer;
			return sPath;
		}
	}

	//no luck to find wab32.dll path.
	//Suppose that the system has IE version less 4.0
	//get default location
	::GetSystemDirectory((LPTSTR) &szBuffer, dwSize);
	sPath=(LPCTSTR) &szBuffer;
	if (sPath.GetAt(sPath.GetLength()-1)!=TEXT('\\'))
		sPath+=_T("\\");

	sPath+=OX_WAB_NAME;
	return sPath;
	
}



BOOL COXWABook::CreateUser(ULONG* pCount,ENTRYID** ppEntry)
{

	//This function call standart dialog to create User/Address
	//The ENTRYID that this function returns have to be freed
	//with IWABObject::FreeBuffer() function. To get pointer
	//to IWABObject, call GetWAB() function.
	//Once you have finished to work with it,
	//call IWABObject::Release()
	//
	// Sample:
	//
	// (m_pABook is COXWABook*)
	//
	// BOOL bRet=m_pABook->CreateAddress(&nCount, &pEntry);
	//
	// if (bRet)
	// IWABObject* pPAB=m_pABook->GetPAB();
	// ASSERT(pPAB);
	// ...//do something with ENTRYID
	// pPAB->FreeBuffer((void*) pEntry);
	// pPAB->Release();
	// 
	// 


	ASSERT(m_pIAddrBook);
	ASSERT(ppEntry);
	ASSERT(pCount);

	m_nError=0;
	*pCount=NULL;
	*ppEntry=NULL;

	ULONG_PTR nParam=(ULONG_PTR) m_hWnd;

	HRESULT hRslt=m_pIAddrBook->NewEntry(PtrToUint(nParam),NULL,NULL,NULL,
		NULL,NULL,pCount,ppEntry);

	if (hRslt==S_OK)
		return TRUE;
	
	m_nError=HRESULT_CODE(hRslt);
	return FALSE;

}


BOOL COXWABook::EditUser(UINT nCount, ENTRYID *pEntry)
{

	//This function calls standart dialog for 
	//editing properties

	ASSERT(m_pIAddrBook);
	ASSERT(pEntry);
	ASSERT(nCount);

	m_nError=NULL;
	ULONG hWnd=NULL;

	HRESULT hRslt=m_pIAddrBook->Details(&hWnd,NULL,NULL,nCount,
		pEntry,NULL,NULL,NULL,NULL);
	
	if (hRslt==S_OK)
		return TRUE;

	m_nError=HRESULT_CODE(hRslt);
	return FALSE;

}

COXMailUser* COXWABook::GetUserFromID(UINT nCount, ENTRYID *pEntry, UINT nFlags)
{
	
	//this function returns pointer to
	//COXMailUser object. 
	
	//NOTE: You are responsible to free this
	//object with operator delete
	ASSERT(m_pIAddrBook);
	ASSERT(pEntry);
	ASSERT(nCount);

	m_nError=NULL;
	IUnknown* pUnk=NULL;
	ULONG nType=NULL;

	HRESULT hRslt=m_pIAddrBook->OpenEntry(nCount, pEntry,
		NULL, nFlags, &nType, &pUnk);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		return NULL;
	}
	
	IMailUser* pUser=NULL;
	
	hRslt=pUnk->QueryInterface(IID_IMailUser,(void**) &pUser);
	
	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		pUnk->Release();
		return FALSE;
	}
	ASSERT(pUser);

	//the caller must free pNewUser
	COXMailUser* pNewUser= new COXMailUser(m_pIWABObject,pUser);
	return pNewUser;
	
}

BOOL COXWABook::FindUser(SPropValue* pProperty, ULONG *pCount, ENTRYID **ppEntry,
							  BOOL bDisplayDialog, LPTSTR lpszTitle)
{

	//The function makes search depending on
	//supplied property type and value.
	//If more than one matched entries has been found
	//and bDisplayDialog is TRUE, the prompt dialog
	//will be displayed to make choice.
	
	//NOTE: If the function returns pointer to the ENTRYID 
	//not equal NULL, the caller are responsible to free
	//ENTRYID by delete[]. Number of bytes in ENTRYID 
	//will be in pCount

	ASSERT(m_pIAddrBook);
	ASSERT(m_pIWABObject);
	ASSERT(pProperty);
	ASSERT(pProperty->ulPropTag);
	ASSERT(pCount);
	ASSERT(ppEntry);

	m_nError=NULL;
	ULONG ulFlags=bDisplayDialog?MAPI_DIALOG:NULL;
	ADRLIST* pAdrList=NULL;
	*pCount=NULL;
	*ppEntry=NULL;

	HRESULT hRslt=m_pIWABObject->AllocateBuffer(sizeof(ADRENTRY)+4,
		(void**) &pAdrList);
	//4 bytes - size of cEntries member of ADRLIST
	
	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		return FALSE;
	}
	
	ASSERT(pAdrList);
	pAdrList->cEntries=1;
	pAdrList->aEntries[0].cValues=1;
	pAdrList->aEntries[0].ulReserved1=NULL;
	pAdrList->aEntries[0].rgPropVals=pProperty;

	hRslt=m_pIAddrBook->ResolveName((ULONG_PTR) m_hWnd,
		ulFlags, lpszTitle, pAdrList);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		if (pAdrList)
			m_pIWABObject->FreeBuffer(pAdrList);
		return FALSE;
	}
	
	//ok, we found someone
	//try to find ENTRYID now
	BOOL bRet=FALSE;

	
	for (UINT n=0;n<pAdrList->aEntries[0].cValues;n++)
	{
		if (pAdrList->aEntries[0].rgPropVals[n].ulPropTag==PR_ENTRYID)
		{
			*pCount=pAdrList->aEntries[0].rgPropVals[n].Value.bin.cb;
			
			//the caller must free memory by delete []
			ENTRYID* pEntry=(ENTRYID*) new BYTE[*pCount];
			::CopyMemory((void*) pEntry,
				(void*) pAdrList->aEntries[0].rgPropVals[n].Value.bin.lpb,
				*pCount);
			*ppEntry=pEntry;
			bRet=TRUE;
			break;
		}
	}
	
	m_pIWABObject->FreeBuffer(pAdrList);
	
	return bRet;

}

void COXWABook::ShowFindDialog()
{
	//simple function to show standart find dialog
	ASSERT(m_pIWABObject);
	ASSERT(m_pIAddrBook);

	m_pIWABObject->Find(m_pIAddrBook,m_hWnd);
}

COXMailUser* COXWABook::SearchLDAP(LPTSTR lpszLDAPURL, ULONG ulFlags)
{

	//This function makes search on the internet
	//by supplied LDAP string. 
	
	//NOTE: If this function returns not NULL,
	//the caller are responsible to delete this pointer 
	//by delete operator

	ASSERT(m_pIWABObject);
	ASSERT(m_pIAddrBook);
	ASSERT(lpszLDAPURL);
	ASSERT(*lpszLDAPURL);

	IMailUser* pUser=NULL;
	m_nError=NULL;

	HRESULT hRslt;

#if (defined _UNICODE) && ((defined WABIMPORTPARAM) || (_MFC_VER >= 0x0700))
	char szBuffer[_MAX_PATH];
	::WideCharToMultiByte(CP_ACP, WC_SEPCHARS, lpszLDAPURL, -1,
		szBuffer, _MAX_PATH, NULL, NULL);
	hRslt = m_pIWABObject->LDAPUrl(m_pIAddrBook, m_hWnd,ulFlags,
		szBuffer, &pUser);
#else
	hRslt = m_pIWABObject->LDAPUrl(m_pIAddrBook, m_hWnd,ulFlags,
		lpszLDAPURL, &pUser);
#endif




	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		return NULL;
	}

	if (pUser)
	{
		return new COXMailUser(m_pIWABObject,pUser);
	}
	else
	{
		return NULL;
	}
}

BOOL COXWABook::DeleteUser(ULONG nCount, ENTRYID *pEntry)
{

	//This function deletes address entry
	//in the address book

	ASSERT(m_pIWABObject);
	ASSERT(m_pIAddrBook);
	ASSERT(nCount);
	ASSERT(pEntry);

	IABContainer* pPAB=NULL;
	pPAB=GetPAB(MAPI_MODIFY);

	if (!pPAB)
		return FALSE;

	m_nError=NULL;

//	pPAB->AddRef();
	SBinary bin;
	ENTRYLIST EntryList;
	EntryList.cValues=1;
	EntryList.lpbin=&bin;
	EntryList.lpbin[0].cb=nCount;
	EntryList.lpbin[0].lpb=(BYTE*) pEntry;

	BOOL bRet=TRUE;
	HRESULT hRslt=pPAB->DeleteEntries(&EntryList, NULL);
	
	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		bRet=FALSE;
	}

	pPAB->Release();
	return bRet;
	
}


ADRLIST* COXWABook::SelectUser(tOXWABOptions* pOptions, ULONG ulFlags,
								 LPFNABSDI lpfnABSDI, LPFNDISMISS lpfnDismiss,
								 void* lpfnDismissCtxt, ULONG ulFocus, HWND* phWndOut)
{
	//This function displays browse dialog in different modes,
	//allowing to browse address book, or select users as
	//recipients MAPI_TO, MAPI_CC and so on.

	//NOTE: If the function returns not NULL,
	//the caller are responsible to free memory
	//by IWABObject::FreeBuffer() method.
	//To know how to, see description of 
	//CreateAddress(ULONG* pCount,ENTRYID** ppEntry)
	//function

	ASSERT(m_pIAddrBook);
	ASSERT(m_pIWABObject);
	ASSERT(m_hWnd);

	m_nError=NULL;

	ADRLIST* pAdrList=NULL;

	//if no options has been specified, set default
	tOXWABOptions Options;
	pOptions=pOptions?pOptions:&Options;


	ADRPARM AdrParm;
	::ZeroMemory(&AdrParm,sizeof(AdrParm));


	//either of the styles must be, but not both
	ASSERT(ulFlags & (DIALOG_MODAL | DIALOG_SDI));
	ASSERT((ulFlags & (DIALOG_MODAL | DIALOG_SDI))!=(DIALOG_MODAL | DIALOG_SDI));

	//check styles
#ifdef _DEBUG
	if (ulFlags & AB_SELECTONLY)
		ASSERT(ulFlags & DIALOG_MODAL);
	if (ulFlags & ADDRESS_ONE)
		ASSERT(ulFlags & DIALOG_MODAL);
#endif

	//load ADRPARM structure
	AdrParm.ulFlags=ulFlags;
	AdrParm.lpfnABSDI=lpfnABSDI;
	AdrParm.lpfnDismiss=lpfnDismiss;
	AdrParm.lpvDismissContext=lpfnDismissCtxt;
	AdrParm.nDestFieldFocus=ulFocus;
	AdrParm.lpulDestComps=&pOptions->ulType1;
	

	LPTSTR lpszCaption=pOptions->sCaption.IsEmpty()?NULL:pOptions->sCaption.GetBuffer(pOptions->sCaption.GetLength());
	LPTSTR lpszNewEntry=pOptions->sNewEntry.IsEmpty()?NULL:pOptions->sNewEntry.GetBuffer(pOptions->sNewEntry.GetLength());
	LPTSTR lpszDest=pOptions->sDest.IsEmpty()?NULL:pOptions->sDest.GetBuffer(pOptions->sDest.GetLength());

	AdrParm.lpszCaption=lpszCaption;
	AdrParm.lpszNewEntryTitle=lpszNewEntry;
	AdrParm.lpszDestWellsTitle=lpszDest;

	//find out number and type of the edit boxes if any
	//set titles for the boxes if specified
	UINT nCount=0;
	BOOL bDefaultTitles=TRUE;
	LPTSTR lpszEdit[3];
	::ZeroMemory((void*) &lpszEdit[0], sizeof(LPTSTR)*3);

	if (pOptions->ulType3)
	{
		ASSERT(pOptions->ulType1);
		ASSERT(pOptions->ulType2);
		nCount=3;
		if (!pOptions->sType1.IsEmpty() ||
			!pOptions->sType2.IsEmpty() ||
			!pOptions->sType3.IsEmpty())
		{
			ASSERT( !pOptions->sType1.IsEmpty() &&
					!pOptions->sType2.IsEmpty() &&
					!pOptions->sType3.IsEmpty());
			lpszEdit[0]=pOptions->sType1.GetBuffer(pOptions->sType1.GetLength());
			lpszEdit[1]=pOptions->sType2.GetBuffer(pOptions->sType2.GetLength());
			lpszEdit[2]=pOptions->sType3.GetBuffer(pOptions->sType3.GetLength());
			bDefaultTitles=FALSE;
		}
	}
	else
		if (pOptions->ulType2)
		{
			ASSERT(pOptions->ulType1);
			nCount=2;
			if (!pOptions->sType1.IsEmpty() ||
				!pOptions->sType2.IsEmpty())
			{
				ASSERT( !pOptions->sType1.IsEmpty() &&
						!pOptions->sType2.IsEmpty());
				lpszEdit[0]=pOptions->sType1.GetBuffer(pOptions->sType1.GetLength());
				lpszEdit[1]=pOptions->sType2.GetBuffer(pOptions->sType2.GetLength());
				bDefaultTitles=FALSE;
			}

		}
		else
			if (pOptions->ulType1)
			{
				nCount=1;
				if (!pOptions->sType1.IsEmpty())
				{
					bDefaultTitles=FALSE;
					lpszEdit[0]=pOptions->sType1.GetBuffer(pOptions->sType1.GetLength());
				}
			}
	
	AdrParm.cDestFields=nCount;

	if (!bDefaultTitles && nCount)
	{
		AdrParm.lppszDestTitles=&lpszEdit[0];
	}
	else
	{
		AdrParm.lppszDestTitles=NULL;
	}

	ULONG_PTR nhWnd=(ULONG_PTR) m_hWnd;

	//we are ready
	HRESULT hRslt=m_pIAddrBook->Address(&nhWnd,&AdrParm,
		&pAdrList);

	//release all buffers of the CString objects we have exposed
	if (lpszCaption)
		pOptions->sCaption.ReleaseBuffer();
	if (lpszNewEntry)
		pOptions->sNewEntry.ReleaseBuffer();
	if (lpszDest)
		pOptions->sDest.ReleaseBuffer();
	if (lpszEdit[0])
		pOptions->sType1.ReleaseBuffer();
	if (lpszEdit[1])
		pOptions->sType2.ReleaseBuffer();
	if (lpszEdit[2])
		pOptions->sType3.ReleaseBuffer();


	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		
		//free memory
		if (pAdrList)
		{
			m_pIWABObject->FreeBuffer((void*) pAdrList);
		}
		return NULL;
	}


	//if the dialog was modeless the return walue is 
	//HWND of the dialog box

	if (phWndOut)
		*phWndOut=(HWND) nhWnd;
	
	return pAdrList;

}


IABContainer* COXWABook::GetPAB(ULONG ulFlags) 
{

	//This function returns pointer to personal address book
	//Once you have finished to work, call Release()
	//method of IABContainer

	ASSERT(m_pIWABObject);
	ASSERT(m_pIAddrBook);


	IABContainer* pPAB=NULL;
	ULONG nCount;
	ENTRYID* pEntry;
	m_nError=NULL;

	HRESULT hRslt=m_pIAddrBook->GetPAB(&nCount, &pEntry);

	if (FAILED(hRslt))
	{
		m_nError=HRESULT_CODE(hRslt);
		return NULL;
	}
	ULONG nType;
	hRslt=m_pIAddrBook->OpenEntry(nCount, pEntry, NULL, ulFlags, &nType,
		(IUnknown**) &pPAB);

	if (FAILED(hRslt))
		m_nError=HRESULT_CODE(hRslt);
	
	m_pIWABObject->FreeBuffer(pEntry);
	return pPAB;
}


COXMailUser* COXWABook::CreateUser()
{
	//This function returns pointer
	//to the COXMailUser object if succeed.
	
	//NOTE: The caller are responsible to free
	//the returned object by operator delete
	ASSERT( m_pIWABObject);

	ULONG nCount=NULL;
	ENTRYID* pEntry=NULL;
	COXMailUser* pUser=NULL;

	BOOL bRet=CreateUser(&nCount, &pEntry);
	if (bRet)
	{
		pUser=GetUserFromID(nCount, pEntry);
	}
	if (pEntry)
		m_pIWABObject->FreeBuffer(pEntry);

	return pUser;
}

BOOL COXWABook::DeleteUser(COXMailUser *pUser)
{
	//This function delete Address/User from the address book

	ASSERT(m_pIWABObject);

	ULONG nCount=NULL;
	ENTRYID* pEntry=NULL;

	if (!pUser->GetID(&nCount, &pEntry))
		return FALSE;
	BOOL bRet=DeleteUser(nCount, pEntry);
	m_pIWABObject->FreeBuffer(pEntry);

	return bRet;
}

BOOL COXWABook::EditUser(COXMailUser *pUser)
{
	//This function calls standart dialog to 
	//edit user properties
	ASSERT(m_pIWABObject);
	ULONG nCount=NULL;
	ENTRYID* pEntry=NULL;
	BOOL bRet=pUser->GetID(&nCount, &pEntry);
	
	if (!bRet)
		return bRet;
	bRet=EditUser(nCount, pEntry);

	m_pIWABObject->FreeBuffer(pEntry);
	return bRet;
}

COXMailUser* COXWABook::FindUserByEMail(CString& sEMail, BOOL bDisplayDialog, LPTSTR lpszTitle, ULONG ulFlags)
{

	//This function makes search in address book by supplied
	//email. 

	//NOTE: If the function returns not NULL,
	//the caller are responsible for freeing 
	//this object by operator delete

	ASSERT(m_pIAddrBook);
	ASSERT(m_pIWABObject);

	SPropValue pProp;
	pProp.ulPropTag=PR_DISPLAY_NAME;
	pProp.dwAlignPad=NULL;
	LPTSTR lpszEmail=sEMail.IsEmpty()?NULL:sEMail.GetBuffer(sEMail.GetLength());

#ifdef _UNICODE
	pProp.Value.lpszW=lpszEmail;
#else
	pProp.Value.lpszA=lpszEmail;
#endif

	ENTRYID* pEntry=NULL;
	ULONG nCount=NULL;

	BOOL bRet=FindUser(&pProp, &nCount, &pEntry, bDisplayDialog, lpszTitle);

	if (!bRet)
	{
		if (pEntry)
			delete []pEntry;
		return NULL;
	}
	else
	{
		COXMailUser* pUser=GetUserFromID(nCount,pEntry,ulFlags);
		if (pEntry)
			delete []pEntry;
		m_pIWABObject->FreeBuffer(pEntry);
		return pUser;
	}

}

COXMailUser* COXWABook::FindUserByName(CString &sName, BOOL bDisplayDialog,LPTSTR lpszTitle,
											   ULONG ulFlags)
{

	//This function makes search in address book by supplied
	//email. 

	//NOTE: If the function returns not NULL,
	//the caller are responsible for freeing 
	//this object by operator delete

	ASSERT(m_pIAddrBook);
	ASSERT(m_pIWABObject);

	SPropValue pProp;
	pProp.ulPropTag=PR_DISPLAY_NAME;
	pProp.dwAlignPad=NULL;
	LPTSTR lpszName=sName.IsEmpty()?NULL:sName.GetBuffer(sName.GetLength());

#ifdef _UNICODE
	pProp.Value.lpszW=lpszName;
#else
	pProp.Value.lpszA=lpszName;
#endif

	ENTRYID* pEntry;
	ULONG nCount;

	BOOL bRet=FindUser(&pProp, &nCount, &pEntry, bDisplayDialog, lpszTitle);

	if (!bRet)
	{
		if (pEntry)
			delete []pEntry;
		return NULL;
	}
	else
	{
		COXMailUser* pUser=GetUserFromID(nCount,pEntry,ulFlags);
		if (pEntry)
			delete []pEntry;
		m_pIWABObject->FreeBuffer(pEntry);
		return pUser;
	}
}





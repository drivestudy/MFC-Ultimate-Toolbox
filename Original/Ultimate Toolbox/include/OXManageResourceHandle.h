
// Version: 9.3

#if !defined(_OX_MANAGERESOURCEHANDLE_)
#define _OX_MANAGERESOURCEHANDLE_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXManageResourceHandle
{
public:
	COXManageResourceHandle(HINSTANCE hNewResourceHandle=AfxGetInstanceHandle())
	{
		m_hOrigResourceHandle=AfxGetResourceHandle();
		AfxSetResourceHandle(hNewResourceHandle);
	}

	virtual ~COXManageResourceHandle()
	{
		AfxSetResourceHandle(m_hOrigResourceHandle);
	}

protected:
	HINSTANCE m_hOrigResourceHandle;
};


#endif	//	_OX_MANAGERESOURCEHANDLE_
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// afx.h is normally included within afxwin.h, but we do so 
// here so that most of the MFC inline asserts will 
// use our Advanced Asserts mechanism. 
#include <afx.h>             // MFC version of ASSERT
#include "OXAdvancedAssert.h"  // Redefine ASSERT our way. 

#include <afxwin.h>         // MFC core and standard components

#include <afxext.h>         // MFC extensions
#include <afxsock.h>		// MFC socket extensions
#include <wininet.h>
#include <afxdisp.h>        // MFC OLE automation classes
#include <afxpriv.h>       	// MFC extensions for help constants
#include <afxmt.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxole.h>       	// MFC extensions for OLE 2.0

#ifdef WIN32
	#include <afxcmn.h>         // MFC new WIN95 components
	#include <afxtempl.h>		// MFC extensions for Templates
#endif

// If TEXT macro not defined in WIN16, define now
#if !defined(WIN32) && !defined(TEXT)
#define TEXT(s) s
#endif

#include <shlobj.h>				// for IShellLink in COXShortcut


#define OX_CUSTOMIZE_NOTSUPPORTING_DOCKING_FRAMEWORK

#define OX_CUSTOMIZE_COMMANDS
#define OX_CUSTOMIZE_TOOLBARS
#define OX_CUSTOMIZE_SHORTKEYS
#define OX_CUSTOMIZE_WORKSPACE_STATE
#define OX_CUSTOMIZE_TABBEDMDI
#define OX_CUSTOMIZE_INSTANCE_MANAGER
#define OX_CUSTOMIZE_TRAYICON 
#define OX_CUSTOMIZE_SPLASHWINDOW 
#define OX_CUSTOMIZE_CAPTION
#define OX_CUSTOMIZE_BACKGROUND
//#define OX_CUSTOMIZE_INTELLIMOUSE (not available yet)
//#define OX_CUSTOMIZE_COOLCONTROLS (not available yet)

#define OX_PREVIEW_RTF
#define OX_PREVIEW_GRAPHICS
	#define OX_PREVIEW_BMP
		#define OXDIB_SUPPORTJPEG
	#define OX_PREVIEW_METAFILE
	#define OX_PREVIEW_ICON
#define OX_PREVIEW_SOUND
#define OX_PREVIEW_HTML
#define OX_PREVIEW_PLAINTEXT



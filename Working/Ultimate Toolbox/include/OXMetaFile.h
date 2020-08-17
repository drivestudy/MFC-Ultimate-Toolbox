// ==========================================================================
// 					Class Specification : COXMetaFile
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class(does not have any objects)
//	NO	Derived from CWnd

//	NO	Is a Cwnd.                     
//	NO	Two stage creation(constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource(template)

//	NO	Persistent objects(saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////
// OXMetaFile.h : header file for the METAFILE Class
// Function: MetaFile class, that plays all kinds of metafiles 
//		(Windows metafiles, WMF with Aldus header, enhanced metafiles)
//		from file or from resource.

/*

COXMetaFile is a class that allows access to MetaFiles in different formats: 

	old Windows 3.x metafile(*.wmf) 
	Windows MetaFile with Aldus header(*.wmf) 
	new enhanced Windows MetaFile(*.emf) 


There are two ways to load the metafile: 
	
	1. The files can be loaded from disk. Use the method LoadFile(), if you don't set 
		the filename to load then COXMetaFile prompt the user for it using standard 
		CFileDialog.
	2. The Metafile can be loaded from the application resource file. You just call 
		LoadFile(nID) with the corresponding resource ID set. Normally it's a user 
		defined resource type called "METAFILE", but other names are possible. This is a 
		useful feature if you want to start your program with a screen picture loaded from 
		within the program or come up with a default picture if a metafile is not found 
		on disk.

It's very straightforward to display the metafile; you need no more than one line of code: 
	
	pDoc->m_MetaFile.PlayFile(pDC); 

If you want to fit the metafile into the current client area of your view, just pass the 
size of this rectangle as an additional parameter: 

    GetClientRect(&rectClient); 
    pDoc->m_MetaFile.PlayFile(pDC, &rectClient); 

all the rest is done by COXMetaFile for you. 

COXMetafile even creates enhanced metafiles: It needs one line to create a metafile, than 
use the pointer to device context for your drawing operations and finally close the 
Metafile - that's it. 
  
 

1.	Loading metafile from resource

COXMetaFile supports all different metafile formats and recognises them internally. 
In your project's Resource View just right click and import the EMF, WMF or Aldus 
metafile.  By default you can use "METAFILE" as resource type. 

	1)	Add a member variable to your document: 
		
			COXMetaFile m_MetaFile;

	2)	Load the metafile; for example in the CDocument constructor: 

			m_MetaFile.LoadFile(IDR_EMF); 

		In case your resource type name is not METAFILE, use 
			m_MetaFile.LoadFile(IDR_EMF, "myresourceTypeName");


  
2.	Loading a metafile from disk

COXMetaFile support all different metafile formats and recognises them internally. 
All you need to do is to call a LoadFile function. As an argument you can provide path 
to the file. If you don't know the filename and want user to pick it then use this 
function without any parameters and standard CFileDialog will be displayed to get a 
file name. E.g. in the overridden CDocument::OnFileOpen(): 
	
	if(! m_MetaFile.LoadFile()) 
          AfxMessageBox("Error opening the MetaFile"); 
	UpdateAllViews(NULL); 
  
If you want to reload the metafile, which has been loaded by the latest call to LoadFile
just use:

	m_MetaFile.LoadFile(m_MetaFile.GetFileName()); 
  


3.	Play a metafile

All you need to do is to call the PlayFile method in your CMyView::OnDraw() method. 

     pDoc->m_MetaFile.PlayFile(pDC); 

If you want to display metafile in the client rectangle maintaining the metafile's ratio, 
you have to pass this rectangle to PlayFile: 

     pDoc-m_MetaFile.PlayFile(pDC, &rectClient); 

Then the MetaFile class internally adjusts MapMode, WindowExt and ViewportExt according 
to the Metafile size.
  

  
	
4.	Create a Metafile(enhanced format)

COXMetaFile only creates enhanced metafiles. The CreateFile() method prompts the user for 
a filename if it is not passed as a parameter. Perform all drawing operations on the DC 
returned by CreateFile() and finally close the metafile using CloseFile().  

*/

#ifndef _OX_METAFILE_
#define _OX_METAFILE_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"


#define META32_SIGNATURE        0x464D4520      // ' EMF'
#define ALDUS_ID				0x9AC6CDD7
#define APMSIZE					22				// needed as workaround for different alignement
#define MAX_BBOX				32767			// Max. vert for BBox in Aldus Header


enum OXMETAFILETYPE { OXMETAFILE_ENHANCED, OXMETAFILE_ALDUS, 
OXMETAFILE_WIN16, OXMETAFILE_NOTDEFINED=-1 };

// Header for Aldus MetaFile(see Born: file Formats handbook)
typedef struct {		//	comment					  size
		long key;		// ALDUS_ID						4
		short hmf;		// unused - alwalys 0			2
		short left;		// boundingbox					2
		short top;		//								2
		short right;	//							    2	
		short bottom;	//								2
		short inch;		// metafile units / inch		2
		long reserved;  // NULL							4
		short checksum;	//								2
} ALDUSHEADER;

class OX_CLASS_DECL COXMetaFile  
{
public:

	// constructor & configuration
	// ============================================
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect :		Contructor of object
	//				    It will initialize the internal state
	COXMetaFile();


	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect :		empty destructor
	virtual ~COXMetaFile();


	// --- In  :		a pointer to a function that is called for every
	//					record in the metafile
	// --- Out :   
	// --- Returns:		always true
	// --- Effect :		Sets the function called for every single record
	//					in the metafile(see PlayRecord). 
	//	You can set the function pointer directly in the
	//	third parameter in PlayRecord function instead of using SetMetaFileProc
	inline BOOL SetMetaFileProc(ENHMFENUMPROC lpEnhMetaFunc) {	
		m_lpEnhMetaProc = lpEnhMetaFunc; 
		return TRUE; 
	}


	// Load Metafile(emf. wmf or Aldus) from file
	// ===========================================
	// --- In  :		<strFileName>  fileName; 
	//		            if empty, a CFileDlg prompts for user input
	// --- Out :   
	// --- Returns :	handle to enhanced windows metafile(even if file was
	//					old style WMF or Aldus format
	// --- Effect :		Brings up the Open file common dialog 
	//					gets a pointer to the data and finally leaves 
	//					all work to LoadFile(lpvData)
	HENHMETAFILE LoadFile(CString strFileName = _T(""));

	
	// --- In  :		<nID>   ID of resource; it can by any metafile type
	//					<strResType> name of the resource type, 
	//						default is "MetaFile"
	// --- Out : 
	// --- Returns :	handle to enhanced windows metafile
	// --- Effect :		loads metafile in any format from resource. 
	HENHMETAFILE LoadFile(int nID, CString strResType = _T("METAFILE"));


	// Create an EMF File
	// ==================
	// --- In  :		<strFileName>  fileName; 
	//		            if empty, a CFileDlg prompts for user input
	//			        <strDescript> Desription in MetaFile
	//			        <sDefExt> default extension used in Open dialog
	//			        <sFilter> filter used in Open dialog
	// --- Out : 
	// --- Returns :	handle to device context for drawing actions
	// --- Effect :		creates EMF on disk
	// --- how to use:
	//		   1) draw the contents in the DC returned by GethDCMeta
	//		   2) close after drawing with CloseFile
	HDC CreateFile(CString strFileName = _T(""), CString strDescript = _T(""), 
							CString sDefExt = _T("emf"), 
							int nIDFilter=IDS_OX_METAFILEFILTER); //"EMF Files(*.emf)|*.emf|All Files(*.*)|*.*|"

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect :		closes a  MetaFile, which has been created using
	//					CreateFile(need not to be called after LoadFile())
	void CloseFile();


	// Play Metafile
	// ========================
	// --- In  :		<pDC> the device context to draw the metafile in
	// --- Out : 
	// --- Returns :	TRUE, if everything is ok
	//					FALSE, if error ocurred in PlayEnhMetaFile
	// --- Effect :		Plays the metafile in the RECT which is 
	//					defined in the metaFiles header, i.e. in original size
	//					The metafile must have been loaded with LoadFile befor;
	//					There is no need to call CloseFile afterwards.
	BOOL PlayFile(CDC* pDC);

	// --- In  :		<pDC> the device context to draw the metafile in
	//					<pClientRect> metafile is fit to this Rect maintaining aspect ratio
	// --- Out : 
	// --- Returns :	TRUE, if everything is ok
	//					FALSE, if error ocurred in PlayEnhMetaFile
	// --- Effect :		Fits the metafile into Rect and plays it
	//					The metafile must have been loaded with LoadFile befor;
	//					There is no need to call CloseFile afterwards.
	BOOL PlayFile(CDC* pDC, CRect* pClientRect);

	// --- In  :		<pDC> the device context to draw the metafile in
	//					<pClientRect> metafile is fit to this Rect maintaining aspect ratio
	//					<lpEnhMetaProc> function to be called for every record
	//					<plData> a pionter to data passed to the function EnhMetaProc
	// --- Out : 
	// --- Returns :	TRUE, if everything is ok
	//					FALSE, if error ocurred in EnumEnhMetaFile
	// --- Effect :		Fits the metafile into Rect and plays it record by record.
	//					For every record <lpEnhMetaProc> is called and plData passed
    //			        as parameter
    //  Use SetMetaFileProc() if you want to pass the function
	//  pointer from some other place in your code.
	BOOL PlayRecords(CDC* pDC, CRect crect, ENHMFENUMPROC lpEnhMetaProc = NULL, 
		LPVOID plData = NULL);


	// Retrieve Information about OXMetaFile
	// =========================================
	// --- In :
	// --- Out :
	// --- Returns :	the bounding Rect as defined in the Metafiles Header
	// --- Effect :		get bounding rect
	inline CRect GetBoundingRect() const { return m_rectBounds; }

	// --- In :
	// --- Out :
	// --- Returns :	the normalized rect(in 0.01-mm units) as defined in the 
	//					Metafile Header
	// --- Effect :		get normalized metafile rect
	inline CRect GetNormalizedRect() const { return m_rectNormalized; }

	// --- In :
	// --- Out :
	// --- Returns :	the size of the bounding Rect as defined in the Metafiles Header
	// --- Effect :		get size of metafile
	inline CSize GetSize() const {	
		return CSize(m_rectBounds.right-m_rectBounds.left, 
			m_rectBounds.bottom-m_rectBounds.top);  
	}

	// --- In :
	// --- Out :
	// --- Returns :	the size(in 0.01-mm units) of the normalized bounding rect 
	//					as defined in the Metafile Header
	// --- Effect :		get normalized size of metafile
	inline CSize GetNormalizedSize() const {	
		return CSize(m_rectNormalized.right-m_rectNormalized.left, 
			m_rectNormalized.bottom-m_rectNormalized.top);  
	}

	// --- In :
	// --- Out :
	// --- Returns :	the name of the last Metafile loaded sucessfully with LoadFile.
	// --- Effect :		get filename of the last used metafile
	inline CString GetFileName() const { return m_strFileName; }

	// --- In :
	// --- Out :
	// --- Returns :	the description of the last Metafile as defined in the header
	// --- Effect :		get description string
	inline CString GetDescription() const { return m_strDescription; }

	// --- In :
	// --- Out :
	// --- Returns :	returns handle to Metafile
	// --- Effect :		get handle to Enhanced Metafile in case you want to use
	//		            special API-operations
	inline HENHMETAFILE GethEMF() const { return m_hEMF; }

	// --- In :
	// --- Out :
	// --- Returns :	returns DC 
	// --- Effect :		is only valid if you have created the metafile with CreateFile
	inline HDC GethDCMeta() const { return m_hDCMeta; }

	// --- In :
	// --- Out :
	// --- Returns :	returns TRUE if the original format of loaded metafile is 
	//					Aldus metafile
	// --- Effect :		retrieves flag that specifies whether the loaded metafile 
	//					original format is Aldus
	inline BOOL IsAldus() const { return(m_metafileType==OXMETAFILE_NOTDEFINED); }

	// --- In :
	// --- Out :
	// --- Returns :	returns TRUE if the original format of loaded metafile is 
	//					enhanced metafile
	// --- Effect :		retrieves flag that specifies whether the loaded metafile 
	//					original format is enhanced
	inline BOOL IsEnhanced() const { return(m_metafileType==OXMETAFILE_ENHANCED); }

	// --- In :
	// --- Out :
	// --- Returns :	returns TRUE if the original format of loaded metafile is 
	//					old Windows metafile 
	// --- Effect :		retrieves flag that specifies whether the loaded metafile 
	//					original format is old Windows
	inline BOOL IsWin16() const { return(m_metafileType==OXMETAFILE_WIN16); }

protected:
	// --- In :			pointer to the metafiles data no matter if from disk or 
	//					from resource
	// --- Out :
	// --- Returns :	handle to enhanced metafile which is also interally stored 
	//					in m_hEMF
	// --- Effect :		this is the one function doing all the work. It tests 
	//					different header formats and finally calls the right API 
	//					function to process the metafile
	HENHMETAFILE LoadFile(LPVOID lpvData);

	CRect		   m_rectBounds;
	CRect		   m_rectNormalized;
	ENHMFENUMPROC  m_lpEnhMetaProc;
	CString        m_strFileName;
	CString		   m_strDescription;
	HENHMETAFILE   m_hEMF;
	HDC            m_hDCMeta;
	OXMETAFILETYPE m_metafileType;
};


/////////////////////////////////////////////////////////////////////////////

#endif // _OX_METAFILE_

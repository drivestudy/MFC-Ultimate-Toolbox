// ==========================================================================
//						   Class Specification 
// 							  COXHistoryCtrl
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
						  
// //////////////////////////////////////////////////////////////////////////

/*
	DESCRIPTION

The	COXHistoryCtrl class is	COXScrollWnd based class which is designed to provide
facilities to display and save log info	(history info).	We define log info as a	set	
of entries (items) where each entry	is displayed on	new	line. Every	log	entry
(history entry,	history	item) is displayed on a	single line. Optionally	you	can	
specify	an offset for any such entry and also you can set text and background color
that will be used to display an	entry. Also	you	can	add	history	entries	that will be 
automatically time stamped.	Scrolling of history items in the control is supported 
automatically through base COXScrollWnd	class functionality. 

In order to	avoid the over population of the control with history entries the max 
number of displayed	items can be specified.	When this number is	reached	and	new	entry
is being added the item	at the top will	be removed.	The	contents of	the	control
can	be cleared at any time either.

This class is thread safe. That	means that you can safely populate it with new 
entries	calling	corresponding functions	from different threads.

Log	info output	can	be also	forwarded in a file. You might specify the name	of the 
log	file or	alternatively choose to	create a time stamped file that	will be
automatically named	based on the day of	creation (note that	if an application that 
uses COXHistoryCtrl	object runs	longer then	one	day	the	multiple log files will	be 
created	for	every day).



	Usage

In order to	use	COXHistoryCtrl object in your application you have to create it
using standard CWnd::Create	function. 

After control was successfully created you might want to set options to	save 
log	info in	a log file (if you don't care about	log	file you might skip	this 
paragraph).	In order to	do that	first of all you have to call the following
function specifying	TRUE as	a parameter:

	void EnableLog(BOOL	bYesNo);

Then if	you	want to	use	time stamped log file you have to call:	

	void EnableTimeStampedLog(BOOL bYesNo);

and	specify	TRUE as	a parameter. Otherwise you have	to explicitly specify the 
name of	the	log	file using:

	void SetLogFileName(CString	sLogFileName);


After you are done with	creating the control and specifying	log	file options you can
start populating the COXHistoryCtrl	object using following functions:

	BOOL AddLine(CString string, BOOL bAddToLog=TRUE);
	BOOL AddStampedLine(CString	string,	BOOL bAddToLog=TRUE);
	BOOL AppendToLine(CString string, BOOL bAddToLog=TRUE);

At any time	the	contents of	the	control	can	be cleared using:
	
	BOOL ClearHistory();


History	control	entries	can	be displayed using different text color	and	alignment and
background color. Also you can specify an offset from the left side	of the control's 
window client area that	will be	used while displaying a	history	entry. Use the
following functions	in order to	specify	these display options:

	void SetTextColor(COLORREF color);
	void SetBackColor(COLORREF color);
	void SetTextAlignment(int nAlignment);
	void SetOffset(int nOffset);

Note that this function	must be	called before(!) adding	new	entries	that must be 
displayed using	new	settings. Existing entries will	not	be affected	by this	functions.


  
The	sample that	demonstrates COXHistoryCtrl	class is called	VisualStudioLikeDemo and 
can	be found in	the	.\Samples\advanced\VisualStudioLikeDemo	subdirectory of	your 
Ultimate Toolbox directory.	This is	an advanced	sample that	demonstrates a lot of 
Ultimate Toolbox classes working together. History control examples	are	located	in 
the	bottom docking window (TabView container with "Build", "Debug",	...	panes)



Dependency:
	#include "OXHistoryCtrl.h"


Source code	files:
	"OXHistoryCtrl.cpp"		
	"OXScrollWnd.cpp"		-	COXScrollWnd implementation
*/

#ifndef	_OXHISTORYCTRL_H_
#define	_OXHISTORYCTRL_H_

#if	_MSC_VER >=	1000
#pragma	once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef	__AFXTEMPL_H__
#include <afxtempl.h>
#define	__AFXTEMPL_H__
#endif

#include "OXScrollWnd.h"

#include "UTB64Bit.h"


// history item	descriptor
typedef	struct _tagHISTORYENTRY	
{
	// history item	text
	CString	m_sText;
	// item	text color 
	COLORREF m_clrText;
	// item	background color
	COLORREF m_clrBack;
	// offset in points	from the left side
	int	m_nOffset;
	// text	alignment. Refer for CDC::DrawText() function documentation	
	// for details on possible values (nFormat parameter description)
	int	m_nAlignment;

	// constructor
	_tagHISTORYENTRY(CString sText=_T(""),
		COLORREF clrText=GetSysColor(COLOR_WINDOWTEXT),
		COLORREF backColor=GetSysColor(COLOR_WINDOW),
		int	nOffset=0, int nAlignment=DT_LEFT)
	{
		m_sText=sText;
		m_clrText=clrText;
		m_clrBack=backColor;
		m_nOffset=nOffset;
		m_nAlignment=nAlignment;
	}

	// assignment operator
	_tagHISTORYENTRY& operator=(const _tagHISTORYENTRY&	entry)
	{
		if(this==&entry)
			return *this;

		m_sText=entry.m_sText;
		m_clrText=entry.m_clrText;
		m_clrBack=entry.m_clrBack;
		m_nOffset=entry.m_nOffset;
		m_nAlignment=entry.m_nAlignment;

		return *this;
	}

} HISTORYENTRY;


//history-logging control class
class OX_CLASS_DECL	COXHistoryCtrl : public	COXScrollWnd
{
	DECLARE_DYNCREATE(COXHistoryCtrl)

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXHistoryCtrl();

protected:
	// font	to be used to display history items
	CFont m_font;

	//log file info
	//
	// filename	of the log file
	CString	m_sLogFileName;
	// flag	that specifies that	all	history	items will be saved	in log file,
	// otherwise the history will be only displayed	in the window
	BOOL m_bEnableLog;
	// flag	that specifies that	the	log	file should	be automatically named depending
	// on the day it was created. That means that if the application that populates
	// history control runs	for	more then 1	day	then for every day it runs new
	// log file	will be	created	with cooresponding date. The filename format 
	// is "ddmmyyyy".
	BOOL m_bTimeStampedLog;
	// if m_bEnableLog and	m_bTimeStampedLog flags	are	set	to TRUE	then the new 
	// log file	will be	automatically created for every	day	it runs	(so	if process 
	// run for three days you will get three log files with	filenames that specify 
	// the date	they were created in the format	"ddmmyyyy")
	int	m_logDay, m_logMonth, m_logYear;
	// log file	CFile object
	CFile m_fileLog;
	// flag	that specifies that	if log file	with specified name	already	exist it
	// will	be truncated or	new	history	entries	will be	added at the end of	the
	// existing	contents. By default it	is set to TRUE (if log file	exists its 
	// contents	will be	cleared	before adding new entry)
	BOOL m_bTruncateLogFile;

	// settings	for	displaying history items that will be added	in future. That	
	// means that whenever new history item	is added we	will use these parameters
	// to define text color, alignment and offset and background color
	//
	COLORREF m_clrText;
	COLORREF m_clrBack;
	COLORREF m_clrLeftOverBack;
	int	m_nAlignment;
	int	m_nOffset;
	//
	////////////////////////////

	// the height of entry
	int	m_nHeight;
	// the symbol size
	CSize m_sizeSymbol;
	
	// list	of history items (entries)
	CList<HISTORYENTRY,HISTORYENTRY&> m_arrEntries;
	// the maximum number of entries that might	be displayed in	the	control's 
	// window. If this number is reached then whenever a new item is added the 
	// one at the top is being removed.	By default this	number is set to 100.
	int	m_nMaxNumEntries;

	//max line length
	int	m_nMaxLengthEntry;

	// format string for displaying	time stamp next	to a history item (time	
	// stamp will be automatically added to	a history entry	when you call 
	// AddStampedLine()	function). Refere to COleDateTime::Format function
	// for information on how this format string might look	like
	CString	m_sDateFormatString;

	// critical	section	variable, allows to	populate history control from 
	// different threads in	a safe manner
	CRITICAL_SECTION m_criticalSection;

public:
	// --- In  :	pFont	-	pointer	to new font	object
	//				bRedraw	-	if TRUE	the	contents will be redrawn right away
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets new font to display history entries
	void SetFont(CFont*	pFont, BOOL	bRedraw=TRUE);


	// --- In  :	color	-	new	text color
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the text color	for	"will be added in future" history items. 
	//				New	text color doesn't affect the existing history entries
	inline void	SetTextColor(COLORREF color) { m_clrText=color;	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Text color
	// --- Effect:	Retrieves the current text color for "will be added	in future" 
	//				history	items. Whenever	new	history	entry is added it uses
	//				this value as its text color setting.
	inline COLORREF	GetTextColor() const { return m_clrText; }


	// --- In  :	color	-	new	background color
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the background	color for "will	be added in	future"	history	
	//				items. New background color	doesn't	affect the existing	history	
	//				entries
	inline void	SetBackColor(COLORREF color) { m_clrBack=color;	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Background color
	// --- Effect:	Retrieves the current background color for "will be	added in 
	//				future"	history	items. Whenever	new	history	entry is added it uses
	//				this value as its background color setting.
	inline COLORREF	GetBackColor() const { return m_clrBack; }

	// --- In  :	color	-	new	background color
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the leftover background color.
	inline void	SetLeftOverBackColor(COLORREF color) { m_clrLeftOverBack=color;	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Leftover Background	color
	// --- Effect:	Retrieves the current leftover background color.
	inline COLORREF	GetLeftOverBackColor() const { return m_clrLeftOverBack; }

	// --- In  :	nAlignment	-	new	text alignment.	It could take the same value
	//								as nFormat parameter in	CDC::DrawText()	function
	//								(DT_LEFT, DT_CENTER, DT_RIGHT)
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the text alignment	for	"will be added in future" history 
	//				items. New text	alignment doesn't affect the existing history 
	//				entries
	inline void	SetTextAlignment(int nAlignment) { m_nAlignment=nAlignment;	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Text alignment
	// --- Effect:	Retrieves the current text alignment for "will be added	in future" 
	//				history	items. Whenever	new	history	entry is added it uses
	//				this value as its text alignment setting.
	inline int GetTextAlignment() const	{ return m_nAlignment; }


	// --- In  :	nOffset	-	new	offset in pixels for history entries
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the offset	in pixels for "will	be added in	future"	history	
	//				items. New offset doesn't affect the existing history entries
	inline void	SetOffset(int nOffset) { m_nOffset=nOffset;	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	History	item offset	from the left side of the control
	// --- Effect:	Retrieves the current offset in	pixels for "will be	added in 
	//				future"	history	items. Whenever	new	history	entry is added it 
	//				uses this value	as its offset setting.
	inline int GetOffset() const { return m_nOffset; }


	// --- In  :	sLogFileName	-	filename of	the	log	file
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the name of the log file. Note	that if	time stamped 
	//				log	file is	enabled	then the log file will be named	
	//				automatically. Still the file extension	will be	retrieved
	//				from this filename
	inline void	SetLogFileName(CString sLogFileName) {
		m_sLogFileName=sLogFileName;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Filename of	the	log	file
	// --- Effect:	Retrieves the name of the log file.	Note that if time stamped 
	//				log	file is	enabled	then the log file will be named	
	//				automatically. Still the file extension	will be	retrieved
	//				from this filename
	inline CString GetLogFileName()	const {	return m_sLogFileName; }


	// --- In  :	bYesNo	-	if TRUE	then new history entries will be saved
	//							in the log file, otherwise the log file	will be	
	//							closed if any was opened before
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Enables/disables the output	of the history entries in the 
	//				log	file
	void EnableLog(BOOL	bYesNo);

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if	output in the log file is enabled or FALSE otherwise
	// --- Effect:	Retrieves the flag that	specifies if output	of history entries is
	//				enabled	or disabled
	inline BOOL	IsEnabledLog() const { return m_bEnableLog;	}

	// --- In  :	bYesNo	-	if TRUE	then log file will be automatically	named
	//							depending on the date it was created 
	//							(format	"ddmmyyyy")
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Enables/disables the output	of the history entries in the 
	//				time stamped (in terms of filename)	log	file
	inline void	EnableTimeStampedLog(BOOL bYesNo) {
		m_bTimeStampedLog=bYesNo;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if	log	file is	automatically named	depending on the date 
	//				it was created (format "ddmmyyyy"),	or FALSE otherwise
	// --- Effect:	Retrieves the flag that	defines	if time	stamped	(in	terms of 
	//				filename) log file is enabled or not
	inline BOOL	IsEnabledTimeStamped() const { return m_bTimeStampedLog; }


	// --- In  :	bTruncateLogFile	-	flag that if set to	TRUE will force
	//										to truncate	the	contents of	the	existing 
	//										log	file before	adding new entry. 
	//										If FALSE specified then	if log file	
	//										already	exists then	its	contents will be 
	//										preserved and new entry	will be	added at 
	//										the	end
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the flag that specifies a truncate	mode for existing log file
	inline void	SetTruncateLogFile(BOOL	bTruncateLogFile) 
	{
		m_bTruncateLogFile=bTruncateLogFile;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	flag that if set to	TRUE will force	to truncate	the	contents 
	//				of the existing	log	file before	adding new entry. If FALSE 
	//				specified then if log file already exists then its contents	
	//				will be	preserved and new entry	will be	added at the end
	// --- Effect:	Retrieves the flag that	specifies a	truncate mode for existing 
	//				log	file
	inline BOOL	GetTruncateLogFile() const { return	m_bTruncateLogFile;	}


	// --- In  :	nMaxNum	-	the	limit for the number of	the	history	entries	
	//							displayed in the control's window
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the maximum number	of the history entries displayed 
	//				in the control's window. If	this number	is reached then	
	//				whenever a new item	is added the one at	the	top	is being removed.
	void SetMaxNumEntries(int nMaxNum);

	// --- In  :	
	// --- Out : 
	// --- Returns:	The	limit for the number of	the	history	entries	displayed in the 
	//				control's window
	// --- Effect:	Retrieves the maximum number of	the	history	entries	that can be	
	//				displayed in the control's window. If this number is reached then 
	//				whenever a new item	is added the one at	the	top	is being removed.
	inline int GetMaxNumEntries() const	{ return m_nMaxNumEntries; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	The	the	number of the history entries currently	displayed in the 
	//				control's window. This number cannot exceed	the	maximum	number
	//				of entries currently set for the control (this number can be 
	//				retrieved using	GetMaxNumEntries() function)
	// --- Effect:	Retrieves the current number of	the	history	entries	displayed 
	//				in the control's window.
	inline int GetNumEntries() const { return PtrToInt(m_arrEntries.GetCount()); }


	// --- In  :	sFormatString	-	format string for displaying time stamp	
	//									next to	a history item (time stamp will	be 
	//									automatically added	to a history entry when	
	//									you	call AddStampedLine() function). Refer 
	//									to COleDateTime::Format() function 
	//									documentation for information on how this 
	//									format string might	look like
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Sets the format	string for adding time stamp to	history	entries
	//				that are being added using AddStampedLine()	function.
	inline void	SetTimeStampFormat(CString sFormatString) {
		m_sDateFormatString=sFormatString;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	The	format string for displaying time stamp	next to	a history 
	//				item (time stamp will be automatically added to	a history entry	
	//				when you call AddStampedLine() function). Refer	to 
	//				COleDateTime::Format() function	documentation for information on 
	//				how	this format	string might look like
	// --- Effect:	Retrieves the format string	used for adding	time stamp to 
	//				history	entries	that are being added using AddStampedLine()	
	//				function.
	inline CString GetTimeStampFormat()	const {	return m_sDateFormatString;	}


	// --- In  :	
	// --- Out : 
	// --- Returns:	The	height in pixels of	history	items in the control
	// --- Effect:	Retrieves the height in	pixels of history items	in the control
	inline int GetHeight() const { return m_nHeight; }


//operations

	// --- In  :	string		-	new	line to	be added to	control	as new 
	//								history	item
	//				bAddToLog	-	if TRUE	this line will be added	to the log file	
	//								(if	any	is enabled)
	// --- Out : 
	// --- Returns:	TRUE if	new	line was successfully added	or FALSE otherwise
	// --- Effect:	Adds a new line	to the control.	If logging is on and the addToLog
	//				param is TRUE then this	line is	also added to the log file.
	BOOL AddLine(CString string, BOOL bAddToLog=TRUE);

	// --- In  :	string		-	new	line to	be added to	control	as new 
	//								history	item. This line	will be	preceded with
	//								time stamp
	//				bAddToLog	-	if TRUE	this line will be added	to the log file	
	//								(if	any	is enabled)
	// --- Out : 
	// --- Returns:	TRUE if	new	time stamped line was successfully added or	
	//				FALSE otherwise
	// --- Effect:	Adds a new time	stamped	line to	the	control. If	logging	is on 
	//				and	the	addToLog param is TRUE then	this line is also added	to 
	//				the	log	file.
	BOOL AddStampedLine(CString	string,	BOOL bAddToLog=TRUE);

	// --- In  :	string		-	new	text to	be added to	the	last history entry 
	//								in the control.
	//				bAddToLog	-	if TRUE	this line will be added	to the log file	
	//								(if	any	is enabled)
	// --- Out : 
	// --- Returns:	TRUE if	new	line was successfully appended or FALSE	otherwise
	// --- Effect:	Appends	the	specified text to the last history entry in	the	
	//				control. If	logging	is on and the addToLog param is	TRUE then 
	//				this line is also added	to the log file.
	BOOL AppendToLine(CString string, BOOL bAddToLog=TRUE);


	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if	the	control's contents were	removed	succesfully	or 
	//				FALSE otherwise
	// --- Effect:	Clears the internal	contents of	the	control. It	doesn't	clear the 
	//				contents of	the	log	file (if any was enabled)
	BOOL ClearHistory();


protected:
	// Returns a string	with a time	stamp that includes	the	date.	
	CString	GetTimeDateStamp() const;

	// writes new string in	the	log	file. If bNewLine is set to	TRUE '\r\n'	will be
	// added to	start new line
	void WriteToLog(CString	string,	BOOL bNewLine);
	// opens up	the	history	log	file for writing
	int	OpenLog();
	// closes the log file
	int	CloseLog();

	// calculates the number of	currently visible history entries
	int	GetVisibleCount();

	// updates internal	settings depending on new font that	was	set	using 
	// SetFont() function
	void UpdateFontInfo();
	// updates scrolling settings whenever the state of	the	control	has	changed
	void UpdateScrollInfo();
	// finds the history item with max width (in pixels	including offset)
	void UpdateMaxLengthInfo();

	// scrolls the contents	to make	the	last item visible
	void ScrollToEnd();

	// overridden virtual function that	prevent	COXHistoryCtrl from	displaying
	// default COXScrollWnd	context	menu
	virtual	BOOL OnPopulateContextMenu(CMenu* pMenu, CPoint& point)	
	{
		UNREFERENCED_PARAMETER(pMenu);
		UNREFERENCED_PARAMETER(point);
		return FALSE;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXHistoryCtrl)
	//}}AFX_VIRTUAL

public:
	CString GetTimeStampedFileName();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Destructs the object
	virtual	~COXHistoryCtrl();

protected:
	CString m_sTimeStampedLogFileName;
	// virtual function	is called to initialize	the	control	when it	was	just 
	// created or subclassed
	virtual	BOOL Initialize();

	// Generated message map functions
	//{{AFX_MSG(COXHistoryCtrl)
	afx_msg	void OnPaint();
	afx_msg	BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif

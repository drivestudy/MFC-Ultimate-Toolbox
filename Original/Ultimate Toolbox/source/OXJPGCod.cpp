// ==========================================================================
// 					Class Implementation : COXJPEGCodec
// ==========================================================================

// Source file : OXJPGCod.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXJPGCod.h"

#include "OXJPGExp.h"	// Special exception handling

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#ifndef EXIT_FAILURE		/* define exit() codes if not provided */
	#define EXIT_FAILURE  1
#endif


/*
 * Create the message string table.
 * We do this from the master message list in jerror.h by re-reading
 * jerror.h with a suitable definition for macro JMESSAGE.
 * The message table is made an external symbol just in case any applications
 * want to refer to it directly.
 */

#ifdef NEED_SHORT_EXTERNAL_NAMES
	#define jpeg_std_message_table	jMsgTable
#endif

#define JMESSAGE(code,string)	string ,

LPCTSTR const jpeg_std_message_table[] = {
							#include "jerror.h"
							NULL
							};


/////////////////////////////////////////////////////////////////////////////
// Definition of static members
	CMapPtrToPtr COXJPEGCodec::m_RunningCodecsMap;


// Data members -------------------------------------------------------------
// protected:


//	CFile*		m_pGraphFile;
//	---			

	// private:

// Member functions ---------------------------------------------------------
// public:

COXJPEGCodec::COXJPEGCodec()
	{
	}

#ifdef _DEBUG
void COXJPEGCodec::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);

	dc << TEXT("\nm_sWarnings : ") << 		m_sWarnings;

	void* pKey;
	void* pObject;
	POSITION pos = m_RunningCodecsMap.GetStartPosition();
	while (pos != NULL)
		{
		m_RunningCodecsMap.GetNextAssoc(pos, pKey, pObject);
		dc << TEXT("\nItem in map :") << (void*)pKey << TEXT("-->")<<  (void*)pObject << TEXT("\n");
		}
	}

void COXJPEGCodec::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXJPEGCodec::~COXJPEGCodec()
	{
	}

/*
 * Error exit handler: must not return to caller.
 *
 * Applications may override this if they want to get control back after
 * an error.  Typically one would longjmp somewhere instead of exiting.
 * The setjmp buffer can be made a private field within an expanded error
 * handler object.  Note that the info needed to generate an error message
 * is stored in the error object, so you can generate the message now or
 * later, at your convenience.
 * You should make sure that the JPEG object is cleaned up (with jpeg_abort
 * or jpeg_destroy) at some point.
 */

void COXJPEGCodec::error_exit (j_common_ptr cinfo)
	{
	void* pErrorCodec = NULL;
	m_RunningCodecsMap.Lookup(cinfo, pErrorCodec);
	if (pErrorCodec != NULL)
		((COXJPEGCodec*)pErrorCodec)->ProcessError(cinfo);
	}


/*
 * Actual output of an error or trace message.
 * Applications may override this method to send JPEG messages somewhere
 * other than stderr.
 */

void COXJPEGCodec::output_message(j_common_ptr cinfo)
	{
	void* pErrorCodec = NULL;
	m_RunningCodecsMap.Lookup(cinfo, pErrorCodec);
	if (pErrorCodec != NULL)
		((COXJPEGCodec*)pErrorCodec)->OutputMessage(cinfo);

	}


/*
 * Decide whether to emit a trace or warning message.
 * msg_level is one of:
 *   -1: recoverable corrupt-data warning, may want to abort.
 *    0: important advisory messages (always display to user).
 *    1: first level of tracing detail.
 *    2,3,...: successively more detailed tracing messages.
 * An application might override this method if it wanted to abort on warnings
 * or change the policy about which messages to display.
 */

void COXJPEGCodec::emit_message(j_common_ptr cinfo, int msg_level)
	{
	void* pErrorCodec = NULL;
	m_RunningCodecsMap.Lookup(cinfo, pErrorCodec);
	if (pErrorCodec != NULL)
		((COXJPEGCodec*)pErrorCodec)->EmitMessage(cinfo, msg_level);
	}

/*
 * Format a message string for the most recent JPEG error or message.
 * The message is stored into buffer, which should be at least JMSG_LENGTH_MAX
 * characters.  Note that no '\n' character is added to the string.
 * Few applications should need to override this method.
 */

void COXJPEGCodec::format_message(j_common_ptr cinfo, TCHAR* buffer)
	{
	void* pErrorCodec = NULL;
	m_RunningCodecsMap.Lookup(cinfo, pErrorCodec);
	if (pErrorCodec != NULL)
		((COXJPEGCodec*)pErrorCodec)->FormatMessage(cinfo, buffer);
	}


/*
 * Reset error state variables at start of a new image.
 * This is called during compression startup to reset trace/error
 * processing to default state, without losing any application-specific
 * method pointers.  An application might possibly want to override
 * this method if it has additional error processing state.
 */

void COXJPEGCodec::reset_error_mgr(j_common_ptr cinfo)
	{
	void* pErrorCodec = NULL;
	m_RunningCodecsMap.Lookup(cinfo, pErrorCodec);
	if (pErrorCodec != NULL)
		((COXJPEGCodec*)pErrorCodec)->ResetError(cinfo);
	}

/*
 * Fill in the standard error-handling methods in a jpeg_error_mgr object.
 * Typical call is:
 *	struct jpeg_compress_struct cinfo;
 *	struct jpeg_error_mgr err;
 *
 *	cinfo.err = jpeg_std_error(&err);
 * after which the application may override some of the methods.
 */

BOOL COXJPEGCodec::SetJPEGErrorHandling(struct jpeg_error_mgr* err)
	{
	err->error_exit = error_exit;
	err->emit_message = emit_message;
	err->output_message = output_message;
	err->format_message = format_message;
	err->reset_error_mgr = reset_error_mgr;

	err->trace_level = 0;		/* default = no tracing */
	err->num_warnings = 0;	/* no warnings emitted yet */
	err->msg_code = 0;		/* may be useful as a flag for "no error" */

	/* Initialize message table pointers */
	err->jpeg_message_table = jpeg_std_message_table;
	err->last_jpeg_message = (int) JMSG_LASTMSGCODE - 1;

	err->addon_message_table = NULL;
	err->first_addon_message = 0;	/* for safety */
	err->last_addon_message = 0;

	return TRUE;
	}

void COXJPEGCodec::OutputMessage(j_common_ptr cinfo)
	{
	TCHAR buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	FormatMessage(cinfo, buffer);

	/* TRACE out in debug */
	TRACE1("%s\n", buffer);

	m_sWarnings += ( CString(buffer) + CString("\n") );

	return;
	}

void COXJPEGCodec::ProcessError(j_common_ptr cinfo)
	{
	// Clean the map
	m_RunningCodecsMap.RemoveKey(cinfo);
	
	/* Always display the message */
	OutputMessage(cinfo);

	/* Let the memory manager delete any temp files before we die */
	jpeg_destroy(cinfo);

	TCHAR buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	FormatMessage(cinfo, buffer);

	THROW(new COXJPEGException(cinfo->err->msg_code, buffer));
	}

void COXJPEGCodec::EmitMessage(j_common_ptr cinfo, int msg_level)
	{
	struct jpeg_error_mgr* err = cinfo->err;

	if (msg_level < 0)
		{
		/* It's a warning message.  Since corrupt files may generate many warnings,
		 * the policy implemented here is to show only the first warning,
		 * unless trace_level >= 3.
		 */
		if (err->num_warnings == 0 || err->trace_level >= 3)
			OutputMessage(cinfo);
		/* Always count warnings in num_warnings. */
		err->num_warnings++;
		}
	else
		{
		/* It's a trace message.  Show it if trace_level >= msg_level. */
		if (err->trace_level >= msg_level)
			OutputMessage(cinfo);
		}
	}

void COXJPEGCodec::FormatMessage(j_common_ptr cinfo, LPTSTR buffer)
	{
	struct jpeg_error_mgr * err = cinfo->err;
	int msg_code = err->msg_code;
	LPCTSTR msgtext = NULL;
	LPCTSTR msgptr;
	TCHAR ch;
	jpeg_boolean isstring;

	/* Look up message string in proper table */
	if (msg_code > 0 && msg_code <= err->last_jpeg_message)
		{
		msgtext = err->jpeg_message_table[msg_code];
		}
	else if (err->addon_message_table != NULL &&
			 msg_code >= err->first_addon_message &&
			 msg_code <= err->last_addon_message)
		{
		msgtext = err->addon_message_table[msg_code - err->first_addon_message];
		}

	/* Defend against bogus message number */
	if (msgtext == NULL)
		{
		err->msg_parm.i[0] = msg_code;
		msgtext = err->jpeg_message_table[0];
		}

	/* Check for string parameter, as indicated by %s in the message text */
	isstring = FALSE;
	msgptr = msgtext;
	while ((ch = *msgptr++) != '\0')
		{
		if (ch == '%')
			{
			if (*msgptr == 's') isstring = TRUE;
				break;
			}
		}

	/* Format the message into the passed buffer */
	if (isstring)
		UTBStr::stprintf(buffer, 2048, msgtext, err->msg_parm.s);
	else
		UTBStr::stprintf(buffer, 2048, msgtext,
	
	err->msg_parm.i[0], err->msg_parm.i[1],
	err->msg_parm.i[2], err->msg_parm.i[3],
	err->msg_parm.i[4], err->msg_parm.i[5],
	err->msg_parm.i[6], err->msg_parm.i[7]);
	}

void COXJPEGCodec::ResetError(j_common_ptr cinfo)
	{
	cinfo->err->num_warnings = 0;
	/* trace_level is not reset since it is an application-supplied parameter */
	cinfo->err->msg_code = 0;	/* may be useful as a flag for "no error" */

	m_sWarnings.Empty();
	}



///////////////////////////////////////////////////////////////////////////


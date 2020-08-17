// ==========================================================================
// 								Message Definitions
// ==========================================================================

// Header File : OXUUDRet.h

// Version: 9.3

//////////////////////////////////////////////////////////////////////////

// This file contains the message definitions for the Lib Manager
// It has been generated from OXUUDRet.MC

// WIN Macros ---------------------------------------------------------------

// Following type is defined by Windows (wintypes.h)
// typedef LONG HRESULT;

// Following macros are defined by Windows (winerror.h)
// #define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
// --- Whether the severity code is Error or Warning (and not Informational or Success)

// #define FAILED(Status) ((HRESULT)(Status)<0)
// --- The inverse of SUCCEEDED

// #define HRESULT_CODE(hr)    ((hr) & 0xFFFF)
// --- The error code itself

// #define HRESULT_FACILITY(hr)  (((hr) >> 16) & 0x1fff)
/// --- The facility code

// #define HRESULT_SEVERITY(hr)  (((hr) >> 31) & 0x1)
// --- The severity code

// #define FACILITY_WIN32                   7
// #define HRESULT_FROM_WIN32(x)   (x ? ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)) : 0 )
// --- Used to pass a WIN32 return code as an HRESULT

// Header Section------------------------------------------------------------

// The header section defines names and language identifiers for use
// by the message definitions later in this file. The MessageIdTypedef,
// SeverityNames, FacilityNames, and LanguageNames keywords are
// optional and not required.

// (The first 256 facility codes are reserved for use by the system software)
// Message Definition Section -----------------------------------------------

// The MessageId statement marks the beginning of the message
// definition. A MessageID statement is required for each message.

// The Severity and Facility statements are optional. If not specified 
// they default to the value last specified for a message definition.

// UUDecoder Messages ---------------------------------------------------

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_UUDEC                   0x103


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: MSG_UUDEC_SUCCESS
//
// MessageText:
//
//  No error occurred
//
#define MSG_UUDEC_SUCCESS                ((DWORD)0x01031000L)

//
// MessageId: MSG_UUDEC_WARNING
//
// MessageText:
//
//  Warning
//
#define MSG_UUDEC_WARNING                ((DWORD)0x81031001L)

//
// MessageId: MSG_UUDEC_OPEN_ENCODED_ERROR
//
// MessageText:
//
//  Could not open encoded file
//
#define MSG_UUDEC_OPEN_ENCODED_ERROR     ((DWORD)0xC1031003L)

//
// MessageId: MSG_UUDEC_WRITING_OF_A_FILE_FAILED
//
// MessageText:
//
//  Could not write one or more target files
//
#define MSG_UUDEC_WRITING_OF_A_FILE_FAILED ((DWORD)0xC1031004L)

// ==========================================================================

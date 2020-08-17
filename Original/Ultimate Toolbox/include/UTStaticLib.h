// Version: 9.3

// Inclide the resource declarations
#include "OXMainRes.h"

#ifdef _AFXDLL
	// Use MFC in a Shared DLL
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma message("Automatically linking with UTLibSharedDU.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibSharedDU.lib")
		#else // not unicode	
			#pragma message("Automatically linking with UTLibSharedD.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibSharedD.lib")
		#endif // _UNICODE
	#else // RELEASE
		#ifdef _UNICODE
			#pragma message("Automatically linking with UTLibSharedRU.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibSharedRU.lib")
		#else // not unicode	
			#pragma message("Automatically linking with UTLibSharedR.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibSharedR.lib")
		#endif // _UNICODE
	#endif // _DEBUG
#else // not _AFXDLL
	// Use MFC in a Static Library
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma message("Automatically linking with UTLibStaticDU.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibStaticDU.lib")
		#else // not unicode	
			#pragma message("Automatically linking with UTLibStaticD.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibStaticD.lib")
		#endif // _UNICODE
	#else // RELEASE
		#ifdef _UNICODE
			#pragma message("Automatically linking with UTLibStaticRU.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibStaticRU.lib")
		#else // not unicode	
			#pragma message("Automatically linking with UTLibStaticR.lib - please make sure this file is built.")
			#pragma comment(lib, "UTLibStaticR.lib")
		#endif // _UNICODE
	#endif // _DEBUG
#endif // _AFXDLL


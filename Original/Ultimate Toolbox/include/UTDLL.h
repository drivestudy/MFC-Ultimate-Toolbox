// Version: 9.3

// Inclide the resource declarations
#include "OXMainRes.h"

#ifdef _DEBUG
	#ifdef _UNICODE
		#pragma message("Automatically linking with UTDU.lib - please make sure this file is built.")
		#pragma comment(lib, "UTDU.lib")
	#else // not unicode	
		#pragma message("Automatically linking with UTD.lib - please make sure this file is built.")
		#pragma comment(lib, "UTD.lib")
	#endif // _UNICODE
#else // RELEASE
	#ifdef _UNICODE
		#pragma message("Automatically linking with UTU.lib - please make sure this file is built.")
		#pragma comment(lib, "UTU.lib")
	#else // not unicode	
		#pragma message("Automatically linking with UT.lib - please make sure this file is built.")
		#pragma comment(lib, "UT.lib")
	#endif // _UNICODE
#endif // _DEBUG


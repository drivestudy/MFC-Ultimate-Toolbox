#include "stdafx.h"
#include "UTBStrOp.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void UTBStr::tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src)
{
#if _MSC_VER >= 1400
	::_tcscpy_s(dest, length, src);
#else
	UNREFERENCED_PARAMETER(length);
	::_tcscpy(dest,src);
# endif
}

char* UTBStr::fcvt(double val, int count, int * dec, int * sign)
{
#if _MSC_VER >= 1400
	UNREFERENCED_PARAMETER(sign);
	char * dest = new char[count * 2 + 10];
	::_fcvt_s(dest, count * 2 + 9, val, count, dec, sign);
	return dest;
#else
	return ::_fcvt(val, count, dec, sign);
# endif		
}

void UTBStr::strncpy(char * dest, size_t size, const char * src, size_t count)
{
	if (size == 0) size = 1;
#if _MSC_VER >= 1400
	::strncpy_s(dest, size, src, count);
#else
	UNREFERENCED_PARAMETER(size);
	::strncpy(dest, src, count);
# endif
}

void UTBStr::stprintf(TCHAR * dest, size_t size, const TCHAR * src, ...)
{
	va_list vl;
	va_start( vl, src );

#if _MSC_VER >= 1400
	_vstprintf_s_l(dest, size, src, NULL, vl);
#else
	UNREFERENCED_PARAMETER(size);
	_vstprintf(dest, src, vl);
# endif

	va_end( vl );
}

void UTBStr::sprintf(char * dest, size_t size, const char * src, ...)
{
	va_list vl;
	va_start( vl, src );

#if _MSC_VER >= 1400
    _vsprintf_s_l(dest, size, src, NULL, vl);
#else
	UNREFERENCED_PARAMETER(size);
	vsprintf(dest, src, vl);
# endif

	va_end( vl );
}


void UTBStr::tcscat(TCHAR * dest, SIZE_T length, const TCHAR* src)
{
#if _MSC_VER >= 1400
	_tcscat_s(dest, length, src);
#else
	UNREFERENCED_PARAMETER(length);
	_tcscat(dest, src);
# endif
}

void UTBStr::tcsncat(TCHAR * dest, size_t size, const TCHAR * src, size_t count)
{
#if _MSC_VER >= 1400
	_tcsncat_s(dest, size, src, count);
#else
	UNREFERENCED_PARAMETER(size);
	_tcsncat(dest, src, count);
# endif
}


void UTBStr::wcstombs(size_t *pConvertedChars, char *mbstr, 
	size_t sizeInBytes, const wchar_t *wcstr, size_t count)
{
#if _MSC_VER >= 1400
	wcstombs_s(pConvertedChars, mbstr, sizeInBytes, wcstr, count);
#else
	UNREFERENCED_PARAMETER(sizeInBytes);
	*pConvertedChars = ::wcstombs(mbstr, wcstr, count);
#endif
}

void UTBStr::itot(int value, TCHAR * dest, size_t size, int radix)
{
#if _MSC_VER >= 1400
	_itot_s(value, dest, size, radix);
#else
	UNREFERENCED_PARAMETER(size);
	_itot(value, dest, radix);
#endif
}

void UTBStr::itoa(int value, char * dest, size_t size, int radix)
{
#if _MSC_VER >= 1400
	_itoa_s(value, dest, size, radix);
#else
	UNREFERENCED_PARAMETER(size);
	_itoa(value, dest, radix);
#endif
}

void UTBStr::ltot(long value, TCHAR * dest, size_t size, int radix)
{
#if _MSC_VER >= 1400
	_ltot_s(value, dest, size, radix);
#else
	UNREFERENCED_PARAMETER(size);
	_ltot(value, dest, radix);
#endif
}

TCHAR* UTBStr::tcstok(TCHAR* strToken, const TCHAR* strDelimit, TCHAR ** context)
{
#if _MSC_VER >= 1400
	return _tcstok_s(strToken, strDelimit, context);
#else
	UNREFERENCED_PARAMETER(context);
	return _tcstok(strToken, strDelimit);
#endif
}

void UTBStr::tsplitpath(const TCHAR * path, TCHAR * drive, size_t driveSizeInTCHARacters, TCHAR * dir,
   size_t dirSizeInTCHARacters, TCHAR * fname, size_t nameSizeInTCHARacters, TCHAR * ext, size_t extSizeInBytes)
{
#if _MSC_VER >= 1400
	_tsplitpath_s(path, drive, driveSizeInTCHARacters, dir, dirSizeInTCHARacters, fname, nameSizeInTCHARacters, ext, extSizeInBytes);
#else
	UNREFERENCED_PARAMETER(driveSizeInTCHARacters);
	UNREFERENCED_PARAMETER(dirSizeInTCHARacters);
	UNREFERENCED_PARAMETER(nameSizeInTCHARacters);
	UNREFERENCED_PARAMETER(extSizeInBytes);
	_tsplitpath(path, drive, dir, fname, ext);
#endif
}
void UTBStr::tcsncpy(TCHAR * dest, size_t dstSize, const TCHAR * src, size_t maxCount)
{
#if _MSC_VER >= 1400
	_tcsncpy_s(dest, dstSize, src, maxCount);
#else
	UNREFERENCED_PARAMETER(dstSize);
#ifdef _UNICODE
	::wcsncpy(dest,src, maxCount);
#else
	::strncpy(dest,src, maxCount);
#endif
#endif
}

TCHAR * UTBStr::tgetenv(const TCHAR *varname) 
{
#if _MSC_VER >= 1400
	TCHAR* retval;
	size_t requiredSize;
	// Get required length
	_tgetenv_s( &requiredSize, NULL, 0, varname);
	retval = new TCHAR[requiredSize];
	if (!_tgetenv_s( &requiredSize, retval, requiredSize, varname ))
		return retval;
	else
	{
		delete [] retval;
		return NULL;
	}
	
#else
	return _tgetenv(varname);
#endif
}

size_t UTBStr::mbstowcs(wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count )
{
#if _MSC_VER >= 1400
	size_t retval;

	if (!mbstowcs_s(&retval, wcstr, sizeInWords, mbstr, count))
		return retval;

	return 0;
#else
	UNREFERENCED_PARAMETER(sizeInWords);
	return ::mbstowcs(wcstr, mbstr, count);
#endif
}

void UTBStr::wcscpy(wchar_t * dest, size_t size, const wchar_t * source)
{
#if _MSC_VER >= 1400

	wcscpy_s(dest, size, source);
#else
	UNREFERENCED_PARAMETER(size);
	::wcscpy(dest, source);
#endif
}

void UTBStr::strcpy(char * dest, size_t size, const char * src)
{
#if _MSC_VER >= 1400
	strcpy_s(dest, size, src);
#else
	UNREFERENCED_PARAMETER(size);
	::strcpy(dest, src);
#endif
}
// OXRegExpression.cpp: implementation of the COXRegExpression class.
//
//////////////////////////////////////////////////////////////////////
// Version: 9.3


#include "stdafx.h"
#include "OXRegExpression.h"
#include "OXMainRes.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
//

IMPLEMENT_SERIAL(COXRegExpression,CObject,OX_REGEXP_VERSION)

//destructor - do cleanup on destroy
COXRegExpression::~COXRegExpression()
{
	RemoveAll();
}

COXRegExpression::COXRegExpression(const COXRegExpression& regExpression)
{
	ASSERT_VALID(&regExpression);
	*this=regExpression;
}

const COXRegExpression& COXRegExpression::operator=(const COXRegExpression& regExpression)
{
	ASSERT_VALID(&regExpression);
	RemoveAll();
	for (int n=0; n<regExpression.GetTagCount();n++)
		AddTag(regExpression.GetTag(n)->nType, &regExpression.GetTag(n)->sValue,
			regExpression.GetTag(n)->nMin, regExpression.GetTag(n)->nMax);
	m_sRule=regExpression.m_sRule;
	m_nNumber=regExpression.m_nNumber;
	m_nError=regExpression.m_nError;
	m_sSpace=regExpression.m_sSpace;
	m_sText=regExpression.m_sText;
	m_nFound=regExpression.m_nFound;
	ASSERT_VALID(this);
	return *this;
}


//adds tag to the list
void COXRegExpression::AddTag(int nType, CString* pValue, int nFirst, int nSecond)
{
	tRegExpTag* pRegtag=new tRegExpTag;
	pRegtag->nType=nType;
	pRegtag->nMin=nFirst;
	pRegtag->nMax=nSecond;
	if (pValue)
		pRegtag->sValue=*pValue;
	m_arTags.Add(pRegtag);
}

//this function do half a job -
//parses rule you've supplied and
//calls AddTag every time, new tag has been parsed.
//Well, the rule can start from any character
BOOL COXRegExpression::Parse(CString& sString, int* pnNumber/*=NULL*/)
{
	int nType = OX_REGEXP_TAG_COMMON_CHAR;
	CString sValue;
	CString sNumber;
	int nNumber=0;
	if (!pnNumber)
		pnNumber=&nNumber;

	int nMin=0;
	int nMax=0;
	
	if (sString.IsEmpty() || sString.GetLength()<=*pnNumber)
		return FALSE;

	//main loop
	while (sString.GetLength()>*pnNumber)
	{
		TCHAR ch=sString.GetAt(*pnNumber);
		(*pnNumber)++;
		switch (nType)
		{
		case OX_REGEXP_TAG_COMMON_CHAR:
			switch (ch)
			{
			case TEXT('\\'):
				{
					int nRet=GetSpecialChar(sString, pnNumber, &ch);
					if (!nRet)
					{
						(*pnNumber)--;
						return FALSE;
					}
					if (nRet==OX_REGEXP_TAG_REFERENCE_BACK)
					{
						CString sPattern;
						int nNumber=(int) ch;
						if (GetPattern(nNumber, sPattern))
							sValue+=sPattern;
						else
						{
							(*pnNumber)--;
							SetError(OX_REGEXP_ERROR_NO_REFERENCE);
							return FALSE;
						}
						continue;
					}
					if (nRet==OX_REGEXP_TAG_COMMON_CHAR)
					{
						sValue+=ch;
						continue;
					}
					if (!sValue.IsEmpty())
						AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
					switch (nRet)
					{
						case OX_REGEXP_TAG_DIGIT:
						case OX_REGEXP_TAG_NON_DIGIT:
						case OX_REGEXP_TAG_WHITESPACE:
						case OX_REGEXP_TAG_NON_WHITESPACE:
						case OX_REGEXP_TAG_ANY_WORD:
						case OX_REGEXP_TAG_ANY_NON_WORD:
							nType=nRet;
							sValue="";
							continue;
						default:
							AddTag(nRet, NULL);
							sValue="";
							continue;
					}
				}
				break;
			case TEXT('^'):
				if (!sValue.IsEmpty())
					AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
				AddTag(OX_REGEXP_TAG_BEGINING_OF_THE_LINE,NULL);
				sValue="";
				break;
			case TEXT('$'):
				if (!sValue.IsEmpty())
					AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
				AddTag(OX_REGEXP_TAG_END_OF_THE_LINE,NULL);
				sValue="";
				break;
			case TEXT('*'):
			case TEXT('?'):
			case TEXT('+'):
				if (sValue.IsEmpty())
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_MUST_CHAR_BEFORE);
					return FALSE;
				}
				switch(ch)
				{
					case TEXT('*'):
						//nType=OX_REGEXP_TAG_CHAR_ZERO_OR_MORE;
						nMin=0;
						nMax=-1;
						break;
					case TEXT('+'):
						//nType=OX_REGEXP_TAG_CHAR_ONE_OR_MORE;
						nMin=1;
						nMax=-1;
						break;
					case TEXT('?'):
						//nType=OX_REGEXP_TAG_CHAR_ZERO_OR_ONE;
						nMin=0;
						nMax=1;
						break;
				}
				ch=sValue.GetAt(sValue.GetLength()-1);
				sValue=(sValue.GetLength()==1)?
					_T(""):sValue.Left(sValue.GetLength()-1);
				if (!sValue.IsEmpty())
				{
					AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
				}
				sValue=ch;
				AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue, nMin, nMax);
				sValue="";
				nType=OX_REGEXP_TAG_COMMON_CHAR;
				break;
				case TEXT('.'):
					if (!sValue.IsEmpty())
					{
						AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
					}
					nType=OX_REGEXP_TAG_CHAR_NOT_NEW_LINE;
					sValue="";
					break;
				case TEXT('('):
					if (!sValue.IsEmpty())
					{
						AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
					}
					sValue="";
					nType=OX_REGEXP_TAG_PATTERN;
					break;
				case TEXT(')'):
				case TEXT(']'):
				case TEXT('}'):
					{
						(*pnNumber)--;
						SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
						return FALSE;
					}
				case TEXT('|'):
					if (sValue.IsEmpty())
					{
						(*pnNumber)--;
						SetError(OX_REGEXP_ERROR_MUST_CHAR_BEFORE);
						return FALSE;
					}
					ch=sValue.GetAt(sValue.GetLength()-1);
					sValue=(sValue.IsEmpty())?_T(""):sValue.Left(
						sValue.GetLength()-1);
					if (!sValue.IsEmpty())
						AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
					sValue=ch;
					nType=OX_REGEXP_TAG_CHARS_EITHER;
					break;
				case TEXT('{'):
					{
						if (sValue.IsEmpty())
						{
							(*pnNumber)--;
							SetError(OX_REGEXP_ERROR_MUST_CHAR_BEFORE);
							return FALSE;
						}
						ch=sValue.GetAt(sValue.GetLength()-1);
						sValue=(sValue.IsEmpty())?_T(""):sValue.Left(
							sValue.GetLength()-1);
						if (!sValue.IsEmpty())
							AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
						sValue=ch;
						int nMin, nMax;
						int nRet=GetMinMaxCount(sString, 
							pnNumber, &nMin, &nMax);
						if (!nRet)
						{
							(*pnNumber)--;
							return FALSE;
						}
						if (nRet==OX_REGEXP_TAG_CHARS_EXACTLY)
						{
							if (nMin)
							{
								for (int n=1;n<nMin;n++)
									sValue+=sValue.Right(1);
								AddTag(OX_REGEXP_TAG_COMMON_CHAR, &sValue);
							}
						}
						else
							AddTag(nRet,&sValue, nMin, nMax);
						sValue="";
					}
					break;
				case TEXT('['):
					if (!sValue.IsEmpty())
						AddTag(OX_REGEXP_TAG_COMMON_CHAR,&sValue);
					sValue="";
					nType=OX_REGEXP_TAG_CHARSET;
					break;
				case TEXT('/'):
					if (!GetAsciiChar(sString, pnNumber, &ch))
					{
						(*pnNumber)--;
						return FALSE;
					}
					sValue+=ch;
					break;
				default:
					sValue+=ch;
					break;

			}
			break;
		case OX_REGEXP_TAG_CHAR_NOT_NEW_LINE:
		case OX_REGEXP_TAG_DIGIT:
		case OX_REGEXP_TAG_NON_DIGIT:
		case OX_REGEXP_TAG_WHITESPACE:
		case OX_REGEXP_TAG_NON_WHITESPACE:
		case OX_REGEXP_TAG_ANY_WORD:
		case OX_REGEXP_TAG_ANY_NON_WORD:
			int nRet;
			switch (ch)
				{
					case TEXT('{'):
					int nMin, nMax;
					nRet=GetMinMaxCount(sString, pnNumber, &nMin, &nMax);
					if (!nRet)
					{
						(*pnNumber)--;
						return FALSE;
					}
					switch (nRet)
					{
					case OX_REGEXP_TAG_CHARS_EXACTLY:
						AddTag(nType, NULL, nMin, nMin);
						break;
					case OX_REGEXP_TAG_CHARS_AT_LEAST:
						AddTag(nType, NULL, nMin, -1);
						break;
					case OX_REGEXP_TAG_CHARS_AT_LEAST_MOST:
						AddTag(nType, NULL, nMin, nMax);
						break;
					default:
						ASSERT(FALSE);
					}
					break;
			case TEXT('?'):
				AddTag(nType, NULL, 0, 1);
				break;
			case TEXT('*'):
				AddTag(nType, NULL, 0, -1);
				break;
			case TEXT('+'):
				AddTag(nType, NULL, 1, -1);
				break;
			case TEXT('|'):
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_MUST_CHAR_BEFORE);
					return FALSE;
				}
			default:
				AddTag(OX_REGEXP_TAG_CHAR_NOT_NEW_LINE, NULL);
				ASSERT(sValue.IsEmpty());
				sValue+=ch;
				break;
			}
			nType=OX_REGEXP_TAG_COMMON_CHAR;
			break;
		case OX_REGEXP_TAG_CHARS_EITHER:
			{
				TCHAR chEither;
				ASSERT(!sValue.IsEmpty());
				(*pnNumber)--;
				BOOL bContinue=FALSE;
				do{
					if (!GetEither(sString, pnNumber, &chEither))
					{
						(*pnNumber)--;
						return FALSE;
					}
					if (sValue.Find(chEither)==-1)
						sValue+=chEither;
					if (sString.GetLength()>*pnNumber &&
						sString.GetAt(*pnNumber)==TEXT('|'))
					{
						bContinue=TRUE;
						(*pnNumber)++;
					}
					else
						bContinue=FALSE;
				}while(sString.GetLength()>*pnNumber 
					&& bContinue);
				if (bContinue)
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_UNEXPECTED_END_EITHER);
					return FALSE;
				}
				AddTag(OX_REGEXP_TAG_CHARSET, &sValue);
				sValue="";
				nType=OX_REGEXP_TAG_COMMON_CHAR;
			}
			break;
		case OX_REGEXP_TAG_CHARSET:
			if (ch==TEXT('^') && sValue.IsEmpty())
			{
				nType=OX_REGEXP_TAG_NON_CHARSET;
				continue;
			}
		case OX_REGEXP_TAG_NON_CHARSET:
		case OX_REGEXP_TAG_PATTERN:
		case OX_REGEXP_TAG_CHARSET_BEGIN_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE:
		case OX_REGEXP_TAG_CHARSET_END_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_END_LINE:
		case OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE:
			switch (ch)
			{
			case TEXT('^'):
				if (nType==OX_REGEXP_TAG_PATTERN)
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
					return FALSE;

				}
				switch (nType)
				{
					case OX_REGEXP_TAG_NON_CHARSET:
						nType=OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET_END_LINE:
						nType=OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET:
						nType=OX_REGEXP_TAG_CHARSET_BEGIN_LINE;
						break;
					case OX_REGEXP_TAG_NON_CHARSET_END_LINE:
						nType=OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET_BEGIN_LINE:
					case OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE:
					case OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE:
					case OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE:
						break;
					default:
						ASSERT(FALSE);
						break;
				}
				break;
			case TEXT('$'):
				if (nType==OX_REGEXP_TAG_PATTERN)
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
					return FALSE;

				}
				switch (nType)
				{
					case OX_REGEXP_TAG_NON_CHARSET:
						nType=OX_REGEXP_TAG_NON_CHARSET_END_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET_BEGIN_LINE:
						nType=OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET:
						nType=OX_REGEXP_TAG_CHARSET_END_LINE;
						break;
					case OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE:
						nType=OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE;
						break;
					case OX_REGEXP_TAG_CHARSET_END_LINE:
					case OX_REGEXP_TAG_NON_CHARSET_END_LINE:
					case OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE:
					case OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE:
						break;
					default:
						ASSERT(FALSE);
						break;
				}
				break;
			case TEXT('*'):
			case TEXT('+'):
			case TEXT('?'):
			case TEXT('.'):
			case TEXT('('):
			case TEXT('|'):
			case TEXT('}'):
			case TEXT('['):
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
					return FALSE;
				}
			case TEXT(']'):
				if (nType!=OX_REGEXP_TAG_PATTERN)
				{
					AddTag(nType,&sValue);
					sValue="";
					nType=OX_REGEXP_TAG_COMMON_CHAR;
					break;
				}
				else
				{
					(*pnNumber)--;
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
					return FALSE;
				}
			case TEXT('\\'):
				{
					int nRet=GetSpecialChar(sString, pnNumber,&ch);
					if (nRet!=OX_REGEXP_TAG_COMMON_CHAR)
					{
						if (nRet)
							SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
						(*pnNumber)--;
						return FALSE;
					}
					if (nType==OX_REGEXP_TAG_PATTERN)
						sValue+=ch;
					else
						if (sValue.Find(ch)==-1)
							sValue+=ch;
				}
				break;
			case TEXT('/'):
				if (!GetAsciiChar(sString, pnNumber, &ch))
				{
					(*pnNumber)--;
					return FALSE;
				}
				if (nType==OX_REGEXP_TAG_PATTERN)
					sValue+=ch;
				else
					if (sValue.Find(ch)==-1)
						sValue+=ch;
				break;
			case TEXT('{'):
				{
					if (sValue.IsEmpty())
					{
						(*pnNumber)--;
						SetError(OX_REGEXP_ERROR_MUST_CHAR_BEFORE);
						return FALSE;
					}
					int nMin,nMax; 
					int nRet=GetMinMaxCount(sString, pnNumber, &nMin, &nMax);
					if (nRet!=OX_REGEXP_TAG_CHARS_EXACTLY)
					{
						if (nRet)
							SetError(OX_REGEXP_ERROR_UNEXPECTED_MATCH);
						(*pnNumber)--;
						return FALSE;
					}
					if (nType==OX_REGEXP_TAG_PATTERN)
					{
						for (int n=1;n<nMin;n++)
							sValue+=sValue.Right(1);
					}
					else
						if (sValue.Find(sValue.Right(1))==-1)
							sValue+=sValue.Right(1);
				}
				break;
			case TEXT(')'):
				if (nType==OX_REGEXP_TAG_PATTERN)
				{
					AddTag(OX_REGEXP_TAG_PATTERN, &sValue);
					sValue="";
					nType=OX_REGEXP_TAG_COMMON_CHAR;
					break;
				}
				else
				{
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
					(*pnNumber)--;
					return FALSE;
				}
			default:
				if (nType==OX_REGEXP_TAG_PATTERN)
					sValue+=ch;
				else
					if (sValue.Find(ch)==-1)
						sValue+=ch;
			}
			break;
		default:
			ASSERT(FALSE);//unknown type!
			break;
		}
	}

	//check for errores
	switch (nType)
	{
		case OX_REGEXP_TAG_COMMON_CHAR:
			if (!sValue.IsEmpty())
				AddTag(nType, &sValue);
			break;
		case OX_REGEXP_TAG_CHAR_NOT_NEW_LINE: 
		case OX_REGEXP_TAG_DIGIT:
		case OX_REGEXP_TAG_NON_DIGIT:
		case OX_REGEXP_TAG_WHITESPACE:
		case OX_REGEXP_TAG_NON_WHITESPACE:
		case OX_REGEXP_TAG_ANY_WORD:
		case OX_REGEXP_TAG_ANY_NON_WORD:
			AddTag(nType, NULL);
			break;
		case OX_REGEXP_TAG_CHARS_EITHER:
			SetError(OX_REGEXP_ERROR_UNEXPECTED_END_EITHER);
				return FALSE;
		case OX_REGEXP_TAG_PATTERN:
			SetError(OX_REGEXP_ERROR_UNEXPECTED_END_OF_STRING);
				return FALSE;
		case OX_REGEXP_TAG_CHARSET:
		case OX_REGEXP_TAG_NON_CHARSET:
		case OX_REGEXP_TAG_CHARSET_BEGIN_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE:
		case OX_REGEXP_TAG_CHARSET_END_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_END_LINE:
		case OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE:
		case OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE:
			SetError(OX_REGEXP_ERROR_UNEXPECTED_END_EITHER);
			return FALSE;

	}
	return TRUE;
}


//when function encounters symbol '|'
//that means either character can be, it 
//calls this function to get these characters
BOOL COXRegExpression::GetEither(CString& sString,int* pnNumber,LPTSTR pchEither)
{
	TCHAR ch;


	if (sString.GetLength()<=(*pnNumber))
	{	
		SetError(OX_REGEXP_ERROR_UNEXPECTED_END_EITHER);
		return FALSE;
	}

	ch=sString.GetAt(*pnNumber);
	(*pnNumber)++;
	switch (ch)
	{
	case TEXT('\\'):
		{
			int nRet=GetSpecialChar(sString, pnNumber, &ch);
			if (nRet==OX_REGEXP_TAG_COMMON_CHAR)
			{
				*pchEither=ch;
				return TRUE;
			}
			else
			{
				if (nRet)
					SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
				return FALSE;
			}
		}
		break;
	case TEXT('*'):
	case TEXT('+'):
	case TEXT('?'):
	case TEXT('.'):
	case TEXT('('):
	case TEXT(')'):
	case TEXT('|'):
	case TEXT('{'):
	case TEXT('}'):
	case TEXT('['):
	case TEXT(']'):
		{
			SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
			return FALSE;
		}
	case TEXT('/'):
			if (GetAsciiChar(sString,pnNumber,&ch))
			{
				*pchEither=ch;
				return TRUE;
			}
			else
				return FALSE;
		break;
	default:
		*pchEither=ch;
		return TRUE;
	}
}


//retrieve special char -
//called for parsing when Parse found '\' character
inline int COXRegExpression::GetSpecialChar(CString& sString, int* pnNumber, LPTSTR pch)
{
	*pch=NULL;
	if (sString.GetLength()<=*pnNumber)
	{
		SetError(OX_REGEXP_ERROR_UNEXPECTED_END_SPECCHAR);
		return FALSE;
	}

	TCHAR ch=sString.GetAt(*pnNumber);
	(*pnNumber)++;

	switch (ch)
	{
	case TEXT('b'):
		return OX_REGEXP_TAG_WORD_BOUNDARY;
	case TEXT('B'):
		return OX_REGEXP_TAG_NON_WORD_BOUNDARY;
	case TEXT('d'):
		return OX_REGEXP_TAG_DIGIT;
	case TEXT('D'):
		return OX_REGEXP_TAG_NON_DIGIT;
	case TEXT('f'):
		*pch=TEXT('\f');
		return OX_REGEXP_TAG_COMMON_CHAR;
	case TEXT('n'):
		*pch=TEXT('\n');
		return OX_REGEXP_TAG_COMMON_CHAR;
	case TEXT('r'):
		*pch=TEXT('\r');
		return OX_REGEXP_TAG_COMMON_CHAR;
	case TEXT('s'):
		return OX_REGEXP_TAG_WHITESPACE;
	case TEXT('S'):
		return OX_REGEXP_TAG_NON_WHITESPACE;
	case TEXT('t'):
		*pch=TEXT('\t');
		return OX_REGEXP_TAG_COMMON_CHAR;
	case TEXT('v'):
		*pch=TEXT('\v');
		return OX_REGEXP_TAG_COMMON_CHAR;
	case TEXT('w'):
		return OX_REGEXP_TAG_ANY_WORD;
	case TEXT('W'):
		return OX_REGEXP_TAG_ANY_NON_WORD;
	case TEXT('\\'):
	case TEXT('^'):
	case TEXT('$'):
	case TEXT('*'):
	case TEXT('+'):
	case TEXT('?'):
	case TEXT('.'):
	case TEXT('('):
	case TEXT(')'):
	case TEXT('|'):
	case TEXT('{'):
	case TEXT('}'):
	case TEXT('['):
	case TEXT(']'):
	case TEXT('/'):
		*pch=ch;
		return OX_REGEXP_TAG_COMMON_CHAR;
	default:
		if (ch<CHAR_ZERO || ch>CHAR_NINE)
		{
			SetError(OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR);
			return FALSE;
		}
		else
		{
			CString sTemp(ch);
			*pch=(TCHAR) _ttoi((LPCTSTR) sTemp);
			return OX_REGEXP_TAG_REFERENCE_BACK;
		}
		break;
	}
}

//called by Parse when found '/' char that
//means ASCII code found
BOOL COXRegExpression::GetAsciiChar(CString& sString, int* pnNumber, LPTSTR pch)
{
	*pch=NULL;
	CString sNumber;
	int nNumber=0;

	if (sString.GetLength()<=*pnNumber)
	{
		SetError(OX_REGEXP_ERROR_UNEXPECTED_END_ASCII);
		return FALSE;
	}

	while (sString.GetLength()>*pnNumber)
	{
		TCHAR ch=sString.GetAt(*pnNumber);
		(*pnNumber)++;
		if (ch==TEXT('/'))
		{
			if (sNumber.IsEmpty())
			{
				SetError(OX_REGEXP_ERROR_EMPTY_PARENTHESES_ASCII);
				return FALSE;
			}
			else
			{
				ASSERT(nNumber>=0 && nNumber<256);
				*pch=(TCHAR) nNumber;	
				return TRUE;
			}
		}
		else
			if (sNumber.GetLength()>=3)
			{
				SetError(OX_REGEXP_ERROR_WRONG_ASCII_NUMBER);
				return FALSE;
			}
			else
				if (ch<CHAR_ZERO || ch>CHAR_NINE)
				{
					SetError(OX_REGEXP_ERROR_MUST_DIGIT);
					return FALSE;
				}
				else
				{
					sNumber+=ch;
					nNumber=_ttoi((LPCTSTR) sNumber);
					if (nNumber<0 || nNumber>255)
					{
						SetError(OX_REGEXP_ERROR_WRONG_ASCII_NUMBER);
						return FALSE;
					}
				}

	}
	SetError(OX_REGEXP_ERROR_NO_LAST_PARENTHES_ASCII);
	return FALSE;
}

//called by Parse when
//encounters '{' character that means
//number of the preceding character found
int COXRegExpression::GetMinMaxCount(CString& sString, int* pnNumber, int* pMin, int* pMax)
{
	*pMin=-1;
	*pMax=-1;
	CString sNumber;
	int nComma=0;
	BOOL bAtLeast=FALSE;

	while(sString.GetLength()>*pnNumber)
	{
		TCHAR ch=sString.GetAt(*pnNumber);
		(*pnNumber)++;
		if (ch==TEXT('}'))
		{
			if (!bAtLeast)
			{
				if (sNumber.IsEmpty())
				{
					SetError(OX_REGEXP_ERROR_EMPTY_PARENTHESES);
					return FALSE;
				}
				*pMin=_ttoi((LPCTSTR) sNumber);
				return OX_REGEXP_TAG_CHARS_EXACTLY;
				
			}
			else
				if (sNumber.IsEmpty())
					return OX_REGEXP_TAG_CHARS_AT_LEAST;
				else
				{
				*pMax=_ttoi((LPCTSTR) sNumber);
					if (*pMax>*pMin)
						return OX_REGEXP_TAG_CHARS_AT_LEAST_MOST;
					else
					{
						*pnNumber=nComma;
						(*pnNumber)++;
						SetError(OX_REGEXP_ERROR_INVALID_MOSTLEAST);
						return FALSE;
					}
				}
		}
		else
			if (ch==TEXT(','))
			{
				if (bAtLeast)
				{
					SetError(OX_REGEXP_ERROR_TOO_MANY_COMMAS);
					return FALSE;
				}
				*pMin=sNumber.IsEmpty()?NULL:_ttoi((LPCTSTR) sNumber);
				sNumber="";
				bAtLeast=TRUE;
				nComma=*pnNumber;
			}
			else
				if (ch<CHAR_ZERO || ch>CHAR_NINE)
				{
					SetError(OX_REGEXP_ERROR_MUST_DIGIT);
					return FALSE;
				}
				else
				{
					sNumber+=ch;
				}
	}
	SetError(OX_REGEXP_ERROR_NO_LAST_PARENTHES);
	return FALSE;
}

//public function
//call this function to load rule for parse
BOOL COXRegExpression::LoadRule(CString sRule, int* pnNumber)
{
	while (m_arTags.GetSize())
	{
		delete (tRegExpTag*) m_arTags.GetAt(0);
		m_arTags.RemoveAt(0);
	}
	m_arTags.RemoveAll();
	m_nError=NULL;

	BOOL bRet=Parse(sRule, pnNumber);
	if (bRet)
		m_sRule=sRule;
	else
		RemoveAll();
	

	return bRet;

}

//public function
//call this function to load rule
BOOL COXRegExpression::LoadRule(LPCTSTR lpszRule, int* pnNumber)
{
	CString sRule=lpszRule;
	return LoadRule(sRule, pnNumber);
}

//this function translates error number to string
void COXRegExpression::TranslateError(int nError, CString& sError)
{
	switch (nError)
	{
	case OX_REGEXP_ERROR_MUST_CHAR_BEFORE:
		VERIFY(sError.LoadString(IDS_OX_REGEXPMUSTCHARBEFORE));//"Must preceed character before."
		break; 
	case OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR:
		VERIFY(sError.LoadString(IDS_OX_REGEXPUNEXPSPECCHAR));//"Unexpected special char has been found."
		break; 
	case OX_REGEXP_ERROR_UNEXPECTED_MATCH:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOTALLOWEDSEQ));//"Not allowed sequence has been found"
		break; 
	case OX_REGEXP_ERROR_UNEXPECTED_END_EITHER:
		VERIFY(sError.LoadString(IDS_OX_REGEXPMUSTFOLLOWBYCHAR));//"Must follow by character."
		break; 
	case OX_REGEXP_ERROR_UNEXPECTED_END_SPECCHAR:
		VERIFY(sError.LoadString(IDS_OX_REGEXPUNEXPENDOFSTR));//"Unexpected end of string has been reached."
		break; 
	case OX_REGEXP_ERROR_MUST_DIGIT:
		VERIFY(sError.LoadString(IDS_OX_REGEXPONLYDIGIT));//"Only digit allowed."
		break; 
	case OX_REGEXP_ERROR_UNEXPECTED_END_ASCII:
		VERIFY(sError.LoadString(IDS_OX_REGEXPUNEXPASCIIEND));//"Unexpected end of the ASCII sequence."
		break; 
	case OX_REGEXP_ERROR_EMPTY_PARENTHESES_ASCII:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOASCII));//"No ASCII value."
		break; 
	case OX_REGEXP_ERROR_WRONG_ASCII_NUMBER:
		VERIFY(sError.LoadString(IDS_OX_REGEXPINVALIDASCIINUMBER));//"Invalid ASCII number."
		break; 
	case OX_REGEXP_ERROR_NO_LAST_PARENTHES_ASCII:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOENDSPLASH));//"No ending slash has been found while processing ASCII value."
		break; 
	case OX_REGEXP_ERROR_EMPTY_PARENTHESES:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOVALUESINPARENTHS));//"No values between parentheses."
		break; 
	case OX_REGEXP_ERROR_TOO_MANY_COMMAS:
		VERIFY(sError.LoadString(IDS_OX_REGEXPTOOMANYCOMMAS));//"Too many commas in the sequence. Only one allowed."
		break; 
	case OX_REGEXP_ERROR_NO_LAST_PARENTHES:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOCLOSEPARENTH));//"No closing parentheses has been found."
		break;
	case OX_REGEXP_ERROR_UNEXPECTED_END_OF_STRING:
		VERIFY(sError.LoadString(IDS_OX_REGEXPUNEXPSTRINGEND));//"Unexpected end of string."
		break; 
	case OX_REGEXP_ERROR_INVALID_MOSTLEAST:
		VERIFY(sError.LoadString(IDS_OX_REGEXPSECONDGREATER));//"Second value must be greater first one."
		break;
	case OX_REGEXP_ERROR_NO_REFERENCE:
		VERIFY(sError.LoadString(IDS_OX_REGEXPNOSUCHREFRENS));//"No such back reference."
		break;
	default:
		VERIFY(sError.LoadString(IDS_OX_REGEXPUNKNOWNERROR));//"Unknown error"
	}
}


//retrieve string (pattern) by number
//called by Parse when '\0'-'\9' found
BOOL COXRegExpression::GetPattern(int nNumber, CString &sPattern)
{
	
	if (nNumber<0 ||nNumber>9)
		return FALSE;
	int nIndex=0;
	for (int n=0;n<GetTagCount();n++)
	{
		tRegExpTag* pTag=GetTag(n);
		if (pTag->nType==OX_REGEXP_TAG_PATTERN)
		{
			if (nIndex==nNumber)
			{
				sPattern=pTag->sValue;
				return TRUE;
			}
			else
				nIndex++;

		}
	}
	return FALSE;
}


//call this function to match your text
//to the rule you've loaded before
int COXRegExpression::Match(CString &sString)
{
	if (sString.IsEmpty() || !GetTagCount())
		return NULL;

	m_sText=sString;
	int nSuccess=0;
	m_nFound=0;
	for (m_nNumber=0;m_nNumber<sString.GetLength();m_nNumber++)
	{
		int nNumber=m_nNumber;
		nSuccess|=MatchNextTag(sString,0,&nNumber);
		if (nSuccess & OX_REGEXP_CANCELED)
			return m_nFound;
	}

	return (nSuccess?m_nFound:NULL);
}

//recursive function - called from
//Match. Do most of job to match text to the rule
int COXRegExpression::MatchNextTag(CString &sString, int nTag, int* pNumber)
{
	ASSERT(nTag<GetTagCount());
	
	tRegExpTag* pTag=GetTag(nTag);
	int nSuccess=FALSE;
	int nMin=pTag->nMin;
	int nMax=(pTag->nMax==-1)?
		(sString.GetLength()-1-*pNumber):pTag->nMax;


	if (!nMin)
	{
		if (nTag<GetTagCount()-1)
		{
			nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
			if (nSuccess & OX_REGEXP_CANCELED)
				return OX_REGEXP_CANCELED;
		}
		else
		{
			if (GetTagCount()==1)//no sense to do search 
				return FALSE;    // for 0 
			ASSERT((*pNumber)-m_nNumber>=0);
			m_nFound++;
			if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
				return OX_REGEXP_CANCELED;
			nSuccess=TRUE;
		}
	}

	switch (pTag->nType)
	{
	case OX_REGEXP_TAG_COMMON_CHAR:
	case OX_REGEXP_TAG_PATTERN:
		ASSERT(!pTag->sValue.IsEmpty());
	case OX_REGEXP_TAG_CHARS_AT_LEAST_MOST:
	case OX_REGEXP_TAG_CHARS_AT_LEAST:
		{

			for (int n=0;n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (sString.Right(sString.GetLength()-*pNumber).Find(
					(LPCTSTR) pTag->sValue)!=NULL)
				{
							return nSuccess;
				}
				else
				{
					*pNumber+=pTag->sValue.GetLength();
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
										return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}

					}
				}
			}
				return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_BEGINING_OF_THE_LINE:
		{
			for (int n=0;n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (sString.GetAt(*pNumber)!=TEXT('\n') && (*pNumber || n))
					return nSuccess;
				if (*pNumber) 
						(*pNumber)++;
				if (nTag==GetTagCount()-1)
					{
						ASSERT((*pNumber)-m_nNumber>=0);
						m_nFound++;
						if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
								return OX_REGEXP_CANCELED;
					}
				else
					{
						nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
						if (nSuccess & OX_REGEXP_CANCELED)
							return OX_REGEXP_CANCELED;
					}
			}
				return nSuccess;
		}
	case OX_REGEXP_TAG_END_OF_THE_LINE:
		{
			for (int n=0;n<nMax;n++)
			{
				if (*pNumber>sString.GetLength())
					return FALSE;
				if (*pNumber==sString.GetLength() ||
					((sString.GetLength()- *pNumber-1) &&
					(sString.GetAt(*pNumber+1)==TEXT('\n'))))
				{
					(*pNumber)++;
					if (nTag==GetTagCount()-1)
						{
							ASSERT((*pNumber)-m_nNumber>=0);
							m_nFound++;
							if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
								return OX_REGEXP_CANCELED;
							nSuccess=TRUE;
						}
					else
						{
							nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
							if (nSuccess & OX_REGEXP_CANCELED)
								return OX_REGEXP_CANCELED;
						}
				}
				else
					return nSuccess;
			}
		}
		break;
	case OX_REGEXP_TAG_CHAR_NOT_NEW_LINE:
		{
			for (int n=0;n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (sString.GetAt(*pNumber)==TEXT('\n'))
					return nSuccess;
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}

					}
				}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_CHARSET:
		{
			for (int n=0;n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (pTag->sValue.Find(sString.GetAt(*pNumber))==-1)
				{
					return nSuccess;
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}
					}
				}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_NON_CHARSET:
		{
			for (int n=0;n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (pTag->sValue.Find(sString.GetAt(*pNumber))!=-1)
				{
					return nSuccess;
				}
				(*pNumber)++;
				if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}

					}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_WORD_BOUNDARY:
		ASSERT(nMin==1 && nMax==1);//only one tag allowed
		if (*pNumber>=sString.GetLength())
			return FALSE;
		if ((*pNumber &&
			(((m_sSpace.Find(sString.GetAt(*pNumber)))==-1)
			^ ((m_sSpace.Find(sString.GetAt(*pNumber-1)))==-1)))
			|| (!*pNumber && 
			(m_sSpace.Find(sString.GetAt(*pNumber)))==-1 ))
		{
			if (nTag==GetTagCount()-1)
			{
				ASSERT((*pNumber)-m_nNumber>=0);
				m_nFound++;
				if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
					return OX_REGEXP_CANCELED;
				return TRUE;
			}
			else
				return MatchNextTag(sString, nTag+1, pNumber);
		}
		else
			return nSuccess;
		break;
	case OX_REGEXP_TAG_NON_WORD_BOUNDARY:
		ASSERT(nMin==1 && nMax==1);//only one tag allowed
		if (*pNumber>=sString.GetLength())
			return FALSE;
		if ((*pNumber &&
			!(((m_sSpace.Find(sString.GetAt(*pNumber)))==-1)
			^ ((m_sSpace.Find(sString.GetAt(*pNumber-1)))==-1)))
			|| (!*pNumber && 
			(m_sSpace.Find(sString.GetAt(*pNumber)))!=-1 ))

		{
			if (nTag==GetTagCount()-1)
			{
				ASSERT((*pNumber)-m_nNumber>=0);
				m_nFound++;
				if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
					return OX_REGEXP_CANCELED;
				return TRUE;
			}
			else
				return MatchNextTag(sString, nTag+1, pNumber);
		}
		else
			return nSuccess;
		break;
	case OX_REGEXP_TAG_DIGIT:
		{
			for (int n=0; n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (sString.GetAt(*pNumber)<TEXT('0') ||
					sString.GetAt(*pNumber)>TEXT('9'))
					return nSuccess;
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}

					}
				}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_NON_DIGIT:
		{
			for (int n=0; n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (sString.GetAt(*pNumber)>=TEXT('0') &&
					sString.GetAt(*pNumber)<=TEXT('9'))
				{
					return nSuccess;						
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
								
							}

					}
				}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_WHITESPACE:
		{
			for (int n=0; n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (m_sSpace.Find(sString.GetAt(*pNumber))==-1)
				{
						return nSuccess;
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
								
							}

					}
				}
			}
			return nSuccess;
		}

		break;
	case OX_REGEXP_TAG_NON_WHITESPACE:
		{
			for (int n=0; n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				if (m_sSpace.Find(sString.GetAt(*pNumber))!=-1)
				{
						return nSuccess;
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}
					}
				}
			}
			return nSuccess;
		}
		break;
	case OX_REGEXP_TAG_ANY_WORD:
		{
			if (sString.GetLength()<=*pNumber)
				return FALSE;
			for (int n=0; n<nMax;n++)
			{
				TCHAR ch=sString.GetAt(*pNumber);
				if (!(((ch>=TEXT('a') && ch<=TEXT('z')) ||
					(ch>=TEXT('A') && ch<=TEXT('Z')) ||
					(ch>=TEXT('0') && ch<=TEXT('9')) ||
					ch==TEXT('_'))))
				{
						return nSuccess;
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
								
							}

					}
				}
			}
			return nSuccess;
		}

		break;
	case OX_REGEXP_TAG_ANY_NON_WORD:
		{
			for (int n=0; n<nMax;n++)
			{
				if (sString.GetLength()<=*pNumber)
					return FALSE;
				TCHAR ch=sString.GetAt(*pNumber);
				if ((ch>=TEXT('a') && ch<=TEXT('z')) ||
					(ch>=TEXT('A') && ch<=TEXT('Z')) ||
					(ch>=TEXT('0') && ch<=TEXT('9')) ||
					ch==TEXT('_'))
				{
						return nSuccess;
				}
				else
				{
					(*pNumber)++;
					if (n+1>=nMin)
					{
							if (nTag<GetTagCount()-1)
							{
								nSuccess |=MatchNextTag(sString,nTag+1,pNumber);
								if (nSuccess & OX_REGEXP_CANCELED)
									return OX_REGEXP_CANCELED;
							}
							else
							{
								ASSERT((*pNumber)-m_nNumber>=0);
								m_nFound++;
								if (!OnMatch(m_nNumber, (*pNumber)-m_nNumber))
									return OX_REGEXP_CANCELED;
								nSuccess=TRUE;
							}
					}
				}
			}
			return nSuccess;
		}
		break;
	default:
		ASSERT(FALSE);//unexpected type
	}
	return nSuccess;

}

//this virtual function do nothing.
//it will be called every time matching substring has been found
//override this function to retrieve data
BOOL COXRegExpression::OnMatch(int nStart, int nLength)
{
	UNREFERENCED_PARAMETER(nStart);
	UNREFERENCED_PARAMETER(nLength);
	return TRUE;
}


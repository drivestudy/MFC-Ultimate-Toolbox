// ==========================================================================
// 					Class Specification : 
//						COXRegExpression 
// ==========================================================================

// Header file : OXRegExpression.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	YES	Derived from CObject

//	NO	Is a Cwnd. 
//	NO	Two stage creation 
//	NO	Has a message map

//	YES	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// COXRegExpression   - CObject derived class.
//
// COXRegExpression - new class that implements regular expression functionality.
//				Most of us had problems with searching some text in text files.
//				Yes, we can use searching capabilities that are provided with
//				many programs. But, in case we have to do some generic search
//				it is not the case. For such cases COXRegExpression class is exactly
//				what we had to have. This class provided generic search regarding
//				the rule you've supplied. To do search, you have to load regular 
//				expression that defines what do you want to search. There are
//				different formats and different implementation for regular expression.
//				This implementation is most close to that one described in
//				MSDN that comes with Visual Studio 6.0
//
//	The rules:
//
//  Character				 Description
//
//		\		 Marks the next character as special. All characters that are
//				 special but you want to define it for search should be 
//				 preceding by this character.
//		^		 Matches the begining of input or line. In this implementation this 
//				 cannot be defined in charset.
//		$		 Matches the end of input or line. In this implementation this 
//				 cannot be defined in charset.
//		*		 Matches preceding character zero or more times. In this implementation
//				 cannot be defined if only one character specified in the regular
//				 expression. That means that /zo*/ matches z and zoo, but
//				 /z*/ will match nothing because only one character has been specified.
//		+		 Matches preceding character one or more times. 
//		?		 Matches preceding character zero or one time. In this implementation
//				 cannot be defined if only one character specified in the regular
//				 expression.
//		.		 Matches any single character except '\n'
//	(pattern)	 Matches pattern and remembers the match. The matched substring can
//				 be retrieved by using '\0'-'\9' in regular expression, where '0'-'9'
//				 are number of the pattern. Example:
//				 regular expression '(re).*\0s+ion' will match 'regular expression'
//				 because first matches pattern 're' and remember the pattern with
//				 index 0. '.*' will match 'gular exp' in 'regular expression'.
//				 Now we rettrieve pattern with index 0, that has been remembered
//				 with index 0, this is 're' that matches 're' in 'regular expression'
//				 before 'ssion' and , finally, 's+ion' matches 'ssion'
//	   x|y		 Matches either character 'x' or 'y'. You can combine more than two
//				 characters like 'x|y|z'
//	   {n}		 Means preceding character will match exactly n times (nonnegative, of course)
//	  {n,}		 Means preseding character will match at least n times (nonnegative)
//	  {n,m}		 Means preceding character will match at keast n times and at 
//				 most m times. (n,m - nonnegative)
//	  [xyz]		 A character set. Matches any one of enclosed characters
//	 [^xyz]		 A non-matching character set. Matches any character that is not in the set.
//		\b		 Matches word boundary, that is boundary between any character excluding 
//				 space characters (" \f\n\r\t\v") and space characters
//		\B		 Matches non-word boundary. Matches any boundary between space 
//				 characters or between nonspace characters.
//		\d		 Matches any digit /0-9/
//		\D		 Matches any non-digit.
//		\f		 Matches a formfeed.
//		\n		 Matches a new-line character
//		\r		 Matches a carridge return character.
//		\s		 Matches any white space character
//		\S		 Matches any non-white space character
//		\t		 Matches a tab character
//		\v		 Matches any vertical tab character
//		\w		 Matches any word character including underscore. [A-Za-z0-9_]
//		\W		 Matches any non-word character (any character that does not match \w)
//		\num	 Where num is number between 0 and 9. Matches remmembered pattern.
//				 (See description of pattern)
//		/n/		 Where n is between 1 and 255. Matches supplied in n ASCII code
//
//
//	Three easy steps to use:
//	1) Create your own derived from COXRegExpression class and override function OnMatch()
//	   This virtual function is always called when a matched substring has been found.
//	   When this function is called you can return TRUE to continue search or FALSE
//	   if you do not want continue search.
//  2) Load your regular expression by function LoadRule()
//  3) Call function Match to do the search in supplied text.
//
//
//	Now how it works.
//	There are two steps in the work -1)load regular expression and 2) search the text
//
//	 1)load regular expression
//  When you are loading regular expression, LoadRule() calls function Parse() that
//  has a huge switch. This function do parsing of the rule and create tags of different
//	types for every sequences of special characters, depends on the type of special 
//	characters. For some special characters founded it calls appropriate functions
//  like GetAsciiChar() or GetEither() to do this special parsing. The tags are added
//  to the collection of tags. Any tag can be retrieved by GetTag().
//  The function Parse is virtual, so, if you need to process some special characters,
//  that are not specified here, you can override it. Once Parse() or called subfunction
//  found error, Parce stops parsing and returns FALSE. You can retrieve error code
//	by GetError() and translate it to description by TranslateError()
//
//	 2) search the text
//	The search text makes function Match() that call recursive function MatchNextTag()
//  with start tag number=0. Match() does loop, every time incrementing start position
//  in the supplied text for the search untill end of the text has been reached.
//  MatchNextTag() does main job and once the tag matches the text at the specified
//  position does either, call OnMatch() if the tag was last in the collection of the tags
//  or calls MatchNextTag() to match next tag from the collection.
//
//  The function MatchNextTag() like Parse() is virtual, so, it's up to you override it
//  if you want to process some special sequences.  				 



#if !defined(_OXREGEXPRESSION_H__)
#define _OXREGEXPRESSION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"
#include "UTB64Bit.h"



const int OX_REGEXP_ERROR_MUST_CHAR_BEFORE=1;
const int OX_REGEXP_ERROR_UNEXPECTED_SPECCHAR=2;
const int OX_REGEXP_ERROR_UNEXPECTED_MATCH=3;
const int OX_REGEXP_ERROR_UNEXPECTED_END_EITHER=4;
const int OX_REGEXP_ERROR_UNEXPECTED_END_SPECCHAR=5;
const int OX_REGEXP_ERROR_MUST_DIGIT=6;
const int OX_REGEXP_ERROR_UNEXPECTED_END_ASCII=7;
const int OX_REGEXP_ERROR_EMPTY_PARENTHESES_ASCII=8;
const int OX_REGEXP_ERROR_WRONG_ASCII_NUMBER=9;
const int OX_REGEXP_ERROR_NO_LAST_PARENTHES_ASCII=10;
const int OX_REGEXP_ERROR_EMPTY_PARENTHESES=11;
const int OX_REGEXP_ERROR_TOO_MANY_COMMAS=12;
const int OX_REGEXP_ERROR_NO_LAST_PARENTHES=13;
const int OX_REGEXP_ERROR_UNEXPECTED_END_OF_STRING=14;
const int OX_REGEXP_ERROR_INVALID_MOSTLEAST=15;
const int OX_REGEXP_ERROR_NO_REFERENCE=16;


const int OX_REGEXP_TAG_COMMON_CHAR=1;
const int OX_REGEXP_TAG_NOT_DEFINED=4;
const int OX_REGEXP_TAG_BEGINING_OF_THE_LINE=5;//^
const int OX_REGEXP_TAG_END_OF_THE_LINE=6;//$
const int OX_REGEXP_TAG_CHAR_NOT_NEW_LINE=13;//.
const int OX_REGEXP_TAG_PATTERN=15;//(PATTERN)
const int OX_REGEXP_TAG_CHARS_EITHER=17;//| //the same as charset
const int OX_REGEXP_TAG_CHARS_EXACTLY=18;//THE SAME common char
const int OX_REGEXP_TAG_CHARS_AT_LEAST=23;//{N,}
const int OX_REGEXP_TAG_CHARS_AT_LEAST_MOST=24;//{N,M}
const int OX_REGEXP_TAG_CHARSET=25;//[XYZ]
const int OX_REGEXP_TAG_NON_CHARSET=26;//[^XYZ]
const int OX_REGEXP_TAG_CHARSET_BEGIN_LINE=27;//[X^YZ]
const int OX_REGEXP_TAG_NON_CHARSET_BEGIN_LINE=28;//[^X^YZ]
const int OX_REGEXP_TAG_CHARSET_END_LINE=29;//[X$YZ]
const int OX_REGEXP_TAG_NON_CHARSET_END_LINE=30;//[^X$YZ]
const int OX_REGEXP_TAG_CHARSET_BEGIN_END_LINE=31;//[X^$YZ]
const int OX_REGEXP_TAG_NON_CHARSET_BEGIN_END_LINE=32;//[^X^$YZ]
const int OX_REGEXP_TAG_WORD_BOUNDARY=33;//\b
const int OX_REGEXP_TAG_NON_WORD_BOUNDARY=34;//\B
const int OX_REGEXP_TAG_DIGIT=35;//\d
const int OX_REGEXP_TAG_NON_DIGIT=36;//\D
const int OX_REGEXP_TAG_WHITESPACE=40;//\s
const int OX_REGEXP_TAG_NON_WHITESPACE=41;//\S
const int OX_REGEXP_TAG_ANY_WORD=44;//\w
const int OX_REGEXP_TAG_ANY_NON_WORD=45;//\W
const int OX_REGEXP_TAG_REFERENCE_BACK=46;//\num

const int OX_REGEXP_CANCELED=0x02;

#define CHAR_ZERO TEXT('0')
#define CHAR_NINE TEXT('9')

#define OX_REGEXP_VERSION 1

typedef struct sRegExpTag
{
	CString sValue;
	int nType;
	int nMin;
	int nMax;
	sRegExpTag():nType(OX_REGEXP_TAG_NOT_DEFINED),
	nMin(0),nMax(0){}

}tRegExpTag;

//////////////////////////////////////////////////////////////////////////////
// class COXRegExpression

class OX_CLASS_DECL COXRegExpression : public CObject
{
	DECLARE_SERIAL(COXRegExpression)
public:

//////////////////
//public functions


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect: Constructs the object
	COXRegExpression():m_nError(0), m_sSpace(_T(" \f\n\r\t\v")),
		m_nNumber(-1), m_nFound(0) {}

	// --- In  :	regExpression - instance COXRegExpression to be create
	//				from
	// --- Out : 
	// --- Returns:
	// --- Effect: Copy constructor
	COXRegExpression(const COXRegExpression& regExpression);
	
	virtual ~COXRegExpression();


	// --- In  :	sRule - regular expression to load into the object
	//				pnNumber - start position in this expression to load from
	// --- Out :    pnNumber - in case of error pointer to last valid character
	//						   in regular expression
	// --- Returns: TRUE on success, FALSE otherwise.
	//				In case of FALSE you can call GetError() 
	//				and TranslateError() functions to get reason of the error
	// --- Effect: Load regular expression into the object
	BOOL LoadRule(CString sRule, int* pnNumber=NULL);

	// --- In  :	lpszRule - regular expression to load into the object
	//				pnNumber - start position in this expression to load from
	// --- Out :    pnNumber - in case of error pointer to last valid character
	//						   in regular expression
	// --- Returns: TRUE on success, FALSE otherwise.
	//				In case of FALSE you can call GetError() 
	//				and TranslateError() functions to get reason of the error
	// --- Effect: Load regular expression into the object
	BOOL LoadRule(LPCTSTR lpszRule, int* pnNumber=NULL);

	// --- In  :	
	// --- Out : 
	// --- Returns: sRule - rule that has been loaded into the object
	// --- Effect: retrieves rule has been loaded into the object
	inline void GetRule(CString& sRule) const {sRule=m_sRule;}

	// --- In  :	sString - text to match to rule
	// --- Out : 
	// --- Returns: number of mathes
	// --- Effect: Searches the supplied text to match the rule
	//				loaded before by LoadRule() function.
	//				Every time matching substring has been found
	//				virtual function OnMatch() will be called
	int Match(CString& sString);

	// --- In  :
	// --- Out :	
	// --- Returns: error code 
	// --- Effect: returns error  that found object in the rule while loading it.
	inline int GetError() const {return m_nError;}

	// --- In  :	nError - error has been retrieved by GetError()
	// --- Out :	sError - description of the error
	// --- Returns:
	// --- Effect: translates error code into description
	void TranslateError(int nError, CString& sError);

	// --- In  :	nNumber - serial number of the tag
	// --- Out : 
	// --- Returns: requested tag
	// --- Effect:	retrieves tag by number, that has been loaded 
	//				by LoadRule()
	inline tRegExpTag* GetTag(int nNumber) const 
	{
		return   
		(nNumber>=0 && nNumber<m_arTags.GetSize())?
			(tRegExpTag*) m_arTags.GetAt(nNumber):NULL;
	}
	
	// --- In  :
	// --- Out : 
	// --- Returns: number of the tags 
	// --- Effect: number of the tags has been loaded by LoadRule()
	inline int GetTagCount() const {return  PtrToInt(m_arTags.GetSize());}

	// --- In  :	regExpression - instance of COXregExpression to assign to
	// --- Out : 
	// --- Returns:  
	// --- Effect: Assignment operator
	const COXRegExpression& operator=(const COXRegExpression& regExpression);

	// --- In  :	
	// --- Out : 
	// --- Returns:  
	// --- Effect: Retrieves searched string 
	inline CString GetText() const { return m_sText;} 

	// --- In  :	
	// --- Out : 
	// --- Returns:  
	// --- Effect: Retrieves searched string 
	inline int GetFound() const {return m_nFound;}

/////////////////
//public members

protected:

/////////////////////
//protected functions

	// --- In  :	nStart - number of first character of 
	//						 the matched substring
	//				nLength - length of the matching substring
	// --- Out : 
	// --- Returns: if you want to continue search you must 
	//				return TRUE, to stop the search return FALSE
	// --- Effect: Override this function to retrieve founded matching
	//				substrings. This function is called by MatchNextTag()
	//				every time searched text matches the rule
	virtual BOOL OnMatch(int nStart,int nLength);
	
	// --- In  :	sString - text to be searched
	//				nTag - number of the tag to match to
	//				pNumber - number of the first character 
	//				in the text to match to
	// --- Out :	
	// --- Returns: non-zero if this tag matches the text 
	//				at the position specified by pNumber
	// --- Effect:  This recursive function called to match the tag
	//				to the supplied text
	virtual BOOL MatchNextTag(CString& sString, int nTag, int* pNumber);
	
	// --- In  :	sString - regular expression to load 
	//						  into the object
	//				pnNumber - first character in this expresiion 
	//						   to parse from	
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise.
	// --- Effect: Called by LoadRule() to parse supplied 
	//				regular expression. If function not succeed,
	//				call GetError() and TranslateError() to
	//				get type of the error
	virtual BOOL Parse(CString& sString, int* pnNumber=NULL);
	
	// --- In  :	sString - regular expression to load
	//				pnNumber - pointer to special char in 
	//						   the regular expression
	//--- Out :		pch - special character found if any	
	// --- Returns: type of the tag depends on type 
	//				the special character
	// --- Effect: Do not call this function directly. 
	//				Override it if you need to process your special characters
	virtual int GetSpecialChar(CString& sString, int* pnNumber, LPTSTR pch);
	
	BOOL GetPattern(int nNumber, CString& sPattern);
	void AddTag(int nType, CString* pValue, int nFirst=1, int nSecond=1);
	inline void SetError(int nError) {	m_nError=nError;}
	int GetMinMaxCount(CString& sString, int* pnNumber, 
		int* pMin, int* pMax);

	BOOL GetEither(CString& sString,int* pnNumber,LPTSTR pchEither);
	BOOL GetAsciiChar(CString& sString, int* pnNumber, LPTSTR pch);

	inline void RemoveAll() 
	{
		while(m_arTags.GetSize())
		{
			delete (tRegExpTag*) m_arTags.GetAt(0);
			m_arTags.RemoveAt(0);
		};
		m_sRule="";
	}

/////////////////////
//protected members

	CPtrArray m_arTags;
	CString m_sRule;
	int m_nNumber;
	int m_nError;
	CString m_sSpace;
	CString m_sText;
	int m_nFound;
};

#endif // !defined(_OXREGEXPRESSION_H__)

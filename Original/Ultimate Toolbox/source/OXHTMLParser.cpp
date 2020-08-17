// HTMLParser.cpp: implementation of the COXHTMLParser class.
//
//////////////////////////////////////////////////////////////////////
// Version: 9.3


#include "stdafx.h"
#include "OXHTMLParser.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Limitations:
// - <a href=http://domain/directory/script.cgi?ord="string"> tags not parsed correctly
// - only VERY limited validation is done. For instance, there is no check on tag 
//   ordering or nesting (eg <head> may appear after <body>)
// - No real validation is performed. For instance, empty <p></p> tags are not removed.
// 
// The full solution is to make COXParser fully XML 1.0 compliant in regards to Document
// Type Definitions, then simply provide a HTML DTD for the given version of HTML to
// be parsed. One day...

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Tags (up to HTML 4.0)
// Tag Flags:
//
//   TAG_BLOCK   - tags that delimit blocks that signify the end of a paragraph 
//   TAG_OPTEND  - tags that have an optional closing tag eg <P>
//   TAG_EMPTY   - tags that do not have a closing tag at all (eg <HR>)
//   TAG_SECTION - tags for special sections (eg <BODY>)
//   TAG_CANCENTER - can be wrapped by a "center" tag
// 
// Text flags - for tags that modify text formatting (eg <B>). These flags
//              are not block elements, but merely font attribute "toggles"
TagDescriptor COXHTMLParser::m_HTMLTags[] = 
{
  //   Tag name            Tag Flags                           Tag Text Flag
     { TEXT("HTML"),       TAG_BLOCK|TAG_OPTEND|TAG_SECTION,   0,              },

     { TEXT("HEAD"),       TAG_BLOCK|TAG_OPTEND|TAG_SECTION,   0,              },

     { TEXT("TITLE"),      TAG_HEAD|TAG_BLOCK,                 0,              },
     { TEXT("BASE"),       TAG_HEAD|TAG_BLOCK|TAG_EMPTY,       0,              },
     { TEXT("LINK"),       TAG_HEAD|TAG_EMPTY,                 0,              },
     { TEXT("META"),       TAG_HEAD|TAG_BLOCK|TAG_EMPTY,       0,              },
     { TEXT("SCRIPT"),     TAG_HEAD|TAG_BLOCK,                 0,              },
     { TEXT("STYLE"),      TAG_HEAD,                           0,              },

     { TEXT("BODY"),       TAG_BLOCK|TAG_OPTEND|TAG_SECTION,   0,              },
     { TEXT("FRAMESET"),   TAG_BLOCK,                          0,              },

     { TEXT("A"),          0,                                  0               },
     { TEXT("ABBR"),       0,                                  0,              },
     { TEXT("ACRONYM"),    0,                                  0,              },
     { TEXT("ADDRESS"),    TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("APPLET"),     TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("AREA"),       TAG_EMPTY,                          0,              },  
     { TEXT("B"),          0,                                  TEXT_BOLD,      },
     { TEXT("BASEFONT"),   TAG_BLOCK|TAG_EMPTY,                0,              },
     { TEXT("BDO"),        0,                                  0,              },
     { TEXT("BIG"),        0,                                  TEXT_BIG,       },
     { TEXT("BLOCKQUOTE"), TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("BR"),         TAG_EMPTY,                          0,              },
     { TEXT("BUTTON"),     TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("CAPTION"),    TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("CENTER"),     TAG_BLOCK,                          TEXT_CENTER,    },
     { TEXT("CITE"),       0,                                  TEXT_CITE,      },
     { TEXT("CODE"),       0,                                  TEXT_CODE,      },
     { TEXT("COL"),        TAG_BLOCK|TAG_EMPTY,                0,              },
     { TEXT("COLGROUP"),   TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("DD"),         TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("DEL"),        0,                                  TEXT_DEFN,      },
     { TEXT("DFN"),        0,                                  0,              },
     { TEXT("DIR"),        TAG_BLOCK,                          0,              },
     { TEXT("DIV"),        TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("DL"),         TAG_BLOCK,                          0,              },
     { TEXT("DT"),         TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("EM"),         0,                                  TEXT_EMPHASIS,  },
     { TEXT("FIELDSET"),   TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("FONT"),       0,                                  TEXT_FONT,      },
     { TEXT("FORM"),       TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("FRAME"),      TAG_BLOCK|TAG_EMPTY|TAG_CANCENTER,  0,              },
     { TEXT("H1"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("H2"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("H3"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("H4"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("H5"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("H6"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("HR"),         TAG_BLOCK|TAG_EMPTY,                0,              },
     { TEXT("I"),          0,                                  TEXT_ITALIC,    },
     { TEXT("IFRAME"),     TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("IMG"),        TAG_EMPTY|TAG_CANCENTER,            0,              },
     { TEXT("INPUT"),      TAG_BLOCK|TAG_EMPTY|TAG_CANCENTER,  0,              },
     { TEXT("INS"),        0,                                  0,              },
     { TEXT("ISINDEX"),    TAG_EMPTY,                          0,              },
     { TEXT("KBD"),        0,                                  TEXT_KEYBOARD,  },
     { TEXT("LABEL"),      TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("LEGEND"),     TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("LI"),         TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("MAP"),        TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("MENU"),       TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("NOFRAMES"),   TAG_BLOCK,                          0,              },
     { TEXT("NOSCRIPT"),   TAG_BLOCK,                          0,              },
     { TEXT("OBJECT"),     TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("OL"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("OPTGROUP"),   0,                                  0,              },
     { TEXT("OPTION"),     TAG_OPTEND,                         0,              },
     { TEXT("P"),          TAG_BLOCK|TAG_OPTEND|TAG_CANCENTER, 0,              },
     { TEXT("PARAM"),      TAG_EMPTY,                          0,              },
     { TEXT("PRE"),        0,                                  TEXT_FORMATTED, },
     { TEXT("Q"),          TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("S"),          0,                                  TEXT_STRIKE,    },
     { TEXT("SAMP"),       0,                                  TEXT_SAMPLE,    },
     { TEXT("SELECT"),     0,                                  0,              },
     { TEXT("SMALL"),      0,                                  TEXT_SMALL,     },
     { TEXT("SPAN"),       0,                                  0,              },
     { TEXT("STRIKE"),     0,                                  TEXT_STRIKE,    },
     { TEXT("STRONG"),     0,                                  TEXT_STRONG,    },
     { TEXT("SUB"),        0,                                  TEXT_SUB,       },
     { TEXT("SUP"),        0,                                  TEXT_SUP,       },
     { TEXT("TABLE"),      TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("TBODY"),      TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("TD"),         TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("TEXTAREA"),   0,                                  0,              },
     { TEXT("TFOOT"),      TAG_OPTEND,                         0,              },
     { TEXT("TH"),         TAG_OPTEND,                         0,              },
     { TEXT("THEAD"),      TAG_OPTEND,                         0,              },
     { TEXT("TR"),         TAG_BLOCK|TAG_OPTEND,               0,              },
     { TEXT("TT"),         0,                                  TEXT_TELETYPE,  },
     { TEXT("U"),          0,                                  TEXT_UNDERLINE, },
     { TEXT("UL"),         TAG_BLOCK|TAG_CANCENTER,            0,              },
     { TEXT("VAR"),        0,                                  TEXT_VAR,       },

     { NULL,               0,                                  0,              },
};

// Taken from the "Tidy" program from the W3C site. Some of the following tags 
// are not recognised by the major browsers, so leave the esoteric ones out for now.
ParserEntity COXHTMLParser::m_HTMLEntity[] =
{
    { TEXT("nbsp"),   TEXT(" "), },
    //{ TEXT("iexcl"),  TEXT("¡"), },
    { TEXT("cent"),   TEXT("¢"), },
    { TEXT("pound"),  TEXT("£"), },
    //{ TEXT("curren"), TEXT("¤"), },
    //{ TEXT("yen"),    TEXT("¥"), },
    //{ TEXT("brvbar"), TEXT("¦"), },
    //{ TEXT("sect"),   TEXT("§"), },
    //{ TEXT("uml"),    TEXT("¨"), },
    { TEXT("copy"),   TEXT("©"), },
    //{ TEXT("ordf"),   TEXT("ª"), },
    //{ TEXT("laquo"),  TEXT("«"), },
    //{ TEXT("not"),    TEXT("¬"), },
    //{ TEXT("shy"),    TEXT("­"), },
    { TEXT("reg"),    TEXT("®"), },
    //{ TEXT("macr"),   TEXT("¯"), },
    //{ TEXT("deg"),    TEXT("°"), },
    //{ TEXT("plusmn"), TEXT("±"), },
    //{ TEXT("sup2"),   TEXT("²"), },
    //{ TEXT("sup3"),   TEXT("³"), },
    //{ TEXT("acute"),  TEXT("´"), },
    //{ TEXT("micro"),  TEXT("µ"), },
    //{ TEXT("para"),   TEXT("¶"), },
    //{ TEXT("middot"), TEXT("·"), },
    //{ TEXT("cedil"),  TEXT("¸"), },
    //{ TEXT("sup1"),   TEXT("¹"), },
    //{ TEXT("ordm"),   TEXT("º"), },
    //{ TEXT("raquo"),  TEXT("»"), },
    //{ TEXT("frac14"), TEXT("¼"), },
    //{ TEXT("frac12"), TEXT("½"), },
    //{ TEXT("frac34"), TEXT("¾"), },
    //{ TEXT("iquest"), TEXT("¿"), },
    //{ TEXT("Agrave"), TEXT("À"), },
    //{ TEXT("Aacute"), TEXT("Á"), },
    //{ TEXT("Acirc"),  TEXT("Â"), },
    //{ TEXT("Atilde"), TEXT("Ã"), },
    //{ TEXT("Auml"),   TEXT("Ä"), },
    //{ TEXT("Aring"),  TEXT("Å"), },
    //{ TEXT("AElig"),  TEXT("Æ"), },
    //{ TEXT("Ccedil"), TEXT("Ç"), },
    //{ TEXT("Egrave"), TEXT("È"), },
    //{ TEXT("Eacute"), TEXT("É"), },
    //{ TEXT("Ecirc"),  TEXT("Ê"), },
    //{ TEXT("Euml"),   TEXT("Ë"), },
    //{ TEXT("Igrave"), TEXT("Ì"), },
    //{ TEXT("Iacute"), TEXT("Í"), },
    //{ TEXT("Icirc"),  TEXT("Î"), },
    //{ TEXT("Iuml"),   TEXT("Ï"), },
    //{ TEXT("ETH"),    TEXT("Ð"), },
    //{ TEXT("Ntilde"), TEXT("Ñ"), },
    //{ TEXT("Ograve"), TEXT("Ò"), },
    //{ TEXT("Oacute"), TEXT("Ó"), },
    //{ TEXT("Ocirc"),  TEXT("Ô"), },
    //{ TEXT("Otilde"), TEXT("Õ"), },
    //{ TEXT("Ouml"),   TEXT("Ö"), },
    //{ TEXT("times"),  TEXT("×"), },
    //{ TEXT("Oslash"), TEXT("Ø"), },
    //{ TEXT("Ugrave"), TEXT("Ù"), },
    //{ TEXT("Uacute"), TEXT("Ú"), },
    //{ TEXT("Ucirc"),  TEXT("Û"), },
    //{ TEXT("Uuml"),   TEXT("Ü"), },
    //{ TEXT("Yacute"), TEXT("Ý"), },
    //{ TEXT("THORN"),  TEXT("Þ"), },
    //{ TEXT("szlig"),  TEXT("ß"), },
    //{ TEXT("agrave"), TEXT("à"), },
    //{ TEXT("aacute"), TEXT("á"), },
    //{ TEXT("acirc"),  TEXT("â"), },
    //{ TEXT("atilde"), TEXT("ã"), },
    //{ TEXT("auml"),   TEXT("ä"), },
    //{ TEXT("aring"),  TEXT("å"), },
    //{ TEXT("aelig"),  TEXT("æ"), },
    //{ TEXT("ccedil"), TEXT("ç"), },
    //{ TEXT("egrave"), TEXT("è"), },
    //{ TEXT("eacute"), TEXT("é"), },
    //{ TEXT("ecirc"),  TEXT("ê"), },
    //{ TEXT("euml"),   TEXT("ë"), },
    //{ TEXT("igrave"), TEXT("ì"), },
    //{ TEXT("iacute"), TEXT("í"), },
    //{ TEXT("icirc"),  TEXT("î"), },
    //{ TEXT("iuml"),   TEXT("ï"), },
    //{ TEXT("eth"),    TEXT("ð"), },
    //{ TEXT("ntilde"), TEXT("ñ"), },
    //{ TEXT("ograve"), TEXT("ò"), },
    //{ TEXT("oacute"), TEXT("ó"), },
    //{ TEXT("ocirc"),  TEXT("ô"), },
    //{ TEXT("otilde"), TEXT("õ"), },
    //{ TEXT("ouml"),   TEXT("ö"), },
    //{ TEXT("divide"), TEXT("÷"), },
    //{ TEXT("oslash"), TEXT("ø"), },
    //{ TEXT("ugrave"), TEXT("ù"), },
    //{ TEXT("uacute"), TEXT("ú"), },
    //{ TEXT("ucirc"),  TEXT("û"), },
    //{ TEXT("uuml"),   TEXT("ü"), },
    //{ TEXT("yacute"), TEXT("ý"), },
    //{ TEXT("thorn"),  TEXT("þ"), },
    //{ TEXT("yuml"),   TEXT("ÿ"), },
    
    NULL, 0
};

COXHTMLParser::COXHTMLParser()
{
    m_FontStack.clear();
    m_TextStyleStack.clear();

    m_bErrorOnMissingTag = FALSE;

    SetCaseSensitive(FALSE);
    m_HTMLTagTable.SetCaseSensitive(FALSE);

    // Fill hash table with tags
	int i = 0;
    for (i = 0; m_HTMLTags[i].szTag; i++)
        m_HTMLTagTable.Add(m_HTMLTags[i].szTag, (DWORD)(INT_PTR) &(m_HTMLTags[i]));

    // Add predefined HTML character entities to the entity list
    for (i = 0; m_HTMLEntity[i].szName; i++)
        m_EntityTable.Add(m_HTMLEntity[i].szName, (DWORD)(INT_PTR) m_HTMLEntity[i].szLiteral);
}

COXHTMLParser::~COXHTMLParser()
{
}

void COXHTMLParser::Clear()
{
    COXParser::Clear();

    // Clear out the font stack
    for (UINT i = 0; i < m_FontStack.size(); i++)
    {
        COXParserElement* pElm = (COXParserElement*) m_FontStack[i];
        delete pElm;
    }
    m_FontStack.clear();
    m_TextStyleStack.clear();
}

BOOL COXHTMLParser::Initialize()
{
    BOOL bResult = COXParser::Initialize();

    PushTextStyle();

    // Delete the &apos; tag from the entity table - HTML browsers don't seem to
    // support it.
    m_EntityTable.Remove(TEXT("apos"));

    return bResult;
}

BOOL COXHTMLParser::Cleanup()
{
    BOOL bResult = COXParser::Cleanup();

    return bResult;
}

// Make a new copy of the font element
COXParserElement* COXHTMLParser::DuplicateFontElement(COXParserElement* pElement)
{
    if (!pElement || !pElement->IsName(TEXT("font")))
        return NULL;
    
    COXParserElement* pFontElement = new COXParserElement(NULL, pElement->GetName());
    for (int i = 0; i < pElement->NumAttributes(); i++)
    {
        if (!pElement->Attribute(i))
            continue;

        COXAttribute* pAttribute = new COXAttribute;
        if (!pAttribute)
        {
            delete pFontElement;
            return NULL;
        }
        
        pAttribute->SetName(pElement->Attribute(i)->GetName());
        pAttribute->SetValue(pElement->Attribute(i)->GetStringValue());
        
        pFontElement->AddAttribute(pAttribute); 
    }

    return pFontElement;
}

COXParserElement* COXHTMLParser::ConstructFontElement()
{
    // Check font stack
    if (m_FontStack.size() <= 0)
        return NULL;
    
    // We need to combine the various font tags that have been encountered
    // into one element. We go through the font stack and add attributes,
    // and if we encounter a repeated attribute, then the latest one found
    // takes precedence.
    COXParserElement* pNewFontElement = new COXParserElement(NULL, TEXT("font"));
    
    if (pNewFontElement)
    {
        for (UINT i = 0; i < m_FontStack.size(); i++)
        {
            // Get the "font" element off the top of the stack
            COXParserElement* pFontElement = (COXParserElement*) m_FontStack[i];
            if (!pFontElement) continue;
            
            // Create a new element that matches this font element, and insert
            // it between the parent and our new object. This effectively "wraps"
            // the current text object in the current font attribute
            for (int i = 0; i < pFontElement->NumAttributes(); i++)
            {
                COXAttribute* pFontAttr = pFontElement->Attribute(i);
                if (!pFontAttr) continue;
                
                COXAttribute* pAttr = pNewFontElement->FindAttribute(pFontAttr->GetName());
                
                // If the attribute exists, copy over it, otherwise add it
                if (pAttr)
                    pAttr->SetValue(pFontAttr->GetStringValue());
                else
                {
                    pAttr = new COXAttribute;
                    pAttr->SetName(pFontAttr->GetName());
                    pAttr->SetValue(pFontAttr->GetStringValue());
                    pNewFontElement->AddAttribute(pAttr);
                }
            }
        }    
    }

    return pNewFontElement;
}

COXParserObject* COXHTMLParser::ParseText(COXParserElement* pParent)
{
    COXParserObject* pObject = COXParser::ParseText(pParent);
    if (!pObject)
        return NULL;
    
    // Stop text modifiers working inside tables 
    //if (pParent->IsName(TEXT("TD")))
    //    return pObject;

    // Set this text element within nested text modifier elements, depending
    // on the current text modifiers that have been turned on.

    UINT nTextStyle = GetTextStyle(); 

    // Add a space to the end of the text
    COXQuickString str = pObject->GetText();

    // Strip out whitespace (unless formatted)
    if (!(nTextStyle & TEXT_FORMATTED) && !(nTextStyle & TEXT_PRE))
        str.Strip();

    // add a space if necessary (160 = non-break space
    TCHAR chNBSP = TEXT(' '); // This is character 160, NOT character 32
    if (str.GetLength() && str[str.GetLength()-1] != chNBSP)
        str.Append(TEXT(' '));

    pObject->SetText(str);
    
    // Check current text mode
    for (int i = 0; m_HTMLTags[i].szTag; i++)
    {
        // Don't add center tags here - they will be added as a wrap around
        // the element holding this text
        if (!m_HTMLTags[i].dwTextFlag || (m_HTMLTags[i].dwTextFlag & TEXT_CENTER))
            continue;


        // If TextModifiers[i] is on, then create a new element with its name,
        // and add it to the beginning of the chain.
        if (nTextStyle & m_HTMLTags[i].dwTextFlag)
        {
            COXParserElement* pElm = new COXParserElement(NULL, m_HTMLTags[i].szTag);
            if (!pElm)
                break;
            
            pElm->AddObject(pObject);   // Add new element to begining of chain
            pObject = pElm;             // Move to beginning of chain
        }
    }
    
    COXParserElement* pNewFontElement = ConstructFontElement();
    if (pNewFontElement)
    {
        pNewFontElement->AddObject(pObject);
        pObject = pNewFontElement;
    }

    // Add a <p> wrapper if we have found standalone text
    if (pParent->GetParent() == NULL)
    {
        COXParserElement* pElm = new COXParserElement(pParent, TEXT("p"));
        pElm->SetFlags(GetTextStyle());
        if (pElm)
        {
            pElm->AddObject(pObject);
            pObject = pElm;             // Move to beginning of chain
            if (!ParseElement(pElm, 1))
            {
                delete pObject;
                pObject = NULL;
            }
        }
    }
    
    return pObject;
}

BOOL COXHTMLParser::GetValueString(COXQuickString& str)
{
    str.Empty();
    str.SetLength(100);
    
    TCHAR ch = GetNextChar();
    while (ch && ch != m_chEndDelim && !_istspace(ch))
    {
        if (ch == m_chTagEnd)
        {
            TCHAR chNext = GetNextChar();
            UngetChar();
            if (chNext == m_chEndDelim)
                break;
        }
        str.Append(ch);
        ch = GetNextChar();
    }

    BOOL bResult = TRUE;

    if (ch == m_chNULL)
    {
        ReportError(ERROR_END_OF_BUFFER,  TEXT("Unexpected end of buffer while name."));
        bResult = FALSE;
    }
    else
        UngetChar();

    if (!bResult)
        str.Empty();

    return bResult;
}

// Add new object to element. Insert text modifiers if necessary
void COXHTMLParser::AddObjectToElement(COXParserElement* pElement, COXParserObject* pObject)
{
    if (!pObject)
        return;
    
    if (pObject->GetType() == COXParserObject::ELEMENT)
    {
        // wrap blocks with "center" if centering was in force
        if ( (pObject->GetFlags() & TEXT_CENTER) && CanCenter(pObject->GetText()))
        {
            COXParserElement* pElm = new COXParserElement(NULL, TEXT("center"));
            if (pElm)
            {
                pElm->AddObject(pObject);   // Add new element to begining of chain
                pObject = pElm;             // Move to beginning of chain
            }
        }
    }

    pElement->AddObject(pObject);
}

// Modify the name/value parser so that all values are string values,
// and value=<string value> (ie no quotes) is OK. Also allow empty values,
// eg <tr nowrap>
BOOL COXHTMLParser::ParseAttributes(COXParserElement* pElement)
{
    BOOL bResult = TRUE;
    while (bResult)
    {
        bResult = GetToken(m_Token);
        if (!bResult)
        {
            ReportError(ERROR_END_OF_BUFFER,
                        TEXT("Unexpected end of buffer while parsing attributes (Element %s)"), 
                        pElement->GetName());
            break;
        }

        // end of attribute list?
        if (m_Token.GetType() != COXToken::STRING)
            break;

        // Create a new name/value
        COXAttribute* pAttribute = new COXAttribute;
        if (!pAttribute)
        {
            ReportError(ERROR_OUT_OF_MEMORY, 
                        TEXT("Unable to create new attribute (Element %s)"), 
                        pElement->GetName());
            bResult = FALSE;
            break;
        }

        // Get the name of the name/value pair
        COXQuickString str;
        if (!GetNameToken(str))
        {
            bResult = FALSE;
            break;
        }
        pAttribute->SetName(str);

        SAVEPOS pos;
        SaveBufferPos(pos);

        // May have "=" sign next
        if (!GetToken(m_Token))
        {
            ReportError(ERROR_BAD_TOKEN, 
                        TEXT("Error while parsing attribute (Element %s, name %s)."),
                        pElement->GetName(), pAttribute->GetName());
            delete pAttribute;
            bResult = FALSE;
            break;
        }

        // If an empty Attribute then continue on without searching for a value
        if (m_Token.GetType() != COXToken::EQUAL_SIGN)
        {
            pAttribute->SetValue(TEXT(""));
            pElement->AddAttribute(pAttribute);
            RestoreBufferPos(pos);
            continue;
        }

        // Should have a number, "string" or 'string' value next.
        if (!GetToken(m_Token))
        {
            ReportError(ERROR_BAD_TOKEN, 
                        TEXT("Error while parsing attribute (element %s, name %s)."),
                        pElement->GetName(), pAttribute->GetName());
            bResult = FALSE;
            break;
        }

        if (m_Token.GetType() == COXToken::STRING)
        {
            if (!GetValueString(str))
            {
                bResult = FALSE;
                break;
            }
            pAttribute->SetValue(str);
        }
        else if (m_Token.GetType() == COXToken::QUOTE)
        {
            if ( !GetStringToken(str, TEXT('"')) ) 
            {
                bResult = FALSE;
                break;
            }
            pAttribute->SetValue(str);
        }
        else if (m_Token.GetType() == COXToken::APOSTROPHE)
        {
            if ( !GetStringToken(str, TEXT('\'')) ) 
            {
                bResult = FALSE;
                break;
            }
            pAttribute->SetValue(str);
        }
        else
        {
            ReportError(ERROR_UNEXPECTED_TOKEN, 
                        TEXT("Unexpected token while parsing attribute (element %s, name %s)."),
                        pElement->GetName(), pAttribute->GetName());
            bResult = FALSE;
            break;
        }

        if (bResult)
            pElement->AddAttribute(pAttribute);
    }

    return bResult;
}

// In HTML, some tags such as <br> don't have an end tag - so treat them
// as Empty Tags.
COXParserElement* COXHTMLParser::ParseStartTag(COXParserElement* pParent, BOOL& bEmptyTag)
{
    COXParserElement* pElement = COXParser::ParseStartTag(pParent, bEmptyTag);
    if (!pElement)
        return NULL;

    // Force to empty tag for "simple" tags (HR, BR etc)
    if (IsEmptyTag(pElement->GetName()))
        bEmptyTag = TRUE;
   
    // Store the current text mode
    pElement->SetFlags(GetTextStyle());

    // Wrap empties with P's
    if (pParent->GetParent() == NULL       && 
        !IsBlockTag(pElement->GetName())   &&
        !IsHeadTag(pElement->GetName())    &&
        !IsTextModifier(pElement->GetName()))
    {
        COXParserElement* pElm = new COXParserElement(NULL, TEXT("P"));
        pElm->AddObject(pElement);   // Add new element to begining of chain
        pElement = pElm;             // Move to beginning of chain
    }

    // For new tables push a new text style onto the stack
    if (_tcsicmp(pElement->GetName(), TEXT("table")) == 0)
        PushTextStyle();

    return pElement;
}

BOOL COXHTMLParser::ParseEndTag(COXParserElement* pElement, COXQuickString& strEndTag)
{
    BOOL bResult = COXParser::ParseEndTag(pElement, strEndTag);
    if (!bResult)
        return FALSE;

    if ( !_tcsicmp(strEndTag, TEXT("table")) && 
         !_tcsicmp(strEndTag, pElement->GetName()) )
    {
        PopTextStyle();
    }

    return TRUE;
}

// Text modifier tags <b>, <i> etc don't work in the normal way. We treat
// them as "toggles" and just get the base parser to ignore them
BOOL COXHTMLParser::IgnoreStartTag(COXParserElement* pElement, BOOL bEmptyTag)
{
    UNUSED_ALWAYS(bEmptyTag);

    BOOL bIgnore = FALSE;

    // If we hit a font tag, then add it to the top of the font stack
    if (pElement->IsName(TEXT("font")))
    {
        COXParserElement* pFontElement = DuplicateFontElement(pElement);
        if (pFontElement)
            m_FontStack.push_back(pFontElement);

        bIgnore = TRUE;
    }
    else
    {   
        TagDescriptor* pTag = GetTagDescriptor(pElement->GetName());
        if (pTag && pTag->dwTextFlag)
        {
            //for (UINT i = 0; i < m_TextStyleStack.size(); i++)
            //    TRACE2("Before: Text style %d: %d\n",i,m_TextStyleStack[i]);

            UINT nTextStyle = GetTextStyle(); 
            SetTextStyle(nTextStyle | pTag->dwTextFlag);

            //for (i = 0; i < m_TextStyleStack.size(); i++)
            //    TRACE2("After: Text style %d: %d\n",i,m_TextStyleStack[i]);

            bIgnore = TRUE;
        }
    }

    return bIgnore;
}

// Text modifier tags <b>, <i> etc don't work in the normal way. We treat
// them as "toggles" and just get the base parser to ignore them
BOOL COXHTMLParser::IgnoreEndTag(LPCTSTR szEndTag)
{
    // If we come across a text modifier then we don't want to check for
    // the normal pair - /pair nesting. The text modifiers merely turn-on
    // and turn-off text attributes
    BOOL bTextModifier = FALSE;

    // If we hit a /font tag, then pop off the top font from the font stack
    if (_tcsicmp(szEndTag, TEXT("font")) == 0)
    {
        if (m_FontStack.size())
        {
            COXParserElement* pFont = (COXParserElement*) m_FontStack.back();
            delete pFont;
            m_FontStack.pop_back();
        }
        bTextModifier = TRUE;
    }
    else
    {
        TagDescriptor* pTag = GetTagDescriptor(szEndTag);
        if (pTag && pTag->dwTextFlag)
        {
            bTextModifier = TRUE;

            //for (UINT i = 0; i < m_TextStyleStack.size(); i++)
            //    TRACE2("Before: Text style %d: %d\n",i,m_TextStyleStack[i]);

            UINT nStyle = GetTextStyle();
            SetTextStyle(nStyle & ~(pTag->dwTextFlag));

            //for (i = 0; i < m_TextStyleStack.size(); i++)
            //    TRACE2("After: Text style %d: %d\n",i,m_TextStyleStack[i]);
        }
    }

    return bTextModifier;
}

// Returns TRUE if the tag is an empty tag (eg HR, BR etc)
BOOL COXHTMLParser::IsEmptyTag(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_EMPTY) == COXHTMLParser::TAG_EMPTY);
}

BOOL COXHTMLParser::IsTextModifier(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return (pTag->dwTextFlag > 0);
}

// Returns TRUE if the tag has an optional end tag (eg P, LI etc)
BOOL COXHTMLParser::IsOptionalEndTag(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_OPTEND) == COXHTMLParser::TAG_OPTEND);
}

// Returns TRUE if the tag is a special section tag (BODY and HEAD)
BOOL COXHTMLParser::IsSectionTag(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_SECTION) == COXHTMLParser::TAG_SECTION);
}

// Returns TRUE if the tag ends paragraphs (eg P, TABLE etc)
BOOL COXHTMLParser::IsBlockTag(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_BLOCK) == COXHTMLParser::TAG_BLOCK);
}

BOOL COXHTMLParser::IsHeadTag(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_HEAD) == COXHTMLParser::TAG_HEAD);
}

BOOL COXHTMLParser::CanCenter(TagDescriptor* pTag)
{
    if (!pTag)
        return FALSE;

    return ((pTag->dwTagFlag & COXHTMLParser::TAG_CANCENTER) == COXHTMLParser::TAG_CANCENTER);
}

// This MUST be rewritten to use hash tables to get some speed
TagDescriptor* COXHTMLParser::GetTagDescriptor(LPCTSTR szTag) const
{
    DWORD_PTR dwData;
    if (!m_HTMLTagTable.Lookup(szTag, dwData))
        return NULL;
    else
        return (TagDescriptor*) dwData;
}

// Return TRUE if 
//  a) szCurrentTag has an optional end tag, and szNewTag specifies a new element, or
//  a) szNewTag is NULL and szCurrentTag has an optional end tag
// Returning TRUE means that an end tag should be inserted for szCurrentTag, and that
// szNewTag represents a new sibling element. Returning FALSE means that szNewTag
// represents a new child element of szCurrentTag
// Since this funnction may be called when either a new start or a new end tag has
// been found, NewTagIsEndTag specifies whether or not szNewTag is an end tag (TRUE)
// or a start tag (NewTagIsEndTag = FALSE)
BOOL COXHTMLParser::IsEndTagMissing(LPCTSTR szCurrentTag, LPCTSTR szNewTag, 
                                  BOOL NewTagIsEndTag)
{
    if (!szCurrentTag || *szCurrentTag == 0)
        return FALSE;

    // Main HTML tag can only be left off when there is nothing else after it
    if (_tcsicmp(szCurrentTag, TEXT("HTML")) == 0)
        return (szNewTag == NULL || *szNewTag == 0);

    TagDescriptor* pElementTag = GetTagDescriptor(szCurrentTag);
    if (!pElementTag)
        return FALSE;

    TagDescriptor* pObjectTag = NULL;
    if (szNewTag && *szNewTag)
    {
        pObjectTag = GetTagDescriptor(szNewTag);
        if (!pObjectTag)
            return FALSE;
    }

    // Deal with main sections (HEAD, BODY) first
    if ( IsSectionTag(pElementTag))
        return (pObjectTag? IsSectionTag(pObjectTag) : TRUE);

    // Check that the tag we are now dealing with has an optional end.
    if (!IsOptionalEndTag(pElementTag))
        return FALSE;

    // We have an optional end tag - if no more data then everything is fine
    if (szNewTag == NULL || *szNewTag == 0)
        return TRUE;

    // Text modifiers do not mean a new element
    if (IsTextModifier(pObjectTag))
        return FALSE;

    // Certain optionally ended tags can only be ended with certain other tags

    if ( _tcsicmp(szCurrentTag, TEXT("P")) == 0 )
        return IsBlockTag(pObjectTag);

    if ( _tcsicmp(szCurrentTag, TEXT("LI")) == 0 )
        return ( _tcsicmp(szNewTag, TEXT("UL")) == 0 ||
                 _tcsicmp(szNewTag, TEXT("OL")) == 0);

    if ( _tcsicmp(szCurrentTag, TEXT("TR")) == 0 )
    {
        if (NewTagIsEndTag)
            return (_tcsicmp(szNewTag, TEXT("TABLE")) == 0);
        else
            return (_tcsicmp(szNewTag, TEXT("TR")) == 0);
    } 

    if ( _tcsicmp(szCurrentTag, TEXT("TD")) == 0 )
    {
        if (NewTagIsEndTag)
            return ( _tcsicmp(szNewTag, TEXT("TR")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TABLE")) == 0);
        else
            return ( _tcsicmp(szNewTag, TEXT("TD")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TR")) == 0 );
    }

    if ( _tcsicmp(szCurrentTag, TEXT("DT")) == 0 || 
         _tcsicmp(szCurrentTag, TEXT("DD")) == 0 )
    {
        if (NewTagIsEndTag)
            return ( _tcsicmp(szNewTag, TEXT("DT")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("DD")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("DL")) == 0);
        else
            return ( _tcsicmp(szNewTag, TEXT("DT")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("DD")) == 0 );
    }

    if ( _tcsicmp(szCurrentTag, TEXT("THEAD")) == 0 ||
         _tcsicmp(szCurrentTag, TEXT("TFOOT")) == 0 ||
         _tcsicmp(szCurrentTag, TEXT("TBODY")) == 0 )
    {
        if (NewTagIsEndTag)
            return ( _tcsicmp(szNewTag, TEXT("TFOOT")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("THEAD")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TBODY")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TABLE")) == 0);
        else
            return ( _tcsicmp(szNewTag, TEXT("TFOOT")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("THEAD")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TBODY")) == 0 );
    }

    if ( _tcsicmp(szCurrentTag, TEXT("COLGROUP")) == 0 )
    {
        if (NewTagIsEndTag)
            return ( _tcsicmp(szNewTag, TEXT("COLGROUP")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TR")) == 0       ||
                     _tcsicmp(szNewTag, TEXT("TD")) == 0       ||
                     _tcsicmp(szNewTag, TEXT("THEAD")) == 0    ||
                     _tcsicmp(szNewTag, TEXT("TFOOT")) == 0    ||
                     _tcsicmp(szNewTag, TEXT("TABLE")) == 0);
        else
            return ( _tcsicmp(szNewTag, TEXT("COLGROUP")) == 0 ||
                     _tcsicmp(szNewTag, TEXT("TR")) == 0       ||
                     _tcsicmp(szNewTag, TEXT("TD")) == 0       ||
                     _tcsicmp(szNewTag, TEXT("THEAD")) == 0    ||
                     _tcsicmp(szNewTag, TEXT("TFOOT")) == 0 );
    }

    // pElement is optionally ended, followed by a new tag that is not a text
    // modifier. By default we close off pElement and start a new sibling element
    return TRUE;
}

BOOL COXHTMLParser::WriteAttributes(HANDLE hFile, COXParserElement* pElement)
{
    USES_CONVERSION;

    static char buffer[512];
    static DWORD nCount;

    for (int i = 0; i < pElement->NumAttributes(); i++)
    {
        COXAttribute* pAttribute = pElement->Attribute(i);
        if (!pAttribute) continue;

		UTBStr::sprintf(buffer, 512, " %s", T2A((LPTSTR) pAttribute->GetName()));
        if (!::WriteFile(hFile, buffer, PtrToUlong(strlen(buffer)), &nCount, NULL))
            return FALSE;

        LPCTSTR szValue = pAttribute->GetStringValue();
        if (szValue && *szValue)
        {
            UTBStr::sprintf(buffer, 512, "=\"%s\"", T2A((LPTSTR) pAttribute->GetStringValue()));       
            if (!::WriteFile(hFile, buffer, PtrToUlong(strlen(buffer)), &nCount, NULL))
                return FALSE;
        }
    }

    return TRUE;
}

BOOL COXHTMLParser::WriteElement(HANDLE hFile, COXParserElement* pElement, int nLevel)
{   
    USES_CONVERSION;

    static char buffer[512];
    static DWORD nCount;

    if (pElement->GetType() != COXParserObject::ELEMENT)
        return FALSE;

    BOOL bSeparateLine = ( IsBlockTag(pElement->GetName())   || 
                           IsHeadTag(pElement->GetName())    || 
                           IsSectionTag(pElement->GetName()) ||
                           pElement->IsName(TEXT("br")) );

    if (bSeparateLine)
    {
        if (!::WriteFile(hFile, "\r\n", 2, &nCount, NULL))
            return FALSE;
    }

    UTBStr::sprintf(buffer, 512, "<%s", T2A((LPTSTR)pElement->GetName()));
    if (!::WriteFile(hFile, buffer, PtrToUlong(strlen(buffer)), &nCount, NULL))
        return FALSE;
    
    if (!WriteAttributes(hFile, pElement))
        return FALSE;
    
    if (!::WriteFile(hFile, ">", 1, &nCount, NULL))
        return FALSE;

    for (int i = 0; i < pElement->NumObjects(); i++)
        WriteObject(hFile, pElement->Object(i), nLevel+1);
        
    //if (!WriteTabs(hFile, nLevel))
    //    return FALSE;

    if (!IsEmptyTag(pElement->GetName()))
    {
		UTBStr::sprintf(buffer, 512, "</%s>", T2A((LPTSTR)pElement->GetName()));
        if (!::WriteFile(hFile, buffer, PtrToUlong(strlen(buffer)), &nCount, NULL))
            return FALSE;

        if (bSeparateLine)
        {
            if (!::WriteFile(hFile, "\r\n", 2, &nCount, NULL))
                return FALSE;
        }
    }

    return TRUE;
}

BOOL COXHTMLParser::WriteText(HANDLE hFile, COXParserObject* pObject, int nLevel)
{   
    USES_CONVERSION;
    UNUSED_ALWAYS(nLevel);

    if (pObject->GetType() != COXParserObject::PLAINTEXT)
        return FALSE;

    BOOL bSeparateLine = FALSE;
    if (pObject->GetParent())
    {
        bSeparateLine = ( IsBlockTag(pObject->GetParent()->GetName())   ||
                          IsSectionTag(pObject->GetParent()->GetName()) ||
                          pObject->GetParent()->IsName(TEXT("br")) );
    }

    DWORD nCount;
    if (bSeparateLine)
    {
        if (!::WriteFile(hFile, "\r\n", 2, &nCount, NULL))
            return FALSE;
    }

    COXQuickString str = EncodeText(pObject->GetText());
    if (str.IsEmpty())
        return FALSE;

    if (!::WriteFile(hFile, T2A((LPTSTR)str.GetString()), str.GetLength(), &nCount, NULL))
        return FALSE;

    if (bSeparateLine)
    {
        if (!::WriteFile(hFile, "\r\n", 2, &nCount, NULL))
            return FALSE;
    }

    return TRUE;
}

LPCTSTR COXHTMLParser::TranslateErrorCode(int nErrorCode)
{
    switch (nErrorCode)
    {
        case WARNING_UNKNOWN_TAG: return TEXT("Unknown tag found");

        default:
            /* fall through */;
    }
    return COXParser::TranslateErrorCode(nErrorCode);
}

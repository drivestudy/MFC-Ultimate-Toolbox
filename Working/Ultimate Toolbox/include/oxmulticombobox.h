// ==========================================================================
// 					Class Specification : COXMultiComboBox
// ==========================================================================

// Header file : OXMultiComboBox.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (But in the case of no CBS_HASSTRINGS user has to handle DrawItem,CompareItem)
//	YES	Derived from CComboBox

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create() or Dialogtemplate and Subclass the window)
//	YES	Has a message map
//	NO	Needs a resource 

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This control implements a combo box with more than one column. Only one
//	 column will be transferred to the edit (or static) control. 
//	The other columns are shown in the dropdown listbox, but this
//	 is only extra information by which the user can exactly choose 
//	 the item he wants. This control also exists in the MS Access 
//	 product.


// Prerequisites (necessary conditions):
//
// The ComboBox should be created as either CBS_OWNERDRAWFIXED. If you want 
// variable height rows you can use CBS_OWNERDRAWVARIABLE. but you have to handle
// the MeasureItem . If you don't specify CBS_HASSTRINGS, have to handle 
// DrawItem,CompareItem yourself and maintain the each columns data in
// in an item yourself.
//
// If the COXMultiComboBox is used as a control in dialog box then set the 
// above said(ownerdrawn) styles in dialog templates. In OnInitDialog you to
// subclass the window handle of the combo to the COXMultiComboBox.
/////////////////////////////////////////////////////////////////////////////

#ifndef OXMULTICOMBOBOX_H
#define OXMULTICOMBOBOX_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXTEMPL_H__
#include <AfxTempl.h>
#define __AFXTEMPL_H__
#endif




class OX_CLASS_DECL COXMultiComboBox : public CComboBox
{
	DECLARE_DYNAMIC(COXMultiComboBox)

protected:
	class OX_CLASS_DECL COXComboLBox;
	friend class COXComboLBox;

//Data members --------------------------------------------------------
public :

protected :
	int m_nColumnCount;
	int m_nMasterColumn;
	CArray<int,int> m_aColumnWidth;
	enum {DEFAULT_COLUMN_WIDTH=60};
	CPen m_GridPen;

	BOOL m_bFitToSize;

	class OX_CLASS_DECL COXComboLBox : public CListBox
	{
	public:
		BOOL m_fVertScrollVisible;
		BOOL m_fHorzScrollVisible;
	private:
		CWnd* m_pWndCombo;

	public:
		COXComboLBox() { };
		void Init(CWnd* pCombo);
		void AdjustHScroll();
		virtual ~COXComboLBox() { };
	protected:
		//{{AFX_MSG(COXComboLBox)
		afx_msg LRESULT OnHScroll(WPARAM wParam,LPARAM lParam);
		afx_msg LRESULT OnWindowPosChanging(WPARAM wParam,LPARAM lParam);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	private:
		int GetMaxHeight();
	};
	COXComboLBox m_ComboLBox;


	class OX_CLASS_DECL COXRowData {
	private :
		CStringArray ColumnString;
		DWORD ItemData;
	public :
		COXRowData(int nColCount); 
		void SetColumnCount(int nCount);
		void SetItemData(DWORD Data);
		DWORD GetItemData()	const;
		void SetColumnString(int nColIndex,const CString& rString);
		CString GetColumnString(int nColIndex) const ;
		void DeleteColumnString(int nColIndex);
	};

private :
	BOOL m_fSizeChanged;
	BOOL m_fMasterColumnChanging;
	int m_nPageWidth;
	int m_nLineWidth;


//Member functions-----------------------------------------------------------------------
public:
	COXMultiComboBox();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Object Constructions and default initializations
	//				of Number of columns and their width

	virtual ~COXMultiComboBox(); 
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Cleanup 

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	// All the Parameters and return type for create is same as CComboBox::Create except
	// style should have either CBS_OWNERDRAWFIXED or CBS_OWNERDRAWVARIABLE

	BOOL SetColumnCount(int nCount); 
	// --- In  : nCount : Column Count
	// --- Out : 
	// --- Returns : Returns FALSE if nCount < 0 or Master Column is beyond nCount
	// --- Effect : Sets the column count. If any columns are added newly sets their 
	//				width to DEFAULT_COLUMN_WIDTH

    int GetColumnCount() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Returns column count
	// --- Effect : 

    BOOL SetMasterColumn(int nCol = 0); 
	// --- In  : nCol : Master column to set 
	// --- Out : 
	// --- Returns : Wether the Master column is changed succesfully
	// --- Effect : If the nCol is Valid(0 to Columncount-1)
	//				Changes the master column to nCol. If CBS_SORT style is there
	//				order of the items may change on the basis of the content 
	//				of the master column

    int GetMasterColumn() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Master column 
	// --- Effect : 

	BOOL SetColumnWidth(int nColIndex, int nWidth); 
	// --- In  : nColWidth : Columns Width to set ,if -1, column width of each
	//						 column is set.
	//			 nWidth : in number of pixels
	// --- Out : 
	// --- Returns : wether successfull or not 
	// --- Effect : Sets the width to the given column

    int GetColumnWidth(int nColIndex) const;
	// --- In  : nColWidth : Index of the Column's width to get
	// --- Out : 
	// --- Returns : Width of the column. 
	// --- Effect : 

	int GetLBText(int nIndex,int nColIndex, LPTSTR lpszText) const;
	// --- In  : nIndex : Items Index
	//			 nColIndex : Column Index
	// --- Out : lpszText : the buffer into witch the text has to be copied
	// --- Returns : wether successfull or not
	// --- Effect : Items text copied into lpszText

	void GetLBText(int nIndex, int nColIndex, CString& rString) const;
	// --- In  : nIndex : Items Index
	//			 nColIndex : Column Index
	// --- Out : rString : the buffer into witch the text has to be copied
	// --- Returns :
	// --- Effect : The Items text is copied into rString

	int GetLBTextLen(int nIndex,int nColIndex) const;
	// --- In  :nIndex : Items Index
	//			nColIndex : Column Index
	// --- Out : 
	// --- Returns : returns the text lenght in the column nColIndex of Item nIndex
	// --- Effect : 

	int FindStringExact(int nIndexStart, int nColIndex, LPCTSTR lpszFind, 
		BOOL bCaseSensitive=FALSE) const;
	// --- In  : nIndexStart : Index from which to start the search
	//			 nColIndex : Column Index
	//			 lpszFind : String to find
	//			 bCaseSensitive : whether search is case sensitive or not
	// --- Out : 
	// --- Returns : Returns the Index of the given string if found, otherwise CB_ERR
	// --- Effect : 

	int AddString(LPCTSTR* lpszString, int nNumStrings);
	// --- In  : lpszString : Array of Strings to be set as columns strings
	//			 nNumStrings : Number of strings supplied
	// --- Out : 
	// --- Returns : CB_ERRORSPACE if insufficient memory,CB_ERR if unsuccessfull,
	//				 otherwise the index at which the string has been added.
	// --- Effect : A new item(row) will be added to the listbox of combobox whose
	//				columns strings will be the strings suplied as lpszString at
	//				end if no CBS_SORT style is set, otherwise at its position in
	//				sorted list of master column strings.

	int InsertString(int nIndex, LPCTSTR* lpszString, int nNumStrings);
	// --- In  : nIndex : Index at which the Item(row) has to be inserted.
	//			 lpszString : lpszString : Array of Strings to be set as columns strings
	//			 nNumStrings : Number of strings supplied
	// --- Out : 
	// --- Returns :CB_ERRORSPACE if insufficient memory,CB_ERR if unsuccessfull,
	//				 otherwise the index at which the string has been added.
	// --- Effect : A new item(row) will be added to the listbox of combobox whose
	//				columns strings will be the strings suplied as lpszString at the 
	//				given index nIndex

	int FindString(int nStartAfter, int nColIndex, LPCTSTR lpszString, 
		BOOL bCaseSensitive=FALSE) const;
	// --- In  : nStartAfter : The index from which to start the search
	//			 nColIndex : The column index in which to search the string
	//			 lpszString : String to find
	//			 bCaseSensitive : whether search is case sensitive or not
	// --- Out : 
	// --- Returns : CB_ERR if unsuccessful, otherwise Index of the item in which 
	//				 in the given column, given string is a prefix to the column text.
	// --- Effect : 

	int SelectString(int nStartAfter, int nColIndex, LPCTSTR lpszString);
	// --- In  :// --- In  : nStartAfter : The index from which to start the search
	//			 nColIndex : The column index in which to search the string
	//			 lpszString : String to select
	// --- Out : 
	// --- Returns : CB_ERR if unsuccessful, otherwise Index of the item in which 
	//				 in the given column, given string is a prefix to the column text.
	// --- Effect : If the given string is found, it will be selected into 
	//				editbox of combo

	int GetLBText(int nIndex, LPTSTR lpszText) const;
	// --- In  : nIndex : Index of the item whose master column text has to be get
	//			 lpszText : String into which the master columns text has to be copied
	// --- Out : 
	// --- Returns : length of the string copied into lpszText if succesfull,
	//				 otherwise CB_ERR
	// --- Effect : The Master column string in the given row at the given Index is
	//				copied into lpszText

	void GetLBText(int nIndex, CString& rString) const;
	// --- In  : // --- In  : nIndex : Index of the item whose master column text has to be get
	//			 lpszText : Buffer into which the master columns text has to be copied
	// --- Out : 
	// --- Returns :
	// --- Effect : The Master column string in the given row at the given Index is
	//				copied into rString

	int GetLBTextLen(int nIndex) const;
	// --- In  : nIndex : Index of the item
	// --- Out : 
	// --- Returns : Returns master columns string length if sucessfull, 
	//				 otherwise CB_ERR
	// --- Effect : 

	int FindStringExact(int nIndexStart, LPCTSTR lpszFind, 
		BOOL bCaseSensitive=FALSE) const;
	// --- In  : nStartIndex : Index from which to start searching
	//			 lpszFind : String to find
	//			 bCaseSensitive : whether search is case sensitive or not
	// --- Out : 
	// --- Returns : return the index at which the exact string lpszFind is found,
	//				 Otberwise CB_ERR.
	// --- Effect : 

	int AddString(LPCTSTR lpszString);
	// --- In  : lpszString : String to be added as master column
	// --- Out : 
	// --- Returns : Index at which the item has been added if sucessfull, 
	//				CB_ERRORSPACE is insufficient memory to stote the string,
	//				otherwise CB_ERR
	// --- Effect : An item with master column as lpszString will be added, 
	//				rest of the columns will be just empty.

	int InsertString(int nIndex, LPCTSTR lpszString);
	// --- In  : nIndex : Index at which to add the string
	//			 lpszString : String to be added as master column
	// --- Out : 
	// --- Returns : Index at which the item has been added if sucessfull, 
	//				CB_ERRORSPACE is insufficient memory to stote the string,
	//				otherwise CB_ERR
	// --- Effect : An item with master column as lpszString will be inserted, 
	//				at the given index. Rest of the columns will be just empty.

	int FindString(int nStartAfter, LPCTSTR lpszString, 
		BOOL bCaseSensitive=FALSE) const;
	// --- In  : nStartAfter : The index from which to start the search
	//			 lpszString : String to find
	//			 bCaseSensitive : whether search is case sensitive or not
	// --- Out : 
	// --- Returns : The index at which lpszString is a prefix to master column
	// --- Effect : 

	int SelectString(int nStartAfter, LPCTSTR lpszString);
	// --- In  : nStartAfter : The index from which to start the search
	//			 lpszString : String to select
	// --- Out : 
	// --- Returns :The index at which lpszString is a prefix to master column,
	//				Otherwise CB_ERR.
	// --- Effect : If successful select the string into editbox of the combo.

    void SetFitToSize(BOOL bFitToSize); 
	// --- In  : bFitToSize : if TRUE then drop down list box will be resized 
	//						  to match the size of all columns
	// --- Out : 
	// --- Returns: 
	// --- Effect : 

    inline BOOL GetFitToSize() { return m_bFitToSize; } 
	// --- In  : 
	// --- Out : 
	// --- Returns: TRUE if drop down list box will be resized to match the size of 
	//				all columns
	// --- Effect : 

protected :
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawColumnItem(HDC hDC, int nIndex, int nColIndex, const CRect& rectColumn, int ItemState);
	virtual void DrawColumnBorder(CDC* pDC,const CRect& rectColumn, int ItemState);

	//{{AFX_MSG(COXMultiComboBox)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnListHScroll(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetItemData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetItemData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCompareItem(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMeasureItem(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDrawItem(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInsertString(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnAddString(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	int GetTotalHeight();
	int GetTotalWidth();
	COXRowData* GetRowData(int nIndex) const;
	BOOL SetRowData(int nIndex, COXRowData* pRowData);
	int AdjustToFitSize();

private :
	BOOL ChangeMasterColumn(int nCol);
};

#include "OXMultiComboBox.inl"

#endif // OXMULTICOMBOBOX_H
/////////////////////////////////////////////////////////////////////////////

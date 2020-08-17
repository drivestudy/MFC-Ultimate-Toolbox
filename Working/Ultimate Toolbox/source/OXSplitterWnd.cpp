// ==========================================================================
//		Class Implementation:	COXSplitterWnd
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COXSplitterWnd pane bar

IMPLEMENT_DYNCREATE(COXSplitterWnd, CSplitterWnd)

COXSplitterWnd::COXSplitterWnd()
{
}


COXSplitterWnd::~COXSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(COXSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(COXSplitterWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COXSplitterWnd::OnDestroy()
{
	CSplitterWnd::OnDestroy();

	// reset variables
	m_nMaxRows=0;
	m_nMaxCols=0;
	m_nRows=0;
	m_nCols=0;
	m_bHasHScroll=FALSE;
	m_bHasVScroll=FALSE;
	delete[] m_pColInfo;
	m_pColInfo=NULL;
	delete[] m_pRowInfo;
	m_pRowInfo=NULL;
}


BOOL COXSplitterWnd::InsertColumn(int nInsertBefore, int nWidth)
{
	ASSERT(nInsertBefore>=0 && nInsertBefore<=GetColumnCount());
	ASSERT(::IsWindow(GetSafeHwnd()));

	// create new array of column info
	CRowColInfo* pColInfo=new CRowColInfo[m_nMaxCols+1];
	int nCol=0;
	for(nCol=0; nCol<m_nMaxCols+1; nCol++)
	{
		int nExistingCol=(nCol<nInsertBefore ? nCol : nCol-1);
		if(nCol==nInsertBefore)
		{
			// set data for inserted column
			pColInfo[nCol].nMinSize=0;
			pColInfo[nCol].nIdealSize=nWidth;
			pColInfo[nCol].nCurSize=-1;
		}
		else
		{
			// copy existing column data
			pColInfo[nCol].nMinSize=m_pColInfo[nExistingCol].nMinSize;
			pColInfo[nCol].nIdealSize=m_pColInfo[nExistingCol].nIdealSize;
			pColInfo[nCol].nCurSize=m_pColInfo[nExistingCol].nCurSize;
		}
	}
	// delete old array
	delete[] m_pColInfo;
	// save new one
	m_pColInfo=pColInfo;

	// set new number of columns
	m_nMaxCols++;
	m_nCols=m_nMaxCols;

	// reset pane IDs (do that in backward direction)
	for(nCol=m_nMaxCols-2; nCol>=nInsertBefore; nCol--)
	{
		for(int nRow=m_nMaxRows-1; nRow>=0; nRow--)
		{
			CWnd* pView=GetDlgItem(IdFromRowCol(nRow,nCol));
			ASSERT(pView!=NULL);
			pView->SetDlgCtrlID(IdFromRowCol(nRow,nCol+1));
		}
	}

	return TRUE;
}


BOOL COXSplitterWnd::RemoveColumn(int nDelCol)
{
	ASSERT(GetColumnCount()>1);
	if(GetColumnCount()<=1)
	{
		return FALSE;
	}
	ASSERT(nDelCol>=0 && nDelCol<GetColumnCount());
	ASSERT(::IsWindow(GetSafeHwnd()));

	// delete all views in specified column
	for(int nRow=0; nRow<m_nMaxRows; nRow++)
	{
		CWnd* pView=GetDlgItem(IdFromRowCol(nRow,nDelCol));
		if(pView!=NULL)
		{
			DeleteView(nRow,nDelCol);
		}
	}

	// create new array of column info
	CRowColInfo* pColInfo=new CRowColInfo[m_nMaxCols-1];
	int nCol=0;
	for(nCol=0; nCol<m_nMaxCols-1; nCol++)
	{
		int nExistingCol=(nCol<nDelCol ? nCol : nCol+1);
		if(nExistingCol!=nDelCol)
		{
			// copy existing column data
			pColInfo[nCol].nMinSize=m_pColInfo[nExistingCol].nMinSize;
			pColInfo[nCol].nIdealSize=m_pColInfo[nExistingCol].nIdealSize;
			pColInfo[nCol].nCurSize=m_pColInfo[nExistingCol].nCurSize;
		}
	}
	// delete old array
	delete[] m_pColInfo;
	// save new one
	m_pColInfo=pColInfo;

	// reset pane IDs (in forward direction)
	for(nCol=nDelCol+1; nCol<m_nMaxCols; nCol++)
	{
		for(int nRow=0; nRow<m_nMaxRows; nRow++)
		{
			CWnd* pView=GetDlgItem(IdFromRowCol(nRow,nCol));
			ASSERT(pView!=NULL);
			pView->SetDlgCtrlID(IdFromRowCol(nRow,nCol-1));
		}
	}

	// set new number of columns
	m_nMaxCols--;
	m_nCols=m_nMaxCols;

	return TRUE;
}


BOOL COXSplitterWnd::InsertRow(int nInsertBefore, int nHeight)
{
	ASSERT(nInsertBefore>=0 && nInsertBefore<=GetRowCount());
	ASSERT(::IsWindow(GetSafeHwnd()));

	// create new array of row info
	CRowColInfo* pRowInfo=new CRowColInfo[m_nMaxRows+1];
	int nRow=0;
	for(nRow=0; nRow<m_nMaxRows+1; nRow++)
	{
		int nExistingRow=(nRow<nInsertBefore ? nRow : nRow-1);
		if(nRow==nInsertBefore)
		{
			// set data for inserted row
			pRowInfo[nRow].nMinSize=0;
			pRowInfo[nRow].nIdealSize=nHeight;
			pRowInfo[nRow].nCurSize=-1;
		}
		else
		{
			// copy existing row data
			pRowInfo[nRow].nMinSize=m_pRowInfo[nExistingRow].nMinSize;
			pRowInfo[nRow].nIdealSize=m_pRowInfo[nExistingRow].nIdealSize;
			pRowInfo[nRow].nCurSize=m_pRowInfo[nExistingRow].nCurSize;
		}
	}
	// delete old array
	delete[] m_pRowInfo;
	// save new one
	m_pRowInfo=pRowInfo;

	// set new number of rows
	m_nMaxRows++;
	m_nRows=m_nMaxRows;

	// reset pane IDs (do that in backward direction)
	for(nRow=m_nMaxRows-2; nRow>=nInsertBefore; nRow--)
	{
		for(int nCol=m_nMaxCols-1; nCol>=0; nCol--)
		{
			CWnd* pView=GetDlgItem(IdFromRowCol(nRow,nCol));
			ASSERT(pView!=NULL);
			pView->SetDlgCtrlID(IdFromRowCol(nRow+1,nCol));
		}
	}

	return TRUE;
}


BOOL COXSplitterWnd::RemoveRow(int nDelRow)
{
	ASSERT(GetRowCount()>1);
	if(GetRowCount()<=1)
	{
		return FALSE;
	}
	ASSERT(nDelRow>=0 && nDelRow<GetRowCount());
	ASSERT(::IsWindow(GetSafeHwnd()));

	// delete all views in specified row
	for(int nCol=0; nCol<m_nMaxCols; nCol++)
	{
		CWnd* pView=GetDlgItem(IdFromRowCol(nDelRow,nCol));
		if(pView!=NULL)
		{
			DeleteView(nDelRow,nCol);
		}
	}

	// create new array of row info
	CRowColInfo* pRowInfo=new CRowColInfo[m_nMaxRows-1];
	int nRow=0;
	for(nRow=0; nRow<m_nMaxRows-1; nRow++)
	{
		int nExistingRow=(nRow<nDelRow ? nRow : nRow+1);
		if(nExistingRow!=nDelRow)
		{
			// copy existing row data
			pRowInfo[nRow].nMinSize=m_pRowInfo[nExistingRow].nMinSize;
			pRowInfo[nRow].nIdealSize=m_pRowInfo[nExistingRow].nIdealSize;
			pRowInfo[nRow].nCurSize=m_pRowInfo[nExistingRow].nCurSize;
		}
	}
	// delete old array
	delete[] m_pRowInfo;
	// save new one
	m_pRowInfo=pRowInfo;

	// reset pane IDs (in forward direction)
	for(nRow=nDelRow+1; nRow<m_nMaxRows; nRow++)
	{
		for(int nCol=0; nCol<m_nMaxCols; nCol++)
		{
			CWnd* pView=GetDlgItem(IdFromRowCol(nRow,nCol));
			ASSERT(pView!=NULL);
			pView->SetDlgCtrlID(IdFromRowCol(nRow-1,nCol));
		}
	}

	// set new number of rows
	m_nMaxRows--;
	m_nRows=m_nMaxRows;

	return TRUE;
}


void COXSplitterWnd::DeleteView(int row, int col)
{
	ASSERT_VALID(this);

	// if active child is being deleted - activate next
	CWnd* pPane=GetPane(row,col);
	ASSERT_VALID(pPane);
	if(GetActivePane()==pPane)
	{
		ActivateNext(FALSE);
	}

	// default implementation assumes view will auto delete in PostNcDestroy
	pPane->DestroyWindow();
}

#ifdef _DEBUG
void COXSplitterWnd::AssertValid() const
{
	CWnd::AssertValid();
}
#endif


BOOL COXSplitterWnd::SwapPanes(CWnd* pFirstWnd, CWnd* pSecondWnd)
{
	ASSERT(pFirstWnd && pSecondWnd);
	
	int nCol1, nCol2, nRow1, nRow2;

	if (!IsChildPane(pFirstWnd, &nRow1, &nCol1) || 
		!IsChildPane(pSecondWnd, &nRow2, &nCol2))
	{
		return FALSE;
	}

	int nID1=IdFromRowCol(nRow1,nCol1);
	int nID2=IdFromRowCol(nRow2,nCol2);
	pSecondWnd->SetDlgCtrlID(nID1);
	pFirstWnd->SetDlgCtrlID(nID2);

	//exchange by row info
	CSplitterWnd::CRowColInfo info;
	CSplitterWnd::CRowColInfo* pInfo1=m_pRowInfo;
	CSplitterWnd::CRowColInfo* pInfo2=m_pRowInfo;
	pInfo1+=nID1 - (AFX_IDW_PANE_FIRST + nRow1 * 16 + nCol1);
	pInfo2+=nID2 - (AFX_IDW_PANE_FIRST + nRow2 * 16 + nCol2);
	memcpy(&info, pInfo1, sizeof(CSplitterWnd::CRowColInfo));
	memcpy(pInfo1, pInfo2, sizeof(CSplitterWnd::CRowColInfo));
	memcpy(pInfo2, &info, sizeof(CSplitterWnd::CRowColInfo));

	RecalcLayout();
	RedrawWindow();
	return TRUE;
}

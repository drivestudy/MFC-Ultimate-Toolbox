// ==========================================================================
//                     Template Implementation : COXCRCTableList
// ==========================================================================

// Source file : OXCRCChk.inl 

// Version: 9.3

// Source : Kris Beerts
// Creation Date : 	   13th August 1996
// Last Modification : 13th August 1996
			  
// //////////////////////////////////////////////////////////////////////////
// Definition of static members

#ifndef new
#define new DEBUG_NEW
#define _REMOVE_DEBUG_NEW
#endif


template<class VALUE> 
inline 
COXCRCTable<VALUE>::COXCRCTable(VALUE nPolynomial) :
	m_pNextTable(NULL),
	m_nPolynomial(nPolynomial)
// --- In: nPolynomial: the polynomial on which the table will be based on
// --- Out: none
// --- Returns: none
// --- Effect: constructs the template object and initialize the internal state
{
	memset(values, 0, 256*sizeof(VALUE));
}

template<class VALUE> 
inline 
COXCRCTableList<VALUE>::COXCRCTableList() :
	m_pTableList(NULL)
// --- In: none
// --- Out: none
// --- Returns: none
// --- Effect: constructs the list
{
}

template<class VALUE> 
inline 
COXCRCTableList<VALUE>::~COXCRCTableList()
// --- In: none
// --- Out: none
// --- Returns: none
// --- Effect: Destructs the template object
{
	COXCRCTable<VALUE>*	pTable = m_pTableList;
	COXCRCTable<VALUE>*	pNextTable = NULL;

	// iterate the list and delete every table
	while (pTable)
	{
		pNextTable = pTable->m_pNextTable;
		delete pTable;
		pTable = pNextTable;
	}
}

template<class VALUE>
inline
COXCRCTable<VALUE>* COXCRCTableList<VALUE>::GetTable(VALUE nPolynomial)
// --- In: nPolynomial: the polynomial of the wanted table
// --- Out: none
// --- Returns: none
// --- Effect: Searches for the table with the specified polynomial.
//			   A new table is created if necessary.
{
	COXCRCTable<VALUE>*	pTable = m_pTableList;
	COXCRCTable<VALUE>*	pLastTable = NULL;
	COXCRCTable<VALUE>*	pNewTable = NULL;

	// search for an existing table
	while (pTable)
	{
		pLastTable = pTable;
		// check polynomial
		if (pTable->m_nPolynomial == nPolynomial)
			return pTable;
		pTable = pTable->m_pNextTable;
	}

	// if an existing table with this polynomial is not found a new one is created
	pNewTable = CreateTable(nPolynomial);

	// check if this is the first table in the list
	if (pLastTable)
		pLastTable->m_pNextTable = pNewTable;
	else
		m_pTableList = pNewTable;

	return pNewTable;
}


#ifdef _REMOVE_DEBUG_NEW
#undef new
#undef _REMOVE_DEBUG_NEW
#endif

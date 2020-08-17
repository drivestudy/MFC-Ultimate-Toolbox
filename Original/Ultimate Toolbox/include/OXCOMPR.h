// ==========================================================================
// 							Class Specification : COXCompressor
// ==========================================================================

// Header file : compress.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class encapsulates an enhanced version of the Lempel Ziv 77 compression
//	 algorithm
//  It is based on the code supplied by Mark Nelson in "The Data Compression Book", (M&T Publishing 1992)
//	This algorithm is based on a sliding window dictionary-based lossless compression

// Remark:
//		

// Prerequisites (necessary conditions):
//		

/////////////////////////////////////////////////////////////////////////////
#ifndef __COMPR_H__
#define __COMPR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

/*
 * Various constants used to define the compression parameters.  The
 * INDEX_BIT_COUNT tells how many bits we allocate to indices into the
 * text window.  This directly determines the WINDOW_SIZE.  The
 * LENGTH_BIT_COUNT tells how many bits we allocate for the length of
 * an encode phrase. This determines the size of the look ahead buffer.
 * The TREE_ROOT is a special node in the tree that always points to
 * the root node of the binary phrase tree.  END_OF_STREAM is a special
 * index used to flag the fact that the file has been completely
 * encoded, and there is no more data.  NOT_USED is the null index for
 * the tree. MOD_WINDOW() is a macro used to perform arithmetic on tree
 * indices.
 *
 */

#define INDEX_BIT_COUNT      9
#define LENGTH_BIT_COUNT     4
#define WINDOW_SIZE          ( 1 << INDEX_BIT_COUNT )
#define TREE_SIZE			 WINDOW_SIZE+1
#define RAW_LOOK_AHEAD_SIZE  ( 1 << LENGTH_BIT_COUNT )
#define BREAK_EVEN           ( ( 1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT ) / 9 )
#define LOOK_AHEAD_SIZE      ( RAW_LOOK_AHEAD_SIZE + BREAK_EVEN )
#define TREE_ROOT            WINDOW_SIZE
#define END_OF_STREAM        0
#define NOT_USED               0
#define MOD_WINDOW( a )      ( ( a ) & ( WINDOW_SIZE - 1 ) )



class OX_CLASS_DECL COXCompressor : public CObject
{
DECLARE_DYNAMIC(COXCompressor)

// Data members -------------------------------------------------------------
public:
	
protected:
	struct CompressTree
		{
		int parent;
		int smaller_child;
		int larger_child;
		};

	unsigned char* m_window;

	CompressTree m_tree[TREE_SIZE];
	
// Member functions ---------------------------------------------------------
public: 
	COXCompressor();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : dynamic allocation of character buffer that act as a dictionary
	//				for the look-ahead buffer
	//				Contructor of object
	//				It will initialize the internal state
    

	int Compress(const LPBYTE pInBuffer, int nInLength, LPBYTE pOutBuffer, int nMaxOutLength);
	// --- In  : pInBuffer : The buffer that contains the (original) input data
	//			 nInLength : The length of the input buffer
	//			 pOutBuffer : The allocated buffer that will receive the (compressed) output data
	//			 nMaxOutLength : The allocated length of the output buffer
	// --- Out : pOutBuffer : The buffer that receives the compressed
	// --- Returns : The length of that part of the output buffer that contains the compressed data or
	//				 -1 when not enough space in the output buffer was available
	// --- Effect : This functions compresses the data and returns it
	//				The output buffer must have been allocated before the function is called
    
	int Expand(const LPBYTE pInBuffer, int nInLength, LPBYTE pOutBuffer, int nMaxOutLength);
	// --- In  : pInBuffer : The buffer that contains the (compressed) input data
	//			 nInLength : The length of the input buffer
	//			 pOutBuffer : The allocated buffer that will receive the (expended) output data
	//			 nMaxOutLength : The allocated length of the output buffer
	// --- Out : pOutBuffer : The buffer that receives the expanded
	// --- Returns : The length of that part of the output buffer that contains the expanded data or
	//				 -1 when not enough space in the output buffer was available
	// --- Effect : This functions expand the data and returns it
	//				The output buffer must have been allocated before the function is called

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXCompressor();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : deletion of the character buffer allocated in constructor
	//				Destructor of object

protected:      
	void InitTree( int r );
	void ContractNode( int old_node, int new_node );
	void ReplaceNode( int old_node, int new_node );
	int FindNextNode( int node );
	void DeleteString( int p );
	int AddString( int new_node, int *match_position );

private:
                   
// Message handlers ---------------------------------------------------------

};

#endif

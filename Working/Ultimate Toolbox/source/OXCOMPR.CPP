// ==========================================================================
// 							Class Implementation : COXCompressor
// ==========================================================================

// Source file :compress.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxcompr.h"	// class specification
#include "oxbitbuf.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif                         

#ifndef WIN32
typedef unsigned char UCHAR;
#endif

IMPLEMENT_DYNAMIC(COXCompressor, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

// private:



// Member functions ---------------------------------------------------------
// public:

COXCompressor::COXCompressor()
{
	TRY	
	{
		m_window = new unsigned char[WINDOW_SIZE];
	}
	CATCH(CMemoryException, e) 
	{                                                                    
		TRACE(_T("COXCompressor::COXCompressor : Memory allocation not succeeded !!"));
		THROW_LAST();
	}
	END_CATCH

		memset(&m_window[0],0,WINDOW_SIZE);
}                        

COXCompressor::~COXCompressor()
{       
	delete [] m_window;
}


int COXCompressor::Compress(const LPBYTE pInBuffer, int nInLength, 
							LPBYTE pOutBuffer, int nMaxOutLength)
							/*
							* This is the compression routine.  It has to first load up the look
							* ahead buffer, then go into the main compression loop.  The main loop
							* decides whether to output a single character or an index/length
							* token that defines a phrase.  Once the character or phrase has been
							* sent out, another loop has to run.  The second loop reads in new
							* characters, deletes the strings that are overwritten by the new
							* character, then adds the strings that are created by the new
							* character.
							*/
{   
	ASSERT ( pInBuffer != NULL );
	ASSERT ( AfxIsValidAddress(pOutBuffer,nMaxOutLength) );
	if (nInLength == 0)
	{
		pOutBuffer = NULL;
		return 0;
	}

	int i;
	int look_ahead_bytes;
	int current_position;
	int replace_count;
	int match_length;
	int match_position;
	int counter = 0;
	LPBYTE pIterateInBuffer = pInBuffer;


	COXBitBuffer bitBuffer(pOutBuffer);

	current_position = 1;
	for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ )
	{
		if ( i >= nInLength || (bitBuffer.GetCompressedLength() > nMaxOutLength) )
			break;
		m_window[current_position + i ] = *pIterateInBuffer++;
		counter++;
	}

	look_ahead_bytes = i;             
	InitTree( current_position );
	match_length = 0;
	match_position = 0;
	while ( look_ahead_bytes > 0 )
	{ 
		if ( match_length > look_ahead_bytes )
			match_length = look_ahead_bytes;

		if ( match_length <= BREAK_EVEN )
		{
			replace_count = 1;
			bitBuffer.OutputBit( 1 );
			bitBuffer.OutputBits( (unsigned long) m_window[ current_position ], 8 );
		}
		else
		{
			bitBuffer.OutputBit( 0 );
			bitBuffer.OutputBits( (unsigned long) match_position, INDEX_BIT_COUNT );
			bitBuffer.OutputBits( (unsigned long) ( match_length - ( BREAK_EVEN + 1 ) ),
				LENGTH_BIT_COUNT );
			replace_count = match_length;
		}

		if (bitBuffer.GetCompressedLength() >= nMaxOutLength)
			return -1;

		for ( i = 0 ; i < replace_count ; i++ )
		{
			DeleteString( MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) );

			if (  counter >= nInLength ) 
				look_ahead_bytes--;
			else
			{
				m_window[ MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) ]
				= (unsigned char) *pIterateInBuffer;
				pIterateInBuffer++;
				counter++;        
			}
			current_position = MOD_WINDOW( current_position + 1 );
			if (look_ahead_bytes)
				match_length=AddString(current_position,&match_position);
		}
	};

	bitBuffer.OutputBit( 0 );                                           
	bitBuffer.OutputBitsEOS( ); 

	return (bitBuffer.GetCompressedLength() >= nMaxOutLength ? -1 : 
		bitBuffer.GetCompressedLength() ); 
}


int COXCompressor::Expand(const LPBYTE pInBuffer, int nInLength, LPBYTE pOutBuffer, 
						  int nMaxOutLength)
						  /*
						  * This is the expansion routine for the LZSS algorithm.  All it has
						  * to do is read in flag bits, decide whether to read in a character or
						  * a index/length pair, and take the appropriate action.
						  */
{
	ASSERT ( pInBuffer != NULL );
	ASSERT ( AfxIsValidAddress(pOutBuffer,nMaxOutLength) );
	ASSERT ( nInLength != 0 );                                  
	int i;
	int current_position;
	int c;
	int match_length;
	int match_position;
	COXBitBuffer bitBuffer( pInBuffer, nInLength );
	LPBYTE pRefOutBuffer = pOutBuffer;
	for (int counter = 0; counter < nMaxOutLength; counter++)
		pOutBuffer[counter] = 0;

	current_position = 1;
	for ( ; ; )
	{
		if ( bitBuffer.InputBit( ) )
		{
			c = (int) bitBuffer.InputBits( 8 );
			*pOutBuffer = (UCHAR)c;
			pOutBuffer++;
			if ( (pOutBuffer - pRefOutBuffer) > nMaxOutLength)
				return -1;

			m_window[ current_position ] = (unsigned char) c;
			current_position = MOD_WINDOW( current_position + 1 );
		}
		else
		{
			match_position = (int) bitBuffer.InputBits( INDEX_BIT_COUNT );
			if ( match_position == END_OF_STREAM 
				&& bitBuffer.GetCompressedLength() == nInLength )
				break;

			ASSERT( bitBuffer.GetCompressedLength()  <= nInLength );
			match_length = (int) bitBuffer.InputBits( LENGTH_BIT_COUNT );
			match_length += BREAK_EVEN;
			for ( i = 0 ; i <= match_length ; i++ )
			{
				c = m_window[ MOD_WINDOW( match_position + i ) ];
				*pOutBuffer = (UCHAR)c;
				pOutBuffer++;
				if ( (pOutBuffer - pRefOutBuffer) > nMaxOutLength)
					return -1;

				m_window[ current_position ] = (unsigned char) c;
				current_position = MOD_WINDOW( current_position + 1 );
			}
		}
	} 

	return PtrToInt(pOutBuffer - pRefOutBuffer);
}

#ifdef _DEBUG
void COXCompressor::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void COXCompressor::AssertValid() const
{
	CObject::AssertValid();
}
#endif

// protected:

void COXCompressor::InitTree( int r )
// --- In      : r eshtablishing a root node for the tree
// --- Out     : 
// --- Returns :
// --- Effect  : initialization of the entire tree to value NOT_USED (0), 
//				 which is good, since 0 is the NOT_USED code.
{
	for ( int counter = 0; counter < TREE_SIZE; counter++)
		m_tree[ counter ].parent        = 
		m_tree[ counter ].larger_child  = 
		m_tree[ counter ].smaller_child = NOT_USED;

	m_tree[ TREE_ROOT ].larger_child = r;
	m_tree[ r ].parent = TREE_ROOT;
	m_tree[ r ].larger_child = NOT_USED;
	m_tree[ r ].smaller_child = NOT_USED;
}

void COXCompressor::ContractNode( int old_node, int new_node )
// --- In      : old_node : node to be removed
//				 new_node : replacing node, child of the old node
// --- Out     : 
// --- Returns :
// --- Effect  : This routine is used when a node is being deleted.  The link to
//				 its descendant is broken by pulling the descendant in to overlay
//				 the existing link.
{
	m_tree[ new_node ].parent = m_tree[ old_node ].parent;
	if ( m_tree[ m_tree[ old_node ].parent ].larger_child == old_node )
		m_tree[ m_tree[ old_node ].parent ].larger_child = new_node;
	else
		m_tree[ m_tree[ old_node ].parent ].smaller_child = new_node;

	m_tree[ old_node ].parent = NOT_USED;
}

void COXCompressor::ReplaceNode( int old_node, int new_node )
// --- In      : old_node : node to be removed
//			     new_node : replacing node, the next smaller node in the list 
// --- Out     : 
// --- Returns :
// --- Effect  : This routine is also used when a node is being deleted.  However,
//				 in this case, it is being replaced by a node that was not previously
//				 in the tree.
{
	int parent;

	parent = m_tree[ old_node ].parent;
	if ( m_tree[ parent ].smaller_child == old_node )
		m_tree[ parent ].smaller_child = new_node;
	else
		m_tree[ parent ].larger_child = new_node;
	m_tree[ new_node ] = m_tree[ old_node ];
	m_tree[ m_tree[ new_node ].smaller_child ].parent = new_node;
	m_tree[ m_tree[ new_node ].larger_child ].parent = new_node;
	m_tree[ old_node ].parent = NOT_USED;
}

int COXCompressor::FindNextNode( int node )
// --- In      : node : node being searched from 
// --- Out     : 
// --- Returns : next smaller node
// --- Effect  : This routine is used to find the next smallest node after the node
// 				 argument.  It assumes that the node has a smaller child.  We find
//				 the next smallest child by going to the smaller_child node, then
//				 going to the end of the larger_child descendant chain.
{
	int next;

	next = m_tree[ node ].smaller_child;
	while ( m_tree[ next ].larger_child != NOT_USED )
		next = m_tree[ next ].larger_child;

	return( next );
}


void COXCompressor::DeleteString( int p )
// --- In      : p : node to be deleted
// --- Out     : 
// --- Returns :
// --- Effect  : This routine performs the classic binary tree deletion algorithm.
//				 If the node to be deleted has a null link in either direction, we
//				 just pull the non-null link up one to replace the existing link.
//				 If both links exist, we instead delete the next link in order, which
//				 is guaranteed to have a null link, then replace the node to be deleted
//				 with the next link.
{
	int  replacement;
	if ( m_tree[ p ].parent == NOT_USED )
		return;

	if ( m_tree[ p ].larger_child == NOT_USED )
		ContractNode( p, m_tree[ p ].smaller_child );
	else if ( m_tree[ p ].smaller_child == NOT_USED )
		ContractNode( p, m_tree[ p ].larger_child );
	else
	{
		replacement = FindNextNode( p );
		DeleteString( replacement );
		ReplaceNode( p, replacement );
	}
}

int COXCompressor::AddString( int new_node, int* match_position )
// --- In      : new_node : new node to be added
//				 match_position : tracking the string currently in the tree that best
//				                  matches the one inserted
// --- Out     : 
// --- Returns : match_length : the length of the matched string
// --- Effect  : This where most of the work done by the encoder takes place.  This
//				 routine is responsible for adding the new node to the binary tree.
//				 It also has to find the best match among all the existing nodes in
//				 the tree, and return that to the calling routine.  To make matters
//				 even more complicated, if the new_node has a duplicate in the tree,
//				 the old_node is deleted, for reasons of efficiency.
{
	int i;
	int test_node;
	int match_length;
	int *child;
	int delta=0;         

	if ( new_node == END_OF_STREAM )
		return( 0 );

	test_node = m_tree[ TREE_ROOT ].larger_child;
	match_length = 0;
	for ( ; ; )
	{
		for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ )
		{
			delta = m_window[MOD_WINDOW( new_node + i ) ] -
				m_window[ MOD_WINDOW( test_node + i ) ];
			if ( delta != 0 )
				break;
		}

		if ( i >= match_length )
		{
			match_length = i;
			*match_position = test_node;
			if ( match_length >= LOOK_AHEAD_SIZE )
			{
				ReplaceNode( test_node, new_node );
				return( match_length );
			}
		}

		if ( delta >= 0 )
			child = &m_tree[ test_node ].larger_child;
		else
			child = &m_tree[ test_node ].smaller_child;

		if ( *child == NOT_USED )
		{
			*child = new_node;
			m_tree[ new_node ].parent = test_node;
			m_tree[ new_node ].larger_child = NOT_USED;
			m_tree[ new_node ].smaller_child = NOT_USED;

			return( match_length );
		}

		test_node = *child;
	}                           
}

// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================

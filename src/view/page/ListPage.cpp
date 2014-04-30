/*
 * DictionaryListPage.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: Dawid Drozd
 */

#include "ListPage.h"
#include <cassert>

ListPage::ListPage( const string& title ) :
	Page( title ),
	m_pListView( nullptr )
{
}

ListPage::~ListPage()
{
	m_pListView = nullptr;
}

void ListPage::setAdapter( ListAdapter* pAdapter )
{
	assert( getContent() );
	assert( getContent()->retainCount() > 0 );
	assert( m_pListView );
	assert( pAdapter );
	assert( m_pListView->getAdapter() == nullptr );
	m_pListView->setAdapter( pAdapter );
	//m_pListView->setCachedRows( 6 );
}

void ListPage::setContentSize( const CCSize& size )
{
	assert( getContent() );
	assert( getContent()->retainCount() > 0 );
	assert( m_pListView );
	m_pListView->setContentSize( size );
}

ListView* ListPage::getListView() const
{
	if( getContent() == nullptr )
	{
		return nullptr;
	}

	assert( getContent()->retainCount() > 0 );
	assert( m_pListView );
	return m_pListView;
}

void ListPage::inflateContent()
{
	LOG( "Inflate content" );

	m_pListView = ListView::create();
	setContent( m_pListView );
}

void ListPage::reloadAllData()
{
	assert( getContent()->retainCount() > 0 );
	assert( getListView() );
	float offset = getListView()->getOffset();
	getListView()->reloadData();
	getListView()->setOffset( offset );
}

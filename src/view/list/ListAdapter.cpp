/*
 * ListAdapter.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: Dawid Drozd
 */

#include "view/list/ListAdapter.h"

namespace KoalaGui
{

ListAdapter::ListAdapter()
{
}

ListAdapter::~ListAdapter()
{
}

View* ListAdapter::getRow( const unsigned index, View* pConvertView )
{
	assert( index < getItemsCount() );

	CCObject* pHolder = nullptr;

	if( pConvertView == nullptr
			|| pConvertView->getNode()->getUserObject() == nullptr )
	{
		pConvertView = getNewRow( index );
		assert( pConvertView );

		pHolder = getNewHolder( index, pConvertView );
		assert( pHolder );
		assert( pHolder->retainCount() >
				pHolder->autoreleaseCount() ); //Please do not release holders
		pConvertView->getNode()->setUserObject( pHolder );
		assert( pHolder->retainCount() == 2 );
		//Now when row will be released Holder will be released too
		pHolder->release();
	}
	else
	{
		assert( pConvertView );
		pHolder = pConvertView->getNode()->getUserObject();
	}

	initializeHolder( index, pHolder );
	return pConvertView;
}

} /* namespace KoalaGui */

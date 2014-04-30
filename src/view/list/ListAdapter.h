/*
 * ListAdapter.h
 *
 *  Created on: Jun 18, 2013
 *      Author: Dawid Drozd
 */

#ifndef LISTADAPTER_H_
#define LISTADAPTER_H_

#include "cocos2d.h"
#include "view/View.h"
#include "view/layout/linear/LinearLayoutParams.h"
USING_NS_CC;

namespace KoalaGui
{

class ListAdapter: public CCObject
{
public:
	friend class ListView;
	virtual ~ListAdapter();

	virtual unsigned getItemsCount() const = 0;

	/**
	 * Prepare new row view
	 *
	 * @param index
	 * @return new row
	 */
	virtual View* getNewRow( const unsigned index ) = 0;

	/**
	 *
	 * @param index
	 * @param pConvertView
	 * @return new holder that has pointers to all needed data in view
	 */
	virtual CCObject* getNewHolder( const unsigned index,
									View* pConvertView ) = 0;

	/**
	 * Simply call on all pointers setters what you want
	 * @param index
	 * @param pHolder
	 */
	virtual void initializeHolder( const unsigned index,
								   CCObject* pHolderObject ) = 0;

protected:
	ListAdapter();

private:
	/**
	 *
	 * @param index
	 * @param pConvertView can be null
	 * @return new inflated view
	 */
	View* getRow( const unsigned index, View* pConvertView );
};

} /* namespace KoalaGui */
#endif /* LISTADAPTER_H_ */

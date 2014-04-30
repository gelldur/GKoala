/*
 * PageViewAdapter.h
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#ifndef PAGEVIEWADAPTER_H_
#define PAGEVIEWADAPTER_H_

#include "view/list/ListView.h"
#include "view/page/Page.h"
#include <vector>

namespace KoalaGui
{

class PageView;

class PageViewAdapter: public ListAdapter
{
public:
	KK_CREATE_METHOD( PageViewAdapter )
	virtual ~PageViewAdapter();

	virtual unsigned getItemsCount() const;
	virtual View* getNewRow( const unsigned index );
	virtual CCObject* getNewHolder( const unsigned index, View* pConvertView );
	virtual void initializeHolder( const unsigned index, CCObject* pHolderObject );

	Page* getPage( unsigned index ) const
	{
		assert( index < m_pages.size() );
		return m_pages[index];
	}

	unsigned getPagesCount() const
	{
		return m_pages.size();
	}

	unsigned addPage( Page* pPage );
	void insertPage( const unsigned index, Page* pPage );
	void removePage( const unsigned index );

	void updatePages()
	{
		for( Page* page : m_pages )
		{
			page->updatePage();
		}
	}

protected:
	PageViewAdapter();

private:

	struct Holder: public CCObject
	{
		LinearLayout* pLayout;
		unsigned index;
	};

	static const int ID_CONTENT;

	PageView* m_pPageView;
	std::vector<Page*> m_pages;
};

} /* namespace KoalaGui */
#endif /* PAGEVIEWADAPTER_H_ */

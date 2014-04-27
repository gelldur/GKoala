/*
 * PageViewAdapter.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#include "view/page/PageViewAdapter.h"
#include "view/page/PageView.h"
#include "utils/Utils.h"
#include "view/layout/linear/LinearLayout.h"

namespace KoalaGui
{

const int PageViewAdapter::ID_CONTENT = 0x721;

PageViewAdapter::PageViewAdapter() :
	m_pPageView ( nullptr )
{
}

PageViewAdapter::~PageViewAdapter()
{
	for ( Page* pPage : m_pages )
	{
		pPage->setContent ( nullptr );
		pPage->release();
	}

	m_pages.clear();
	m_pPageView = nullptr;
}

unsigned PageViewAdapter::addPage ( Page* pPage )
{
	pPage->retain();
	m_pages.push_back ( pPage );
	return m_pages.size() - 1;
}

void PageViewAdapter::insertPage ( const unsigned index, Page* pPage )
{
	pPage->retain();
	m_pages.insert ( m_pages.begin() + index, pPage );
}

void PageViewAdapter::removePage ( const unsigned index )
{
	assert (  index < m_pages.size() );
	m_pages[index]->setContent ( nullptr );
	m_pages[index]->release();
	m_pages.erase ( m_pages.begin() + index );
}

unsigned PageViewAdapter::getItemsCount() const
{
	return m_pages.size();
}

View* PageViewAdapter::getNewRow ( const unsigned index )
{
	LinearLayout* pLayoutRow = LinearLayout::create ( Orientation::VERTICAL );
	pLayoutRow->setOpacity ( 0 );
	return View::create ( pLayoutRow,
						  new LinearLayoutParams (
							  LinearLayoutParams ( SizePolicy ( Policy::PERCENT_X, 1 ),
									  SizePolicyMatchContent ) ) );
}

CCObject* PageViewAdapter::getNewHolder ( const unsigned index,
		View* pConvertView )
{
	Holder* pHolder = new Holder();
	pHolder->index = index;
	pHolder->pLayout = dynamic_cast<LinearLayout*> ( pConvertView->getNode() );
	assert ( pHolder->pLayout );
	return pHolder;
}

void PageViewAdapter::initializeHolder ( const unsigned index,
		CCObject* pHolderObject )
{
	Holder* pHolder = dynamic_cast<Holder*> ( pHolderObject );
	assert ( pHolder );

	pHolder->pLayout->removeAllChildren();
	m_pages[pHolder->index]->setContent ( nullptr );

	assert ( index < m_pages.size() );
	Page* pPage = m_pages[index];
	pPage->inflateContent();
	pHolder->index = index;
	pHolder->pLayout->addView ( pPage->getContent(), LayoutParamsMatchMatch );
}

} /* namespace KoalaGui */

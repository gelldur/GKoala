/*
 * PageIndicator.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#include "view/page/PageIndicator.h"
#include "view/page/PageView.h"
#include "view/layout/Positioner.h"
#include "utils/Utils.h"

namespace KoalaGui
{

PageIndicatorController::PageIndicatorController() :
	m_pPageView( nullptr )
{
}

PageIndicatorController::~PageIndicatorController()
{
	m_pPageView = nullptr;
}

bool PageIndicatorController::nextPage()
{
	assert( m_pPageView );

	if( m_pPageView->getCurrentPage() + 1 >= m_pPageView->getItemsCount() )
	{
		return false;
	}

	setCurrentPage( m_pPageView->getCurrentPage() + 1 );

	return true;
}

bool PageIndicatorController::previousPage()
{
	LOG( "method: %s::%s is called.", __FILE__, __func__ );
	assert( m_pPageView );

	if( m_pPageView->getCurrentPage() < 1 )
	{
		return false;
	}

	setCurrentPage( m_pPageView->getCurrentPage() - 1 );

	return true;
}

void PageIndicatorController::setPageView( PageView* pPageView )
{
	m_pPageView = pPageView;
	assert( m_pPageView );
}

void PageIndicatorController::setCurrentPage( unsigned index )
{
	LOG( "method: %s::%s is called.", __FILE__, __func__ );

	if( m_pPageView->getCurrentPage() == index )
	{
		return;
	}

	LOG( "Page indicator Index %u", index );
	m_pPageView->setPage( index );
}

} /* namespace KoalaGui */



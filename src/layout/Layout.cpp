/*
 * Layout.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/Layout.h"

namespace GKoala
{

Layout::Layout() :
	m_pLayoutConfiguration ( nullptr )
{
}

Layout::~Layout()
{
	CC_SAFE_RELEASE_NULL ( m_pLayoutConfiguration );
}

bool Layout::initWithConfiguration ( LayoutConfiguration* pConfiguration )
{
	if ( inherited::init() == false )
	{
		return false;
	}

	m_pLayoutConfiguration = pConfiguration;

	if ( pConfiguration == nullptr )
	{
		GKoala_assert ( pConfiguration != nullptr,
						"You can't init Layout with null configuration" );
		return false;
	}

	pConfiguration->retain();

	return true;
}

void Layout::addChild ( cocos2d::CCNode* pChild, int zOrder, int tag )
{
	GKoala_assert ( pChild != nullptr, "pChild can't be null" );
	GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );

	pChild->setZOrder ( zOrder );
	pChild->setTag ( tag );

	addChild ( pChild, m_pLayoutConfiguration->getDefaultLayoutParameter() );
}

void Layout::addChild ( cocos2d::CCNode* pChild,
						LayoutParameter* pLayoutParameter )
{
	GKoala_assert ( pChild != nullptr, "pChild can't be null" );
	GKoala_assert ( pLayoutParameter != nullptr, "pLayoutParameter can't be null" );
	GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );

	inherited::addChild ( pChild, pChild->getZOrder(), pChild->getTag() );
	m_pLayoutConfiguration->addView ( pChild, pLayoutParameter );

	if ( isRunning() )
	{
		m_pLayoutConfiguration->updateStructure();
	}
}

void Layout::removeChild ( CCNode* pChild, bool cleanup )
{
	GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );
	inherited::removeChild ( pChild, cleanup );

	if ( isRunning() )
	{
		m_pLayoutConfiguration->updateStructure();
	}
}

} /* namespace GKoala */

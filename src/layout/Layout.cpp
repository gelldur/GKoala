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

} /* namespace GKoala */

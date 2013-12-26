/*
 * LinearConfiguration.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/configuration/linear/LinearConfiguration.h"

namespace GKoala
{

LinearConfiguration::LinearConfiguration()
{
}

LinearConfiguration::~LinearConfiguration()
{
	for ( auto view : m_views )
	{
		view.first->release();
		view.second->release();
	}

	m_views.clear();
}

void LinearConfiguration::addView ( cocos2d::CCNode* pChild,
									LayoutParameter* pLayoutParameter )
{
	GKoala_assert ( pChild != nullptr, "pChild can't be null" );
	GKoala_assert ( pLayoutParameter != nullptr, "pLayoutParameter can't be null" );

	LinearLayoutParameter* pParameter = dynamic_cast<LinearLayoutParameter*>
										( pLayoutParameter );

	if ( pParameter == nullptr )
	{
		GKoala_assert ( pParameter != nullptr,
						"To linear layout configuration you can pass only LinearLayoutParameter" );
		return;
	}

	m_views.push_back ( std::make_pair ( pChild, pParameter ) );
	pParameter->retain();
	pChild->retain();
}

void LinearConfiguration::removeChild ( cocos2d::CCNode* pChild )
{
	if ( pChild == nullptr )
	{
		return;
	}

	int i = 0;

	for ( auto view : m_views )
	{
		if ( view.first == pChild )
		{
			view.first->release();
			view.second->release();
			m_views.erase ( m_views.begin() + i );
			return;
		}

		++i;
	}

	GKoala_assert ( false, "pChild can't be found!" );
}

LayoutParameter* LinearConfiguration::getDefaultLayoutParameter()
{
	return  LinearLayoutParameter::createWrapWrap();
}

void LinearConfiguration::updateStructure()
{
	//TODO make it recursive
	onMeasure();
	onLayout();
}

void LinearConfiguration::onMeasure()
{
	//Measure all children recursive
}

void LinearConfiguration::onLayout()
{
	//Layout all children recursive
}

} /* namespace GKoala */

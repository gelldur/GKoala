/*
 * LinearConfiguration.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/configuration/linear/LinearConfiguration.h"
#include "layout/configuration/Utils.h"

using namespace cocos2d;

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

void LinearConfiguration::addView ( cocos2d::CCNode* pChild,LayoutParameter* pLayoutParameter )
{
	GKoala_assert ( pChild != nullptr, "pChild can't be null" );
	GKoala_assert ( pLayoutParameter != nullptr, "pLayoutParameter can't be null" );

	LinearLayoutParameter* pParameter = dynamic_cast<LinearLayoutParameter*>( pLayoutParameter );
	if ( pParameter == nullptr )
	{
		GKoala_assert ( false,"To linear layout configuration you can pass only LinearLayoutParameter" );
		return;
	}

	m_views.push_back ( std::make_pair ( pChild, pParameter ) );
	pParameter->retain();
	pChild->retain();
}

void LinearConfiguration::removeChild ( cocos2d::CCNode* pChild )
{
	GKoala_assert ( pChild, "pChild can't be found!" );

	int i = 0;
	for ( auto&& view : m_views )
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

void LinearConfiguration::updateStructure(LayoutParameter* pLayoutParameter)
{
	CCSize measuredChildren = onMeasure(pLayoutParameter);

	CCSize workingLayoutSize = m_pWorkingLayout->getContentSize();
	if(pLayoutParameter->getWidth().isConst())
	{
		workingLayoutSize.width = pLayoutParameter->getWidth().getValue();
	}
	else if(pLayoutParameter->getWidth().isWrapContent())
	{
		workingLayoutSize.width = measuredChildren.width;
	}
	else if(pLayoutParameter->getWidth().isMatchParent())
	{
		workingLayoutSize.width = 0;
	}

	if(pLayoutParameter->getHeight().isConst())
	{
		workingLayoutSize.height = pLayoutParameter->getHeight().getValue();
	}
	else if(pLayoutParameter->getHeight().isWrapContent())
	{
		workingLayoutSize.height = measuredChildren.height;
	}
	else if(pLayoutParameter->getHeight().isMatchParent())
	{
		workingLayoutSize.height = 0;
	}

	//Now we have measured all children (only children with match_parent aren't measured)
	//But now we don't care about MATCH_PARENT because they will get what is left space only
	m_pWorkingLayout->setContentSize(workingLayoutSize);

	onLayout();
}

CCSize LinearConfiguration::onMeasure(LayoutParameter* pLayoutParameter)
{
	CCSize measuredSize;
	float CCSize::*pMaxValue = nullptr;
	float CCSize::*pSumValue = nullptr;
	if(m_orientation == VERTICAL)
	{
		pMaxValue = &CCSize::width;
		pSumValue = &CCSize::height;
	}
	else
	{
		pMaxValue = &CCSize::height;
		pSumValue = &CCSize::width;
	}

	//Measure all children recursive
	for(auto&& view : m_views)
	{
		//TODO We can fix it. Simply wrap will be match
		GKoala_assert(!(view.second->getWidth().isMatchParent() && pLayoutParameter->getWidth().isWrapContent()),"You can't have MATCH_PARENT inside WRAP_CONTENT");
		GKoala_assert(!(view.second->getHeight().isMatchParent() && pLayoutParameter->getHeight().isWrapContent()),"You can't have MATCH_PARENT inside WRAP_CONTENT");

		auto&& pView = view.first;
		auto&& pViewParameters = view.second;

		prepareViewForMeasure(pView,pViewParameters);

		LayoutInterface* pLayout = dynamic_cast<LayoutInterface*>(pView);
		if(pLayout != nullptr)
		{
			pLayout->updateStructure(pViewParameters);
		}

		const CCSize&& viewSize = Utils::getFullSize(pView,pViewParameters);
		measuredSize.*pMaxValue = std::max(measuredSize.*pMaxValue , viewSize.*pMaxValue);
		measuredSize.*pSumValue += viewSize.*pSumValue;
	}

	return measuredSize;
}

void LinearConfiguration::prepareViewForMeasure(cocos2d::CCNode* pView,LinearLayoutParameter* pViewParameters)
{
	CCSize viewSize;

	LayoutInterface* pLayout = dynamic_cast<LayoutInterface*>(pView);
	if(pLayout != nullptr)
	{
		//If layout doesn't have const width or height we must measure them
		if(pViewParameters->getWidth().isConst() == false)
		{
			viewSize.width = 0;
		}
		if(pViewParameters->getHeight().isConst() == false)
		{
			viewSize.height = 0;
		}
	}

	//If we have const size we set it
	if(pViewParameters->getWidth().isConst())
	{
		viewSize.width = pViewParameters->getWidth().getValue();
	}
	else if(pViewParameters->getWidth().isMatchParent())
	{
		//If this is not layout we want measure space for it if we set MATCH_PARENT
		//if you look up you will see that layouts also need measure (we set it to 0)
		viewSize.width = 0;
	}
	//If we have WRAP_CONTENT we leave value as it is

	//If we have const size we set it
	if(pViewParameters->getHeight().isConst())
	{
		viewSize.height = pViewParameters->getHeight().getValue();
	}
	else if(pViewParameters->getHeight().isMatchParent())
	{
		//If this is not layout we want measure space for it if we set MATCH_PARENT
		//if you look up you will see that layouts also need measure (we set it to 0)
		viewSize.height = 0;
	}
	//If we have WRAP_CONTENT we leave value as it is

	pView->setContentSize(viewSize);
}

void LinearConfiguration::onLayout()
{
	//Layout all children recursive
}



} /* namespace GKoala */

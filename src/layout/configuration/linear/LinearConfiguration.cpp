/*
 * LinearConfiguration.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/configuration/linear/LinearConfiguration.h"
#include "layout/configuration/Utils.h"
#include "Positioner.h"

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

void LinearConfiguration::setOptions(int options)
{
	int previousGravity = m_gravity;
	m_gravity = Gravity::MASK & options;

	if(m_gravity == 0)
	{
		//gravity wasn't set so we restore
		m_gravity = previousGravity;
	}

	Orientation previousOrientation = m_orientation;
	m_orientation = static_cast<Orientation>(Orientation::MASK & options);

	if(m_orientation == 0)
	{
		//Notihing is set so we restore previous
		m_orientation = previousOrientation;
	}
	GKoala_assert(m_orientation == HORIZONTAL || m_orientation == VERTICAL,"Sorry invalid option for orientation");
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

	if(pLayoutParameter != nullptr)
	{
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
	}

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
		if(pLayoutParameter != nullptr)
		{
		//TODO We can fix it. Simply wrap will be match
		GKoala_assert(!(view.second->getWidth().isMatchParent() && pLayoutParameter->getWidth().isWrapContent()),"You can't have MATCH_PARENT inside WRAP_CONTENT");
		GKoala_assert(!(view.second->getHeight().isMatchParent() && pLayoutParameter->getHeight().isWrapContent()),"You can't have MATCH_PARENT inside WRAP_CONTENT");
		}

		auto&& pView = view.first;
		auto&& pViewParameters = view.second;

		prepareViewForMeasure(pView,pViewParameters);

		LayoutInterface* pLayout = dynamic_cast<LayoutInterface*>(pView);
		if(pLayout != nullptr)
		{
			pLayout->updateStructure(pViewParameters);
		}

		const CCSize viewSize = Utils::getFullSize(pView,pViewParameters);
		measuredSize.*pMaxValue = std::max(measuredSize.*pMaxValue , viewSize.*pMaxValue);
		measuredSize.*pSumValue += viewSize.*pSumValue;
	}

	return measuredSize;
}

void LinearConfiguration::prepareViewForMeasure(cocos2d::CCNode* pView,LinearLayoutParameter* pViewParameters)
{
	CCSize viewSize = pView->getContentSize();

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
	if(m_orientation == VERTICAL)
	{
		onLayoutVertical();
	}
	else
	{
		onLayoutHorizontal();
	}
}

void LinearConfiguration::onLayoutVertical()
{
	if((m_gravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL)
	{
		int i=0;
		for(auto&& view : m_views)
		{
			if(++i == 1)
			{
				//This is first view
				Positioner::centerInParentVertically(view.first,(view.second->getMarginTop().getValue() - view.second->getMarginBottom().getValue()) * 0.5F);
			}
			else
			{
				applyLayoutGravityVerticalCenterVertical(view);
			}

			applyViewGravityVertical(view);
		}
	}
	else if((m_gravity & Gravity::TOP) == Gravity::TOP)
	{
		View previousView;
		for(auto&& view : m_views)
		{
			applyLayoutGravityVerticalTop(view,previousView);
			applyViewGravityVertical(view);
			previousView = view;
		}
	}
	else if((m_gravity & Gravity::BOTTOM) == Gravity::BOTTOM)
	{
		View previousView;
		for(auto& it = m_views.rbegin() ; it != m_views.rend(); ++it)
		{
			View& view = *it;
			applyLayoutGravityVerticalBottom(view,previousView);
			applyViewGravityVertical(view);
			previousView = view;
		}
	}
	else
	{
		GKoala_assert(false,"You don't set gravity on layout?");
	}

}

void LinearConfiguration::onLayoutHorizontal()
{
	if((m_gravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL)
	{
		int i=0;
		for(auto&& view : m_views)
		{
			if(++i == 1)
			{
				//This is first view
				Positioner::centerInParentHorizontally(view.first,(view.second->getMarginLeft().getValue() - view.second->getMarginRight().getValue()) * 0.5F);
			}
			else
			{
				applyLayoutGravityHorizontalCenterHorizontal(view);
			}

			applyViewGravityHorizontal(view);
		}
	}
	else if((m_gravity & Gravity::LEFT) == Gravity::LEFT)
	{
		View previousView;
		for(auto&& view : m_views)
		{
			applyLayoutGravityHorizontalLeft(view,previousView);
			applyViewGravityHorizontal(view);
			previousView = view;
		}
	}
	else if((m_gravity & Gravity::RIGHT) == Gravity::RIGHT)
	{
		View previousView;
		for(auto& it = m_views.rbegin() ; it != m_views.rend(); ++it)
		{
			View& view = *it;
			applyLayoutGravityHorizontalRight(view,previousView);
			applyViewGravityHorizontal(view);
			previousView = view;
		}
	}
	else
	{
		GKoala_assert(false,"You don't set gravity on layout?");
	}
}

void LinearConfiguration::applyLayoutGravityHorizontalLeft(View& view, View& previousView)
{
	if(previousView.first != nullptr)
	{
		Positioner::toRightOf(view.first,previousView.first,previousView.second->getMarginRight().getValue() + view.second->getMarginLeft().getValue());
	}
	else
	{
		Positioner::leftEdge(view.first,view.second->getMarginLeft().getValue());
	}
}

void LinearConfiguration::applyLayoutGravityHorizontalRight(View& view,View& previousView)
{
	if(previousView.first != nullptr)
	{
		Positioner::toLeftOf(view.first,previousView.first,previousView.second->getMarginLeft().getValue() + view.second->getMarginRight().getValue());
	}
	else
	{
		Positioner::rightEdge(view.first,view.second->getMarginRight().getValue());
	}
}

void LinearConfiguration::applyLayoutGravityHorizontalCenterHorizontal(View& view)
{
	CCSize sizeOfEnteringView = Utils::getFullSize(view.first,view.second);
	View previousSubView;

	for(auto&& subView : m_views)
	{
		if(subView.first == view.first)
		{
			Positioner::toRightOf(view.first,previousSubView.first,view.second->getMarginLeft().getValue() + previousSubView.second->getMarginRight().getValue());
			break;
		}
		previousSubView = subView;
		subView.first->setPositionX( subView.first->getPositionX() - sizeOfEnteringView.width* 0.5F  );
	}
}

void LinearConfiguration::applyViewGravityHorizontal(View& view)
{
	int viewGravity = view.second->getGravity();

	if((viewGravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL)
	{
		Positioner::centerInParentVertically(view.first,view.second->getMarginTop().getValue() - view.second->getMarginBottom().getValue());
	}
	else if((viewGravity & Gravity::TOP) == Gravity::TOP)
	{
		Positioner::topEdge(view.first,view.second->getMarginTop().getValue());
	}
	else if((viewGravity & Gravity::BOTTOM) == Gravity::BOTTOM)
	{
		Positioner::bottomEdge(view.first,view.second->getMarginBottom().getValue());
	}
	else
	{
		GKoala_assert(false,"You don't set VERTICAL gravity on view?");
	}
}

void LinearConfiguration::applyLayoutGravityVerticalTop(View& view, View& previousView)
{
	if(previousView.first != nullptr)
	{
		Positioner::toBelowOf(view.first,previousView.first,previousView.second->getMarginBottom().getValue() + view.second->getMarginTop().getValue());
	}
	else
	{
		Positioner::topEdge(view.first,view.second->getMarginTop().getValue());
	}
}

void LinearConfiguration::applyLayoutGravityVerticalBottom(View& view, View& previousView)
{
	if(previousView.first != nullptr)
	{
		Positioner::toAboveOf(view.first,previousView.first,previousView.second->getMarginTop().getValue() + view.second->getMarginBottom().getValue());
	}
	else
	{
		Positioner::bottomEdge(view.first,view.second->getMarginBottom().getValue());
	}
}

void LinearConfiguration::applyLayoutGravityVerticalCenterVertical(View& view)
{
	CCSize sizeOfEnteringView = Utils::getFullSize(view.first,view.second);
	View previousSubView;

	for(auto&& subView : m_views)
	{
		if(subView.first == view.first)
		{
			Positioner::toBelowOf(view.first,previousSubView.first,view.second->getMarginTop().getValue() + previousSubView.second->getMarginBottom().getValue());
			break;
		}
		previousSubView = subView;
		subView.first->setPositionY( subView.first->getPositionY() - sizeOfEnteringView.height* 0.5F  );
	}
}

void LinearConfiguration::applyViewGravityVertical(View& view)
{
	int viewGravity = view.second->getGravity();

	if((viewGravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL)
	{
		Positioner::centerInParentHorizontally(view.first,view.second->getMarginLeft().getValue() - view.second->getMarginRight().getValue());
	}
	else if((viewGravity & Gravity::LEFT) == Gravity::LEFT)
	{
		Positioner::leftEdge(view.first,view.second->getMarginLeft().getValue());
	}
	else if((viewGravity & Gravity::RIGHT) == Gravity::RIGHT)
	{
		Positioner::rightEdge(view.first,view.second->getMarginRight().getValue());
	}
	else
	{
		GKoala_assert(false,"You don't set HORIZONTAL gravity on view?");
	}
}

} /* namespace GKoala */

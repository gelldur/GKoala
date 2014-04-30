/*
 * RelativeLayout.cpp
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#include "view/layout/relative/RelativeLayout.h"
#include "view/layout/Positioner.h"

namespace KoalaGui
{

RelativeLayout::RelativeLayout( int tag ) :
	AbstractLayout( tag )
{
}

bool RelativeLayout::init()
{
	if( !AbstractLayout::init() )
	{
		CCAssert( false, "Failed to init" );
		return false;
	}

	return true;
}

RelativeLayout* RelativeLayout::create( const int tag )
{
	RelativeLayout* pLayout = new RelativeLayout( tag );

	if( pLayout && pLayout->init() )
	{
		pLayout->autorelease();
		return pLayout;
	}
	else
	{
		CC_SAFE_DELETE( pLayout );
		return NULL;
	}
}

RelativeLayout::~RelativeLayout()
{
}

bool RelativeLayout::updateView( View* pView )
{
	AbstractLayout::updateView( pView );

	applyPositionRule( pView );
	return true;
}

View* RelativeLayout::addView( CCNode* pNode,
							   const RelativeLayoutParams& params )
{
	assert( pNode );
	RelativeLayoutParams* pParams = new RelativeLayoutParams( params );
	return addViewWithParams( pNode, pParams );
}

bool RelativeLayout::applyPositionRule( View* pView )
{
	RelativeLayoutParams* pParams = ( RelativeLayoutParams* )( pView
									->getLayoutParams() );
	std::vector<RelativeLayoutParams::Align> rules = pParams->getRules();

	CCAssert( rules.size() > 0, "You must add at least 1 rule for X and Y" );

	for( unsigned i = 0; i < rules.size(); i++ )
	{
		const short align = rules[i].getAlignType();
		const int relativeTag = rules[i].getTag();
		View* pRelativeView = getViewByTag( relativeTag );

		applyXPosition( pView, pRelativeView, align );
		applyYPosition( pView, pRelativeView, align );
	}

	return true;
}

void RelativeLayout::applyXPosition( View* pView, const View* pRelativeTo,
									 const short align )
{

	if( align & RelativeLayoutParams::PARENT_LEFT )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(),
										 ccp( 0, pView->getMarginLeftTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::PARENT_RIGHT )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginRightTopCorner(),
										 ccp( Positioner::getScaledWidth( this ),
											  pView->getMarginRightTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::PARENT_CENTER_HORIZONTAL )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginCenterPoint(),
										 ccp( Positioner::getScaledWidth( this ) * 0.5F,
											  pView->getMarginCenterPoint().y ) );
	}
	else if( align & RelativeLayoutParams::LEFT )
	{
		CCAssert( pRelativeTo, "Can't be null?" );

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginRightTopCorner(),
										 ccp( pRelativeTo->getMarginLeftTopCorner().x,
											  pView->getMarginRightTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::RIGHT )
	{
		CCAssert( pRelativeTo, "Can't be null?" );
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(),
										 ccp( pRelativeTo->getMarginRightTopCorner().x,
											  pView->getMarginLeftTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::CENTER_HORIZONTAL )
	{
		CCAssert( pRelativeTo, "Can't be null?" );

		Positioner::setCornerToPosition( pView->getNode(),
										 ccp( Positioner::getCenterPoint( pView->getNode() ).origin.x, 0 ),
										 ccp( Positioner::getCenterPoint( pRelativeTo->getNode() ).origin.x,
											  0 ) );
	}
	else if( align & RelativeLayoutParams::TO_LEFT )
	{
		CCAssert( pRelativeTo, "Can't be null?" );

		Positioner::setCornerToPosition( pView->getNode(),
										 ccp( Positioner::getLeftBottomCorner( pView->getNode() ).origin.x, 0 ),
										 ccp( Positioner::getLeftBottomCorner( pRelativeTo->getNode() ).origin.x,
											  0 ) );
	}
	else if( align & RelativeLayoutParams::TO_RIGHT )
	{
		CCAssert( pRelativeTo, "Can't be null?" );

		Positioner::setCornerToPosition( pView->getNode(),
										 ccp( Positioner::getRightBottomCorner( pView->getNode() ).origin.x, 0 ),
										 ccp( Positioner::getRightBottomCorner( pRelativeTo->getNode() ).origin.x,
											  0 ) );
	}
}

void RelativeLayout::applyYPosition( View* pView, const View* pRelativeTo,
									 const short align )
{
	if( align & RelativeLayoutParams::PARENT_TOP )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(),
										 ccp( pView->getMarginLeftTopCorner().x,
											  Positioner::getScaledHeight( this ) ) );
	}
	else if( align & RelativeLayoutParams::PARENT_BOTTOM )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftBottomCorner(),
										 ccp( pView->getMarginLeftBottomCorner().x, 0 ) );
	}
	else if( align & RelativeLayoutParams::PARENT_CENTER_VERTICAL )
	{
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginCenterPoint(),
										 ccp( pView->getMarginCenterPoint().x,
											  Positioner::getScaledHeight( this ) * 0.5F ) );
	}
	else if( align & RelativeLayoutParams::ABOVE )
	{
		CCAssert( pRelativeTo, "Can't be null?" );

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftBottomCorner(),
										 ccp( pView->getMarginLeftBottomCorner().x,
											  pRelativeTo->getMarginLeftTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::BELOW )
	{
		CCAssert( pRelativeTo, "Can't be null?" );
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(),
										 ccp( pView->getMarginLeftTopCorner().x,
											  pRelativeTo->getMarginLeftBottomCorner().y ) );
	}
	else if( align & RelativeLayoutParams::CENTER_VERTICAL )
	{
		CCAssert( pRelativeTo, "Can't be null?" );
		Positioner::setCornerToPosition( pView->getNode(),
										 ccp( 0, Positioner::getCenterPoint( pView->getNode() ).origin.y ),
										 ccp( 0,
											  Positioner::getCenterPoint( pRelativeTo->getNode() ).origin.y ) );
	}
	else if( align & RelativeLayoutParams::TO_TOP )
	{
		CCAssert( pRelativeTo, "Can't be null?" );
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(),
										 ccp( pView->getMarginLeftTopCorner().x,
											  pRelativeTo->getMarginLeftTopCorner().y ) );
	}
	else if( align & RelativeLayoutParams::TO_BOTTOM )
	{
		CCAssert( pRelativeTo, "Can't be null?" );
		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftBottomCorner(),
										 ccp( pView->getMarginLeftBottomCorner().x,
											  pRelativeTo->getMarginLeftBottomCorner().y ) );
	}
}

CCSize RelativeLayout::onMeasure( float CCSize::* pValue,
								  const bool withBackground )
{
	CCSize size;

	//Find max width and max height
	for( View* pView : getViews() )
	{
		const CCSize tmpSize = pView->getFullSize();
		size.*pValue = max( size.*pValue, tmpSize.*pValue );
	}

	CCSize layoutSize = getContentSize();
	layoutSize.*pValue = size.*pValue;
	setContentSize( layoutSize );

	onLayout();

	for( View* pView : getViews() )
	{
		CCSize tmpSize = getRelatedSizeForView( pView );

		if( size.*pValue < tmpSize.*pValue )
		{
			size.*pValue = tmpSize.*pValue;
		}
	}

	if( withBackground )
	{
		measureBackground( size );
	}

	return size;
}

CCSize RelativeLayout::getLeftSpaceForView( CCNode* pNode,
		const LayoutParams& params )
{
	//For relative it is always full parent size - margins
	CCSize size = getContentSize();
	size.width -= ( params.getMarginLeft().getValue()
					+ params.getMarginRight().getValue() );
	size.height -= ( params.getMarginTop().getValue()
					 + params.getMarginBottom().getValue() );
	return size;
}

bool RelativeLayout::onLayout()
{
	if( AbstractLayout::onLayout() == false )
	{
		return false;
	}

	for( View* pView : getViews() )
	{
		updateView( pView );
	}

	return true;
}

CCSize RelativeLayout::getRelatedSizeForView( View* pView )
{
	assert( pView );
	//We get all views
	vector<View*> relatedViews;
	relatedViews.reserve( 16 );
	relatedViews.push_back( pView );

	for( unsigned i = 0; i < relatedViews.size(); ++i )
	{
		assert( relatedViews[i] );

		RelativeLayoutParams* pParams =
			dynamic_cast<RelativeLayoutParams*>( relatedViews[i]->getLayoutParams() );
		assert( pParams );

		std::vector<RelativeLayoutParams::Align> rules( pParams->getRules() );

		for( unsigned j = 0; j < rules.size(); ++j )
		{
			const int relativeTag = rules[j].getTag();
			View* pRelativeView = getViewByTag( relativeTag );

			if( pRelativeView != nullptr
					&& std::find( relatedViews.begin(), relatedViews.end(),
								  pRelativeView ) == relatedViews.end() )
			{
				relatedViews.push_back( pRelativeView );
			}
		}
	}

	CCSize size;

	float maxRight = ( float ) INT_MIN;
	float maxTop = ( float )  INT_MIN;
	float minLeft = ( float )  INT_MAX;
	float minBottom = ( float )  INT_MAX;

	for( const View* pView : relatedViews )
	{
		CCPoint pointRight = pView->getMarginRightTopCorner();
		CCPoint pointLeft = pView->getMarginLeftBottomCorner();

		if( pointRight.x > maxRight )
		{
			maxRight = pointRight.x;
		}

		if( pointLeft.x < minLeft )
		{
			minLeft = pointLeft.x;
		}

		if( pointRight.y > maxTop )
		{
			maxTop = pointRight.y;
		}

		if( pointLeft.y < minBottom )
		{
			minBottom = pointLeft.y;
		}

		assert( maxRight != INT_MIN && maxRight != INT_MAX );
		assert( maxTop != INT_MIN && maxTop != INT_MAX );
		assert( minLeft != INT_MAX && minLeft != INT_MIN );
		assert( minBottom != INT_MAX && minBottom != INT_MIN );

		size.width = fabs( maxRight - minLeft );
		size.height = fabs( maxTop - minBottom );
	}

	return size;
}

bool RelativeLayout::measureSelf()
{
	if( updateStructure() == false )
	{
		return false;
	}

	bool result = applySizeRuleForLayout( KK_WIDTH, this,
										  LayoutParams( SizePolicyWrapContent, SizePolicyWrapContent ) );
	result = result && applySizeRuleForLayout( KK_HEIGHT, this,
			 LayoutParams( SizePolicyWrapContent, SizePolicyWrapContent ) );

	return result && updateStructure();
}

} /* namespace KoalaGui */

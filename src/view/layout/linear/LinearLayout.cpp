#include "view/layout/linear/LinearLayout.h"
#include "view/layout/Positioner.h"

#include <algorithm>

namespace KoalaGui
{

LinearLayout::LinearLayout( int tag ) :
	AbstractLayout( tag ),
	m_orientation( LinearLayout::HORIZONTAL ),
	m_gravity( Gravity::LEFT | Gravity::TOP ),
	m_isReversed( false )
{
	updateViewsOrder();
}

bool LinearLayout::init( const Orientation& orientation )
{
	if( AbstractLayout::init() == false )
	{
		CCAssert( false, "Failed to init" );
		return false;
	}

	m_orientation = orientation;
	return true;
}

LinearLayout* LinearLayout::create( const Orientation& orientation,
									int tag )
{
	LinearLayout* pRet = new LinearLayout( tag );

	if( pRet && pRet->init( orientation ) )
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

LinearLayout::~LinearLayout()
{
}

const LinearLayout::Orientation LinearLayout::getOrientation() const
{
	return m_orientation;
}

void LinearLayout::setOrientation( const Orientation& orientation )
{
	CCAssert( orientation == VERTICAL ||
			  orientation == HORIZONTAL, "You can set only 2 orientation configurations" );
	m_orientation = orientation;
}

bool LinearLayout::updateView( View* pView )
{
	CCAssert( pView != NULL, "Fail" );

	AbstractLayout::updateView( pView );

	applyPositionRule( pView );
	applyGravityRule( pView );
	return true;
}

bool LinearLayout::applyGravityRule( View* pView )
{
	//We can safely cast it because when we addView we create LinearLayoutParams
	CCAssert( dynamic_cast<LinearLayoutParams*>( pView->getLayoutParams() )
			  , "Problem while casting ;/" );

	if( m_orientation == Orientation::VERTICAL )
	{
		//We apply only LEFT, RIGHT and CENTER_HORIZONTAL gravity
		return applyVerticalGravity( pView );
	}
	else if( m_orientation == Orientation::HORIZONTAL )
	{
		//We apply only TOP, BOTTOM and CENTER_VERTICAL gravity
		return applyHorizontalGravity( pView );
	}

	return false;
}

bool LinearLayout::applyPositionRule( View* pView )
{
	vector<View*>::iterator iterator = getViewIterator( pView );

	if( m_orientation == Orientation::VERTICAL )
	{
		if( iterator == getViews().begin() )
		{
			return applyVerticalPosition( nullptr, *iterator );
		}
		else
		{
			return applyVerticalPosition( * ( iterator - 1 ), *iterator );
		}
	}
	else if( m_orientation == Orientation::HORIZONTAL )
	{
		if( iterator == getViews().begin() )
		{
			return applyHorizontalPosition( nullptr, *iterator );
		}
		else
		{
			return applyHorizontalPosition( * ( iterator - 1 ), *iterator );
		}
	}

	return false;
}

bool LinearLayout::applyVerticalPosition( View* pPreviousView,
		View* pView )
{
	CCPoint point( CCPointZero );

	if( ( m_gravity & Gravity::CENTER_VERTICAL ) == Gravity::CENTER_VERTICAL )
	{
		//...
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginLeftBottomCorner();
		}
		else
		{
			//It will be called only once.
			float height = onMeasure( KK_HEIGHT, false ).height;
			point.y = ( Positioner::getScaledHeight( this ) + height )
					  * 0.5F;
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(), point );
	}
	else if( m_gravity & Gravity::TOP )
	{
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginLeftBottomCorner();
		}
		else
		{
			point.y = Positioner::getScaledHeight( this );
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(), point );
	}
	else if( m_gravity & Gravity::BOTTOM )
	{
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginLeftTopCorner();
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftBottomCorner(), point );
	}
	else
	{
		CCAssert( false, "No such gravity!" );
	}

	return true;
}

bool LinearLayout::applyHorizontalPosition( View* pPreviousView,
		View* pView )
{
	CCPoint point( CCPointZero );

	if( ( m_gravity & Gravity::CENTER_HORIZONTAL ) == Gravity::CENTER_HORIZONTAL )
	{
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginRightTopCorner();
		}
		else
		{
			//It will be called only once.
			float width = onMeasure( KK_WIDTH, false ).width;
			point.x = ( Positioner::getScaledWidth( this ) - width ) * 0.5F;
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(), point );
	}
	else if( m_gravity & Gravity::LEFT )
	{
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginRightTopCorner();
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginLeftTopCorner(), point );
	}
	else if( m_gravity & Gravity::RIGHT )
	{
		if( pPreviousView != nullptr )
		{
			point = pPreviousView->getMarginLeftTopCorner();
		}
		else
		{
			point.x = Positioner::getScaledWidth( this );
		}

		Positioner::setCornerToPosition( pView->getNode(),
										 pView->getMarginRightTopCorner(), point );
	}
	else
	{
		CCAssert( false, "No such gravity!" );
	}

	return true;
}

bool LinearLayout::applyVerticalGravity( View* pView )
{
	//We can safely cast it because when we addView we create LinearLayoutParams
	CCAssert( dynamic_cast<LinearLayoutParams*>( pView->getLayoutParams() )
			  , "Problem while casting ;/" );
	LinearLayoutParams* pLinearLayoutParams = ( LinearLayoutParams* )( pView
			->getLayoutParams() );

	//Center horizontal is a bitmask
	int gravity = pLinearLayoutParams->getGravity()
				  & Gravity::CENTER_HORIZONTAL;

	//It means that there isn't set any flag
	if( gravity == 0 )
	{
		gravity = m_gravity & Gravity::CENTER_HORIZONTAL;
	}

	CCNode* pNode = pView->getNode();

	switch( gravity )
	{
		case Gravity::LEFT:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pView->getMarginLeftBottomCorner().x, pNode->getPositionY() ),
											 CCPoint( 0, pNode->getPositionY() ) );
		}
		break;

		case Gravity::RIGHT:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pView->getMarginRightBottomCorner().x, pNode->getPositionY() ),
											 CCPoint( Positioner::getScaledWidth( this ), pNode->getPositionY() ) );
		}
		break;

		case Gravity::CENTER_HORIZONTAL:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pView->getMarginCenterPoint().x, pNode->getPositionY() ),
											 CCPoint( Positioner::getScaledWidth( this ) * 0.5F,
													 pNode->getPositionY() ) );
		}
		break;

		default:
		{
			CCAssert( false, "Such gravity is't supported in this orientation" );
			return false;
		}
	}

	return true;
}

bool LinearLayout::applyHorizontalGravity( View* pView )
{
	//We can safely cast it because when we addView we create LinearLayoutParams
	CCAssert( dynamic_cast<LinearLayoutParams*>( pView->getLayoutParams() )
			  , "Problem while casting ;/" );
	LinearLayoutParams* pLinearLayoutParams = ( LinearLayoutParams* )( pView
			->getLayoutParams() );

	//Center vertical is a bitmask
	int gravity = pLinearLayoutParams->getGravity() & Gravity::CENTER_VERTICAL;

	//It means that there isn't set any flag
	if( gravity == 0 )
	{
		gravity = m_gravity & Gravity::CENTER_VERTICAL;
	}

	CCNode* pNode = pView->getNode();

	switch( gravity )
	{
		case Gravity::TOP:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pNode->getPositionX(), pView->getMarginLeftTopCorner().y ),
											 CCPoint( pNode->getPositionX(), Positioner::getScaledHeight( this ) ) );
		}
		break;

		case Gravity::BOTTOM:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pNode->getPositionX(), pView->getMarginLeftBottomCorner().y ),
											 CCPoint( pNode->getPositionX(), 0 ) );
		}
		break;

		case Gravity::CENTER_VERTICAL:
		{
			Positioner::setCornerToPosition( pNode,
											 ccp( pNode->getPositionX(), pView->getMarginCenterPoint().y ),
											 CCPoint( pNode->getPositionX(),
													 Positioner::getScaledHeight( this ) * 0.5F ) );
		}
		break;

		default:
		{
			CCAssert( false, "Such gravity is't supported in this orientation" );
			return false;
		}
	}

	return true;
}

View* LinearLayout::addView( CCNode* pNode )
{
	return addView( pNode, LayoutParamsWrapWrap );
}

View* LinearLayout::addView( CCNode* pNode,
							 const LinearLayoutParams& params )
{
	LinearLayoutParams* pParams = new LinearLayoutParams( params );
	return addViewWithParams( pNode, pParams );
}

View* LinearLayout::addView( CCNode* pNode,
							 const LayoutParams& params )
{
	LinearLayoutParams* pParams = new LinearLayoutParams( params );
	return addViewWithParams( pNode, pParams );
}

View* LinearLayout::addView( View* pView )
{
	return addViewWithParams( pView );
}

View* LinearLayout::getMaxWidthView()
{
	View* pViewProper = nullptr;
	//We must set it for thisvalue because sometimes view has values like -5 :(
	float maxProperSize = FLT_MIN_10_EXP;

	for( View* pView : getViews() )
	{
		if( pView->getFullSize().width > maxProperSize )
		{
			pViewProper = pView;
			maxProperSize = pView->getFullSize().width;
		}
	}

	CCAssert( pViewProper != nullptr, "Layout should contain at least one child" );
	return pViewProper;
}

View* LinearLayout::getMaxHeightView()
{
	View* pViewProper = nullptr;
	float maxProperSize = -1;

	for( View* pView : getViews() )
	{
		if( pView->getFullSize().height > maxProperSize )
		{
			pViewProper = pView;
			maxProperSize = pView->getFullSize().height;
		}
	}

	CCAssert( pViewProper != NULL, "Layout should contain at least one child" );
	return pViewProper;
}

bool LinearLayout::insertView( unsigned index, CCNode* pNode,
							   const LinearLayoutParams& params )
{
	LinearLayoutParams* pParams = new LinearLayoutParams( params );
	return insertViewParams( index, pNode, pParams );
}

CCSize LinearLayout::onMeasure( float CCSize::* pValue,
								const bool withBackground )
{
	if( getViews().size() < 1 )
	{
		//No children no measure
		return CCSizeZero;
	}

	CCSize size; //zero size
	View* pView = nullptr;
	vector<View*>& views = getViews();

	if( m_orientation == Orientation::HORIZONTAL )
	{
		pView = getMaxHeightView();
		size.height = pView->getFullSize().height;

		if( pValue == KK_WIDTH )
		{
			for( unsigned i = 0; i < views.size(); ++i )
			{
				pView = views[i];
				size.width += pView->getFullSize().width;
			}
		}

		CCAssert( pView != nullptr, "No children?" );
	}
	else if( m_orientation == Orientation::VERTICAL )
	{
		pView = getMaxWidthView();
		size.width = pView->getFullSize().width;

		if( pValue == KK_HEIGHT )
		{
			for( unsigned i = 0; i < views.size(); ++i )
			{
				pView = views[i];
				size.height += pView->getFullSize().height;
			}
		}

		CCAssert( pView != nullptr, "No children?" );
	}

	if( withBackground )
	{
		measureBackground( size );
	}

	return size;
}

CCSize LinearLayout::getLeftSpaceForView( CCNode* pNode,
		const LayoutParams& params )
{
	CCSize size = getContentSize();
	size.width -= ( params.getMarginLeft().getValue()
					+ params.getMarginRight().getValue() );
	size.height -= ( params.getMarginTop().getValue()
					 + params.getMarginBottom().getValue() );
	//We know that previous views was measured so we know how much space we have.
	//Also we know that if we are calling this method no parent/grandparent...
	//doesn't have wrap content

	if( getBackground() != nullptr && pNode == getBackground()->getNode() )
	{
		return size;
	}

	vector<View*>::iterator iterator = getViewIterator( pNode );
	assert( iterator != getViews().end() );

	//If this is first view we quit :)
	if( iterator == getViews().begin() )
	{
		return size;
	}

	if( m_orientation == Orientation::HORIZONTAL )
	{
		while( iterator != getViews().begin() )
		{
			--iterator;
			size.width -= ( *iterator )->getFullSize().width;
		}
	}
	else if( m_orientation == Orientation::VERTICAL )
	{
		while( iterator != getViews().begin() )
		{
			--iterator;
			size.height -= ( *iterator )->getFullSize().height;
		}
	}

	return size;
}

bool LinearLayout::onLayout()
{
	updateViewsOrder();

	if( AbstractLayout::onLayout() == false )
	{
		return false;
	}

	vector<View*>::iterator iterator = getViews().begin();

	while( iterator != getViews().end() )
	{
		View* pView = *iterator;

		updateView( pView );

		++iterator;
	}

	return true;
}

void LinearLayout::updateViewsOrder()
{

	if( ( m_orientation == HORIZONTAL
			&& ( m_gravity & Gravity::CENTER_HORIZONTAL ) == Gravity::RIGHT )
			|| ( m_orientation == VERTICAL
				 && ( m_gravity & Gravity::CENTER_VERTICAL ) == Gravity::BOTTOM ) )
	{
		if( m_isReversed == false )
		{
			std::reverse( getViews().begin(), getViews().end() );
			m_isReversed = true;
		}
	}
	else
	{
		if( m_isReversed == true )
		{
			std::reverse( getViews().begin(), getViews().end() );
			m_isReversed = false;
		}
	}
}

void LinearLayout::setGravity( const unsigned short gravity )
{
	m_gravity = gravity;

	if( ( m_gravity & Gravity::CENTER_HORIZONTAL ) == 0 )
	{
		m_gravity |= Gravity::LEFT;
	}

	if( ( m_gravity & Gravity::CENTER_VERTICAL ) == 0 )
	{
		m_gravity |= Gravity::TOP;
	}
}

bool LinearLayout::measureSelf()
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

}


#include "AbstractLayout.h"
#include "utils/TouchManager.h"
#include <algorithm>

using namespace KoalaLib;
namespace KoalaGui
{

AbstractLayout::AbstractLayout( int tag ) :
	m_pBackground( nullptr ),
	m_isTouchEnabled( true ),
	m_pClickCallback( nullptr ),
	m_pTouchCallback( nullptr ),
	m_pTouchReleaseInsideCallback( nullptr )
{
	CCAssert( tag < 0, "Tag shoudl be < 0" );
	CCLayerColor::setTag( tag );
}

AbstractLayout::~AbstractLayout()
{
	KK_RELEASE_VECTOR( m_views );

	m_views.clear();

	CC_SAFE_RELEASE_NULL( m_pBackground );
	CC_SAFE_RELEASE_NULL( m_pClickCallback );
	CC_SAFE_RELEASE_NULL( m_pTouchCallback );
	CC_SAFE_RELEASE_NULL( m_pTouchReleaseInsideCallback );
}

bool AbstractLayout::init()
{
	//Default white color
	if( CCLayerColor::initWithColor( ccc4( 255, 255, 255, 255 ) ) == false )
	{
		CCAssert( false, "Fail to create AbstractLayout" );
		return false;
	}

	//For now commentet out did it work now?
	//setAnchorPoint ( CCPoint ( 0, 0 ) );
	setPosition( CCPoint( 0, 0 ) );
	setTouchMode( kCCTouchesOneByOne );
	return true;
}

bool AbstractLayout::onLayout()
{

	for( View* pViewChild : m_views )
	{
		//We must find containers in our views
		if( isLayout( pViewChild->getNode() ) )
		{
			CCAssert( dynamic_cast<AbstractLayout*>( pViewChild->getNode() ) != nullptr,
					  "Fuckup with tags?" );
			//Its container of views, inherited from AbstractLayout
			//Its not safe do something else?
			AbstractLayout* pLayout = dynamic_cast<AbstractLayout*>
									  ( pViewChild->getNode() );
			CCAssert( pLayout, "Fuckup withs tags?" );

			if( pLayout == nullptr )
			{
				return false;
			}

			pLayout->onLayout();
		}
	}

	return true;
}

bool AbstractLayout::updateView( View* pView )
{
	CCAssert( pView != nullptr, "Fail" );
	CCAssert( pView->getNode() != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCAssert( pView->getNode()->getParent() == this, "Should return this object" );
	CCAssert( pView->getNode()->getTag() != kCCNodeTagInvalid,
			  "Tag should be set!" );
	CCAssert( isChildView( pView->getNode() ),
			  "CCNode* pNode is not a Layout child!" );

	return true;
}

bool AbstractLayout::removeViewFromLayout( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "Fail" );
	vector<View*>::iterator view = getViewIterator( pNode );

	if( view != m_views.end() )
	{
		//View remove CCNode from parent with cleanup
		CC_SAFE_RELEASE( *view );
		m_views.erase( view );
		return true;
	}

	return false;
}

bool AbstractLayout::removeViewFromLayout( int tag )
{
	return removeViewFromLayout( getChildByTag( tag ) );
}

vector<View*>::iterator AbstractLayout::getViewIterator( const CCNode* pNode )
{
	vector<View*>::iterator it;

	for( it = m_views.begin(); it != m_views.end(); ++it )
	{
		if( ( *it )->getNode() == pNode )
		{
			return it;
		}
	}

	return it;
}

View* AbstractLayout::getViewByTag( int tag ) const
{
	if( tag == kViewTagUnused )
	{
		CCAssert( false, "You can't access unused View by this tag!" );
		return nullptr;
	}

	if( tag == kViewTagUnusedLayout )
	{
		return nullptr;
	}

	for( View* pView : m_views )
	{
		if( pView->getNode()->getTag() == tag )
		{
			return pView;
		}
	}

	return nullptr;
}

void AbstractLayout::addChild( CCNode* child, int zOrder, int tag )
{
	CCAssert( false, "You must add child with layout parameters!" );
}

bool AbstractLayout::isTagUnique( int tag )
{
	if( tag == kViewTagUnused || tag == kViewTagUnusedLayout )
	{
		return true;
	}

	for( View* pView : m_views )
	{
		if( pView->getNode()->getTag() == tag )
		{
			return false;
		}
	}

	return true;
}

void AbstractLayout::setTag( int nTag )
{
	CCAssert( false, "Can't use setTag!" );
}

vector<View*>::iterator AbstractLayout::getViewIterator( const View* pView )
{
	return getViewIterator( pView->getNode() );
}

void AbstractLayout::setBackground( View* pBackground )
{
	if( m_pBackground != nullptr )
	{
		m_pBackground->getNode()->removeFromParentAndCleanup( true );
	}

	CC_SAFE_RELEASE_NULL( m_pBackground );

	if( pBackground == nullptr )
	{
		//Maybe someone would like to remove background
		return;
	}

	m_pBackground = pBackground;
	m_pBackground->retain();
	//TODO add ability for setting position of background for now default left bottom corner
	m_pBackground->getNode()->setAnchorPoint( CCPointZero );
	CCLayerColor::addChild( m_pBackground->getNode(), 0,
							m_pBackground->getNode()->getTag() );
}

View* AbstractLayout::getBackground() const
{
	return m_pBackground;
}


void AbstractLayout::removeAllChildren()
{
	removeAllChildrenWithCleanup( true );
}

void AbstractLayout::removeAllChildrenWithCleanup( bool cleanup )
{
	KK_RELEASE_VECTOR( m_views );
	CC_SAFE_RELEASE_NULL( m_pBackground );

	CCLayerColor::removeAllChildrenWithCleanup( cleanup );
}

bool AbstractLayout::isChildView( const CCNode* pNode )
{
	return getViewIterator( pNode ) != m_views.end();
}

bool AbstractLayout::applySizeRule( CCNode* pNode, const LayoutParams& params,
									float CCSize::* pValue )
{
	if( params.getVisibility() == Visibility::GONE )
	{
		return true;
	}

	CCAssert( pNode != nullptr, "Can't be nullptr" );
	CCSize size = params.getSize();
	CCSize orginalSize = pNode->getContentSize();

	if( size.*pValue == Const::MATCH_PARENT )
	{
		orginalSize.*pValue = getLeftSpaceForView( pNode, params ).*pValue;
	}
	else if( size.*pValue == Const::WRAP_CONTENT )
	{
		//We can't set here width on scaled width.
		//Later when we use getScaledSize() we will again scale this node
		//orginalSize.*pValue = pNode->getContentSize().*pValue;
		//we do nothing
	}
	else if( params.getSizePolicy( pValue ).getType() == Policy::PP )
	{
		//Here parent should be measured
		CCNode* pParentNode = pNode->getParent();
		assert( pParentNode );

		CCSize parentSize = pParentNode->getContentSize();
		orginalSize.*pValue = parentSize.*pValue * size.*pValue;
	}
	else
	{
		orginalSize.*pValue = size.*pValue;
	}

	pNode->setContentSize( orginalSize );
	return true;
}

bool AbstractLayout::applySizeRuleForLayout( float CCSize::*pValue,
		AbstractLayout* pLayout, const LayoutParams& params )
{
	if( params.getVisibility() == Visibility::GONE )
	{
		return true;
	}

	CCAssert( pLayout != nullptr, "Can't be nullptr" );
	CCSize size = params.getSize();

	if( size.*pValue == Const::WRAP_CONTENT )
	{
		CCSize layoutAfterMeasure = pLayout->onMeasure( pValue, true );
		size.*pValue = layoutAfterMeasure.*pValue;
	}

	if( size.*pValue == Const::MATCH_PARENT )
	{
		CCSize maxSpace = getLeftSpaceForView( pLayout, params );
		size.*pValue = maxSpace.*pValue;
	}

	CCSize layoutSize = pLayout->getContentSize();
	layoutSize.*pValue = size.*pValue;
	pLayout->setContentSize( layoutSize );
	return true;
}

bool AbstractLayout::applyPaddingRule( CCNode* pNode,
									   const LayoutParams& params )
{
	//TODO implement padding rule EK-98 (for now stopped report if needed)
	assert( false );
	return false;
}

bool AbstractLayout::applyVisibilityRule( CCNode* pNode, LayoutParams& params )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );

	//No one update our settings
	if( params.getVisibility() == params.getLastVisibility() )
	{
		if( pNode->isVisible() )
		{
			//We do it double time because we want detect changes in lastVisibility
			params.setVisibility( LayoutParams::VISIBLE );
			params.setVisibility( LayoutParams::VISIBLE );
		}
		else if( params.getVisibility() == LayoutParams::VISIBLE )
		{
			//We do it double time because we want detect changes in lastVisibility
			params.setVisibility( LayoutParams::GONE );
			params.setVisibility( LayoutParams::GONE );
		}
	}

	switch( params.getVisibility() )
	{
		case LayoutParams::VISIBLE:
		{
			if( params.getPreviousNodeScale().equals( CCPointZero ) == false )
			{
				pNode->setScaleX( params.getPreviousNodeScale().x );
				pNode->setScaleY( params.getPreviousNodeScale().y );

				params.setPreviousNodeScale( CCPointZero );
			}

			pNode->setVisible( true );
		}
		break;

		case LayoutParams::INVISIBLE:
		{
			if( params.getPreviousNodeScale().equals( CCPointZero ) == false )
			{
				pNode->setScaleX( params.getPreviousNodeScale().x );
				pNode->setScaleY( params.getPreviousNodeScale().y );

				params.setPreviousNodeScale( CCPointZero );
			}

			pNode->setVisible( false );
		}
		break;

		case LayoutParams::GONE:
		{
			if( params.getPreviousNodeScale().equals( CCPointZero ) == false )
			{
				//it is already gone
				return true;
			}

			params.setPreviousNodeScale( CCPoint( pNode->getScaleX(),
												  pNode->getScaleY() ) );
			pNode->setScale( 0 );
			pNode->setVisible( false );
		}
		break;

		default:
			CCAssert( false, "No such visibility" );
			return false;
	}

	return true;
}

void AbstractLayout::prepareView( CCNode* pNodeView, LayoutParams* pParams )
{
	CCAssert( pNodeView != nullptr, "pNode cannot be nullptr!" );
	CCAssert( pParams != nullptr, "pParams can't be nullptr!" );
	CCAssert( isChildView( pNodeView ) == false, "You already add this view!" );

	//We must set tag to 0 because tag with value less than 0 we treat as AbstractLayout!
	if( pNodeView->getTag() == kCCNodeTagInvalid )
	{
		pNodeView->setTag( kViewTagUnused );
	}

	CCAssert(
		( pNodeView->getTag() < 0 ) ?
		dynamic_cast<AbstractLayout*>( pNodeView ) != nullptr : true,
		"pNodeView tag should be >= 0 if you are trying add AbstractLayout use other metohd for that :)" );
	//We ignore unused tags
	CCAssert( isTagUnique( pNodeView->getTag() ) == true,
			  "You must add view with other tag we already have that one!" );
	//For now commentet out did it work now?
	//pNodeView->setAnchorPoint ( CCPointZero );
}

View* AbstractLayout::addViewWithParams( CCNode* pNodeView,
		LayoutParams* pParams )
{
	assert( pNodeView != this );
	return addViewWithParams( View::create( pNodeView, pParams ) );
}

View* AbstractLayout::addViewWithParams( View* pView )
{
	assert( pView );
	prepareView( pView->getNode(), pView->getLayoutParams() );
	pView->retain();
	m_views.push_back( pView );

	//TODO Think about it children (not root) -- onMeasure problem in Dialog order of measure
	if( isLayout( pView->getNode() ) )
	{
		AbstractLayout* pLayout = dynamic_cast<AbstractLayout*>( pView->getNode() );
		assert( pLayout );
		pLayout->setContentSize( CCSize( 5, 5 ) );
	}

	//+1 because we have background on 0 level
	CCLayerColor::addChild( pView->getNode(), pView->getNode()->getZOrder() + 1,
							pView->getNode()->getTag() );
	return pView;
}

bool AbstractLayout::insertViewParams( unsigned index, CCNode* pNodeView,
									   LayoutParams* pParams )
{
	assert( index <= m_views.size() );
	prepareView( pNodeView, pParams );
	View* pView = View::create( pNodeView, pParams );
	pView->retain();
	m_views.insert( m_views.begin() + index, pView );
	CCLayerColor::addChild( pNodeView, pNodeView->getZOrder(),
							pNodeView->getTag() );
	return true;
}

void AbstractLayout::measureRecursive( float CCSize::*pValue,
									   bool isWrapContentSet/* = false*/ )
{
	for( View* pViewChild : m_views )
	{

		CCAssert(
			isWrapContentSet == false
			|| pViewChild->getLayoutParams()->getSizePolicy( pValue ).getValue()
			!= Const::MATCH_PARENT,
			"In wrap content match parent can't be set" );
		assert(
			isWrapContentSet == false
			|| pViewChild->getLayoutParams()->getSizePolicy( pValue ).getType()
			!= Policy::PP );

		applyVisibilityRule( pViewChild->getNode(), *pViewChild->getLayoutParams() );

		//We must find containers in our views
		if( isLayout( pViewChild->getNode() ) == false )
		{
			applySizeRule( pViewChild->getNode(), *pViewChild->getLayoutParams(), pValue );
			continue;
		}

		CCAssert( dynamic_cast<AbstractLayout*>( pViewChild->getNode() ) != nullptr,
				  "Fuckup with tags?" );

		//Its container of views, inherited from AbstractLayout
		AbstractLayout* pLayout = dynamic_cast<AbstractLayout*>
								  ( pViewChild->getNode() );
		CCAssert( pLayout, "Fuckup withs tags?" );

		CCSize sizeLayoutParams = pViewChild->getLayoutParams()->getSize();

		CCSize layoutSize = pLayout->getContentSize();

		if( sizeLayoutParams.*pValue < 0 )
		{
			assert(
				sizeLayoutParams.*pValue == Const::WRAP_CONTENT
				|| sizeLayoutParams.*pValue == Const::MATCH_PARENT );
			layoutSize.*pValue = 0;
			pLayout->setContentSize( layoutSize );
		}

		if( pViewChild->getLayoutParams()->getSizePolicy( pValue ).getValue()
				== Const::WRAP_CONTENT )
		{
			//Now we can only have wrap content or some const size
			pLayout->measureRecursive( pValue, true );

			applySizeRuleForLayout( pValue, pLayout, *pViewChild->getLayoutParams() );
		}
		else
		{
			//Now we have match parent or some const size
			applySizeRuleForLayout( pValue, pLayout, *pViewChild->getLayoutParams() );

			pLayout->measureRecursive( pValue, false );
		}

		View* pBackground = pLayout->getBackground();

		if( pBackground != nullptr )
		{
			pLayout->applySizeRuleForBackground( pValue, pBackground->getNode(),
												 *pBackground->getLayoutParams() );
		}

	}

}

bool AbstractLayout::updateStructure()
{
	LOG( "Update structure called" );
	measureRecursive( KK_WIDTH );
	measureRecursive( KK_HEIGHT );

	View* pBackground = getBackground();

	if( pBackground != nullptr )
	{
		applySizeRuleForBackground( KK_WIDTH, pBackground->getNode(),
									*pBackground->getLayoutParams() );
		applySizeRuleForBackground( KK_HEIGHT, pBackground->getNode(),
									*pBackground->getLayoutParams() );
	}

	return onLayout();
}

void AbstractLayout::measureBackground( CCSize& parentSize ) const
{
	View* pBackground = getBackground();

	if( pBackground != nullptr )
	{
		CCSize background = Positioner::getScaledSize( pBackground->getNode() );

		if( parentSize.width < background.width )
		{
			parentSize.width = background.width;
		}

		if( parentSize.height < background.height )
		{
			parentSize.height = background.height;
		}
	}
}

bool AbstractLayout::applySizeRuleForBackground( float CCSize::*pValue,
		CCNode* pNode,
		const LayoutParams& params )
{
	if( params.getVisibility() == Visibility::GONE )
	{
		return true;
	}

	CCAssert( pNode != nullptr, "Can't be nullptr" );
	CCSize size = params.getSize();

	if( size.*pValue == SizePolicy::Const::MATCH_PARENT )
	{
		size.*pValue = Positioner::getScaledSize( pNode->getParent() ).*pValue;
	}
	else if( size.*pValue == SizePolicy::Const::WRAP_CONTENT )
	{
		size.*pValue = Positioner::getScaledSize( pNode ).*pValue;
	}

	CCSize sizeNode = pNode->getContentSize();
	sizeNode.*pValue = size.*pValue;

	pNode->setContentSize( sizeNode );
	return true;
}

bool AbstractLayout::hasVisibleParents()
{
	CCNode* pParent = this->getParent();
	assert( pParent );	//No parents?

	while( pParent != nullptr )
	{
		if( pParent->isVisible() == false )
		{
			return false;
		}

		pParent = pParent->getParent();
	}

	return true;
}

bool AbstractLayout::ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent )
{
	if( m_isTouchEnabled == false )
	{
		return false;
	}

	bool result = TouchManager::detectTouchOnNode( this, pTouch );

	if( result == false )
	{
		return result;
	}

	LOG( "method: %s::%s is called.", __FILE__, __func__ );

	vector<View*> views = m_views;
	auto comparer = []( View * pA, View * pB )
	{
		assert( pA && pB );
		assert( pA->getNode() && pB->getNode() );
		return pA->getNode()->getZOrder() > pB->getNode()->getZOrder();
	};                                       //descending
	stable_sort( views.begin(), views.end(), comparer );

	for( View* pView : views )
	{
		assert( pView );
		bool isTouched = TouchManager::detectTouchOnNode( pView->getNode(), pTouch );
		pView->setTouched( isTouched );

		if( isTouched )
		{
			Utils::CallbackOld* pCallback = pView->getTouchCallback();

			if( pCallback && pCallback->pObject && pCallback->methodCCNode )
			{
				LOG( "View touched" );
				pCallback->callMethodCCNode( pView->getNode() );

				return true;
			}
		}
	}

	if( m_pTouchCallback && m_pTouchCallback->methodCCNode )
	{
		m_pTouchCallback->callMethodCCNode( this );
		return true;
	}

	return result;
}

void AbstractLayout::ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent )
{
	if( m_isTouchEnabled == false )
	{
		return;
	}

	if( TouchManager::detectClickGesture( this, pTouch ) )
	{
		for( View* pView : m_views )
		{
			assert( pView );

			Utils::CallbackOld* pCallback = pView->getClickCallback();

			if( pView->isTouched() && pView->getNode()
					&& TouchManager::detectTouchOnNode( pView->getNode(), pTouch )
					&& pCallback && pCallback->methodCCNode )
			{
				LOG( "View released" );
				pCallback->callMethodCCNode( pView->getNode() );
				pView->setTouched( false );
				return;
			}

			pView->setTouched( false );
		}

		if( m_pClickCallback && m_pClickCallback->methodCCNode )
		{
			LOG( "Layout released" );
			m_pClickCallback->callMethodCCNode( this );
			return;
		}

		return;
	}

	if( TouchManager::detectTouchOnNode( this, pTouch ) == false )
	{
		return;
	}

	for( View* pView : m_views )
	{
		assert( pView );
		Utils::CallbackOld* pCallback = pView->getTouchReleaseInsideCallback();

		const bool checkView = pView->isTouched() && pView->getNode();
		const bool checkTouch = checkView && pTouch
								&& TouchManager::detectTouchOnNode( pView->getNode(), pTouch );
		const bool checkCallback = checkTouch && pCallback && pCallback->methodCCNode
								   && pCallback->pObject;

		if( checkCallback )
		{
			LOG( "View touched and released inside" );
			pCallback->callMethodCCNode( pView->getNode() );
			pView->setTouched( false );
			return;
		}

		pView->setTouched( false );
	}

	if( m_pTouchReleaseInsideCallback &&
			m_pTouchReleaseInsideCallback->methodCCNode )
	{
		LOG( "Layout touched and released inside" );
		m_pTouchReleaseInsideCallback->callMethodCCNode( this );
	}
}

void AbstractLayout::setOnClickCallback( Utils::CallbackOld* pCallback )
{
	CC_SAFE_RELEASE( m_pClickCallback );
	m_pClickCallback = pCallback;

	if( m_pClickCallback )
	{
		assert( m_pClickCallback->methodCCNode );
	}

	CC_SAFE_RETAIN( m_pClickCallback );
}

void AbstractLayout::setOnTouchCallback( Utils::CallbackOld* pCallback )
{
	CC_SAFE_RELEASE( m_pTouchCallback );
	m_pTouchCallback = pCallback;

	if( m_pTouchCallback )
	{
		assert( m_pTouchCallback->methodCCNode );
	}

	CC_SAFE_RETAIN( m_pTouchCallback );
}

void AbstractLayout::setOnTouchReleaseInsideCallback( Utils::CallbackOld*
		pCallback )
{
	CC_SAFE_RELEASE( m_pTouchReleaseInsideCallback );
	m_pTouchReleaseInsideCallback = pCallback;

	if( m_pTouchReleaseInsideCallback )
	{
		assert( m_pTouchReleaseInsideCallback->methodCCNode );
	}

	CC_SAFE_RETAIN( m_pTouchReleaseInsideCallback );
}

void AbstractLayout::removeChild( CCNode* pChild, bool cleanup )
{
	CCLayerColor::removeChild( pChild, cleanup );

	if( !removeViewFromLayout( pChild ) )
	{
		assert( false );
	}
}

}

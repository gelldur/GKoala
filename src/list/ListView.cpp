/*
 * ListView.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: Dawid Drozd
 */

#include "view/list/ListView.h"
#include <algorithm>
#include "assets/FontManager.h"
#include "view/layout/relative/RelativeLayout.h"
#include "utils/TouchManager.h"

namespace KoalaGui
{

ListView::ListView ( const int tag ) :
	LinearLayout ( tag ),
	m_scale ( KoalaLib::TouchManager::calculateScale() ),
	m_pContent ( nullptr ),
	m_pAdapter ( nullptr ),
	m_cachedRowsMax ( 2 ),
	m_cachedRows ( 0 ),
	m_allRowsMaxSize ( 0 ),
	m_index ( 0 ),
	m_pAxis ( nullptr ),
	m_pValue ( nullptr ),
	m_factor ( 0 ),
	m_pOnRowClickCallback ( nullptr ),
	m_pOnRowClickObject ( nullptr ),
	m_pTocuhedRow ( nullptr ),
	m_scrollSpeed ( 0 ),
	m_isTouched ( false ),
	m_touchSampleCount ( 0 ),
	m_isReloadOnEnter ( false )
{
}

ListView::~ListView()
{
	CC_SAFE_RELEASE_NULL ( m_pAdapter );
	CC_SAFE_RELEASE_NULL ( m_pContent );
}

bool ListView::init ( const Orientation& orientation )
{

	m_pContent = ContentView::create ( orientation );
	m_pContent->retain();

	if ( Orientation::VERTICAL == orientation )
	{
		m_pAxis = &CCPoint::y;
		m_pValue = &CCSize::height;
		m_factor = 1;
	}
	else if ( Orientation::HORIZONTAL == orientation )
	{
		m_pAxis = &CCPoint::x;
		m_pValue = &CCSize::width;
		m_factor = -1;
	}
	else
	{
		assert ( false ); //WTF?
	}

	if ( LinearLayout::init ( Orientation::VERTICAL ) == false )
	{
		assert ( false );
		return false;
	}

	addView ( m_pContent, LayoutParamsMatchMatch );
	m_pContent->setOpacity ( 0 );

	setTouchEnabled ( true );
	setTouchPriority ( 0 );
	setTouchMode ( kCCTouchesOneByOne );

	setOpacity ( 0 );

	return true;
}

ListView* ListView::create ( const Orientation& orientation, int tag )
{
	ListView* pRet = new ListView ( tag );

	if ( pRet && pRet->init ( orientation ) )
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
		return nullptr;
	}
}

void ListView::setContentSize ( const CCSize& size )
{
	LinearLayout::setContentSize ( size );

	assert ( m_pContent );
	m_pContent->setContentSize ( size );
	m_isReloadOnEnter = true;
}

void ListView::visit()
{
	if ( isVisible() == false )
	{
		//If view is not visible it won't draw children
		return;
	}

	kmGLPushMatrix();
	beforeDraw ( getViewRect ( this ) );

	CCLayerColor::visit();

	afterDraw();
	kmGLPopMatrix();
}

void ListView::onExit()
{
	LinearLayout::onExit();
	unscheduleUpdate();

	if ( m_pContent )
	{
		m_pContent->stopAllActions();
	}
}

void ListView::setCachedRows ( unsigned count )
{
	CCAssert ( count >= 2, "Count must be >= 2 " );
	CCAssert ( ( count % 2 ) == 0, "Count must be % 2 == 0" );

	m_cachedRowsMax = count;
}

void ListView::setAdapter ( ListAdapter* pAdapter )
{
	assert ( pAdapter );

	CC_SAFE_RELEASE_NULL ( m_pAdapter );
	pAdapter->retain();
	m_pAdapter = pAdapter;
}

void ListView::update ( float delta )
{
	//Moving down V
	float tmp = m_pContent->getFirstRowsSize ( m_cachedRows ).*m_pValue * m_factor;

	// We must use while because if someone do really quick scroll, we will move
	// each cell on each frame. It doesn't look good.
	while ( ( m_pContent->getOrientation() == VERTICAL
			  && m_pContent->getPosition().*m_pAxis > tmp )
			|| ( m_pContent->getOrientation() == HORIZONTAL
				 && m_pContent->getPosition().*m_pAxis < tmp ) )
	{
		if ( m_index + m_pContent->getViewsCount() >= m_pAdapter->getItemsCount() )
		{
			if ( m_pContent->getPosition().*m_pAxis * m_factor
					+ m_pContent->getContentSize().*m_pValue
					> m_pContent->getFirstRowsSize ( m_pContent->getViewsCount() ).*m_pValue )
			{
				LOG ( "End of list" );
				handleEndOfList();
			}

			m_scrollSpeed = 0;
			return;
		}

		CCPoint point = m_pContent->getPosition();
		point.*m_pAxis = m_pContent->getPosition().*m_pAxis
						 - m_pContent->getFirstRowsSize ( 1 ).*m_pValue * m_factor;
		m_movingTo.*m_pAxis -= m_pContent->getFirstRowsSize ( 1 ).*m_pValue * m_factor;
		m_pContent->stopAllActions();
		setOffset ( point.*m_pAxis );

		if ( m_isTouched == false )
		{
			m_pContent->runAction ( getMovingAction ( m_movingTo ) );
		}

		m_pAdapter->initializeHolder ( m_index + m_pContent->getViewsCount(),
									   m_pContent->getView ( 0 )->getNode()->getUserObject() );

		//This method calls update structure for cell
		m_pContent->moveRowToBottom();
		++m_index;

		getMoreCelss();
	}

	///////////////////////////////////////////////////////////////////////////

	//Moving up ^
	// We must use while because if someone do really quick scroll, we will move
	// each cell on each frame. It doesn't look good.
	while ( ( m_pContent->getOrientation() == VERTICAL
			  && m_pContent->getPosition().*m_pAxis < 0 )
			|| ( m_pContent->getOrientation() == HORIZONTAL
				 && m_pContent->getPosition().*m_pAxis > 0 ) )
	{
		if ( m_index <= 0 )
		{
			LOG ( "Begin of list" )
			handleBeginOfList();
			return;
		}

		assert ( m_index > 0 );
		assert ( m_pContent->getViewsCount() > 0 );
		--m_index;
		m_pAdapter->initializeHolder ( m_index,
									   m_pContent->getView ( m_pContent->getViewsCount() -
											   1 )->getNode()->getUserObject() );

		//This method calls update structure for cell
		m_pContent->moveRowToTop();

		CCPoint point = m_pContent->getPosition();
		point.*m_pAxis = m_pContent->getPosition().*m_pAxis
						 + m_pContent->getFirstRowsSize ( 1 ).*m_pValue * m_factor;

		m_movingTo.*m_pAxis += m_pContent->getFirstRowsSize ( 1 ).*m_pValue * m_factor;
		m_pContent->stopAllActions();
		setOffset ( point.*m_pAxis );

		if ( m_isTouched == false )
		{
			m_pContent->runAction ( getMovingAction ( m_movingTo ) );
		}

		getMoreCelss();
	}
}

void ListView::handleEndOfList()
{
	if ( m_isTouched == false && m_pContent->numberOfRunningActions() == 0 )
	{
		CCPoint point = m_pContent->getPosition();

		point.*m_pAxis = m_pContent->getFirstRowsSize ( m_pContent->getViewsCount() )
						 .*m_pValue * m_factor - m_pContent->getContentSize().*m_pValue * m_factor;

		CCFiniteTimeAction* pAction = CCEaseSineOut::create ( CCMoveTo::create ( 0.15F,
									  m_movingTo = point ) );
		pAction->setTag ( getActionBounceTag() );
		m_pContent->runAction ( pAction );
	}
	else
	{
		CCAction* pAction = m_pContent->getActionByTag ( getActionScrollTag() );

		if ( pAction != nullptr )
		{
			m_pContent->stopAllActions();
			setSpeedAfterAction();
		}
	}
}

void ListView::handleBeginOfList()
{
	if ( m_isTouched == false && m_pContent->numberOfRunningActions() == 0 )
	{
		CCPoint point = m_pContent->getPosition();
		point.*m_pAxis = 0;

		CCFiniteTimeAction* pAction = CCEaseSineOut::create ( CCMoveTo::create ( 0.15F,
									  m_movingTo = point ) );
		pAction->setTag ( getActionBounceTag() );
		m_pContent->runAction ( pAction );

	}
	else
	{
		CCAction* pAction = m_pContent->getActionByTag ( getActionScrollTag() );

		if ( pAction != nullptr )
		{
			m_pContent->stopAllActions();
			setSpeedAfterAction();
		}
	}

}


CCAction* ListView::getMovingAction ( const CCPoint& moveTo,
									  float duration /* = 0.3f */ )
{
	CCAction* pAction = CCSequence::createWithTwoActions (
							CCEaseSineOut::create ( CCMoveBy::create
									( duration, moveTo ) ),
							CCCallFunc::create ( this,
									callfunc_selector ( ListView::setSpeedAfterAction ) ) );
	pAction->setTag ( getActionScrollTag() );

	return pAction;
}

void ListView::getMoreCelss()
{
	unsigned iterations = 0;

	while ( m_allRowsMaxSize
			> m_pContent->getFirstRowsSize ( m_pContent->getViewsCount() ).*m_pValue )
	{
		LOG ( "I need new row!" );

		if ( m_index + m_pContent->getViewsCount() + iterations
				>= m_pAdapter->getItemsCount() )
		{
			LOG ( "NO MORE!" );
			break;
		}

		View* pView = m_pAdapter->getRow (
						  m_index + m_pContent->getViewsCount() + iterations, nullptr );
		m_pContent->addView ( pView );

		m_pContent->updateStructure();
		++iterations;
	}
}

void ListView::reloadData()
{
	if ( m_pAdapter == nullptr )
	{
		CCLog ( "No Adapter was added!" );
		return;
	}

	LOG ( "Reload DATA!" );
	m_pContent->removeAllChildren();

	CCSize size = Positioner::getScaledSize ( m_pContent );

	unsigned i = 0;

	if ( m_index >= m_pAdapter->getItemsCount() )
	{
		m_index = 0;
	}

	for ( i = m_index; i < m_pAdapter->getItemsCount(); ++i )
	{
		View* pView = m_pAdapter->getRow ( i, nullptr );
		m_pContent->addView ( pView );
		m_pContent->measureRecursive ( m_pValue );

		size.*m_pValue -= pView->getFullSize().*m_pValue;

		if ( size.*m_pValue <= 0 )
		{
			++i;
			LOG ( "I have all needed rows %u", i );
			break;
		}
	}

	if ( i >= m_pAdapter->getItemsCount() + m_index )
	{
		if ( m_pContent->getViewsCount() == 0 )
		{
			//Is empty
			m_pContent->addView ( getEmptyView(), LayoutParamsMatchMatch );
		}

		updateStructure();
		return;
	}

	m_cachedRows = m_cachedRowsMax;
	LOG ( "Items count %u", m_pAdapter->getItemsCount() );

	if ( size.*m_pValue <= 0 )
	{
		for ( unsigned j = 0; j < m_cachedRowsMax && ( j + i ) < m_pAdapter->getItemsCount(); ++j )
		{
			View* pView = m_pAdapter->getRow ( j + i, nullptr );
			m_pContent->addView ( pView );
			assert ( m_cachedRows > 0 );
			--m_cachedRows;
		}
	}

	//If we have less cached views than max it will be > 0
	if ( m_cachedRows == 0 )
	{
		m_cachedRows = m_cachedRowsMax;
	}

	updateStructure();

	m_allRowsMaxSize = m_pContent->getFirstRowsSize ( m_pContent->getViewsCount() )
					   .*m_pValue;
}

bool ListView::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( isVisible() == false || hasVisibleParents() == false )
	{
		return false;
	}

	m_touchBeginPoint = pTouch->getLocation();

	m_isTouched = KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch );

	if ( m_isTouched == false )
	{
		return false;
	}

	unsigned position = 0;
	LOG ( "speed: %f", m_scrollSpeed );

	assert ( m_pContent );

	if ( m_pOnRowClickCallback != nullptr  && m_scrollSpeed == 0
			&& m_pContent->getActionByTag ( getActionBounceTag() ) == nullptr )
	{
		m_pTocuhedRow = getCellForTouch ( pTouch, &position );
	}

	LOG ( "My position %f moving to %f", getPositionY(), m_movingTo.y );
	m_pContent->stopAllActions();
	m_scrollSpeed = 0;

	return true;
}

void ListView::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( KoalaLib::TouchManager::isClickGesture ( pTouch->getLocation(),
			m_touchBeginPoint ) == false )
	{
		m_pTocuhedRow = nullptr;
	}

	CCPoint point = m_pContent->getPosition();
	point.*m_pAxis += pTouch->getLocation().*m_pAxis -
					  pTouch->getPreviousLocation().*m_pAxis;
	m_movingTo = point;
	setOffset ( point.*m_pAxis );

	if ( m_scrollSpeed == 0 )
	{
		m_touchSampleCount = 0;
	}

	m_scrollSpeed += pTouch->getDelta().*m_pAxis;
	m_scrollSpeed /= ( float )++m_touchSampleCount;

	LOG ( "Moved %f  count %u", m_scrollSpeed, m_touchSampleCount );

}

void ListView::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_isTouched = false;

	if ( fabs ( m_scrollSpeed ) < 2 * m_scale && m_touchSampleCount < 4 )
	{
		m_scrollSpeed = 0;
	}

	m_scrollSpeed *= ( 115.F * m_scale );

	const float maxValue = 2500 * m_scale;

	if ( m_scrollSpeed > maxValue || m_scrollSpeed < -maxValue )
	{
		m_scrollSpeed = m_scrollSpeed > 0 ? maxValue : -maxValue;
	}

	const float valueForMove = 60 * m_scale;
	LOG ( "On End Speed %f       %f", m_scrollSpeed, valueForMove );

	if ( m_pContent->numberOfRunningActions() == 0 &&
			fabs ( m_scrollSpeed ) > valueForMove )
	{
		CCPoint point;
		point.*m_pAxis = m_scrollSpeed;

		CCAction* pAction = getMovingAction ( m_movingTo = point );

		m_pContent->runAction ( pAction );
	}
	else
	{
		setSpeedAfterAction();
	}

	if ( m_pTocuhedRow == nullptr )
	{
		//We canceled touched row
		return;
	}

	unsigned position = 0;
	CCNode* pNode = getCellForTouch ( pTouch, &position );

	if ( pNode != nullptr && m_pOnRowClickCallback != nullptr
			&& m_pOnRowClickObject != nullptr && pNode == m_pTocuhedRow )
	{
		m_pContent->stopActionByTag ( getActionScrollTag() );
		( m_pOnRowClickObject->*m_pOnRowClickCallback ) ( position, m_pTocuhedRow );
		m_pTocuhedRow = nullptr;
	}
}

CCNode* ListView::getCellForTouch ( CCTouch* pTouch, unsigned* const pPosition )
{
	CCRect rect;
	View* pView;

	for ( unsigned i = 0; i < m_pContent->getViewsCount(); ++i )
	{
		pView = m_pContent->getView ( i );

		if ( pView->getNode()->getTag() == EMPTY_VIEW )
		{
			continue;
		}

		rect.origin = m_pContent->convertToWorldSpace (
						  pView->getMarginLeftBottomCorner() );
		rect.size = pView->getFullSize();

		if ( rect.containsPoint ( pTouch->getLocation() ) )
		{
			CCNode* pNode = pView->getNode();
			assert ( pNode );
			*pPosition = i + m_index;
			return pNode;
		}
	}

	return nullptr;
}

void ListView::ccTouchCancelled ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_pTocuhedRow = nullptr;
	m_isTouched = false;
}

void ListView::setColor ( const ccColor3B& color )
{
	m_pContent->setColor ( color );
	LinearLayout::setColor ( color );
	LOG ( "Setting the listViewColor" );
}

void ListView::setOpacity ( GLubyte opacity )
{
	m_pContent->setOpacity ( opacity );
	LinearLayout::setOpacity ( opacity );
	LOG ( "Setting the listView opacity" );
}

void ListView::setOffset ( float offset )
{
	CCPoint point = m_pContent->getPosition();
	point.*m_pAxis = offset;
	m_pContent->setPosition ( point );
}

float ListView::getOffset()
{
	assert ( m_pContent );
	return m_pContent->getPosition().*m_pAxis;
}

void ListView::setGravity ( const unsigned short gravity )
{
	m_pContent->setGravity ( gravity );
}

void ListView::onEnter()
{
	LinearLayout::onEnter();
	//We need it because list can be added to CCNode or removed.
	scheduleUpdate();

	if ( m_isReloadOnEnter )
	{
		reloadData();
		m_isReloadOnEnter = false;
	}
}

CCNode* ListView::getEmptyView()
{
	RelativeLayout* pLayout = RelativeLayout::create ( EMPTY_VIEW );

	CCLabelTTF* pLabel = FontManager::getInstance()->getStandardLabel (
							 tr ( "No items in list" ) );
	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule (
			AlignType::PARENT_CENTER_HORIZONTAL | AlignType::PARENT_CENTER_VERTICAL );
		pLayout->addView ( pLabel, params );
	}

	pLayout->setOpacity ( 0 );
	return pLayout;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ContentView::ContentView ( const int tag ) :
	LinearLayout ( tag )
{
}

ContentView* ContentView::create ( const Orientation& orientation, int tag )
{
	ContentView* pRet = new ContentView ( tag );

	if ( pRet && pRet->init ( orientation ) )
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
		return nullptr;
	}
}

bool ContentView::moveRowToTop()
{
	vector<View*>& views = getViews();
	std::rotate ( views.begin(), views.begin() + views.size() - 1, views.end() );

	updateCell ( views.front() );

	onLayout();
	return true;
}

bool ContentView::moveRowToBottom()
{
	vector<View*>& views = getViews();
	std::rotate ( views.begin(), views.begin() + 1, views.end() );

	updateCell ( views.back() );

	onLayout();
	return true;
}

unsigned ListView::getCachedRows() const
{
	return m_cachedRows;
}

unsigned ListView::getIndex() const
{
	return m_index;
}

} /* namespace KoalaGui */


/*
 * PageView.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#include "view/page/PageView.h"
#include "view/page/PageIndicator.h"
#include <cmath>
namespace KoalaGui
{

PageView::PageView( const int tag ) :
	ListView( tag ),
	m_pPageIndicator( nullptr ),
	m_isIgnoring( false ),
	m_checkOnce( true ),
	m_startMovePage( 0 ),
	m_currentPage( 0 )
{
}

PageView::~PageView()
{
	m_pPageIndicator = nullptr;
}

bool PageView::init( const Orientation& orientation )
{
	if( ListView::init( Orientation::HORIZONTAL ) == false )
	{
		assert( false );
		return false;
	}

	setTouchPriority( INT_MIN );

	return true;
}

PageView* PageView::create( const Orientation& orientation, int tag )
{
	PageView* pRet = new PageView( tag );

	if( pRet && pRet->init( orientation ) )
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE( pRet );
		return nullptr;
	}
}

void PageView::update( float delta )
{
	ListView::update( delta );
}

void PageView::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher =
		CCDirector::sharedDirector()->getTouchDispatcher();

	if( getTouchMode() == kCCTouchesAllAtOnce )
	{
		pDispatcher->addStandardDelegate( this, 0 );
	}
	else
	{
		pDispatcher->addTargetedDelegate( this, getTouchPriority(), false );
	}
}

bool PageView::ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent )
{
	bool isTouched = ListView::ccTouchBegan( pTouch, pEvent );

	if( !isTouched )
	{
		return false;
	}

	m_isIgnoring = false;
	m_checkOnce = true;

	m_startMovePage = pTouch->getLocation().x;

	return true;
}

void PageView::ccTouchMoved( CCTouch* pTouch, CCEvent* pEvent )
{
	if( m_isIgnoring )
	{
		return;
	}

	if( m_checkOnce )
	{
		m_checkOnce = false;
		float xDifference = abs(
								( float ) pTouch->getPreviousLocation().x - pTouch->getLocation().x );
		float yDifference = abs(
								pTouch->getPreviousLocation().y - pTouch->getLocation().y );

		if( xDifference < yDifference || xDifference < 1.5F * getScaleForValues() )
		{
			m_isIgnoring = true;
			ccTouchEnded( pTouch, pEvent );
			return;
		}
	}

	ListView::ccTouchMoved( pTouch, pEvent );

	const float previous = getCurrentPage();
	setCurrentPage( calculatePage() );
	LOG( "Current Page changed: %u", m_currentPage );

	if( previous != m_currentPage )
	{
		LOG( "Current page %u", m_currentPage );

		if( m_pPageIndicator != nullptr )
		{
			m_pPageIndicator->setCurrentPage( getCurrentPage() );
		}
	}

	//LOG ( "Position on move end %f", getContentView()->getPositionX() );

	//We need do it twice :)
	pTouch->setTouchInfo( pTouch->getID(), pTouch->getLocation().x, 0 );
	pTouch->setTouchInfo( pTouch->getID(), pTouch->getLocation().x, 0 );
}

void PageView::ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent )
{
	setTouched( false );

	if( m_isIgnoring == true )
	{
		return;
	}

	m_startMovePage = pTouch->getLocationInView().x - m_startMovePage;
	float offset = getContentView()->getPositionX()
				   / getContentView()->getFirstRowsSize( 1 ).width;

	if( offset > 0 )
	{
		offset = 0;
	}
	else if( -offset > ( int ) getCachedRows() + 1 )
	{
		offset = - ( int ) getCachedRows();
	}

	const float part = offset - ( int ) offset;

	//To Right
	if( m_startMovePage > 0 )
	{
		if( part > -0.8F )
		{
			offset = ( int ) offset;
		}
		else
		{
			offset = ( int ) offset - 1;
		}
	}

	//To left
	if( m_startMovePage < 0 )
	{
		if( part < -0.2F )
		{
			offset = ( int ) offset - 1;
		}
		else
		{
			offset = ( int ) offset;
		}
	}

	CCPoint point = getContentView()->getPosition();
	point.x = getContentView()->getFirstRowsSize( 1 ).width * offset;
	movePageTo( point );
}

void PageView::setPage( unsigned index )
{
	LOG( "method: %s::%s is called.", __FILE__, __func__ );
	assert( index < getItemsCount() );

	if( index == m_currentPage )
	{
		return;
	}

	//We are probably moving page now
	assert( getContentView() );

	if( getMovingTo().x != getContentView()->getPosition().x )
	{
		getContentView()->stopAllActions();
		getContentView()->setPosition( getMovingTo() );
		update( 0 );
	}

	//Assert for moving correctly pages
	assert( ( int ) getContentView()->getFirstRowsSize( 1 ).width );
	assert( ( ( int ) fabs( getContentView()->getPositionX() ) )
			% ( ( int ) getContentView()->getFirstRowsSize( 1 ).width ) == 0 );

	float offset = getContentView()->getPositionX()
				   / getContentView()->getFirstRowsSize( 1 ).width;

	if( index > m_currentPage )
	{
		offset -= 1;
	}

	else
	{
		offset += 1;
	}

	CCPoint point = getContentView()->getPosition();
	point.x = getContentView()->getFirstRowsSize( 1 ).width * offset;

	setCurrentPage( index );

	LOG( "Current page %u", m_currentPage );

	if( m_pPageIndicator != nullptr )
	{
		m_pPageIndicator->setCurrentPage( getCurrentPage() );
	}

	movePageTo( point );
}

void PageView::setPageIndicator( PageIndicatorController* pPageIndicator )
{
	m_pPageIndicator = pPageIndicator; //TODO retain?
	assert( m_pPageIndicator );
}

unsigned PageView::getCurrentPage()
{
	return m_currentPage;
}

string PageView::getCurrentPageTitle()
{
	const PageViewAdapter* pAdapter = dynamic_cast<const PageViewAdapter*>
									  ( getAdapter() );
	assert( pAdapter );
	return pAdapter->getPage( getCurrentPage() )->getTitle();
}

void PageView::onFinishPageMoveCallback()
{
	LOG( "onFinishPageMoveCallback" );

	const float previous = m_currentPage;
	setCurrentPage( calculatePage() );

	LOG( "Current Page changed: %u", m_currentPage );

	if( previous != m_currentPage )
	{
		LOG( "Current page %u", m_currentPage );

		if( m_pPageIndicator != nullptr )
		{
			m_pPageIndicator->setCurrentPage( getCurrentPage() );
		}
	}
}

unsigned PageView::calculatePage()
{
	float offset = ( -getContentView()->getPositionX()
					 / getContentView()->getFirstRowsSize( 1 ).width );

	if( offset < 0 )
	{
		offset = 0;
	}
	else if( offset > ( int ) getCachedRows() + 1 )
	{
		offset = - ( int ) getCachedRows();
	}

	return getIndex() + ( unsigned ) offset;
}

void PageView::movePageTo( const CCPoint& point )
{
	LOG( "method: %s::%s is called.", __FILE__, __func__ );
	//We want to wait for update, at least once call.
	CCAction* pAction = getMovingAction( point );

	getContentView()->stopAllActions();
	getContentView()->runAction( pAction );
	setMovingTo( point );
}

void PageView::setAdapter( ListAdapter* pAdapter )
{
	assert( pAdapter->getItemsCount() > 0 );
	ListView::setAdapter( pAdapter );
	setPage( 0 );
}

CCAction* PageView::getMovingAction( const CCPoint& moveTo,
									 float duration )
{
	CCAction* pAction = CCSequence::createWithTwoActions(
							CCEaseSineOut::create( CCMoveTo::create( 0.15F, moveTo ) ),
							CCCallFunc::create( this,
									callfunc_selector( PageView::onFinishPageMoveCallback ) ) );

	pAction->setTag( getActionScrollTag() );
	return pAction;
}

}
/* namespace KoalaGui */

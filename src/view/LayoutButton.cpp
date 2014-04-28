/*
 * LayoutButton.cpp
 *
 *  Created on: September 17, 2013
 *      Author: Jakub Kuderski
 */

#include "view/LayoutButton.h"
#include "utils/TouchManager.h"

namespace KoalaGui
{

LayoutButton::LayoutButton ( int tag ) :
	LinearLayout ( tag ),
	m_pTouchable ( nullptr ),
	m_pTextProtocol ( nullptr )
{
}

LayoutButton::~LayoutButton()
{
}

bool LayoutButton::initWithButtonAndText ( CCNode* pNode, CCNode* pText )
{
	if ( LinearLayout::init ( Orientation::VERTICAL ) == false )
	{
		return false;
	}

	setGravity ( Gravity::CENTER_HORIZONTAL );

	m_pTouchable = dynamic_cast<CCTouchDelegate*> ( pNode );
	assert ( m_pTouchable ); //Node should be touchable
	addView ( pNode );

	if ( pText != nullptr )
	{
		LinearLayoutParams params ( LayoutParamsWrapWrap );
		params.setMarginTop ( SizePolicy ( Policy::SP, 8 ) );
		m_pTextProtocol = dynamic_cast<CCLabelProtocol*> ( pText );
		assert ( m_pTextProtocol );
		addView ( pText, params );
	}

	setTouchMode ( kCCTouchesOneByOne );
	setTouchEnabled ( true );

	return true;
}

bool LayoutButton::initWithButtonAndText ( const string& buttonFileName,
		const string& text )
{
	KKImageButton* pButton = KKImageButton::createWithBackground (
								 buttonFileName.c_str() );
	//TODO FIX
	CCLabelTTF* pLabel = nullptr; //FontManager::getInstance()->getStandardLabel ( text );

	if ( initWithButtonAndText ( pButton, pLabel ) == false )
	{
		return false;
	}

	return true;
}

bool LayoutButton::initWithButton ( const string& buttonFileName )
{
	KKImageButton* pButton = KKImageButton::createWithBackground (
								 buttonFileName.c_str() );

	if ( initWithButtonAndText ( pButton, nullptr ) == false )
	{
		return false;
	}

	return true;
}

bool LayoutButton::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	assert ( m_pTouchable );

	if ( KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch ) == false )
	{
		return false;
	}

	const CCPoint location = CCDirector::sharedDirector()->convertToGL (
								 convertToWorldSpace ( Positioner::getCenterPoint ( getButtonNode() ).origin ) );
	pTouch->setTouchInfo ( pTouch->getID(), location.x, location.y );

	m_pTouchable->ccTouchBegan ( pTouch, pEvent );
	return true;
}

void LayoutButton::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	assert ( m_pTouchable );

	if ( KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch ) )
	{
		const CCPoint location = CCDirector::sharedDirector()->convertToGL (
									 convertToWorldSpace ( Positioner::getCenterPoint ( getButtonNode() ).origin ) );
		pTouch->setTouchInfo ( pTouch->getID(), location.x, location.y );
	}

	m_pTouchable->ccTouchEnded ( pTouch, pEvent );
}

void LayoutButton::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	assert ( m_pTouchable );

	if ( KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch ) )
	{
		const CCPoint location = CCDirector::sharedDirector()->convertToGL (
									 convertToWorldSpace ( Positioner::getCenterPoint ( getButtonNode() ).origin ) );
		pTouch->setTouchInfo ( pTouch->getID(), location.x, location.y );
	}

	m_pTouchable->ccTouchMoved ( pTouch, pEvent );
}

void LayoutButton::ccTouchCancelled ( CCTouch* pTouch, CCEvent* pEvent )
{
	assert ( m_pTouchable );

	if ( KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch ) )
	{
		const CCPoint location = CCDirector::sharedDirector()->convertToGL (
									 convertToWorldSpace ( Positioner::getCenterPoint ( getButtonNode() ).origin ) );
		pTouch->setTouchInfo ( pTouch->getID(), location.x, location.y );
	}

	m_pTouchable->ccTouchCancelled ( pTouch, pEvent );
}

} /* namespace KoalaGui */

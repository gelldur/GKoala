/*
 * KKImageButton.cpp
 *
 *  Created on: May 10, 2013
 *      Author: Dawid Drozd
 */

#include "view/KKImageButton.h"

namespace KoalaGui
{

KKImageButton::KKImageButton()
{
}

KKImageButton::~KKImageButton()
{
}

bool KoalaGui::KKImageButton::initWithLabelAndBackground ( const string& text,
		const char* pSpriteName, bool is9Patch /*=true*/ )
{
	CCNode* pSprite;

	if ( is9Patch )
	{
		pSprite = CCScale9Sprite::create ( pSpriteName );
	}
	else
	{
		pSprite = CCSprite::create ( pSpriteName );
	}

	CCLabelTTF* pLabel =  CCLabelTTF::create ( "", "Arial", 18 );

	return initWithLabelAndBackgroundSprite ( pLabel, pSprite );
}

bool KKImageButton::initWithLabelAndBackgroundSprite ( CCNode* pNodeLabel,
		CCNode* pNodeBackground )
{
	if ( CCControlButton::initWithLabelAndBackgroundSprite ( pNodeLabel,
			pNodeBackground )
			== false )
	{
		assert ( false );
		return false;
	}

	setZoomOnTouchDown ( false );
	setTouchPriority ( INT16_MIN );

	return true;
}

bool KKImageButton::initWithBackgroundSprite ( CCNode* pNodeBackground )
{
	return initWithLabelAndBackgroundSprite (
			   CCLabelTTF::create ( "", "Arial", 18 ) , pNodeBackground );
}

bool KKImageButton::initWithBackgroundSprite ( const char* pSpriteName,
		bool is9Patch )
{
	if ( initWithLabelAndBackground ( "", pSpriteName, is9Patch ) == false )
	{
		return false;
	}

	return true;
}

KKImageButton* KKImageButton::setTarget ( CCObject* pTarget,
		SEL_CCControlHandler callback,
		CCControlEvent controlEvent /*= CCControlEventTouchUpInside*/ )
{
	//Remove all events on controlEvent
	removeTargetWithActionForControlEvent ( nullptr, nullptr, controlEvent );
	addTargetWithActionForControlEvent ( pTarget, callback, controlEvent );
	return this;
}

void KKImageButton::setContentSize ( const CCSize& contentSize )
{
	CCControlButton::setContentSize ( contentSize );
	setPreferredSize ( contentSize );
}

void KKImageButton::setText ( const string& text )
{
	CCString* pString = CCString::create ( text );
	setTitleForState ( pString, CCControlStateSelected );
	setTitleForState ( pString, CCControlStateNormal );
	setTitleForState ( pString, CCControlStateHighlighted );
	setTitleForState ( pString, CCControlStateDisabled );
}

bool KKImageButton::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( CCControlButton::ccTouchBegan ( pTouch, pEvent ) )
	{
		CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*> ( getBackgroundSprite() );

		if ( pNode )
		{
			if ( ccc3BEqual ( pNode->getColor(), ccWHITE ) )
			{
				pNode->setColor ( ccc3 ( 0x8c, 0x8c, 0x8c ) );
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

void KKImageButton::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCControlButton::ccTouchEnded ( pTouch, pEvent );
	CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*> ( getBackgroundSprite() );

	if ( pNode )
	{
		if ( ccc3BEqual ( pNode->getColor(), ccc3 ( 0x8c, 0x8c, 0x8c ) ) )
		{
			pNode->setColor ( ccWHITE );
		}
	}
}

void KKImageButton::ccTouchCancelled ( CCTouch* pTouch, CCEvent* pEvent )
{
	ccTouchEnded ( pTouch, pEvent );
}

} /* namespace KoalaGui */


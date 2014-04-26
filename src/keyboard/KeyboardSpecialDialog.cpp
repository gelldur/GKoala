/*
 * KeyboardSpecialDialog.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: Filip Miklaś
 */

#include "KeyboardSpecialDialog.h"
#include "view/layout/linear/LinearLayout.h"
#include "view/keyboard/KeyboardView.h"
#include "dictionaries/language/Utf8.h"
#include "view/keyboard/KeyboardController.h"
#include "configuration/UserConfiguration.h"
#include "utils/TouchManager.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

namespace keyboard
{

KeyboardSpecialDialog::KeyboardSpecialDialog ( int tag ) :
	LinearLayout ( tag ),
	m_keypressedSoundName ( "" ),
	m_pSemiTransparentBackground ( nullptr ),
	m_wasSceneKeypadEnabled ( false ),
	m_isShown ( false )
{
}

KeyboardSpecialDialog::~KeyboardSpecialDialog()
{
	m_pSemiTransparentBackground->removeFromParentAndCleanup ( true );
	m_pSemiTransparentBackground->release();

	m_isShown = false;
}

bool KeyboardSpecialDialog::init ( const Alphabet& letters,
								   const string& buttonName,
								   const string& fontName, const string& soundName, const CCSize& buttonSize )
{
	if ( LinearLayout::init ( Orientation::VERTICAL ) == false )
	{
		return false;
	}

	m_pSemiTransparentBackground = CCLayerColor::create ( ccc4 ( 0x44, 0x44, 0x44,
								   0 ) );
	m_pSemiTransparentBackground->retain();

	//We want to be first
	setTouchEnabled ( true );
	setTouchPriority ( INT_MIN );
	setTouchMode ( kCCTouchesOneByOne );

	assert ( letters.getLetters().size() > 0u );
	assert ( buttonName.empty() == false );
	assert ( fontName.empty() == false );
	assert ( soundName.empty() == false );

	m_keypressedSoundName = soundName;

	setColor ( ccc3 ( 77, 64, 68 ) );
	setOpacity ( 255 );
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	winSize.width *= 0.9f;
	winSize.height = 80;
	setContentSize ( winSize );

	LinearLayout* pLayout = LinearLayout::create ( Orientation::VERTICAL, LAYOUT );
	{
		KoalaGui::LinearLayoutParams params ( KoalaGui::LayoutParamsMatchMatch );
		params.setGravity ( Gravity::CENTER_HORIZONTAL );
		addView ( pLayout, params );

		pLayout->setColor ( ccc3 ( 77, 64, 68 ) );
	}

	LinearLayout* pButtonLayout = LinearLayout::create ( Orientation::HORIZONTAL,
								  BUTTON_LAYOUT );
	{
		KoalaGui::LinearLayoutParams params ( KoalaGui::LayoutParamsWrapWrap );
		params.setGravity ( Gravity::CENTER_HORIZONTAL );
		params.setMarginTop ( SizePolicy ( Policy::SP, 8 ) );
		params.setMarginBottom ( SizePolicy ( Policy::SP, 4 ) );
		pLayout->addView ( pButtonLayout, params );
		pButtonLayout->setOpacity ( 0 );
	}

	vector<Letter> signs = letters.getLetters();

	for ( const Letter& letter : signs )
	{
		string sign = letter.getSign();
		CCControlButton* pButton = getButton ( sign, buttonName, fontName, buttonSize );
		KoalaGui::LinearLayoutParams params ( KoalaGui::LayoutParamsWrapWrap );
		params.setMarginLeft ( SizePolicy ( Policy::SP, 4 ) );
		params.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );
		pButtonLayout->addView ( pButton, params );

		CCString* pString = CCString::create ( sign );
		pButton->setUserObject ( pString );

		m_buttons.push_back ( pButton );
	}

	return true;
}

void KeyboardSpecialDialog::showAbove ( CCNode* pNode )
{
	if ( m_isShown || getParent() )
	{
		CCAssert ( false, "Already visible" );
		return;
	}

	m_isShown = true;
	assert ( pNode );
	KKScene* pScene = dynamic_cast<KKScene*>
					  ( CCDirector::sharedDirector()->getRunningScene() );
	assert ( pScene );

	pScene->addChild ( m_pSemiTransparentBackground,
					   INT8_MAX + pNode->getChildrenCount() );
	pScene->addChild ( this, m_pSemiTransparentBackground->getZOrder() );

	if ( pScene->isKeypadEnabled() )
	{
		m_wasSceneKeypadEnabled = true;
		pScene->setKeypadEnabled ( false );
	}

	updateStructure();

	CCPoint nodePoint = pNode->convertToWorldSpace ( CCPoint ( 0, 0 ) );

	CCPoint point = Positioner::getCenterPoint ( pScene ).origin;
	point.y = nodePoint.y + pNode->getContentSize().height * 1.8f;
	Positioner::setCornerToPosition ( this, Positioner::getCenterPoint ( this ).origin,
									  point );

	setScale ( 0 );
	runAction ( CCEaseSineIn::create ( CCScaleTo::create ( 0.1F, 1 ) ) );
	m_pSemiTransparentBackground->runAction ( CCFadeTo::create ( 0.1F, 160 ) );
}

void KeyboardSpecialDialog::dismiss ( float duration )
{
	if ( m_isShown == false )
	{
		return;
	}

	m_isShown = false;

	assert ( duration >= 0 );
	bool animation = duration != 0;

	KKScene* pScene = dynamic_cast<KKScene*> ( getParent() );
	assert ( pScene );

	if ( pScene == nullptr )
	{
		return;
	}

	if ( m_wasSceneKeypadEnabled )
	{
		m_wasSceneKeypadEnabled = false;
		pScene->setKeypadEnabled ( true );
	}

	if ( animation )
	{
		runAction (
			CCEaseSineOut::create (
				CCSequence::create ( CCScaleTo::create ( duration, 0 ),
									 CCRemoveSelf::create ( true ), static_cast<void*> ( nullptr ) ) ) );
		m_pSemiTransparentBackground->runAction ( CCFadeTo::create ( duration, 0 ) );
	}
	else
	{
		pScene->removeChild ( m_pSemiTransparentBackground );
		pScene->removeChild ( this );
	}
}

bool KeyboardSpecialDialog::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	for ( CCControlButton* pButton : m_buttons )
	{
		assert ( pButton->retainCount() > 0 );

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch ) )
		{
			//OK!
			SimpleAudioEngine::sharedEngine()->playEffect ( m_keypressedSoundName.c_str() );
			LOG ( "Hit!" );
			pButton->setHighlighted ( true );

			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 0.9f );
			pButton->runAction ( pAction );
		}
	}

	return true;
}

void KeyboardSpecialDialog::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	for ( CCControlButton* pButton : m_buttons )
	{
		assert ( pButton->retainCount() > 0 );
		bool isTouchMoveInside = false;

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch ) )
		{
			isTouchMoveInside = true;
		}

		if ( isTouchMoveInside )
		{
			if ( pButton->isHighlighted() )
			{
				pButton->sendActionsForControlEvents ( CCControlEventTouchDragInside );
			}
			else
			{
				pButton->setHighlighted ( true );
				CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 0.9f );
				pButton->runAction ( pAction );
				pButton->sendActionsForControlEvents ( CCControlEventTouchDragEnter );
			}
		}
		else
		{
			if ( pButton->isHighlighted() )
			{
				pButton->setHighlighted ( false );
				CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 1 );
				pButton->runAction ( pAction );

				pButton->sendActionsForControlEvents ( CCControlEventTouchDragExit );
			}
			else
			{
				pButton->sendActionsForControlEvents ( CCControlEventTouchDragOutside );
			}
		}
	}
}

void KeyboardSpecialDialog::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	for ( CCControl* pButton : m_buttons )
	{
		if ( pButton->isHighlighted() )
		{
			pButton->setHighlighted ( false );
			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 1 );
			pButton->runAction ( pAction );
		}

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch ) )
		{
			CCString* pString = dynamic_cast<CCString*> ( pButton->getUserObject() );
			assert ( pString );
			CCNotificationCenter::sharedNotificationCenter()->postNotification (
				keyboard::KeyboardController::getNotificationKeyPressed(), pButton );
		}
	}

	dismiss();
	CCNotificationCenter::sharedNotificationCenter()->postNotification (
		KeyboardSpecialDialog::getNotificationDismissSpecialSigns() );
}

void KeyboardSpecialDialog::ccTouchCancelled ( CCTouch* pTouch,
		CCEvent* pEvent )
{
	for ( CCControl* pButton : m_buttons )
	{
		if ( pButton->isHighlighted() )
		{
			pButton->setHighlighted ( false );
			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 1 );
			pButton->runAction ( pAction );
		}
	}

	dismiss();
	CCNotificationCenter::sharedNotificationCenter()->postNotification (
		KeyboardSpecialDialog::getNotificationDismissSpecialSigns() );
}

void KeyboardSpecialDialog::onEnter()
{
	LinearLayout::onEnter();
	KKScene* pScene = dynamic_cast<KKScene*> ( getParent() );

	if ( pScene == nullptr )
	{
		CCAssert ( false, "Should have KKScene as a parent" );
		return;
	}

	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate ( this );
}

void KeyboardSpecialDialog::onExit()
{
	LinearLayout::onExit();
	CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate ( this );
}

void KeyboardSpecialDialog::keyBackClicked()
{
	if ( m_isShown == false )
	{
		return;
	}

	CCNode* pNode = getParent();

	if ( pNode == nullptr )
	{
		return;
	}

	CCArray* pChildren = pNode->getChildren();
	CCObject* pObject = nullptr;

	for ( int i = pChildren->count() - 1; i >= 0; --i )
	{
		pObject = pChildren->objectAtIndex ( i );
		KeyboardSpecialDialog* pSpecial = dynamic_cast<KeyboardSpecialDialog*>
										  ( pObject );

		if ( pSpecial != nullptr )
		{
			if ( pSpecial == this )
			{
				runAction ( CCCallFunc::create ( this,
												 callfunc_selector ( KeyboardSpecialDialog::defaultDismiss ) ) );

			}

			return;
		}
	}
}

CCControlButton* KeyboardSpecialDialog::getButton ( const string& letter,
		const string& buttonName, const string& fontName, const CCSize& buttonSize )
{
	CCControlButton* pButton = CCControlButton::create (
								   CCLabelTTF::create ( letter.c_str(), fontName.c_str(), 24 ),
								   CCScale9Sprite::create ( buttonName.c_str() ) );

	pButton->setColor ( ccGRAY );
	CCSize size ( buttonSize );
	size.width *= 1.4f;
	pButton->setPreferredSize ( size );
	pButton->setZoomOnTouchDown ( false );
	pButton->setUserObject ( CCString::create ( letter.c_str() ) );
	return pButton;
}

void KeyboardSpecialDialog::draw()
{
	glLineWidth ( 2.0F );
	ccDrawColor4B ( 0, 0, 0, 255 );
	ccDrawRect ( CCPoint ( 0, 0 ), CCPoint ( getContentSize().width,
				 getContentSize().height ) );
}

} /* namespace keyboard */

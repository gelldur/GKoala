/*
 * KeyboardController.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: Dawid Drozd
 */

#include "view/keyboard/KeyboardController.h"
#include "dictionaries/language/LanguageManager.h"
#include "view/keyboard/KeyboardView.h"
#include "SimpleAudioEngine.h"
#include "utils/TouchManager.h"
#include "view/dialog/AbstractDialog.h"
#include <algorithm>
using namespace CocosDenshion;

namespace keyboard
{
KeyboardController::KeyboardController ( const KeyboardSettings& settings ) :
	m_keyboardSettings ( settings ),
	m_pViewCurrent ( nullptr ),
	m_pNotificationCenter ( CCNotificationCenter::sharedNotificationCenter() ),
	m_isSpecialDialogShown ( false ),
	m_marginSize ( KoalaGui::SizePolicy ( KoalaGui::Policy::SP, 2 ).getValue(),
				   KoalaGui::SizePolicy ( KoalaGui::Policy::SP, 6 ).getValue() )
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver ( this,
			callfuncO_selector ( KeyboardController::onNotificationSpecialDialogDismissed ),
			KeyboardSpecialDialog::getNotificationDismissSpecialSigns(), nullptr );
}

void KeyboardController::initSoundNames ( const string& keypressSoundName,
		const string& deletePressSoundName )
{
	assert ( keypressSoundName.empty() == false );
	assert ( deletePressSoundName.empty() == false );

	m_keyboardSettings.soundKey = keypressSoundName;
	m_keyboardSettings.soundDelete = deletePressSoundName;
}

KeyboardController::~KeyboardController()
{
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers ( this );

	for ( KeyboardView* pView : m_views )
	{
		pView->removeFromParentAndCleanup ( true );
		AttorneyKeyboardView::setController ( pView, nullptr );
		CC_SAFE_RELEASE_NULL ( pView );
	}

	for ( KeyboardView* pView : m_viewsSymbols )
	{
		pView->removeFromParentAndCleanup ( true );
		AttorneyKeyboardView::setController ( pView, nullptr );
		CC_SAFE_RELEASE_NULL ( pView );
	}
}

bool KeyboardController::ccTouchBegan ( CCTouch* pTouch )
{
	assert ( m_views.size() > 0 && m_viewsSymbols.size() > 0 );
	assert ( m_pViewCurrent );

	const vector<CCControl*>& buttons = AttorneyKeyboardView::getButtons (
											m_pViewCurrent );

	for ( CCControl* pButton : buttons )
	{
		if ( pButton->isVisible() == false )
		{
			continue;
		}

		assert ( pButton->retainCount() > 0 );

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch,
				m_marginSize ) )
		{
			//OK!
			CCString* pKeyString = dynamic_cast<CCString*> ( pButton->getUserObject() );
			assert ( pKeyString );
			assert ( m_keyboardSettings.soundKey.empty() == false );
			assert ( m_keyboardSettings.soundDelete.empty() == false );
			string soundName = m_keyboardSettings.soundKey;

			if ( pKeyString->m_sString == "\b" )
			{
				soundName = m_keyboardSettings.soundDelete;
			}

			SimpleAudioEngine::sharedEngine()->playEffect ( soundName.c_str() );
			LOG ( "Hit!" );
			pButton->setHighlighted ( true );

			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 0.9f );
			pButton->runAction ( pAction );

			pButton->sendActionsForControlEvents ( CCControlEventTouchDown );

			CCString* pContent = dynamic_cast<CCString*> ( pButton->getUserObject() );
			assert ( pContent );

			if ( pButton == m_pViewCurrent->getButtonDelete()
					&& pButton->getActionByTag ( DeleteScheduleTag ) == nullptr )
			{
				scheduleDeletePress ( getInitialDeleteDelayinSeconds() );
			}

			startCountAction ( pButton );

			manageHint ( pButton, true );
			return true;
		}
	}

	return true;
}

void KeyboardController::ccTouchMoved ( CCTouch* pTouch )
{
	assert ( m_pViewCurrent );

	if ( m_isSpecialDialogShown )
	{
		return;
	}

	const vector<CCControl*>& buttons = AttorneyKeyboardView::getButtons (
											m_pViewCurrent );

	for ( CCControl* pButton : buttons )
	{
		if ( pButton->isVisible() == false )
		{
			continue;
		}

		bool isTouchMoveInside = false;

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch,
				m_marginSize ) )
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

			if ( pButton->getActionByTag ( ACTION ) == nullptr )
			{
				startCountAction ( pButton );
			}

			if ( pButton == m_pViewCurrent->getButtonDelete()
					&& m_pViewCurrent->getActionByTag ( DeleteScheduleTag ) == nullptr )
			{
				scheduleDeletePress ( getInitialDeleteDelayinSeconds() );
			}
			else if ( pButton != m_pViewCurrent->getButtonDelete() )
			{
				unscheduleDeletePress();
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

			pButton->stopActionByTag ( ACTION );

			if ( pButton == m_pViewCurrent->getButtonDelete() )
			{
				unscheduleDeletePress();
			}
		}

		manageHint ( pButton, isTouchMoveInside );
	}
}

void KeyboardController::ccTouchEnded ( CCTouch* pTouch )
{
	assert ( m_pViewCurrent );

	//TODO cancel touch in ccTouchManager
	if ( m_isSpecialDialogShown )
	{
		ccTouchCancelled ( pTouch );
		return;
	}

	unscheduleDeletePress();

	const vector<CCControl*>& buttons = AttorneyKeyboardView::getButtons (
											m_pViewCurrent );

	for ( CCControl* pButton : buttons )
	{
		if ( pButton->isVisible() == false )
		{
			continue;
		}

		if ( pButton->isHighlighted() )
		{
			pButton->setHighlighted ( false );
			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 1 );
			pButton->runAction ( pAction );
		}

		if ( KoalaLib::TouchManager::detectTouchOnNode ( pButton, pTouch,
				m_marginSize ) )
		{
			if ( pButton == m_pViewCurrent->getButtonLetters() )
			{
				m_pNotificationCenter->postNotification (
					KeyboardController::getNotificationLayoutLettersPressed(), pButton );
				assert ( m_views.size() > 1 );

				UserConfiguration* pConfiguration = UserConfiguration::getInstance();

				//It toggles between native and learning, where native is 0 and learning is 1,
				//with a simple idea that abs( 0 - 1 ) == 1 and abs( 1 - 1 ) == 0
				pConfiguration->setKeyboardSettings (
					( LayoutSettings ) abs ( pConfiguration->getKeyboardSettings() - 1 ) );

				assert ( pConfiguration->getKeyboardSettings() >= 0 );

				assert ( pConfiguration->getKeyboardSettings() < ( int ) m_views.size() );

				changeLayout ( m_views[pConfiguration->getKeyboardSettings()] );

				updateLinks();
			}
			else if ( pButton == m_pViewCurrent->getButtonSymbols() )
			{
				m_pNotificationCenter->postNotification (
					KeyboardController::getNotificationLayoutSymbolsPressed(), pButton );
				assert ( m_viewsSymbols.size() > 0 );
				pickLayout ( m_viewsSymbols );
			}
			else if ( pButton == m_pViewCurrent->getButtonSpecial() )
			{
				m_pNotificationCenter->postNotification (
					KeyboardController::getNotificationDonePressed(), pButton );
			}
			else if ( pButton == m_pViewCurrent->getButtonLettersOrder() )
			{
				m_pNotificationCenter->postNotification (
					KeyboardController::getNotificationChangeKeyboardLayoutPressed(),
					this );
			}
			else
			{
				m_pNotificationCenter->postNotification (
					KeyboardController::getNotificationKeyPressed(), pButton );
			}

			pButton->stopActionByTag ( ACTION );

			manageHint ( pButton, false );
			return;
		}

		pButton->stopActionByTag ( ACTION );
		m_pViewCurrent->stopActionByTag ( DeleteScheduleTag );
		manageHint ( pButton, false );
	}
}

void KeyboardController::ccTouchCancelled ( CCTouch* pTouch )
{
	const vector<CCControl*>& buttons = AttorneyKeyboardView::getButtons (
											m_pViewCurrent );

	for ( CCControl* pButton : buttons )
	{
		if ( pButton->isHighlighted() )
		{
			pButton->setHighlighted ( false );
			CCActionInterval* pAction = CCScaleTo::create ( 0.03f, 1 );
			pButton->runAction ( pAction );
		}

		manageHint ( pButton, false );

		pButton->stopActionByTag ( ACTION );
		pButton->stopActionByTag ( DeleteScheduleTag );
	}
}

void KeyboardController::startCountAction ( CCControl* pButton )
{
	CCSequence* pSequence = CCSequence::createWithTwoActions ( CCDelayTime::create (
								0.8f ),
							CCCallFuncN::create ( this,
									callfuncN_selector ( KeyboardController::holdAction ) ) );
	pSequence->setTag ( ACTION );
	pButton->runAction ( pSequence );
}

void KeyboardController::resetView ( LettersOrder order )
{
	KeyboardLayout keyboardLayout = m_pViewCurrent->getKeyboardLayout();

	switch ( order )
	{
		case LettersOrder::QWERTY:
			keyboardLayout.alphabet = Alphabet::AlphabetQWERTY();
			break;

		case LettersOrder::QWERTZ:
			keyboardLayout.alphabet = Alphabet::AlphabetQWERTZ();
			break;

		case LettersOrder::AZERTY:
			keyboardLayout.alphabet = Alphabet::AlphabetAZERTY();
			break;

		case LettersOrder::NONE:
			//should not happened
			assert ( false );
			break;
	}

	replaceCurrentLayout ( keyboardLayout );
}

void KeyboardController::holdAction ( CCNode* pNode )
{
	assert ( pNode );
	CCString* pChar = dynamic_cast<CCString*> ( pNode->getUserObject() );
	assert ( pChar );

	if ( isExceptionSign ( pChar ) )
	{
		return;
	}

	ccTouchCancelled ( nullptr );
	setSpecialSignsDialog ( m_pViewCurrent, pNode );
}

void KeyboardController::replaceCurrentLayout ( const KeyboardLayout&
		keyboardLayout )
{
	if ( keyboardLayout.alphabet.getLetters().size() < 1 )
	{
		assert ( false );
		return;
	}

	KeyboardView* pView = KeyboardView::create ( this, keyboardLayout,
						  m_keyboardSettings );
	pView->retain();

	assert ( m_views.size() > 0 );
	assert ( m_pViewCurrent );
	vector<KeyboardView*>::iterator iterator = std::find ( m_views.begin(),
			m_views.end(), m_pViewCurrent );

	assert ( iterator != m_views.end() );
	m_views.erase ( iterator );
	m_views.insert ( iterator, pView );

	CCNode* pParentNode = m_pViewCurrent->getParent();
	assert ( pParentNode );
	m_pViewCurrent->removeFromParentAndCleanup ( true );
	CC_SAFE_RELEASE_NULL ( m_pViewCurrent );

	updateLinks();
	assert ( pView->retainCount() - pView->autoreleaseCount() == 1 );

	m_pViewCurrent = pView;
	pParentNode->addChild ( m_pViewCurrent, 100 );
	Positioner::bottomEdge ( m_pViewCurrent );
}

void KeyboardController::addKeyboardLayout ( const KeyboardLayout&
		keyboardLayout,
		bool isSymbolLayout )
{
	if ( keyboardLayout.alphabet.getLetters().size() < 1 )
	{
		assert ( false );
		return;
	}

	KeyboardView* pView = KeyboardView::create ( this, keyboardLayout,
						  m_keyboardSettings );
	pView->retain();

	if ( isSymbolLayout )
	{
		m_viewsSymbols.push_back ( pView );
	}
	else
	{
		m_views.push_back ( pView );
	}

	updateLinks();
	assert ( pView->retainCount() - pView->autoreleaseCount() == 1 );
}

void KeyboardController::show ( CCScene* pScene )
{
	assert ( m_views.size() > 0 ); //keyboards not ready

	if ( m_views.size() == 1 )
	{
		changeLayout ( m_views[0], pScene );
		//if there is only one view,
		//We start from first keyboard view
		return;
	}

	UserConfiguration* pConfiguration = UserConfiguration::getInstance();

	if ( pConfiguration->getKeyboardSettings() != LayoutSettings::NOT_SET )
	{
		assert ( pConfiguration->getKeyboardSettings() > -1 );
		assert ( pConfiguration->getKeyboardSettings() < ( int ) m_views.size() );
		changeLayout ( m_views[pConfiguration->getKeyboardSettings()], pScene );
		return;
	}

	//if settings was not set, we by default start from learning view
	pConfiguration->setKeyboardSettings ( LayoutSettings::LEARNING );
	assert ( LayoutSettings::LEARNING < m_views.size() );
	changeLayout ( m_views[LayoutSettings::LEARNING], pScene );
}

void KeyboardController::pickLayout ( const vector<KeyboardView*>& views )
{
	assert ( views.size() > 0 );
	assert ( m_pViewCurrent );

	short int next = m_pViewCurrent->getNextIndex();

	if ( next != -1 )
	{
		assert ( next < ( int ) views.size() );
		changeLayout ( views[next] );
		return;
	}

	if ( m_views.size() == 1 )
	{
		changeLayout ( m_views[0] );
		return;
	}

	UserConfiguration* pConfiguration = UserConfiguration::getInstance();

	assert ( pConfiguration->getKeyboardSettings() < ( int ) m_views.size() );
	changeLayout ( m_views[pConfiguration->getKeyboardSettings()] );
}

void KeyboardController::changeLayout ( KeyboardView* pKeyboardView,
										CCNode* pParentNode /*= nullptr*/ )
{
	if ( pParentNode == nullptr )
	{
		pParentNode = m_pViewCurrent->getParent();
		assert ( pParentNode );
		m_pViewCurrent->removeFromParentAndCleanup ( true );
	}

	assert ( pParentNode );
	m_pViewCurrent = pKeyboardView;

	pParentNode->addChild ( m_pViewCurrent,
							100 );	//We start from first keyboard view
	Positioner::bottomEdge ( m_pViewCurrent );

}

void KeyboardController::updateLinks()
{
	if ( m_views.size() < 1 || m_viewsSymbols.size() < 1 )
	{
		return;
	}

	CCString* pNextSigns = nullptr;

	for ( unsigned int i = 0; i < m_viewsSymbols.size(); ++i )
	{
		KeyboardView* pView = m_viewsSymbols[i];

		//hide letters view button for symbols
		CCControlButton* pButton = pView->getButtonLetters();

		if ( pButton->isVisible() )
		{
			pButton->setVisible ( false );
		}

		//hide letters order button for symbols
		pButton = pView->getButtonLettersOrder();

		if ( pButton->isVisible() )
		{
			pButton->setVisible ( false );
		}

		//reindex symbols views
		if ( i + 1 == m_viewsSymbols.size() )
		{
			pView->setNextIndex ( -1 );

			if ( m_views.size() == 1 )
			{
				pNextSigns = getButtonSignFromButtons (
								 AttorneyKeyboardView::getButtons ( m_views[0] ) );
			}
			else
			{
				//there is possible that getKeyboardSettings will return -1,
				//so thats why we need abs
				assert (
					abs ( ( int ) UserConfiguration::getInstance()->getKeyboardSettings() )
					< ( int ) m_views.size() );

				pNextSigns = getButtonSignFromButtons (
								 AttorneyKeyboardView::getButtons ( m_views[abs (
											 ( int ) UserConfiguration::getInstance()->getKeyboardSettings() )] ) );
			}
		}
		else
		{
			assert ( i + 1 < m_viewsSymbols.size() );
			pView->setNextIndex ( i + 1 );
			pNextSigns = getButtonSignFromButtons (
							 AttorneyKeyboardView::getButtons ( m_viewsSymbols[i + 1] ) );
		}

		setTextOnButton ( pView->getButtonSymbols(), pNextSigns );
	}

	CCString* pNextLettersIso = nullptr;

	if ( m_views.size() == 1 )
	{
		pNextLettersIso = CCString::create ( "" );
		setTextOnButton ( m_views[0]->getButtonLetters(), pNextLettersIso );

		m_views[0]->setNextIndex ( 0 );

		if ( m_views[0]->getButtonLetters()->isVisible() )
		{
			m_views[0]->getButtonLetters()->setVisible ( false );
		}

		if ( m_views[0]->getKeyboardLayout().isLatinaLanguage == false )
		{
			if ( m_views[0]->getButtonLettersOrder()->isVisible() )
			{
				m_views[0]->getButtonLettersOrder()->setVisible ( false );
			}
		}

		return;
	}

	for ( unsigned int i = 0; i < m_views.size(); ++i )
	{
		if ( i + 1 < m_views.size() )
		{
			assert ( i + 1 < m_views.size() );
			pNextLettersIso = CCString::create ( m_views[i + 1]->getKeyboardLayout().iso );
			m_views[i]->setNextIndex ( 0 );
		}
		else
		{
			assert ( i  < m_views.size() );
			pNextLettersIso = CCString::create ( m_views[0]->getKeyboardLayout().iso );
			m_views[i]->setNextIndex ( 0 );
		}

		setTextOnButton ( m_views[i]->getButtonLetters(), pNextLettersIso );

		if ( i > 0 && m_views[i - 1]->getKeyboardLayout().isLatinaLanguage
				&& m_views[i]->getKeyboardLayout().isLatinaLanguage )
		{
			assert ( i > 0 && i - 1 < m_views.size() && i < m_views.size() );

			if ( m_views[i - 1]->getButtonLetters()->isVisible() )
			{
				m_views[i - 1]->getButtonLetters()->setVisible ( false );
			}

			if ( m_views[i]->getButtonLetters()->isVisible() )
			{
				m_views[i]->getButtonLetters()->setVisible ( false );
			}
		}

		if ( m_views[i]->getKeyboardLayout().isLatinaLanguage == false )
		{
			if ( m_views[i]->getButtonLettersOrder()->isVisible() )
			{
				m_views[i]->getButtonLettersOrder()->setVisible ( false );
			}
		}
	}
}

void KeyboardController::addNativeKeyboardLayout ( const KeyboardLayout&
		keyboardLayout )
{
	if ( keyboardLayout.alphabet.getLetters().size() < 1 )
	{
		assert ( false );
		return;
	}

	KeyboardLayout temporaryKeyboardLayout ( keyboardLayout );

	//if settings were done
	if ( temporaryKeyboardLayout.isLatinaLanguage == true
			&& UserConfiguration::getInstance()->getKeyboardLettersOrder()
			!= LettersOrder::NONE )
	{
		switch ( UserConfiguration::getInstance()->getKeyboardLettersOrder() )
		{
			case LettersOrder::QWERTY:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetQWERTY();
				break;

			case LettersOrder::QWERTZ:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetQWERTZ();
				break;

			case LettersOrder::AZERTY:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetAZERTY();
				break;

			case LettersOrder::NONE:
				//this is not possible
				assert ( false );
				break;
		}
	}

	KeyboardView* pView = KeyboardView::create ( this, temporaryKeyboardLayout,
						  m_keyboardSettings );
	pView->retain();

	m_views.push_back ( pView );
}

void KeyboardController::addLearningKeyboardLayout ( const KeyboardLayout&
		keyboardLayout )
{
	if ( keyboardLayout.alphabet.getLetters().size() < 1 )
	{
		assert ( false );
		return;
	}

	assert ( m_views.size() > 0 );

	KeyboardView* pNative = m_views[0];

	//if learning is the same as native then don't add new layout
	if ( pNative->getKeyboardLayout().iso.compare ( keyboardLayout.iso ) == 0 )
	{
		return;
	}

	//if native and learning are latina dont add new layout but copy special letters
	if ( pNative->getKeyboardLayout().isLatinaLanguage &&
			keyboardLayout.isLatinaLanguage )
	{
		pNative->concatenateSpecialSigns ( keyboardLayout.specialSigns );
		return;
	}

	KeyboardLayout temporaryKeyboardLayout ( keyboardLayout );

	//settings for order is not set
	if ( UserConfiguration::getInstance()->getKeyboardLettersOrder() ==
			LettersOrder::NONE )
	{
		//and native is not latina, so we have QWERTY layout
		if ( pNative->getKeyboardLayout().isLatinaLanguage == false )
		{
			temporaryKeyboardLayout.alphabet = Alphabet::AlphabetQWERTY();
		}
	}
	else if ( temporaryKeyboardLayout.isLatinaLanguage )
	{
		switch ( UserConfiguration::getInstance()->getKeyboardLettersOrder() )
		{
			case LettersOrder::QWERTY:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetQWERTY();
				break;

			case LettersOrder::QWERTZ:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetQWERTZ();
				break;

			case LettersOrder::AZERTY:
				temporaryKeyboardLayout.alphabet = Alphabet::AlphabetAZERTY();
				break;

			case LettersOrder::NONE:
				//this is not possible
				assert ( false );
				break;
		}
	}

	KeyboardView* pView = KeyboardView::create ( this, temporaryKeyboardLayout,
						  m_keyboardSettings );
	pView->retain();

	m_views.push_back ( pView );
}

CCString* KeyboardController::getButtonSignFromButtons (
	const vector<CCControl*>& buttons )
{
	CCString* pString = CCString::create ( "" );

	for ( unsigned i = 0; i < 3 && i < buttons.size(); ++i )
	{
		CCString* pStringSign = dynamic_cast<CCString*> ( buttons[i]->getUserObject() );
		assert ( pStringSign );

		if ( pStringSign->m_sString != "\b" )
		{
			pString->m_sString += pStringSign->m_sString;
		}
	}

	return pString;
}

void KeyboardController::manageHint ( CCControl* pButton, bool show )
{
	assert ( pButton );
	CCString* pChar = dynamic_cast<CCString*> ( pButton->getUserObject() );
	assert ( pChar );

	if ( isExceptionSign ( pChar ) )
	{
		return;
	}

	CCNode* pChild = pButton->getChildByTag ( KeyboardView::ID::HINT );

	if ( show )
	{
		if ( pChild == nullptr )
		{
			m_pViewCurrent->setHintForButton ( pButton, pChar->m_sString );
			pButton->setColor ( MAKE_CCC3 ( 0x169ce1 ) );
		}
	}
	else if ( pChild )
	{
		pChild->removeFromParentAndCleanup ( true );
		pButton->setColor ( ccWHITE );
	}
}

KeyboardController* KeyboardController::prepareStandardKeyboard (
	const KeyboardSettings& settings )
{
	KeyboardController* pController = create ( settings );
	pController->initSoundNames ( settings.soundKey, settings.soundDelete );

	pController->addKeyboardLayout (
		KeyboardLayout ( Alphabet::AlphabetSymbols(), map<const string, const string>(),
						 KeyboardLayoutRows(), false, true ) );
	pController->addKeyboardLayout (
		KeyboardLayout ( Alphabet::AlphabetSymbols2(),
						 map<const string, const string>(),
						 KeyboardLayoutRows(), false, true ) );

	const Language* pNativeLanguage =
		LanguageManager::getInstance()->getNativeLangauge();
	const Language* pLearningLanguage = LanguageManager::getInstance()
										->getLearningLangauge();

	//don't add kayboards that are not supported
	if ( LanguageManager::getInstance()->isKeyboardAvailable (
				pNativeLanguage->getId() ) )
	{
		pController->addNativeKeyboardLayout ( pNativeLanguage->getKeyboardLayout() );
		pController->addLearningKeyboardLayout (
			pLearningLanguage->getKeyboardLayout() );
	}
	else if ( LanguageManager::getInstance()->isKeyboardAvailable (
				  pLearningLanguage->getId() ) )
	{
		pController->addNativeKeyboardLayout ( pLearningLanguage->getKeyboardLayout() );
	}
	else
	{
		LOG ( "This situation shouldn't happened!" );
		assert ( false );
	}

	pController->updateLinks();

	return pController;
}

void KeyboardController::setSpecialSignsDialog ( KeyboardView* pView,
		CCNode* pNode )
{
	CCString* pChar = dynamic_cast<CCString*> ( pNode->getUserObject() );
	assert ( pChar );

	string character = pChar->m_sString;

	ostringstream stream;
	stream << character;

	map<string, string> specialSigns = pView->getSpecialSigns();

	map<string, string>::iterator it = specialSigns.find ( character.c_str() );

	if ( it != specialSigns.end() )
	{
		stream << it->second;
	}

	Alphabet alphabet ( stream.str() );

	KeyboardSpecialDialog* pDialog = KeyboardSpecialDialog::create (
										 m_keyboardSettings.buttonFile, m_keyboardSettings.fontName, alphabet,
										 m_keyboardSettings.soundKey, pView->getPrefferedButtonSize() );
	pDialog->showAbove ( pNode );

	m_isSpecialDialogShown = true;
}

bool KeyboardController::isExceptionSign ( CCString* pChar )
{
	if ( pChar->m_sString == "\b" || pChar->m_sString == "\t" ||
			pChar->m_sString == " "
			|| pChar->m_sString.empty() )
	{
		return true;
	}

	return false;
}

void KeyboardController::onNotificationSpecialDialogDismissed (
	CCObject* pObject )
{
	m_isSpecialDialogShown = false;
}

void  KeyboardController::manageDeletePress()
{
	assert ( m_pViewCurrent );
	assert ( m_pViewCurrent->getButtonDelete() );
	m_pNotificationCenter->postNotification (
		KeyboardController::getNotificationKeyPressed(),
		m_pViewCurrent->getButtonDelete() );
	scheduleDeletePress();
}

void KeyboardController::scheduleDeletePress ( float delay /* = 0.1f */ )
{
	assert ( m_pViewCurrent );

	CCSequence* pSequence = CCSequence::createWithTwoActions
							( CCDelayTime::create ( delay ), CCCallFunc::create ( this,
									callfunc_selector ( KeyboardController::manageDeletePress ) ) );
	pSequence->setTag ( DeleteScheduleTag );
	m_pViewCurrent->runAction ( pSequence );
}

void KeyboardController::unscheduleDeletePress()
{
	assert ( m_pViewCurrent );
	m_pViewCurrent->stopActionByTag ( DeleteScheduleTag );
}

} /* namespace keyboard */

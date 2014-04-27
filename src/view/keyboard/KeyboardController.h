/*
 * KeyboardController.h
 *
 *  Created on: Jul 15, 2013
 *      Author: Dawid Drozd
 */

#ifndef KEYBOARDCONTROLLER_H_
#define KEYBOARDCONTROLLER_H_

#include "view/layout/linear/LinearLayout.h"
#include "view/layout/relative/RelativeLayout.h"
#include "dictionaries/language/Alphabet.h"
#include "view/keyboard/KeyboardLayout.h"
#include "view/keyboard/KeyboardSpecialDialog.h"
#include "configuration/UserConfiguration.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace KoalaGui;

namespace keyboard
{

class KeyboardView;

struct KeyboardSettings
{
	std::string fontName;
	std::string buttonFile;
	std::string hintName;
	std::string deleteName;
	std::string settingsName;
	std::string inactiveName;
	std::string activeName;

	std::string soundKey;
	std::string soundDelete;
};

class KeyboardController: public CCObject
{
public:
	static KeyboardController* create ( const KeyboardSettings& settings )
	{
		KeyboardController* pRet = new KeyboardController ( settings );

		if ( pRet )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE ( pRet );
		return pRet = nullptr;
	}

	static KeyboardController* prepareStandardKeyboard ( const KeyboardSettings&
			settings );

	virtual ~KeyboardController();

	void setSpecialSignsDialog ( KeyboardView* pView, CCNode* pNode );

	void initSoundNames ( const string& keypressSoundName,
						  const string& deletePressSoundName );

	bool ccTouchBegan ( CCTouch* pTouch );
	void ccTouchMoved ( CCTouch* pTouch );
	void ccTouchEnded ( CCTouch* pTouch );
	void ccTouchCancelled ( CCTouch* pTouch );

	void resetView ( LettersOrder order );

	void addKeyboardLayout ( const KeyboardLayout& keyboardLayout,
							 bool isSymbolLayout = true );

	void show ( CCScene* pScene );

	GET (  char*, QwertyOption, "QWERTY" )
	GET (  char*, QwertzOption, "QWERTZ" )
	GET (  char*, AzertyOption, "AZERTY" )

	GET (  char*, NotificationKeyPressed,
		   "keyboard_key_pressed" ) //
	GET (  char*, NotificationDonePressed,
		   "keyboard_done_key_pressed" ) //
	GET (  char*, NotificationLayoutSymbolsPressed,
		   "keyboard_layout_symbols_pressed" ) //
	GET (  char*, NotificationLayoutLettersPressed,
		   "keyboard_layout_letters_pressed" ) //
	GET (  char*, NotificationChangeKeyboardLayoutPressed,
		   "change_letters_order_pressed" ) //
	GET (  char*, NotificationSpecialSignsActivation,
		   "activate_special_signs_dialog" ) //


	enum
	{
		DeleteScheduleTag = 445343
	};


protected:
	KeyboardController ( const KeyboardSettings& settings );

	void addNativeKeyboardLayout ( const KeyboardLayout& keyboardLayout );
	void addLearningKeyboardLayout ( const KeyboardLayout& keyboardLayout );

private:
	KeyboardSettings m_keyboardSettings;
	vector<KeyboardView*> m_views;
	vector<KeyboardView*> m_viewsSymbols;
	KeyboardView* m_pViewCurrent;
	CCNotificationCenter* m_pNotificationCenter;
	bool m_isSpecialDialogShown;
	CCSize m_marginSize;

	enum ID
	{
		ACTION = 6458
	};

	GET (  float, InitialDeleteDelayinSeconds, 0.6f );

	void manageDeletePress();
	void scheduleDeletePress ( float delay = 0.1f );
	void unscheduleDeletePress();

	void startCountAction ( CCControl* pButton );
	void holdAction ( CCNode* pNode );

	void pickLayout ( const vector<KeyboardView*>& views );

	void changeLayout ( KeyboardView* pKeyboardView,
						CCNode* pParentNode = nullptr );
	void manageHint ( CCControl* pButton, bool show );

	void replaceCurrentLayout ( const KeyboardLayout& keyboardLayout );

	void updateLinks();
	void setTextOnButton ( CCControlButton* pButton, CCString* pString )
	{
		pButton->setTitleForState ( pString, CCControlStateSelected );
		pButton->setTitleForState ( pString, CCControlStateNormal );
		pButton->setTitleForState ( pString, CCControlStateHighlighted );
		pButton->setTitleForState ( pString, CCControlStateDisabled );
	}

	CCString* getButtonSignFromButtons ( const vector<CCControl*>& buttons );

	bool isExceptionSign ( CCString* pChar );

	void onNotificationSpecialDialogDismissed ( CCObject* pObject );
};

} /* namespace keyboard */
#endif /* KEYBOARDCONTROLLER_H_ */

/*
 * KeyboardSpecialDialog.h
 *
 *  Created on: Oct 16, 2013
 *      Author: Filip Miklaś
 */

#ifndef KEYBOARDSPECIALDIALOG_H_
#define KEYBOARDSPECIALDIALOG_H_

#include "view/layout/linear/LinearLayout.h"
#include "view/KKScene.h"
#include "dictionaries/language/Alphabet.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

namespace keyboard
{

class KeyboardSpecialDialog: public KoalaGui::LinearLayout
{
public:

	static KeyboardSpecialDialog* create ( const string& buttonName,
										   const string& fontName,
										   const Alphabet& letters, const string& soundName, const CCSize& buttonSize,
										   int tag = kViewTagUnusedLayout )
	{
		KeyboardSpecialDialog* pRet = new KeyboardSpecialDialog ( tag );

		if ( pRet &&
				pRet->init ( letters, buttonName, fontName, soundName, buttonSize ) )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return pRet;
	}

	virtual bool init ( const Alphabet& letters, const string& buttonName,
						const string& fontName, const string& soundName, const CCSize& buttonSize );

	virtual ~KeyboardSpecialDialog();

	virtual bool ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchCancelled ( CCTouch* pTouch, CCEvent* pEvent );

	virtual void draw();

	virtual void onEnter();
	virtual void onExit();

	virtual void showAbove ( CCNode* pNode );
	virtual void dismiss ( float duration = 0.2f );

	virtual void keyBackClicked();

	GET (  char*, NotificationDismissSpecialSigns,
		   "notification_special_signs_dismiss" ) //

protected:
	KeyboardSpecialDialog ( int tag );

private:
	vector<CCControlButton*> m_buttons;
	string m_keypressedSoundName;

	CCLayerColor* m_pSemiTransparentBackground;

	bool m_wasSceneKeypadEnabled;
	bool m_isShown;

	enum ID
	{
		LAYOUT = -123, BUTTON_LAYOUT
	};

	CCControlButton* getButton ( const string& letter, const string& buttonName,
								 const string& fontName, const CCSize& buttonSize );

	/**
	 * This method is called by key dispatcher
	 */
	void defaultDismiss()
	{
		dismiss();
	}

};

} /* namespace keyboard */
#endif /* KEYBOARDSPECIALDIALOG_H_ */

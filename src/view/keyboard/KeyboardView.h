/*
 * KeyboardView.h
 *
 *  Created on: Jul 14, 2013
 *      Author: Dawid Drozd
 */

#ifndef KEYBOARDVIEW_H_
#define KEYBOARDVIEW_H_

#include "view/layout/linear/LinearLayout.h"
#include "view/layout/relative/RelativeLayout.h"
#include "dictionaries/language/Alphabet.h"
#include "view/keyboard/KeyboardLayout.h"
#include "view/keyboard/KeyboardSpecialDialog.h"
#include "configuration/UserConfiguration.h"
#include "view/keyboard/KeyboardController.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace KoalaGui;

namespace keyboard
{

class KeyboardView: public LinearLayout
{
public:
	friend class AttorneyKeyboardView;
	static KeyboardView* create ( KeyboardController* pController,
								  const KeyboardLayout& keyboardLayout, const KeyboardSettings& settings )
	{
		KeyboardView* pRet = new KeyboardView ( pController, settings, keyboardLayout );

		if ( pRet && pRet->init () )
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return pRet;
		}
	}
	virtual bool init ();
	virtual ~KeyboardView();

	virtual void ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
	virtual void ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
	virtual void ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
	virtual void ccTouchesCancelled ( CCSet* pTouches, CCEvent* pEvent );

	virtual bool onLayout();

	virtual void onEnter();
	virtual void onExit();

	CCControlButton* getButtonSymbols() const
	{
		return m_pButtonSymbols;
	}

	CCControlButton* getButtonLetters() const
	{
		return m_pButtonLetters;
	}

	CCControlButton* getButtonSpecial() const
	{
		return m_pButtonDone;
	}

	CCControlButton* getButtonLettersOrder() const
	{
		return m_pButtonLettersOrder;
	}

	CCControlButton* getButtonDelete() const
	{
		return m_pButtonDelete;
	}

	KeyboardLayout getKeyboardLayout() const
	{
		return m_keyboardLayout;
	}

	void setHintForButton ( CCControl* pButton, const string& character );

	void setNextIndex ( short int next )
	{
		m_nextView = next;
	}

	short int getNextIndex()
	{
		return m_nextView;
	}

	map<string, string> getSpecialSigns() const
	{
		return m_specialSigns;
	}

	CCSize getPrefferedButtonSize() const
	{
		return m_preferedButtonSize;
	}

	void concatenateSpecialSigns ( const map<const string, const string>&
								   specialSigns );

	enum ID
	{
		HINT = 546660,
		DONE,
		SPACEBAR,
		LETTERS_ORDER,
		LAYOUT_LETTERS_CHANGE
	};

	GET ( CCSize, PrefferedKeyboardButtonSize, CCSize ( 43, 65 ) )

protected:
	KeyboardView ( KeyboardController* pController,
				   const KeyboardSettings& settings,
				   const KeyboardLayout& keyboardLayout, const int tag = kViewTagUnusedLayout );

	const vector<CCControl*>& getButtons() const
	{
		return m_buttons;
	}

	void setController ( KeyboardController* pController )
	{
		m_pController = pController;
	}

private:
	KeyboardSettings m_settings;
	short int m_firstRow;
	short int m_secondRow;
	short int m_thirdRow;
	short int m_nextView;
	vector<AbstractLayout*> m_rows;
	vector<CCControl*> m_buttons;
	CCSize m_preferedButtonSize;
	CCSize m_lastRowButtonSize;

	KeyboardController* m_pController;
	CCControlButton* m_pButtonSymbols;
	CCControlButton* m_pButtonLetters;
	CCControlButton* m_pButtonDone;
	CCControlButton* m_pButtonLettersOrder;
	CCControlButton* m_pButtonDelete;

	KeyboardLayout m_keyboardLayout;

	map<string, string> m_specialSigns;

	KeyboardSpecialDialog* m_pSpecialDialog;

	CCSet* m_pRegisteredTouches;

	LinearLayout* getRow ( unsigned position );
	RelativeLayout* getLastRow();
	CCControlButton* getSpaceButton();
	CCControlButton* getDeleteButton();
	CCControlButton* getDoneButton();
	CCControlButton* getChangeLayoutSymbolsButton();
	CCControlButton* getChangeLayoutLettersButton();
	CCControlButton* getChangeLettersOrderButton();

	CCControlButton* getNewButton ( const string& sign, CCSize size = CCSize ( 0,
									0 ) );
	CCSize getButtonSize ( CCNode* pNode, const LinearLayoutParams& params );

};

class AttorneyKeyboardView
{
	friend class KeyboardController;
	static const vector<CCControl*>& getButtons ( const KeyboardView*
			pKeyboardView )
	{
		return pKeyboardView->getButtons();
	}

	static void setController ( KeyboardView* pKeyboardView,
								KeyboardController* pController )
	{
		pKeyboardView->setController ( pController );
	}
};

} /* namespace keyboard */
#endif /* KEYBOARDVIEW_H_ */

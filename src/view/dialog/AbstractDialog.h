/*
 * AbstractDialog.h
 *
 *  Created on: Aug 23, 2013
 *      Author: kolo
 */

#ifndef ABSTRACTDIALOG_H_
#define ABSTRACTDIALOG_H_

#include "view/layout/linear/LinearLayout.h"
#include "view/KKScene.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

namespace KoalaGui
{

class AbstractDialog: public LinearLayout
{
public:
	static AbstractDialog* create( int tag = kViewTagUnusedLayout )
	{
		AbstractDialog* pRet = new AbstractDialog( tag );

		if( pRet && pRet->init() )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;
		return nullptr;
	}

	virtual ~AbstractDialog();

	virtual bool init();

	virtual bool ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent );
	virtual bool onLayout();

	virtual void onEnter();
	virtual void onExit();

	static KKScene* getCurrentScene()
	{
		KKScene* pScene = dynamic_cast<KKScene*>
						  ( CCDirector::sharedDirector()->getRunningScene() );
		assert( pScene );
		return pScene;
	}

	virtual void show( KKScene* pScene = getCurrentScene() );
	virtual void dismiss( bool showAnimation = true );
	bool isShown();

	bool getDismissOnTouch() const;
	void setDismissOnTouch( const bool& isDismissable );

	bool getDismissOnBack() const;
	void setDismissOnBack( const bool& isDismissable );

	void setOnDismissCallback( Utils::CallbackOld* pDismissCallback );
	void onDismissClickCallback( CCObject* pObject, CCControlEvent event );

	virtual void keyBackClicked();

protected:
	AbstractDialog( int tag );

	static const SizePolicy HEADER_HEIGHT;

	/**
	 * Here you should run your custom animation for showing dialog
	 */
	virtual void showAnimation( KKScene* pShowingScene );

	/**
	 * Look at showAnimation :)
	 * @param pShowingScene
	 */
	virtual void dismissAnimation( KKScene* pShowingScene );

	CCLayerColor* getDialogBackground()
	{
		return m_pDialogBackground;
	}

private:

	CCLayerColor* m_pDialogBackground;

	bool m_dismissedOnTouch;
	bool m_dismissedOnBack;
	bool m_wasSceneKeypadEnabled;
	bool m_isShown;
	Utils::CallbackOld* m_pDismissCallback;

	/**
	 * This method is called by key dispatcher
	 */
	void defaultDismiss()
	{
		dismiss();
	}

	void sheduledDismiss( float dt )
	{
		defaultDismiss();
	}
};

} /* namespace KoalaGui */
#endif /* ABSTRACTDIALOG_H_ */

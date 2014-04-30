/*
 * LayoutButton.h
 *
 *  Created on: September 17, 2013
 *      Author: Jakub Kuderski
 */

#ifndef LABEL_BUTTON_H
#define LABEL_BUTTON_H

#include "cocos2d.h"
#include "view/layout/linear/LinearLayout.h"
#include <string>

#include "view/KKImageButton.h"

USING_NS_CC;
using namespace std;

namespace KoalaGui
{

class LayoutButton: public LinearLayout
{
public:
	static LayoutButton* createButtonAndText( CCNode* pNode, CCNode* pText,
			int tag =
				kViewTagUnusedLayout )
	{
		LayoutButton* pRet = new LayoutButton( kViewTagUnusedLayout );

		if( pRet && pRet->initWithButtonAndText( pNode, pText ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}
	static LayoutButton* createButtonAndText( const string& buttonFileName,
			const string& text, int tag = kViewTagUnusedLayout )
	{
		LayoutButton* pRet = new LayoutButton( kViewTagUnusedLayout );

		if( pRet && pRet->initWithButtonAndText( buttonFileName, text ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}
	static LayoutButton* createButtonAndText( const string& buttonFileName,
			int tag =
				kViewTagUnusedLayout )
	{
		LayoutButton* pRet = new LayoutButton( kViewTagUnusedLayout );

		if( pRet && pRet->initWithButton( buttonFileName ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}

	virtual bool initWithButtonAndText( CCNode* pNode, CCNode* pText );
	virtual bool initWithButtonAndText( const string& buttonFileName,
										const string& text );
	virtual bool initWithButton( const string& buttonFileName );

	virtual ~LayoutButton();

	virtual bool ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchMoved( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchCancelled( CCTouch* pTouch, CCEvent* pEvent );

	CCTouchDelegate* getTouchable() const
	{
		return m_pTouchable;
	}

	KKImageButton* getButton() const
	{
		return dynamic_cast<KKImageButton*>( m_pTouchable );
	}

	CCNode* getButtonNode() const
	{
		return dynamic_cast<CCNode*>( m_pTouchable );
	}

	CCLabelProtocol* getLabel() const
	{
		return m_pTextProtocol;
	}

	CCLabelTTF* getLabelTTF() const
	{
		return dynamic_cast<CCLabelTTF*>( m_pTextProtocol );
	}

protected:
	LayoutButton( int tag );

private:
	CCTouchDelegate* m_pTouchable;
	CCLabelProtocol* m_pTextProtocol;
};

}
/* namespace KoalaGui */
#endif /* LABEL_BUTTON_H */

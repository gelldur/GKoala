/*
 * KKImageButton.h
 *
 *  Created on: May 10, 2013
 *      Author: Dawid Drozd
 */

#ifndef KKIMAGEBUTTON_H_
#define KKIMAGEBUTTON_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "view/KKScaleSprite.h"
using std::string;
USING_NS_CC;
USING_NS_CC_EXT;

namespace KoalaGui
{

class KKImageButton: public CCControlButton
{
public:
	KKImageButton();
	virtual ~KKImageButton();

	static KKImageButton* create( const string& text, const char* pSpriteName,
								  bool is9Patch = true )
	{
		KKImageButton* pRet = new KKImageButton();

		if( pRet
				&& pRet->initWithLabelAndBackground( text, pSpriteName, is9Patch ) )
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			CC_SAFE_DELETE( pRet );
			return pRet = nullptr;
		}
	}

	static KKImageButton* createWithBackground( const char* pSpriteName,
			bool is9Patch = false )
	{
		KKImageButton* pRet = new KKImageButton();

		if( pRet && pRet->initWithBackgroundSprite( pSpriteName, is9Patch ) )
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			CC_SAFE_DELETE( pRet );
			return pRet = nullptr;
		}
	}

	static KKImageButton* createWithLabelAndBackgroundSprite( CCNode* pNodeLabel,
			CCNode* pNodeBackground )
	{
		KKImageButton* pRet = new KKImageButton();

		if( pRet
				&& pRet->initWithLabelAndBackgroundSprite( pNodeLabel,
						pNodeBackground ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}

	static KKImageButton* createWithBackgroundSprite( CCNode* pNodeBackground )
	{
		KKImageButton* pRet = new KKImageButton();

		if( pRet && pRet->initWithBackgroundSprite( pNodeBackground ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}

	virtual bool initWithLabelAndBackground( const string& text,
			const char* pSpriteName, bool is9Patch = true );
	virtual bool initWithLabelAndBackgroundSprite( CCNode* pNodeLabel,
			CCNode* pNodeBackground );
	virtual bool initWithBackgroundSprite( CCNode* pNodeBackground );
	virtual bool initWithBackgroundSprite( const char* pSpriteName,
										   bool is9Patch = false );

	virtual bool ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchCancelled( CCTouch* pTouch, CCEvent* pEvent );

	KKImageButton* setTarget( CCObject* pTarget, SEL_CCControlHandler callback,
							  CCControlEvent controlEvent = CCControlEventTouchUpInside );

	virtual void setContentSize( const CCSize& contentSize );

	void setText( const string& text );
};

}
/* namespace KoalaGui */
#endif /* KKIMAGEBUTTON_H_ */

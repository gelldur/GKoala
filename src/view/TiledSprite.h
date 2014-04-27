/*
 * TiledSprite.h
 *
 *  Created on: June 6, 2013
 *      Author: Michal Tomczyk
 */

#ifndef CCTILEDSPRITE_H_
#define CCTILEDSPRITE_H_

#include "cocos2d.h"
#include "view/layout/linear/LinearLayout.h"
#include "view/ScissorHelper.h"

USING_NS_CC;

namespace KoalaGui
{

class TiledSprite: public CCSpriteBatchNode, private ScissorHelper
{
public:

	enum Fill
	{
		X, Y, XY
	};

	static TiledSprite* createWithSprite ( CCSprite* pSprite, const Fill fill )
	{
		TiledSprite* pRet = new TiledSprite();

		if ( pRet && pRet->initWithSprite ( pSprite, fill ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE ( pRet );
		return pRet = nullptr;
	}

	static TiledSprite* create ( const char* pTextureName, const Fill fill )
	{
		TiledSprite* pRet = new TiledSprite();

		if ( pRet && pRet->initWithTextureName ( pTextureName, fill ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE ( pRet );
		return pRet = nullptr;
	}

	virtual ~TiledSprite();
	virtual void visit();

	bool initWithSprite ( CCSprite* pSprite, const Fill fill );
	bool initWithTextureName ( const char* pTextureName, const Fill fill );

	virtual void setContentSize ( const CCSize& contentSize );

protected:
	TiledSprite();

private:
	CCRect m_textureRect;
	Fill m_fill;

	void fillX ( const float yPosition = 0 );
	void fillY ( const float xPosition = 0 );
	void fillXY ( const float dummyVariable = 0 );
};

} /* namespace KoalaGui */
#endif /* CCTILEDSPRITE_H_ */

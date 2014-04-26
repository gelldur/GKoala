/*
 * AnimationFrame.h
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#ifndef ANIMATIONFRAME_H_
#define ANIMATIONFRAME_H_

#include "cocos2d.h"
USING_NS_CC;

namespace KoalaLib
{

class AnimationFrame : public CCObject
{
public:

	static AnimationFrame* create ( CCSpriteFrame* pSpriteFrame, float duration )
	{
		AnimationFrame* pRet = new AnimationFrame();

		if ( pRet && pRet->init ( pSpriteFrame, duration ) )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return nullptr;
	}

	virtual ~AnimationFrame();

	CCSpriteFrame* getSpriteFrame()
	{
		return m_pSpriteFrame;
	}

	float getDuration()
	{
		return m_duration;
	}

	virtual bool init ( CCSpriteFrame* pSpriteFrame, float duration );

protected:
	AnimationFrame();

private:
	CCSpriteFrame* m_pSpriteFrame;
	float m_duration;

};

} /* namespace KoalaLib */
#endif /* ANIMATIONFRAME_H_ */

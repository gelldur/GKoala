/*
 * Animation.h
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "view/animation/AnimationFrame.h"
#include "cocos2d.h"
#include "utils/Utils.h"
USING_NS_CC;

using namespace std;

namespace KoalaLib
{

class Animation : public CCObject
{
public:

	friend class AnimationBuilder;

	virtual ~Animation();

	void addSpriteFrame( CCSpriteFrame* pFrame, float duration );
	void addSpriteFrameWithFileName( const char* pFileName, float duration );

	void addAnimationFrame( AnimationFrame* pFrame );
	void addAnimationFrames( vector<AnimationFrame*>& frames );

	bool init();

	float getDuration();

	vector<AnimationFrame*> getFrames();
	AnimationFrame* getFrame( unsigned int index );

	unsigned int getFramesAmount();

protected:
	Animation();

private:
	vector<AnimationFrame*> m_frames;

	static Animation* create()
	{
		Animation* pRet = new Animation();

		if( pRet && pRet->init() )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return nullptr;
	}
};

} /* namespace KoalaLib */
#endif /* ANIMATION_H_ */

/*
 * Animate.h
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#ifndef ANIMATE_H_
#define ANIMATE_H_

#include "view/animation/Animation.h"
#include "cocos2d.h"
USING_NS_CC;

using namespace std;

namespace KoalaLib
{

class Animate: public CCActionInterval
{
public:
	Animate();
	~Animate();

	/** initializes the action with an Animation and will restore the original frame when the animation is over */
	bool initWithAnimation( Animation* pAnimation );

	virtual void startWithTarget( CCNode* pTarget );
	virtual void update( float t );
	virtual void step( float dt );
	virtual CCActionInterval* reverse( void );

	/** creates the action with an Animation and will restore the original frame when the animation is over */
	static Animate* create( Animation* pAnimation )
	{
		Animate* pRet = new Animate();

		if( pRet && pRet->initWithAnimation( pAnimation ) )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return nullptr;
	}

	Animation* getAnimation()
	{
		return m_pAnimation;
	}

protected:
	Animation* m_pAnimation;
	int m_currentFrame;
	float m_currentTime;
	bool m_isDone;

};

} /* namespace KoalaLib */
#endif /* ANIMATE_H_ */

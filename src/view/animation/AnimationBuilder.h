/*
 * AnimationBuilder.h
 *
 *  Created on: Sep 6, 2013
 *      Author: kolo
 */

#ifndef ANIMATIONBUILDER_H_
#define ANIMATIONBUILDER_H_

#include "view/animation/Animation.h"

namespace KoalaLib
{

class AnimationBuilder
{
public:

	virtual ~AnimationBuilder();

	static AnimationBuilder build ();
	Animation* finish();

	AnimationBuilder& addSpriteFrame ( CCSpriteFrame* pFrame, float duration );
	AnimationBuilder& addSpriteFrameWithFileName ( const char* pFileName,
			float duration );
	AnimationBuilder& addAnimationFrame ( AnimationFrame* pFrame );
	AnimationBuilder& addAnimationFrames ( vector<AnimationFrame*>& frames );

private:
	AnimationBuilder ();
	Animation* m_pAnimation;
};

} /* namespace KoalaLib */
#endif /* ANIMATIONBUILDER_H_ */

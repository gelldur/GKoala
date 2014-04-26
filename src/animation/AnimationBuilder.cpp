/*
 * AnimationBuilder.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: kolo
 */

#include "AnimationBuilder.h"

namespace KoalaLib
{

AnimationBuilder::AnimationBuilder () :
	m_pAnimation ( nullptr )
{
	m_pAnimation = Animation::create();
}

AnimationBuilder::~AnimationBuilder()
{
}

AnimationBuilder AnimationBuilder::build ()
{
	return AnimationBuilder ();
}

Animation* AnimationBuilder::finish()
{
	return m_pAnimation;
}

AnimationBuilder& AnimationBuilder::addSpriteFrame ( CCSpriteFrame* pFrame,
		float duration )
{
	m_pAnimation->addSpriteFrame ( pFrame, duration );
	return *this;
}

AnimationBuilder& AnimationBuilder::addSpriteFrameWithFileName (
	const char* pFileName, float duration )
{
	m_pAnimation->addSpriteFrameWithFileName ( pFileName, duration );
	return *this;
}

AnimationBuilder& KoalaLib::AnimationBuilder::addAnimationFrame (
	AnimationFrame* pFrame )
{
	m_pAnimation->addAnimationFrame ( pFrame );
	return *this;
}

AnimationBuilder& AnimationBuilder::addAnimationFrames (
	vector<AnimationFrame*>& frames )
{
	m_pAnimation->addAnimationFrames ( frames );
	return *this;
}

} /* namespace KoalaLib */

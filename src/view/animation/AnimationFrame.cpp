/*
 * AnimationFrame.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#include "AnimationFrame.h"

namespace KoalaLib
{

AnimationFrame::AnimationFrame() :
	m_pSpriteFrame( nullptr ),
	m_duration( 0 )
{
}

AnimationFrame::~AnimationFrame()
{
	CC_SAFE_RELEASE( m_pSpriteFrame );
}

bool AnimationFrame::init( CCSpriteFrame* pSpriteFrame, float duration )
{
	m_pSpriteFrame = pSpriteFrame;
	m_duration = duration;
	m_pSpriteFrame->retain();

	return true;
}

} /* namespace KoalaLib */



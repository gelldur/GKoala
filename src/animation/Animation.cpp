/*
 * Animation.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#include "Animation.h"

namespace KoalaLib
{

Animation::Animation()
{
}

Animation::~Animation()
{
	KK_RELEASE_VECTOR ( m_frames );
}

bool Animation::init()
{
	return true;
}

float Animation::getDuration()
{
	float duration = 0;

	for ( AnimationFrame* pFrame : m_frames )
	{
		duration += pFrame->getDuration();
	}

	return duration;
}

vector<AnimationFrame*> Animation::getFrames()
{
	return m_frames;
}

AnimationFrame* Animation::getFrame ( unsigned int index )
{
	assert ( index < m_frames.size() && index >= 0 );
	return m_frames[index];
}

void Animation::addSpriteFrame ( CCSpriteFrame* pFrame, float duration )
{
	AnimationFrame* pAnimationFrame = AnimationFrame::create ( pFrame, duration );
	m_frames.push_back ( pAnimationFrame );
	pAnimationFrame->retain();
}

void Animation::addSpriteFrameWithFileName ( const char* pFileName,
		float duration )
{
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage (
								pFileName );
	CCRect rect = CCRectZero;
	rect.size = pTexture->getContentSize();
	CCSpriteFrame* pFrame = CCSpriteFrame::createWithTexture ( pTexture, rect );
	addSpriteFrame ( pFrame, duration );
}

void Animation::addAnimationFrame ( AnimationFrame* pFrame )
{
	m_frames.push_back ( pFrame );
	pFrame->retain();
}

void Animation::addAnimationFrames ( vector<AnimationFrame*>& frames )
{
	m_frames.insert ( m_frames.end(), frames.begin(), frames.end() );

	for ( AnimationFrame* pFrame : frames )
	{
		pFrame->retain();
	}
}

unsigned int Animation::getFramesAmount()
{
	return m_frames.size();
}

} /* namespace KoalaLib */

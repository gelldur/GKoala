/*
 * Animate.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: kolo
 */

#include "view/animation/Animate.h"
#include "view/animation/AnimationBuilder.h"
#include <algorithm>

namespace KoalaLib
{

Animate::Animate() :
	m_pAnimation ( nullptr ),
	m_currentFrame ( 0 ),
	m_currentTime ( 0 ),
	m_isDone ( false )
{
}

Animate::~Animate()
{
	CC_SAFE_RELEASE ( m_pAnimation );
}

bool Animate::initWithAnimation ( Animation* pAnimation )
{
	CCAssert ( pAnimation != nullptr,
			   "Animate: argument Animation must be non-NULL" );

	if ( CCActionInterval::initWithDuration ( pAnimation->getDuration() ) == false )
	{
		return false;
	}

	m_currentFrame = 0;

	m_pAnimation = pAnimation;
	m_pAnimation->retain();

	m_currentTime = m_pAnimation->getFrame ( 0 )->getDuration();

	return true;
}

void Animate::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
	CCSprite* pSprite = dynamic_cast<CCSprite*> ( pTarget );
	assert ( pSprite );

	m_currentTime = m_pAnimation->getFrame ( 0 )->getDuration();
	m_currentFrame = 0;
}

void Animate::update ( float t )
{
}

void Animate::step ( float dt )
{
	CCActionInterval::step ( dt );

	AnimationFrame* pFrame = m_pAnimation->getFrame ( m_currentFrame );

	float delta = m_currentTime - dt;

	while ( delta <= 0 )
	{
		++m_currentFrame;

		if ( ( unsigned int ) m_currentFrame >= m_pAnimation->getFramesAmount() )
		{
			m_isDone = true;
			m_currentFrame = 0;
		}

		pFrame = m_pAnimation->getFrame ( m_currentFrame );
		m_currentTime = pFrame->getDuration();
		delta = m_currentTime + delta;
	}

	if ( m_isDone )
	{
		return;
	}

	m_currentTime = delta;
	assert ( m_currentTime > 0 );

	CCSpriteFrame* pFrameToDisplay = pFrame->getSpriteFrame();
	CCSprite* pSprite = dynamic_cast<CCSprite*> ( m_pTarget );
	assert ( pSprite );
	pSprite->setDisplayFrame ( pFrameToDisplay );
}

CCActionInterval* Animate::reverse ( void )
{
	vector<AnimationFrame*> oldFrames = m_pAnimation->getFrames();
	vector<AnimationFrame*> newFrames;

	reverse_copy ( oldFrames.begin(), oldFrames.end(), newFrames.begin() );

	Animation* pNewAnimation = AnimationBuilder::build().addAnimationFrames (
								   newFrames ).finish();

	return create ( pNewAnimation );
}

} /* namespace KoalaLib */

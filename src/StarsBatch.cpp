/*
 * StarsBatch.cpp
 *
 *  Created on: October 7, 2013
 *      Author: Jakub Kuderski
 */

#include "view/StarsBatch.h"
#include "utils/Utils.h"
using namespace KoalaGui;

namespace KoalaLib
{

StarsBatch::StarsBatch() :
	m_starsCount ( 0 )
{
}

StarsBatch::~StarsBatch()
{
}

bool StarsBatch::init ( StarsBatchParams starsParams, float starsCount )
{
	assert ( starsParams.batchFilename.empty() == false );
	assert ( starsParams.maxStarsCount >= 0 );

	m_starsParams = starsParams;
	assert ( starsCount >= 0 );
	m_starsCount = starsCount;

	if ( CCSpriteBatchNode::initWithFile ( m_starsParams.batchFilename.c_str(),
										   m_starsParams.maxStarsCount ) == false )
	{
		return false;
	}

	m_stars = vector<CCSprite*> ( m_starsParams.maxStarsCount, nullptr );

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile (
		m_starsParams.plistFilename.c_str() );

	float spaceBetween = 0;
	float height = 0;

	for ( int i = 0; i < m_starsParams.maxStarsCount; ++i )
	{
		m_stars[i] = CCSprite::createWithSpriteFrameName ( getStarFrameForIndex (
						 i ).c_str() );
		assert ( m_stars[i] );

		if ( spaceBetween == 0 )
		{
			spaceBetween = m_stars[i]->getContentSize().width * 1.5f;
			height = m_stars[i]->getContentSize().height;
			setContentSize (
				CCSize ( spaceBetween * ( m_starsParams.maxStarsCount - 1 ), height ) );
		}

		m_stars[i]->setAnchorPoint ( CCPoint ( 0.5f, 0.0f ) );
		m_stars[i]->setPositionX ( i * spaceBetween );
		addChild ( m_stars[i] );
	}

	return true;
}

string StarsBatch::getStarFrameForIndex ( int index )
{
	assert ( index >= 0 );
	float part = index + 1 - m_starsCount;

	if ( part < 0.25F )
	{
		return m_starsParams.onStarName;
	}

	if ( part >= 0.25F && part <= 0.75F )
	{
		return m_starsParams.halfStarName;
	}
	else
	{
		return m_starsParams.offStarName;
	}
}

void StarsBatch::updateStarsCount ( float starsCount )
{
	assert ( starsCount >= 0 );

	if ( m_starsCount == starsCount )
	{
		return;
	}

	m_starsCount = starsCount;

	int i = 0;

	for ( CCSprite* pSprite : m_stars )
	{
		CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()
								->spriteFrameByName ( getStarFrameForIndex ( i++ ).c_str() );
		assert ( pFrame );
		pSprite->setTextureRect ( pFrame->getRect() );
	}
}

} //namespace KoalaLib

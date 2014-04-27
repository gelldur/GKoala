/*
 * TiledSprite.cpp
 *
 *  Created on: June 6, 2013
 *      Author: Michal Tomczyk
 */

#include "view/TiledSprite.h"
#include <assert.h>
namespace KoalaGui
{

TiledSprite::TiledSprite() :
	m_fill ( XY )
{

}

TiledSprite::~TiledSprite()
{
}

bool TiledSprite::initWithSprite ( CCSprite* pSprite, const Fill fill )
{
	if ( CCSpriteBatchNode::initWithTexture ( pSprite->getTexture(), 32 ) == false )
	{
		return false;
	}

	m_fill = fill;
	m_textureRect = pSprite->getTextureRect();
	setContentSize ( pSprite->getContentSize() );

	return true;
}

bool TiledSprite::initWithTextureName ( const char* pTextureName,
										const Fill fill )
{
	if ( CCSpriteBatchNode::initWithFile ( pTextureName, 32 ) == false )
	{
		return false;
	}

	m_textureRect.size = getTexture()->getContentSize();
	setContentSize ( m_textureRect.size );

	return true;
}

void TiledSprite::visit()
{
	kmGLPushMatrix();

	beforeDraw ( getViewRect ( this ) );

	CCSpriteBatchNode::visit();

	afterDraw();

	kmGLPopMatrix();
}

void TiledSprite::setContentSize ( const CCSize& contentSize )
{
	CCSpriteBatchNode::setContentSize ( contentSize );

	removeAllChildren();

	switch ( m_fill )
	{
		case Fill::X:
			fillX();
			break;

		case Fill::Y:
			fillY();
			break;

		case Fill::XY:
			fillXY();
			break;

		default:
			CCAssert ( false, "Wrong Fill Value" );
			break;
	}
}

void TiledSprite::fillX ( const float yPosition /*= 0*/ )
{
	float translationX = 0;

	while ( translationX <= getContentSize().width )
	{
		CCSprite* pSprite = CCSprite::createWithTexture ( getTexture(), m_textureRect );
		addChild ( pSprite );
		pSprite->setAnchorPoint ( CCPointZero );
		pSprite->setPosition ( ccp ( translationX, yPosition ) );

		translationX += m_textureRect.size.width;
	}
}

void TiledSprite::fillY ( const float xPosition /*= 0*/ )
{
	float translationY = getContentSize().height - m_textureRect.size.height;

	while ( translationY > -m_textureRect.size.height )
	{
		CCSprite* pSprite = CCSprite::createWithTexture ( getTexture(), m_textureRect );
		addChild ( pSprite );
		pSprite->setAnchorPoint ( CCPointZero );
		pSprite->setPosition ( ccp ( xPosition, translationY ) );

		translationY -= m_textureRect.size.height;
	}
}

void TiledSprite::fillXY ( const float dummyVariable/*= 0*/ )
{
	CC_UNUSED_PARAM ( dummyVariable );

	float translationY = getContentSize().height - m_textureRect.size.height;

	while ( translationY > -m_textureRect.size.height )
	{
		fillX ( translationY );
		translationY -= m_textureRect.size.height;
	}
}

} /* namespace KoalaGui */

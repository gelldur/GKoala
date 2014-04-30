/*
 * KKScaleSprite.cpp
 *
 *  Created on: May 20, 2013
 *      Author: Dawid Drozd
 */

#include "view/KKScaleSprite.h"
#include "utils/Utils.h"

KKScaleSprite::KKScaleSprite( Scale::Type scaleType ) :
	m_scaleType( scaleType )
{
}

KKScaleSprite::~KKScaleSprite()
{
}

KKScaleSprite* KKScaleSprite::create( const char* pFileName,
									  Scale::Type scaleType )
{
	KKScaleSprite* pSprite = new KKScaleSprite( scaleType );

	if( pSprite && pSprite->initWithFile( pFileName ) )
	{
		pSprite->autorelease();
		return pSprite;
	}

	CC_SAFE_DELETE( pSprite );
	return nullptr;
}

void KKScaleSprite::setContentSize( const CCSize& contentSize )
{
	if( getContentSize().equals( CCSizeZero ) )
	{
		//Init of content size
		CCSprite::setContentSize( contentSize );
	}

	CCSize oryginalSize( getContentSize() );

	if( contentSize.width == oryginalSize.width )
	{
		oryginalSize.width *= getScaleX();
	}
	else
	{
		oryginalSize.width = contentSize.width;
	}

	if( contentSize.height == oryginalSize.height )
	{
		oryginalSize.height *= getScaleY();
	}
	else
	{
		oryginalSize.height = contentSize.height;
	}

	Scale::scale( this, m_scaleType, getContentSize(), oryginalSize );
}

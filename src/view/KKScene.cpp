/*
 * KKScene.cpp
 *
 *  Created on: May 10, 2013
 *      Author: Dawid Drozd
 */

#include "view/KKScene.h"

#include "view/KKImageButton.h"

namespace KoalaGui
{

KKScene::KKScene() :
	m_isKeypadEnabled( false )
{
}

KKScene::~KKScene()
{
}

bool KKScene::init()
{
	if( CCScene::init() == false )
	{
		CCAssert( false, "Failed to create CCScene" );
		return false;
	}

	return true;
}

void KKScene::onBack()
{
	CCDirector::sharedDirector()->popScene();
}

void KKScene::setKeypadEnabled( bool isEnabled )
{
	if( isEnabled == m_isKeypadEnabled )
	{
		return;
	}

	m_isKeypadEnabled = isEnabled;

	if( m_bRunning )
	{
		CCDirector* pDirector = CCDirector::sharedDirector();

		if( isEnabled )
		{
			pDirector->getKeypadDispatcher()->addDelegate( this );
		}
		else
		{
			pDirector->getKeypadDispatcher()->removeDelegate( this );
		}
	}
}

void KKScene::onEnter()
{
	CCScene::onEnter();

	// add this layer to concern the keypad msg
	if( m_isKeypadEnabled )
	{
		CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate( this );
	}
}

void KKScene::onExit()
{
	CCScene::onExit();

	// remove this layer from the delegates who concern the keypad msg
	if( m_isKeypadEnabled )
	{
		CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(
			this );
	}
}

} /* namespace KoalaGui */


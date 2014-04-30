/*
 * ClickCounterComponent.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: filip
 */

#include "view/ClickCounterComponent.h"
#include "database/components/key/value/KeyValueManager.h"
#include "actions/ActionPack.h"

namespace KoalaLib
{

ClickCounterComponent::ClickCounterComponent() :
	m_pTarget( nullptr ),
	m_threshold( 0 ),
	m_actionId( 1245 ),
	m_scale( 0 )
{
}

bool ClickCounterComponent::init( const string& key, int threshold )
{
	if( key.empty() )
	{
		assert( false );
		return false;
	}

	m_key = key;
	m_threshold = threshold;

	return true;
}


void ClickCounterComponent::onEnter()
{
	database::KeyValueManager manager;
	m_scale = m_pTarget->getScale();

	if( manager.getValue( m_key, 0 ).asInteger() < m_threshold )
	{
		assert( m_pTarget != nullptr );
		CCAction* pAction = m_pTarget->runAction( getAction() );
		pAction->setTag( m_actionId );
	}
}

void ClickCounterComponent::onExit()
{
	assert( m_pTarget != nullptr );
	m_pTarget->stopActionByTag( m_actionId );
	m_pTarget->setScale( m_scale );

	//TODO :(
	if( dynamic_cast<CCNodeRGBA*>( m_pTarget ) != nullptr )
	{
		dynamic_cast<CCNodeRGBA*>( m_pTarget )->setOpacity( 255 );
	}

}

CCAction* BumpClickCounterComponent::getAction()
{
	assert( m_pTarget != nullptr );
	return ActionPack::getActionBump( m_pTarget->getScale() );
}

} /* namespace KoalaLib */



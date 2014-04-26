/*
 * MarqueeLabel.cpp
 *
 *  Created on: July 1, 2013
 *      Author: Jakub Kuderski
 */

#include "view/MarqueeLabel.h"
#include "view/layout/Positioner.h"
#include "utils/Utils.h"
#include <algorithm>
using namespace KoalaGui;

MarqueeLabel::MarqueeLabel() :
	m_MOVE_OFFSET ( 100.F ),
	m_alignmentOffset ( 0 ),
	m_pLabel ( nullptr ),
	m_alignment ( kCCTextAlignmentLeft )
{

}

MarqueeLabel::~MarqueeLabel()
{
}

bool MarqueeLabel::initWithString ( const string& text, const string& fontName,
									float fontSize )
{
	if ( CCNode::init() == false )
	{
		return false;
	}

	m_pLabel = CCLabelTTF::create ( text.c_str(), fontName.c_str(), fontSize );

	if ( m_pLabel == nullptr )
	{
		return false;
	}

	m_pLabel->setAnchorPoint ( CCPointZero );

	addChild ( m_pLabel );
	setContentSize ( m_pLabel->getContentSize() );
	return true;
}

void MarqueeLabel::onEnter()
{
	CCNode::onEnter();
	restartMovingActionOnText();
}

void MarqueeLabel::onExit()
{
	CCNode::onExit();
	stopActionOnText();
}

void MarqueeLabel::visit()
{
	//You don't set size?
	assert ( getContentSize().equals ( CCSizeZero ) == false );
	const CCRect& rect = getViewRect ( this );

	//TODO throw this code to ScisorHelper as method eg. is Visible on Screen
	if ( rect.origin.x + rect.size.width < 0
			|| rect.origin.x > CCDirector::sharedDirector()->getWinSize().width )
	{
		//We don't see it
		return;
	}

	kmGLPushMatrix();

	beforeDraw ( rect );

	CCNode::visit();

	if ( m_pLabel->getPositionX() <= - ( m_MOVE_OFFSET + m_alignmentOffset )
			&& m_pLabel->getActionByTag ( MAIN_ANIMATION ) != nullptr )
	{
		float offset = m_MOVE_OFFSET + m_pLabel->getContentSize().width;
		setPositionX ( getPositionX() + offset );
		CCNode::visit();
		setPositionX ( getPositionX() - offset );
	}

	afterDraw();

	kmGLPopMatrix();
}

void MarqueeLabel::applyMovingActionOnText ( const float frequency/* = 1*/,
		const float delay/* = 1.5*/, const int repeats/* = -1*/ )
{
	assert ( repeats >= -1 && frequency > 0 );
	float time = ( 1 / frequency ) * Positioner::getScaledWidth ( m_pLabel ) * 0.02;

	m_parameters.frequency = frequency;
	m_parameters.time = time;
	m_parameters.delay = delay;
	m_parameters.repeats = repeats;

	stopActionOnText();

	if ( getContentSize().width >= m_pLabel->getContentSize().width || time == 0 )
	{
		//We dont want play aimation if we have desired space or 0 time
		return;
	}

	runActionOnText ( getDefaultMovingAction ( time, delay, repeats ) );
}

void KoalaGui::MarqueeLabel::setTextAlignment ( CCTextAlignment alignment )
{
	m_alignment = alignment;

	if ( alignment == kCCTextAlignmentLeft )
	{
		Positioner::leftEdge ( m_pLabel );
	}
	else if ( alignment == kCCTextAlignmentCenter )
	{
		Positioner::centerInParentHorizontally ( m_pLabel );
	}
	else if ( alignment == kCCTextAlignmentRight )
	{
		Positioner::rightEdge ( m_pLabel );
	}

	m_alignmentOffset = m_pLabel->getPositionX();

	if ( m_alignmentOffset < 0 )
	{
		m_alignmentOffset = 0;
		m_pLabel->setPositionX ( 0 );
	}

	restartMovingActionOnText();
}

void KoalaGui::MarqueeLabel::setContentSize ( const CCSize& contentSize )
{
	CCNode::setContentSize ( contentSize );

	setTextAlignment ( m_alignment );
}

CCAction* MarqueeLabel::getDefaultMovingAction ( float time, float delay,
		int repeats )
{
	assert ( time > 0 );
	assert ( delay >= 0 );
	CCMoveBy* pMove = CCMoveBy::create ( time,
										 CCPoint ( - ( m_pLabel->getContentSize().width + m_MOVE_OFFSET
												 + m_alignmentOffset ), 0 ) );

	CCMoveTo* pReturn = CCMoveTo::create ( m_alignmentOffset,
										   CCPoint ( m_alignmentOffset, m_pLabel->getPositionY() ) );

	CCSequence* pSequence = CCSequence::create ( CCDelayTime::create ( delay ),
							pMove, pReturn, static_cast<void*> ( nullptr ) ); //VC

	CCAction* pDefaultAction = nullptr;

	if ( repeats == -1 )
	{
		pDefaultAction = CCRepeatForever::create ( pSequence );
	}
	else
	{
		pDefaultAction = CCRepeat::create ( pSequence, repeats );
	}

	pDefaultAction->setTag ( ID::MAIN_ANIMATION );

	return pDefaultAction;
}

void MarqueeLabel::setString ( const char* pText )
{
	m_pLabel->setString ( pText );
	setTextAlignment ( m_alignment );
	restartMovingActionOnText();
}

void MarqueeLabel::stopActionOnText()
{
	m_pLabel->stopActionByTag ( ID::MAIN_ANIMATION );
}

void MarqueeLabel::runActionOnText ( CCAction* pAction )
{
	m_pLabel->runAction ( pAction );
}

const char* MarqueeLabel::getString()
{
	return m_pLabel->getString();
}

void MarqueeLabel::restartMovingActionOnText()
{
	applyMovingActionOnText ( m_parameters.frequency, m_parameters.delay,
							  m_parameters.repeats );
}

void KoalaGui::MarqueeLabel::setColor ( const ccColor3B& color )
{
	m_pLabel->setColor ( color );
}

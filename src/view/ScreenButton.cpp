/*
 * ScreenButton.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: filip
 */

#include "view/ScreenButton.h"
#include "utils/string/Utf8.h"

namespace KoalaGui
{

ScreenButton::ScreenButton() :
	m_pBackground ( nullptr ),
	m_pLabel ( nullptr ),
	m_pScreen ( nullptr ),
	m_firstLetterWidth ( 0 )
{
}

ScreenButton::~ScreenButton()
{
}

bool ScreenButton::init ( const ScreenButtonSettings& settings )
{
	if ( RelativeLayout::init() == false )
	{
		return false;
	}

	m_pBackground = CCScale9Sprite::create ( settings.backgroundFile.c_str() );
	{
		setBackground ( View::create ( m_pBackground,
									   new LayoutParams ( LayoutParamsMatchMatch ) ) );
	}

	m_pLabel = MarqueeLabel::create ( "Word", settings.labelFont.c_str(),
									  settings.labelFontSize );
	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule ( AlignType::PARENT_LEFT );
		params.addRule ( AlignType::PARENT_CENTER_VERTICAL );
		params.setMarginLeft ( SizePolicy ( Policy::SP, 10 ) );
		addView ( m_pLabel, params );

		m_pLabel->setColor ( settings.fontColor );
		m_pLabel->setTextAlignment ( kCCTextAlignmentLeft );
		m_pLabel->setAnchorPoint ( CCPoint ( 0.F, 0.F ) );
		m_pLabel->stopActionOnText();
	}

	m_pScreen = CCLayerColor::create ( settings.screenColor );
	{
		RelativeLayoutParams params ( LayoutParamsMatchMatch );
		params.addRule ( AlignType::PARENT_LEFT );
		params.addRule ( AlignType::PARENT_CENTER_VERTICAL );
		addView ( m_pScreen, params );

		m_pScreen->setAnchorPoint ( CCPoint ( 1, 0.5F ) );
	}

	m_settings = settings;

	return true;
}

void ScreenButton::onEnter()
{
	RelativeLayout::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate (
		this, INT16_MAX, true );
}

void ScreenButton::setText ( const string& text )
{
	m_pLabel->setString ( text.c_str() );

	CCSize size = m_pLabel->getLabel()->getContentSize();

	if ( size.width > getContentSize().width - 20 )
	{
		size.width = getContentSize().width - 20;
	}

	CCLabelTTF* pLabel = CCLabelTTF::create (
							 Utf8::getFirstLetter ( m_pLabel->getLabel()->getString() ).c_str(),
							 m_settings.labelFont.c_str(),
							 m_settings.labelFontSize );

	m_firstLetterWidth = pLabel->getContentSize().width;

	m_pLabel->setContentSize ( size );

	m_pLabel->stopActionOnText();
}

void ScreenButton::onExit()
{
	RelativeLayout::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate ( this );
}

void ScreenButton::setColor ( const ccColor3B& color )
{
	m_pBackground->setColor ( color );
}

void ScreenButton::setWordVisibility ( float value )
{
	float labelWidth = 0;

	if ( m_pLabel->getLabel()->getContentSize().width >
			m_pLabel->getContentSize().width )
	{
		labelWidth = m_pLabel->getContentSize().width;
	}
	else
	{
		labelWidth = m_pLabel->getLabel()->getContentSize().width;
	}

	//the hardcoded 10 is for left margin
	float result = ( m_pScreen->getContentSize().width -
					 labelWidth * ( value ) -
					 SizePolicy ( Policy::SP,
								  m_firstLetterWidth + 10 ).getValue() ) / m_pScreen->getContentSize().width;

	m_pScreen->setScaleX ( result );
}

void ScreenButton::setScreenVisibility ( bool value )
{
	if ( m_pScreen->isVisible() == value )
	{
		return;
	}

	m_pScreen->setVisible ( value );
}

} /* namespace KoalaGui */



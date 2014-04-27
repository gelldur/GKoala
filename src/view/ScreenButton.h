/*
 * ScreenButton.h
 *
 *  Created on: Nov 22, 2013
 *      Author: filip
 */

#ifndef SCREENBUTTON_H_
#define SCREENBUTTON_H_

#include "view/layout/relative/RelativeLayout.h"
#include "view/MarqueeLabel.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

namespace KoalaGui
{

struct ScreenButtonSettings
{
	string backgroundFile;
	string labelFont;
	float labelFontSize;
	ccColor3B fontColor;
	ccColor4B screenColor;
};

class ScreenButton: public KoalaGui::RelativeLayout
{
public:
	static ScreenButton* create ( const ScreenButtonSettings& settings )
	{
		ScreenButton* pRet = new ScreenButton();

		if ( pRet && pRet->init ( settings ) )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return nullptr;
	}

	bool init ( const ScreenButtonSettings& settings );

	virtual void setColor ( const ccColor3B& color );

	void setWordVisibility ( float value );

	void setScreenVisibility ( bool value );

	void setText ( const string& text );

	virtual void onEnter();
	virtual void onExit();

	virtual ~ScreenButton();

protected:
	ScreenButton();

private:
	enum ID
	{
		LABEL = 12546,
		SCREEN
	};
	CCScale9Sprite* m_pBackground;
	MarqueeLabel* m_pLabel;
	CCLayerColor* m_pScreen;
	float m_firstLetterWidth;

	ScreenButtonSettings m_settings;

};

} /* namespace KoalaGui */

#endif /* SCREENBUTTON_H_ */

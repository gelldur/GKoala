/*
 * MarqueeLabel.h
 *
 *  Created on: July 1, 2013
 *      Author: Jakub Kuderski
 */

#ifndef MARQUEELABEL_H
#define MARQUEELABEL_H

#include "cocos2d.h"
#include <string>
#include "view/ScissorHelper.h"

USING_NS_CC;
using namespace std;

namespace KoalaGui
{

class MarqueeLabel: public CCNode, public CCLabelProtocol, private ScissorHelper
{
public:
	static MarqueeLabel* create( const string& text, const string& fontName,
								 float fontSize )
	{
		MarqueeLabel* pRet = new MarqueeLabel();

		if( pRet && pRet->initWithString( text, fontName, fontSize ) )
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			return pRet = nullptr;
		}
	}

	virtual bool initWithString( const string& text, const string& fontName,
								 float fontSize );

	virtual ~MarqueeLabel();

	virtual void onEnter();
	virtual void onExit();

	virtual void visit();

	virtual void setString( const char* pText );
	virtual const char* getString();

	/**
	 * @param frequency
	 * @param delay
	 * @param repeats -1 repeat for ever
	 */
	void applyMovingActionOnText( const float frequency = 1,
								  const float delay = 1.5F,
								  const int repeats = -1 );

	void restartMovingActionOnText();

	CCLabelTTF* getLabel() const
	{
		return m_pLabel;
	}

	//Its only virtual for feature
	virtual void setColor( const ccColor3B& color );

	void runActionOnText( CCAction* pAction );
	void stopActionOnText();

	enum ID
	{
		MAIN_ANIMATION = 678
	};

	void setTextAlignment( CCTextAlignment alignment );

	virtual void setContentSize( const CCSize& contentSize );

protected:
	MarqueeLabel();

private:
	const float m_MOVE_OFFSET;
	float m_alignmentOffset;
	CCLabelTTF* m_pLabel;
	CCTextAlignment m_alignment;

	struct Parameters
	{
		float frequency;
		float time;
		float delay;
		int repeats;

		Parameters() :
			frequency( 1 ),
			time( 0 ),
			delay( 1 ),
			repeats( -1 )
		{
		}
	} m_parameters;

	CCAction* getDefaultMovingAction( float time, float delay, int repeats );
};

} // namespace KoalaGui

#endif // MARQUEELABEL_H

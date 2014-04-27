/*
 * ScaleSprite.h
 *
 *  Created on: May 20, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCSCALESPRITE_H_
#define CCSCALESPRITE_H_

#include "cocos2d.h"
#include "view/Scale.h"
USING_NS_CC;

class KKScaleSprite: public CCSprite
{
public:
	virtual ~KKScaleSprite();

	static KKScaleSprite* create ( const char* pFileName, Scale::Type scaleType =
									   Scale::Type::NONE );

	virtual void setContentSize ( const CCSize& contentSize );

protected:
	KKScaleSprite ( Scale::Type scaleType );

private:
	const Scale::Type m_scaleType;

	KKScaleSprite ( const KKScaleSprite& ); //delete
	KKScaleSprite& operator= ( const KKScaleSprite& ); //delete
	KKScaleSprite ( KKScaleSprite&& ); //delete
	KKScaleSprite& operator= ( KKScaleSprite && ); //delete

};

#endif /* CCSCALESPRITE_H_ */

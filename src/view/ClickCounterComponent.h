/*
 * ClickCounterComponent.h
 *
 *  Created on: Nov 18, 2013
 *      Author: filip
 */

#ifndef CLICKCOUNTERCOMPONENT_H_
#define CLICKCOUNTERCOMPONENT_H_

#include "utils/Utils.h"
#include "cocos2d.h"
USING_NS_CC;

namespace KoalaLib
{

//TODO this should work as component
class ClickCounterComponent : public CCObject
{
public:
	virtual bool init( const std::string& key, int threshold );

	virtual CCAction* getAction() = 0;

	void onEnter();
	void onExit();

	void setTarget( CCNode* pNode )
	{
		m_pTarget = pNode;
	}

protected:
	ClickCounterComponent();

	CCNode* m_pTarget;

private:
	std::string m_key;
	int m_threshold;
	const int m_actionId;
	float m_scale;
};

class BumpClickCounterComponent : public ClickCounterComponent
{
public:
	static BumpClickCounterComponent* create( const std::string& key,
			int threshold )
	{
		BumpClickCounterComponent* pRet = new BumpClickCounterComponent();

		if( pRet && pRet->init( key, threshold ) )
		{
			pRet->autorelease();
			return pRet;
		}

		delete pRet;
		pRet = nullptr;

		return pRet;
	}

	virtual CCAction* getAction();
};

} /* namespace KoalaLib */

#endif /* CLICKCOUNTERCOMPONENT_H_ */

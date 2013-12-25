/*
 * Layout.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "Config.h"
#include "interface/LayoutInterface.h"
#include "layout/configuration/LayoutConfiguration.h"

namespace GKoala
{

class Layout : public cocos2d::CCNode , public LayoutInterface
{
private:
	typedef CCNode inherited;

public:
	static Layout* createWithConfiguration ( LayoutConfiguration* pConfiguration )
	{
		Layout* pRet = new Layout();

		if ( pRet && pRet->initWithConfiguration ( pConfiguration ) )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE ( pRet );
		return pRet = nullptr;
	}

	virtual ~Layout();

	virtual bool initWithConfiguration ( LayoutConfiguration* pConfiguration );

protected:
	Layout();

private:
	LayoutConfiguration* m_pLayoutConfiguration;
};

} /* namespace GKoala */

#endif /* LAYOUT_H_ */
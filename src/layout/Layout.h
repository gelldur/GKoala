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
#include <vector>

//TODO
// 1. should layouts update while running? I mean we run CCScaleAction on child

namespace GKoala
{

class Layout : public LayoutInterface
{
private:
	typedef LayoutInterface inherited;

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

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void addChild ( cocos2d::CCNode* pChild, int zOrder, int tag ) override;
	virtual void addChildWith ( cocos2d::CCNode* pChild,
								LayoutParameter* pLayoutParameter ) override;

	virtual void removeChild ( CCNode* pChild, bool cleanup ) override;
	virtual void updateStructure() override;

protected:
	Layout();

private:
	LayoutConfiguration* m_pLayoutConfiguration;

	void onUpdateStructureCallback ( CCObject* pCaller );

	const char* getNotificationUpdateStructure() const
	{
		return "_notify_update_structure";
	}
};

} /* namespace GKoala */

#endif /* LAYOUT_H_ */

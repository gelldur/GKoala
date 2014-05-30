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

template < typename Base = cocos2d::CCNode >
class Layout : public Base,  public LayoutInterface
{
private:
	typedef Base inherited;

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

	virtual ~Layout()
	{
		CC_SAFE_RELEASE_NULL ( m_pLayoutConfiguration );
	}

	virtual bool initWithConfiguration ( LayoutConfiguration* pConfiguration )
	{
		if ( inherited::init() == false )
		{
			return false;
		}

		m_pLayoutConfiguration = pConfiguration;
		pConfiguration->setWorkingLayout(this);

		if ( pConfiguration == nullptr )
		{
			GKoala_assert ( pConfiguration != nullptr,
							"You can't init Layout with null configuration" );
			return false;
		}

		pConfiguration->retain();

		return true;
	}

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void addChild ( cocos2d::CCNode* pChild, int zOrder, int tag ) override
	{
		GKoala_assert ( pChild != nullptr, "pChild can't be null" );
		GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );

		pChild->setZOrder ( zOrder );
		pChild->setTag ( tag );

		addChildWith ( pChild, m_pLayoutConfiguration->getDefaultLayoutParameter() );
	}
	virtual void addChildWith ( cocos2d::CCNode* pChild,LayoutParameter* pLayoutParameter ) override
	{
		GKoala_assert ( pChild != nullptr, "pChild can't be null" );
		GKoala_assert ( pLayoutParameter != nullptr, "pLayoutParameter can't be null" );
		GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );

		inherited::addChild ( pChild, pChild->getZOrder(), pChild->getTag() );
		m_pLayoutConfiguration->addView ( pChild, pLayoutParameter );

		updateStructure(nullptr);
	}

	virtual void removeChild ( CCNode* pChild, bool cleanup ) override
	{
		GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );
		inherited::removeChild ( pChild, cleanup );

		updateStructure(nullptr);
	}
	virtual void updateStructure(LayoutParameter* pLayoutParameter) override
	{
		GKoala_assert ( m_pLayoutConfiguration != nullptr, "Wrong initialization!" );
		m_pLayoutConfiguration->updateStructure(pLayoutParameter);
	}

	virtual void setOptions(int options) override
	{
		m_pLayoutConfiguration->setOptions(options);
	}

protected:
	Layout();

private:
	LayoutConfiguration* m_pLayoutConfiguration = nullptr;

	void onUpdateStructureCallback ( CCObject* pCaller )
	{
		updateStructure(nullptr);
	}
};

} /* namespace GKoala */

#endif /* LAYOUT_H_ */

/*
 * LayoutFactory.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTFACTORY_H_
#define LAYOUTFACTORY_H_

#include "interface/LayoutFactoryInterface.h"

#include "layout/Layout.h"

#include "layout/configuration/linear/LinearConfiguration.h"
#include "layout/configuration/relative/RelativeConfiguration.h"

namespace GKoala
{

/**
 * Abstract Factory pattern
 */
class LayoutFactory : public LayoutFactoryInterface
{
public:
	virtual ~LayoutFactory() {}

	virtual cocos2d::CCNode* createLinearLayout()
	{
		return Layout::createWithConfiguration ( LinearConfiguration::create() );
	}
	virtual cocos2d::CCNode* createRelativeLayout()
	{
		return Layout::createWithConfiguration ( RelativeConfiguration::create() );
	}
};

} /* namespace GKoala */

#endif /* LAYOUTFACTORY_H_ */

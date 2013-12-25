/*
 * LayoutFactoryInterface.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTFACTORYINTERFACE_H_
#define LAYOUTFACTORYINTERFACE_H_

#include "Config.h"

namespace GKoala
{

/**
 * Abstract Factory pattern
 */
class LayoutFactoryInterface
{
public:
	virtual ~LayoutFactoryInterface() {}

	virtual cocos2d::CCNode* createLinearLayout() = 0;
	virtual cocos2d::CCNode* createRelativeLayout() = 0;
};

} /* namespace GKoala */

#endif /* LAYOUTFACTORYINTERFACE_H_ */

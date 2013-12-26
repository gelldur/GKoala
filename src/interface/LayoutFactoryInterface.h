/*
 * LayoutFactoryInterface.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTFACTORYINTERFACE_H_
#define LAYOUTFACTORYINTERFACE_H_

#include "Config.h"
#include "interface/LayoutInterface.h"

namespace GKoala
{

/**
 * Abstract Factory pattern
 */
class LayoutFactoryInterface
{
public:
	virtual ~LayoutFactoryInterface() {}

	virtual LayoutInterface* createLinearLayout() = 0;
	virtual LayoutInterface* createRelativeLayout() = 0;
};

} /* namespace GKoala */

#endif /* LAYOUTFACTORYINTERFACE_H_ */

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

	virtual LayoutInterface* createLinearLayout() override
	{
		return Layout::createWithConfiguration ( LinearConfiguration::create() );
	}
	virtual LayoutInterface* createRelativeLayout() override
	{
		return Layout::createWithConfiguration ( RelativeConfiguration::create() );
	}
};

} /* namespace GKoala */

#endif /* LAYOUTFACTORY_H_ */

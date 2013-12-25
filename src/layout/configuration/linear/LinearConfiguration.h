/*
 * LinearConfiguration.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LINEARCONFIGURATION_H_
#define LINEARCONFIGURATION_H_

#include "layout/configuration/LayoutConfiguration.h"

namespace GKoala
{

class LinearConfiguration : public LayoutConfiguration
{
public:
	GK_CREATE ( LinearConfiguration )
	virtual ~LinearConfiguration();
private:
	LinearConfiguration();
};

} /* namespace GKoala */

#endif /* LINEARCONFIGURATION_H_ */

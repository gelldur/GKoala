/*
 * RelativeConfiguration.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef RELATIVECONFIGURATION_H_
#define RELATIVECONFIGURATION_H_

#include "layout/configuration/LayoutConfiguration.h"

namespace GKoala
{

class RelativeConfiguration: public LayoutConfiguration
{
public:
	GK_CREATE ( RelativeConfiguration )
	virtual ~RelativeConfiguration();

private:
	RelativeConfiguration();
};

} /* namespace GKoala */

#endif /* RELATIVECONFIGURATION_H_ */

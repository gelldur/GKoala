/*
 * LayoutInterface.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTINTERFACE_H_
#define LAYOUTINTERFACE_H_

#include "layout/LayoutParameter.h"

namespace GKoala
{

class LayoutInterface
{
public:
	virtual ~LayoutInterface() {};

	virtual void addChild ( cocos2d::CCNode* pNode,
							LayoutParameter* pLayoutParameter ) = 0;
};

} /* namespace GKoala */

#endif /* LAYOUTINTERFACE_H_ */

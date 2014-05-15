/*
 * LayoutInterface.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTINTERFACE_H_
#define LAYOUTINTERFACE_H_

#include "Config.h"

namespace GKoala
{

class LayoutParameter;

class LayoutInterface : public cocos2d::CCNode
{
public:
	virtual ~LayoutInterface() {};
	virtual void addChildWith ( cocos2d::CCNode* pNode,LayoutParameter* pLayoutParameter ) = 0;
	virtual void updateStructure(LayoutParameter* pLayoutParameter) = 0;
};

} /* namespace GKoala */

#endif /* LAYOUTINTERFACE_H_ */

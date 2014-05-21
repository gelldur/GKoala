/*
 * Utils.cpp
 *
 *  Created on: May 15, 2014
 *      Author: dawid
 */

#include "layout/configuration/Utils.h"

using namespace cocos2d;

namespace GKoala
{

namespace Utils
{
CCSize getFullSize(cocos2d::CCNode* pNode,LayoutParameter* pParameter)
{
	CCSize size = pNode->getContentSize();
	size = size + pParameter->getFullSize();
	return size;
}

}

} /* namespace GKoala */



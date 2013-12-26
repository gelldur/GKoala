/*
 * RelativeConfiguration.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/configuration/relative/RelativeConfiguration.h"

namespace GKoala
{

RelativeConfiguration::RelativeConfiguration()
{
}

RelativeConfiguration::~RelativeConfiguration()
{
}

void RelativeConfiguration::addView ( cocos2d::CCNode* pChild,
									  LayoutParameter* pLayoutParameter )
{
}

void RelativeConfiguration::removeChild ( cocos2d::CCNode* pChild )
{
}

LayoutParameter* RelativeConfiguration::getDefaultLayoutParameter()
{
	return nullptr;
}

void RelativeConfiguration::updateStructure()
{
}

void RelativeConfiguration::onMeasure()
{
}

void RelativeConfiguration::onLayout()
{
}

} /* namespace GKoala */

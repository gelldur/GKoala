/*
 * RelativeConfiguration.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/configuration/relative/RelativeConfiguration.h"

namespace GKoala
{

void RelativeConfiguration::setOptions(int options)
{
}

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

void RelativeConfiguration::updateStructure(LayoutParameter* pLayoutParameter)
{
}

cocos2d::CCSize RelativeConfiguration::onMeasure(LayoutParameter* pLayoutParameter)
{
	return cocos2d::CCSizeZero;
}

void RelativeConfiguration::onLayout()
{
}

} /* namespace GKoala */

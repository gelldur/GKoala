/*
 * LayoutConfiguration.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTCONFIGURATION_H_
#define LAYOUTCONFIGURATION_H_

#include "Config.h"
#include "layout/LayoutParameter.h"

namespace GKoala
{

class LayoutConfiguration : public cocos2d::CCObject
{
public:
	LayoutConfiguration();
	virtual ~LayoutConfiguration();

	virtual void addView ( cocos2d::CCNode* pChild,
						   LayoutParameter* pLayoutParameter ) = 0;
	virtual void removeChild ( cocos2d::CCNode* pChild ) = 0;

	//Abstract method factory
	virtual LayoutParameter* getDefaultLayoutParameter() = 0;

	virtual void updateStructure() = 0;

protected:

	virtual void onMeasure() = 0;
	virtual void onLayout() = 0;
};

} /* namespace GKoala */

#endif /* LAYOUTCONFIGURATION_H_ */

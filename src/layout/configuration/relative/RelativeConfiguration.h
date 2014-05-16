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

	virtual void addView ( cocos2d::CCNode* pChild,
						   LayoutParameter* pLayoutParameter ) override;
	virtual void removeChild ( cocos2d::CCNode* pChild ) override;

	//Abstract method factory
	virtual LayoutParameter* getDefaultLayoutParameter() override;

	virtual void updateStructure(LayoutParameter* pLayoutParameter) override;

	virtual void setOptions(int options) override;

protected:

	virtual cocos2d::CCSize onMeasure(LayoutParameter* pLayoutParameter) override;
	virtual void onLayout() override;

private:
	RelativeConfiguration();
};

} /* namespace GKoala */

#endif /* RELATIVECONFIGURATION_H_ */

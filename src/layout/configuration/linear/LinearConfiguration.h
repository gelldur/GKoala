/*
 * LinearConfiguration.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LINEARCONFIGURATION_H_
#define LINEARCONFIGURATION_H_

#include <vector>
#include <utility>
#include "layout/configuration/LayoutConfiguration.h"
#include "layout/configuration/linear/LinearLayoutParameter.h"

namespace GKoala
{

class LinearConfiguration : public LayoutConfiguration , public cocos2d::CCObject
{
public:
	GK_CREATE ( LinearConfiguration )
	virtual ~LinearConfiguration();

	virtual void addView ( cocos2d::CCNode* pChild,LayoutParameter* pLayoutParameter ) override;
	virtual void removeChild ( cocos2d::CCNode* pChild ) override;

	//Abstract method factory
	virtual LayoutParameter* getDefaultLayoutParameter() override;

	virtual void updateStructure(LayoutParameter* pLayoutParameter = nullptr) override;

protected:
	virtual cocos2d::CCSize onMeasure(LayoutParameter* pLayoutParameter) override;
	virtual void onLayout() override;

private:
	enum Orientation
	{
		VERTICAL,HORIZONTAL
	}m_orientation =VERTICAL;
	std::vector< std::pair<cocos2d::CCNode*, LinearLayoutParameter*> > m_views;

	LinearConfiguration();

	void prepareViewForMeasure(cocos2d::CCNode* pView,LinearLayoutParameter* pViewParameters);
};

} /* namespace GKoala */

#endif /* LINEARCONFIGURATION_H_ */

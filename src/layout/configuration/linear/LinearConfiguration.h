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

class LinearConfiguration : public LayoutConfiguration
{
public:
	GK_CREATE ( LinearConfiguration )
	virtual ~LinearConfiguration();

	virtual void addView ( cocos2d::CCNode* pChild,LayoutParameter* pLayoutParameter ) override;
	virtual void removeChild ( cocos2d::CCNode* pChild ) override;

	//Abstract method factory
	virtual LayoutParameter* getDefaultLayoutParameter() override;

	virtual void updateStructure(LayoutParameter* pLayoutParameter) override;

	virtual void setOptions(int options) override;

	enum Orientation
	{
		INVALID = 0,
		VERTICAL = 16,
		HORIZONTAL = 32,
		MASK = 48
	};

protected:
	virtual cocos2d::CCSize onMeasure(LayoutParameter* pLayoutParameter) override;
	virtual void onLayout() override;

private:
	typedef LinearLayoutParameter::Gravity Gravity;
	typedef std::pair<cocos2d::CCNode*, LinearLayoutParameter*> View;

	Orientation m_orientation = HORIZONTAL;

	int m_gravity = Gravity::LEFT | Gravity::TOP;

	std::vector< View> m_views;

	LinearConfiguration();

	void prepareViewForMeasure(cocos2d::CCNode* pView,LinearLayoutParameter* pViewParameters);

	void onLayoutVertical();
	void onLayoutHorizontal();

	void applyLayoutGravityHorizontalLeft(View& view, View& previousView);
	void applyLayoutGravityHorizontalRight(View& view, View& previousView);
	void applyLayoutGravityHorizontalCenterHorizontal(View& view);
	void applyViewGravityHorizontal(View& view);

	void applyLayoutGravityVerticalTop(View& view, View& previousView);
	void applyLayoutGravityVerticalBottom(View& view, View& previousView);
	void applyLayoutGravityVerticalCenterVertical(View& view);
	void applyViewGravityVertical(View& view);
};

} /* namespace GKoala */

#endif /* LINEARCONFIGURATION_H_ */

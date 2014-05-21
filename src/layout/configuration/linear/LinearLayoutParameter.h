/*
 * LinearLayoutParameter.h
 *
 *  Created on: Dec 26, 2013
 *      Author: dawid
 */

#ifndef LINEARLAYOUTPARAMETER_H_
#define LINEARLAYOUTPARAMETER_H_

#include "Config.h"
#include "layout/LayoutParameter.h"

namespace GKoala
{

class LinearLayoutParameter : public LayoutParameter
{
public:
	static LinearLayoutParameter* createWrapWrap();
	static LinearLayoutParameter* createWrapMatch();
	static LinearLayoutParameter* createMatchMatch();
	static LinearLayoutParameter* createMatchWrap();
	static LinearLayoutParameter* create ( const SizePolicy& width = SP::sp(0),
										   const SizePolicy& height = SP::sp(0) );

	virtual ~LinearLayoutParameter();

	enum Gravity
	{
		LEFT = 1,
		RIGHT = 2,
		CENTER_HORIZONTAL = 3,
		TOP = 4,
		BOTTOM = 8,
		CENTER_VERTICAL = 12,
		CENTER = 15,
		MASK = 15
	};

	/**
	 * @param gravity Bit mask of gravity eg. TOP | LEFT
	 */
	void setGravity ( int gravity );
	int getGravity() const
	{
		return m_gravity;
	}

private:
	int m_gravity = Gravity::LEFT | Gravity::TOP;

	LinearLayoutParameter();
};

} /* namespace GKoala */

#endif /* LINEARLAYOUTPARAMETER_H_ */

/*
 * LayoutParameter.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTPARAMETER_H_
#define LAYOUTPARAMETER_H_

#include "layout/SizePolicy.h"

namespace GKoala
{

class LayoutParameter
{
public:

	/**
	 * VISIBLE - we see it on screen and element occupy place
	 * INVISIBLE - we <b>don't</b> see it on screen and element occupy place
	 * GONE - we <b>don't</b> see it on screen and element <b>don't</b> occupy place
	 */
	enum Visibility
	{
		VISIBLE, INVISIBLE, GONE
	};

	LayoutParameter();
	virtual ~LayoutParameter();

private:
	Visibility m_visibility;
	SizePolicy m_width;
	SizePolicy m_height;

	/**
	 * m_margins[0] - left
	 * m_margins[1] - top
	 * m_margins[2] - right
	 * m_margins[3] - bottom
	 */
	SizePolicy m_margins[4];
};

} /* namespace GKoala */

#endif /* LAYOUTPARAMETER_H_ */

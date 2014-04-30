/*
 * LinearLayoutParams.h
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCLINEARLAYOUTPARAMS_H_
#define CCLINEARLAYOUTPARAMS_H_

#include "view/layout/LayoutParams.h"

namespace KoalaGui
{

class LinearLayoutParams: public LayoutParams
{
public:
	explicit LinearLayoutParams( const SizePolicy& width,
								 const SizePolicy& height );
	LinearLayoutParams( const LayoutParams& params );
	virtual ~LinearLayoutParams();

	enum Gravity
	{
		TOP = 1,
		BOTTOM = 2,
		CENTER_VERTICAL = 3,
		LEFT = 4,
		RIGHT = 8,
		CENTER_HORIZONTAL = 12
	};

	/**
	 * Bit mask of gravity eg. TOP | LEFT
	 */
	void setGravity( int gravity );
	int getGravity() const;

private:
	int m_gravity;
	LinearLayoutParams& operator= ( LayoutParams& params );

};

typedef LinearLayoutParams::Gravity Gravity;
}
/* namespace KoalaGui */
#endif /* CCLINEARLAYOUTPARAMS_H_ */

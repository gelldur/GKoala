/*
 * LinearLayoutParams.cpp
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#include "view/layout/linear/LinearLayoutParams.h"

namespace KoalaGui
{

LinearLayoutParams::LinearLayoutParams( const SizePolicy& width,
										const SizePolicy& height ) :
	LayoutParams( width, height ),
	m_gravity( 0 )
{
}

LinearLayoutParams::LinearLayoutParams( const LayoutParams& params ) :
	LayoutParams( params ),
	m_gravity( 0 )
{
}

LinearLayoutParams::~LinearLayoutParams()
{
}

void LinearLayoutParams::setGravity( int gravity )
{
	if( ( gravity & Gravity::CENTER_HORIZONTAL ) == 0 )
	{
		gravity |= Gravity::LEFT;
	}

	if( ( gravity & Gravity::CENTER_VERTICAL ) == 0 )
	{
		gravity |= Gravity::TOP;
	}

	m_gravity = gravity;
}

int LinearLayoutParams::getGravity() const
{
	return m_gravity;
}

} /* namespace KoalaGui */

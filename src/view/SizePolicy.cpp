/*
 * SizePolicy.cpp
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#include "view/SizePolicy.h"
#include "utils/Utils.h"

#include <assert.h>

namespace KoalaGui
{

SizePolicy::SizePolicy( const Policy& type, const float& value )
{
	init( type, value );
}

SizePolicy::~SizePolicy()
{
}

float SizePolicy::getValue() const
{
	return m_value;
}

SizePolicy::SizePolicy( const Const& value )
{
	init( Policy::SP, value );
}

void SizePolicy::setValue( const float value )
{
	m_value = convert( m_type, value );
}

bool SizePolicy::init( const Policy& type, const float& value )
{
	assert( value >= 0 || value == MATCH_PARENT || value == WRAP_CONTENT );

	// When we want to set MATCH_PARENT or WRAP_CONTENT
	// we must set type for SP because we don't want to
	// transform value when LinearLayout or other
	// Layout transform such const
	if( value == MATCH_PARENT || value == WRAP_CONTENT )
	{
		m_type = Policy::SP;
	}
	else
	{
		m_type = type;
	}

	setValue( value );
	return true;
}

SizePolicy::Policy SizePolicy::getType() const
{
	return m_type;
}

float SizePolicy::convert( const Policy& type, const float& value )
{
	switch( type )
	{
		case PP:
		case SP:
		{
			return value;
		}

		case PERCENT_X:
		{
			return CCDirector::sharedDirector()->getWinSize().width * value;
		}

		case PERCENT_Y:
		{
			return CCDirector::sharedDirector()->getWinSize().height * value;
		}

		default:
			CCAssert( false, "Unknown policy" );
	}

	return value;
}

} /* namespace KoalaGui */

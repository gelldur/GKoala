/*
 * SizePolicy.cpp
 *
 *      Author: Dawid Drozd
 */

#include "layout/SizePolicy.h"
#include "Config.h"

namespace GKoala
{

SizePolicy::SizePolicy ( Policy type, const float& value ) :
	m_value ( value ),
	m_type ( type )
{
	//We can't make it negative because what mean -500 width or -20 margin left?
	GKoala_assert ( value >= 0, "value must be >= 0" );

	if ( type != Policy::MATCH_PARENT && type != Policy::WRAP_CONTENT )
	{
		setValue ( value );
	}
}

SizePolicy::~SizePolicy()
{
}

float SizePolicy::getValue() const
{
	return m_value;
}

void SizePolicy::setValue ( float value )
{
	m_value = convert ( m_type, value );
}

SizePolicy::Policy SizePolicy::getType() const
{
	return m_type;
}

float SizePolicy::convert ( Policy type, const float& value )
{
	switch ( type )
	{
		case Policy::WRAP_CONTENT:
		case Policy::MATCH_PARENT:
			GKoala_assert ( type != Policy::WRAP_CONTENT &&
							type != Policy::MATCH_PARENT, "we can't convert value with this type" );
			return 1;

		case Policy::PP:
		case Policy::SP:
		{
			return value;
		}

		case Policy::PSW:
		{
			return cocos2d::CCDirector::sharedDirector()->getWinSize().width * value;
		}

		case Policy::PSH:
		{
			return cocos2d::CCDirector::sharedDirector()->getWinSize().height * value;
		}

		default:
			CCAssert ( false, "Unknown policy" );
	}

	return value;
}

} /* namespace GKoala */

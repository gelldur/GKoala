/*
 * SizePolicy.cpp
 *
 *      Author: Dawid Drozd
 */

#include "layout/SizePolicy.h"
#include "Config.h"

namespace GKoala
{

SizePolicy::SizePolicy ( Policy type, const float& value )
{
	init ( type, value );
}

SizePolicy::SizePolicy ( Const value )
{
	init ( Policy::SP, value );
}

SizePolicy::~SizePolicy()
{
}

float SizePolicy::getValue() const
{
	return m_value;
}

void SizePolicy::setValue ( const float value )
{
	m_value = convert ( m_type, value );
}

void SizePolicy::init ( Policy type, const float& value )
{
	assert ( value >= 0 || value == MATCH_PARENT || value == WRAP_CONTENT );

	//When we have match parent or wrap content we change it to SP
	if ( value == MATCH_PARENT || value == WRAP_CONTENT )
	{
		type = Policy::SP;
	}

	m_type = type;
	setValue ( value );
}

SizePolicy::Policy SizePolicy::getType() const
{
	return m_type;
}

float SizePolicy::convert ( Policy type, const float& value )
{
	switch ( type )
	{
		case PP:
		case SP:
		{
			return value;
		}

		case PERCENT_X:
		{
			return cocos2d::CCDirector::sharedDirector()->getWinSize().width * value;
		}

		case PERCENT_Y:
		{
			return cocos2d::CCDirector::sharedDirector()->getWinSize().height * value;
		}

		default:
			CCAssert ( false, "Unknown policy" );
	}

	return value;
}

} /* namespace GKoala */

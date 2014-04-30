/*
 * SizePolicy.h
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#ifndef SIZEPOLICY_H_
#define SIZEPOLICY_H_

namespace KoalaGui
{

class SizePolicy
{
public:

	/**
	 * SP - scaled pixels value in pixels eg. 25.5 pixels
	 * PERCENT_X - percent  of width of screen eg. 0.05F means 5% of width of screen
	 * PERCENT_Y - percent  of height of screen eg. 1.15F means 115% of height of screen
	 * PP - parent percent of width or height eg. parent is 100 X 400 when we set it to 0.5 we get 50 (50% of parent width)
	 */
	enum Policy
	{
		SP, PERCENT_X, PERCENT_Y, PP
	};

	enum Const
	{
		WRAP_CONTENT = -1, MATCH_PARENT = -2
	};

private:
	float m_value;
	Policy m_type;

	bool init( const Policy& type, const float& value );

public:
	SizePolicy( const Policy& type, const float& value );
	SizePolicy( const Const& value );
	virtual ~SizePolicy();

	float getValue() const;
	void setValue( const float value );

	Policy getType() const;

	static float convert( const Policy& type, const float& value );
};

typedef SizePolicy::Policy Policy;
typedef SizePolicy::Const Const;


const SizePolicy SizePolicyWrapContent( Const::WRAP_CONTENT );
const SizePolicy SizePolicyMatchContent( Const::MATCH_PARENT );
const SizePolicy SizePolicyZero( Policy::SP, 0 );

} /* namespace KoalaGui */
#endif /* SIZEPOLICY_H_ */

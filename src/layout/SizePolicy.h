/*
 * SizePolicy.h
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#ifndef SIZEPOLICY_H_
#define SIZEPOLICY_H_

namespace GKoala
{

class SizePolicy
{
public:
	/**
	 * SP - scaled pixels value in scaled pixels eg. 25.5 scaled pixels
	 * PERCENT_X - percent  of width of screen eg. 0.05F means 5% of width of screen
	 * PERCENT_Y - percent  of height of screen eg. 1.15F means 115% of height of screen
	 * PP - parent percent of width/height eg. parent is 100 X 400 when we set it to 0.5 we get 50 (50% of parent width) or 200 (50% of parent height)
	 */
	enum Policy
	{
		SP, PERCENT_X, PERCENT_Y, PP
	};

	enum Const
	{
		WRAP_CONTENT = -1122, MATCH_PARENT = -3344
	};

	SizePolicy ( Policy type, const float& value );
	SizePolicy ( Const value = WRAP_CONTENT );
	~SizePolicy();

	float getValue() const;
	void setValue ( const float value );

	Policy getType() const;

	static float convert ( Policy type, const float& value );

private:
	float m_value;
	Policy m_type;

	void init ( Policy type, const float& value );
};

} /* namespace GKoala */
#endif /* SIZEPOLICY_H_ */

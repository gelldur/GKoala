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
	 * PSW - percent  of width of screen eg. 0.05F means 5% of width of screen
	 * PSH - percent  of height of screen eg. 1.15F means 115% of height of screen
	 * PP - parent percent of width/height eg. parent is 100 X 400 when we set it to 0.5 we get 50 (50% of parent width) or 200 (50% of parent height)
	 */
	enum class Policy
	{
		SP, PSW, PSH, PP, WRAP_CONTENT, MATCH_PARENT
	};

	explicit SizePolicy ( Policy type, const float& value = 1 );
	~SizePolicy();

	static SizePolicy sp ( float value )
	{
		return SizePolicy ( Policy::SP, value );
	}
	static SizePolicy psw ( float value )
	{
		return SizePolicy ( Policy::PSW, value );
	}
	static SizePolicy psh ( float value )
	{
		return SizePolicy ( Policy::PSH, value );
	}
	static SizePolicy pp ( float value )
	{
		return SizePolicy ( Policy::PP, value );
	}
	static SizePolicy wrapContent()
	{
		return SizePolicy ( Policy::WRAP_CONTENT );
	}
	static SizePolicy matchParent()
	{
		return SizePolicy ( Policy::MATCH_PARENT );
	}

	float getValue() const;
	void setValue ( float value );

	Policy getType() const;

	static float convert ( Policy type, const float& value );

	/**
	 * This method will return true if size can change during measuring of layout.
	 * @return
	 */
	bool isConst()const;
	bool isWrapContent()const;
	bool isMatchParent()const;

private:
	float m_value;
	Policy m_type;
};

typedef SizePolicy SP;

} /* namespace GKoala */
#endif /* SIZEPOLICY_H_ */

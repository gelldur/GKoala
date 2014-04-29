/*
 * RelativeLayoutParams.h
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCRELATIVELAYOUTPARAMS_H_
#define CCRELATIVELAYOUTPARAMS_H_

#include "view/layout/LayoutParams.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <cocos2d.h>

USING_NS_CC;

using namespace std;
namespace KoalaGui
{

class RelativeLayoutParams: public LayoutParams
{
public:
	/**
	 * ABOVE - above other component
	 * BELOW - below other component
	 * PARENT_BOTTOM - bottom side to bottom side of parent
	 * PARENT_TOP - top side to top side of parent
	 * CENTER_VERTICAL - center object relative to other vertically
	 * PARENT_CENTER_VERTICAL - center object inside parent vertically
	 * LEFT - on left side of other object, right side to the left side of other object
	 * RIGHT - on right side of other object, left side to the right side of other object
	 * PARENT_LEFT - left side to the left side of parent
	 * PARENT_RIGHT - right side to the right side of parent
	 * PARENT_CENTER_HORIZONTAL - center object inside parent horizontally
	 * CENTER_HORIZONTAL - center object relative to other horizontally
	 * TO_RIGHT - right side to right side of other
	 * TO_LEFT - left side to left side of other
	 */
	enum AlignType : unsigned short
	{
		ABOVE = 1,
		BELOW = 2,
		PARENT_BOTTOM = 4,
		PARENT_TOP = 8,
		CENTER_VERTICAL = 16,
		PARENT_CENTER_VERTICAL = 32,
		LEFT = 64,
		RIGHT = 128,
		PARENT_LEFT = 256,
		PARENT_RIGHT = 512,
		PARENT_CENTER_HORIZONTAL = 1024,
		PARENT_CENTER = PARENT_CENTER_HORIZONTAL | PARENT_CENTER_VERTICAL,
		CENTER_HORIZONTAL = 2048,
		CENTER = CENTER_HORIZONTAL | CENTER_VERTICAL,
		TO_RIGHT = 4096,
		TO_LEFT = 8192,
		TO_TOP = 16384,
		TO_BOTTOM = 32768
	};

	struct Align
	{
	private:
		unsigned short m_align;
		int m_tag;
	public:
		Align ( const unsigned short align, const int tag );
		void addAlignType ( const unsigned short align );
		const unsigned short getAlignType() const;
		const int getTag() const;
	};

	explicit RelativeLayoutParams ( const SizePolicy& width,
									const SizePolicy& height );
	RelativeLayoutParams ( const LayoutParams& params );

	virtual ~RelativeLayoutParams();

	void addRule (
		const unsigned short alignType = RelativeLayoutParams::PARENT_TOP
										 | RelativeLayoutParams::PARENT_LEFT, const int tag
		= kCCNodeTagInvalid );

	const vector<RelativeLayoutParams::Align>& getRules() const;

private:
	vector<RelativeLayoutParams::Align> m_params;
	vector<RelativeLayoutParams::Align>::iterator getIteratorByTag (
		const int tag );
};

typedef RelativeLayoutParams::AlignType AlignType;

} /* namespace KoalaGui */
#endif /* CCRELATIVELAYOUTPARAMS_H_ */

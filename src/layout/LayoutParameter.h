/*
 * LayoutParameter.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef LAYOUTPARAMETER_H_
#define LAYOUTPARAMETER_H_

#include "layout/SizePolicy.h"
#include "interface/LayoutInterface.h"

namespace GKoala
{

class LayoutParameter : public cocos2d::CCObject
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

	virtual ~LayoutParameter();

	LayoutParameter* setVisibility ( Visibility visibility );
	LayoutParameter* setWidth ( const SizePolicy& width );
	LayoutParameter* setHeight ( const SizePolicy& height );
	LayoutParameter* setMarginLeft ( const SizePolicy& margin );
	LayoutParameter* setMarginRight ( const SizePolicy& margin );
	LayoutParameter* setMarginTop ( const SizePolicy& margin );
	LayoutParameter* setMarginBottom ( const SizePolicy& margin );

	LayoutParameter ( const LayoutParameter& params ) = delete;
	LayoutParameter& operator= ( LayoutParameter& params ) = delete;

protected:
	LayoutParameter();

	void notifyLayout()
	{
		if ( m_pPairedLayout != nullptr )
		{
			m_pPairedLayout->updateStructure();
		}
	}

private:
	Visibility m_visibility;
	SizePolicy m_width;
	SizePolicy m_height;

	/**
	 * We want notify paired layout to updateStructure when any change occured
	 */
	LayoutInterface* m_pPairedLayout;

	enum Margin
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	/**
	 * m_margins[0] - left
	 * m_margins[1] - top
	 * m_margins[2] - right
	 * m_margins[3] - bottom
	 */
	SizePolicy m_margins[4] = {SP::sp ( 0 ), SP::sp ( 0 ), SP::sp ( 0 ), SP::sp ( 0 ) };
};

} /* namespace GKoala */

#endif /* LAYOUTPARAMETER_H_ */

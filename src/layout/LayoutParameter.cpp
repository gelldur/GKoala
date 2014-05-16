/*
 * LayoutParameter.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/LayoutParameter.h"

using namespace cocos2d;

namespace GKoala
{

LayoutParameter::LayoutParameter() :
	m_visibility ( VISIBLE ),
	m_width ( SP::wrapContent() ),
	m_height ( SP::wrapContent() ),
	m_pPairedLayout ( nullptr )
{
}

LayoutParameter::~LayoutParameter()
{
}

LayoutParameter* LayoutParameter::setVisibility ( Visibility visibility )
{
	m_visibility = visibility;
	return this;
}

LayoutParameter* LayoutParameter::setWidth ( const SizePolicy& width )
{
	m_width = width;
	return this;
}

LayoutParameter* LayoutParameter::setHeight ( const SizePolicy& height )
{
	m_height = height;
	return this;
}

LayoutParameter* LayoutParameter::setMarginLeft ( const SizePolicy& margin )
{
	GKoala_assert(margin.isConst(),"Margins must be const!. You can't use MATCH_PARENT or WRAP_CONTENT");
	m_margins[LEFT] = margin;
	return this;
}

LayoutParameter* LayoutParameter::setMarginRight ( const SizePolicy& margin )
{
	GKoala_assert(margin.isConst(),"Margins must be const!. You can't use MATCH_PARENT or WRAP_CONTENT");
	m_margins[RIGHT] = margin;
	return this;
}

LayoutParameter* LayoutParameter::setMarginTop ( const SizePolicy& margin )
{
	GKoala_assert(margin.isConst(),"Margins must be const!. You can't use MATCH_PARENT or WRAP_CONTENT");
	m_margins[TOP] = margin;
	return this;
}

LayoutParameter* LayoutParameter::setMarginBottom ( const SizePolicy& margin )
{
	GKoala_assert(margin.isConst(),"Margins must be const!. You can't use MATCH_PARENT or WRAP_CONTENT");
	m_margins[BOTTOM] = margin;
	return this;
}

cocos2d::CCSize GKoala::LayoutParameter::getFullSize() const
{
	CCSize size;
	if(m_width.isConst())
	{
		size.width = m_width.getValue();
	}
	if(m_height.isConst())
	{
		size.height = m_height.getValue();
	}

	size.width += m_margins[LEFT].getValue();
	size.width += m_margins[RIGHT].getValue();

	size.height += m_margins[TOP].getValue();
	size.height += m_margins[BOTTOM].getValue();

	return size;
}


} /* namespace GKoala */

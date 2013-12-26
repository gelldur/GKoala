/*
 * LayoutParameter.cpp
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#include "layout/LayoutParameter.h"

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
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setWidth ( const SizePolicy& width )
{
	m_width = width;
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setHeight ( const SizePolicy& height )
{
	m_height = height;
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setMarginLeft ( const SizePolicy& margin )
{
	m_margins[LEFT] = margin;
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setMarginRight ( const SizePolicy& margin )
{
	m_margins[RIGHT] = margin;
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setMarginTop ( const SizePolicy& margin )
{
	m_margins[TOP] = margin;
	notifyLayout();
	return this;
}

LayoutParameter* LayoutParameter::setMarginBottom ( const SizePolicy& margin )
{
	m_margins[BOTTOM] = margin;
	notifyLayout();
	return this;
}

} /* namespace GKoala */

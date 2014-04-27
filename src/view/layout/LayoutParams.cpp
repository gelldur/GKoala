#include "view/layout/LayoutParams.h"

#include <assert.h>
namespace KoalaGui
{

LayoutParams::LayoutParams ( const LayoutParams& object ) :
	m_width ( object.m_width ),
	m_height ( object.m_height ),
	m_visibility ( object.m_visibility ),
	m_lastVisibility ( object.m_lastVisibility ),
	m_paddingBottom ( object.m_paddingBottom ),
	m_paddingTop ( object.m_paddingTop ),
	m_paddingLeft ( object.m_paddingLeft ),
	m_paddingRight ( object.m_paddingRight ),
	m_marginBottom ( object.m_marginBottom ),
	m_marginTop ( object.m_marginTop ),
	m_marginLeft ( object.m_marginLeft ),
	m_marginRight ( object.m_marginRight )
{
}

LayoutParams::LayoutParams ( const SizePolicy& width,
							 const SizePolicy& height ) :
	m_width ( width ),
	m_height ( height ),
	m_visibility ( LayoutParams::VISIBLE ),
	m_lastVisibility ( LayoutParams::VISIBLE ),
	m_paddingBottom ( SizePolicyZero ),
	m_paddingTop ( SizePolicyZero ),
	m_paddingLeft ( SizePolicyZero ),
	m_paddingRight ( SizePolicyZero ),
	m_marginBottom ( SizePolicyZero ),
	m_marginTop ( SizePolicyZero ),
	m_marginLeft ( SizePolicyZero ),
	m_marginRight ( SizePolicyZero )
{
}

LayoutParams::~LayoutParams()
{
}

float LayoutParams::getWidth() const
{
	return ( m_visibility == GONE ) ? 0 : m_width.getValue();
}

float LayoutParams::getHeight() const
{
	return ( m_visibility == GONE ) ? 0 : m_height.getValue();
}

SizePolicy LayoutParams::getSizePolicyWidth() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_width;
}

SizePolicy LayoutParams::getSizePolicyHeight() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_height;
}

LayoutParams::Visibility LayoutParams::getVisibility() const
{
	return m_visibility;
}

void LayoutParams::setVisibility ( const LayoutParams::Visibility value )
{
	m_lastVisibility = m_visibility;
	m_visibility = value;
}

Visibility LayoutParams::getLastVisibility() const
{
	return m_lastVisibility;
}

void LayoutParams::setWidth ( SizePolicy width )
{
	m_width = width;
}

void LayoutParams::setHeight ( SizePolicy height )
{
	m_height = height;
}

void LayoutParams::setPadding ( const SizePolicy& paddingLeft,
								const SizePolicy& paddingTop, const SizePolicy& paddingRight,
								const SizePolicy& paddingBottom )
{
	setPaddingLeft ( paddingLeft );
	setPaddingTop ( paddingTop );
	setPaddingRight ( paddingRight );
	setPaddingBottom ( paddingBottom );
}

void LayoutParams::setPaddingBottom ( const SizePolicy& paddingBottom )
{
	m_paddingBottom = paddingBottom;
}

SizePolicy LayoutParams::getPaddingBottom() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_paddingBottom;
}

void LayoutParams::setPaddingTop ( const SizePolicy& paddingTop )
{
	m_paddingTop = paddingTop;
}

SizePolicy LayoutParams::getPaddingTop() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_paddingTop;
}

void LayoutParams::setPaddingLeft ( const SizePolicy& paddingLeft )
{
	m_paddingLeft = paddingLeft;
}

SizePolicy LayoutParams::getPaddingLeft() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_paddingLeft;
}

void LayoutParams::setPaddingRight ( const SizePolicy& paddingRight )
{
	m_paddingRight = paddingRight;
}

SizePolicy LayoutParams::getPaddingRight() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_paddingRight;
}

void LayoutParams::setMargin ( const SizePolicy& marginLeft,
							   const SizePolicy& marginTop, const SizePolicy& marginRight,
							   const SizePolicy& marginBottom )
{
	setMarginLeft ( marginLeft );
	setMarginTop ( marginTop );
	setMarginRight ( marginRight );
	setMarginBottom ( marginBottom );
}

void LayoutParams::setMarginBottom ( const SizePolicy& marginBottom )
{
	m_marginBottom = marginBottom;
}

SizePolicy LayoutParams::getMarginBottom() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_marginBottom;
}

void LayoutParams::setMarginTop ( const SizePolicy& marginTop )
{
	m_marginTop = marginTop;
}

SizePolicy LayoutParams::getMarginTop() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_marginTop;
}

void LayoutParams::setMarginLeft ( const SizePolicy& marginLeft )
{
	m_marginLeft = marginLeft;
}

SizePolicy LayoutParams::getMarginLeft() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_marginLeft;
}

void LayoutParams::setMarginRight ( const SizePolicy& marginRight )
{
	m_marginRight = marginRight;
}

SizePolicy LayoutParams::getMarginRight() const
{
	return ( m_visibility == GONE ) ? SizePolicyZero : m_marginRight;
}

cocos2d::CCSize LayoutParams::getSize() const
{
	if ( m_visibility == GONE )
	{
		return cocos2d::CCSizeZero;
	}
	else
	{
		return cocos2d::CCSize ( m_width.getValue(), m_height.getValue() );
	}
}

SizePolicy KoalaGui::LayoutParams::getSizePolicy (
	float cocos2d::CCSize::*pValue ) const
{
	if ( pValue == &cocos2d::CCSize::width )
	{
		return m_width;
	}
	else if ( pValue == &cocos2d::CCSize::height )
	{
		return m_height;
	}
	else
	{
		assert ( false ); //Wron pointer?
		return SizePolicyZero;
	}
}
}

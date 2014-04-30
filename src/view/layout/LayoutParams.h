#ifndef CCLAYOUTPARAMS_H
#define CCLAYOUTPARAMS_H

#include <vector>
#include <utility>

#include "view/SizePolicy.h"
#include "cocoa/CCGeometry.h"

using namespace std;
namespace KoalaGui
{

class LayoutParams
{
public:
	enum Visibility
	{
		VISIBLE, INVISIBLE, GONE
	};

private:
	SizePolicy m_width;
	SizePolicy m_height;
	Visibility m_visibility;
	Visibility m_lastVisibility;

	SizePolicy m_paddingBottom;
	SizePolicy m_paddingTop;
	SizePolicy m_paddingLeft;
	SizePolicy m_paddingRight;

	SizePolicy m_marginBottom;
	SizePolicy m_marginTop;
	SizePolicy m_marginLeft;
	SizePolicy m_marginRight;

	cocos2d::CCPoint m_previousNodeScale;

public:

	LayoutParams( const SizePolicy& width, const SizePolicy& height );
	LayoutParams( const LayoutParams& object );

	virtual ~LayoutParams();

	Visibility getVisibility() const;
	Visibility getLastVisibility() const;
	void setVisibility( const Visibility value );

	void setWidth( SizePolicy width );
	float getWidth() const;
	SizePolicy getSizePolicyWidth() const;

	void setHeight( SizePolicy height );
	float getHeight() const;
	SizePolicy getSizePolicyHeight() const;

	cocos2d::CCPoint getPreviousNodeScale() const
	{
		return m_previousNodeScale;
	}

	void setPreviousNodeScale( const cocos2d::CCPoint& previousScale )
	{
		m_previousNodeScale = previousScale;
	}

	/**
	 * Get SizePolicy depend on pValue
	 * @param pValue pointer to width or height od CCSize
	 * @return desired SizePolicy
	 */
	SizePolicy getSizePolicy( float cocos2d::CCSize::*pValue ) const;

	cocos2d::CCSize getSize() const;

	//Padding///////////////////////////////////////////////////////////////////
	void setPadding( const SizePolicy& paddingLeft,
					 const SizePolicy& paddingTop, const SizePolicy& paddingRight,
					 const SizePolicy& paddingBottom );

	void setPaddingBottom( const SizePolicy& paddingBottom );
	SizePolicy getPaddingBottom() const;

	void setPaddingTop( const SizePolicy& paddingTop );
	SizePolicy getPaddingTop() const;

	void setPaddingLeft( const SizePolicy& paddingLeft );
	SizePolicy getPaddingLeft() const;

	void setPaddingRight( const SizePolicy& paddingRight );
	SizePolicy getPaddingRight() const;
	//Padding///////////////////////////////////////////////////////////////////

	//Margin////////////////////////////////////////////////////////////////////
	void setMargin( const SizePolicy& marginLeft,
					const SizePolicy& marginTop, const SizePolicy& marginRight,
					const SizePolicy& marginBottom );

	void setMarginBottom( const SizePolicy& marginBottom );
	SizePolicy getMarginBottom() const;

	void setMarginTop( const SizePolicy& marginTop );
	SizePolicy getMarginTop() const;

	void setMarginLeft( const SizePolicy& marginLeft );
	SizePolicy getMarginLeft() const;

	void setMarginRight( const SizePolicy& marginRight );
	SizePolicy getMarginRight() const;
	//Margin////////////////////////////////////////////////////////////////////

};

typedef LayoutParams::Visibility Visibility;

const LayoutParams LayoutParamsMatchMatch( SizePolicyMatchContent,
		SizePolicyMatchContent );

const LayoutParams LayoutParamsWrapWrap( SizePolicyWrapContent,
		SizePolicyWrapContent );

const LayoutParams LayoutParamsMatchWrap( SizePolicyMatchContent,
		SizePolicyWrapContent );

const LayoutParams LayoutParamsWrapMatch( SizePolicyWrapContent,
		SizePolicyMatchContent );
}
#endif /* CCLAYOUTPARAMS_H */

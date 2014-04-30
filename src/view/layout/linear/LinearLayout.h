#ifndef CCLINEARLAYOUT_H
#define CCLINEARLAYOUT_H

#include "view/layout/AbstractLayout.h"
#include <iostream>

#include "view/SizePolicy.h"
#include "view/layout/linear/LinearLayoutParams.h"

using namespace std;
namespace KoalaGui
{

class RelativeLayoutParams;

class LinearLayout: public AbstractLayout
{
public:
	enum Orientation
	{
		HORIZONTAL, VERTICAL
	};

private:

	Orientation m_orientation;
	unsigned short m_gravity;
	bool m_isReversed;

	void updateViewsOrder();

protected:
	LinearLayout( int tag = kViewTagUnusedLayout );

	//TODO refactor them copy&paste
	View* getMaxWidthView();
	View* getMaxHeightView();

	virtual bool updateView( View* pView );

	virtual CCSize getLeftSpaceForView( CCNode* pNode,
										const LayoutParams& params );

	virtual bool onLayout();
	virtual CCSize onMeasure( float CCSize::* pValue, const bool withBackground );

public:
	static LinearLayout* create( const Orientation& orientation, int tag =
									 kViewTagUnusedLayout );

	virtual bool init( const Orientation& orientation );
	virtual ~LinearLayout();

	View* addView( CCNode* pNode, const LinearLayoutParams& params );
	View* addView( CCNode* pNode, const LayoutParams& params );
	View* addView( View* pView );
	View* addView( CCNode* pNode );
	bool insertView( unsigned index, CCNode* pNode,
					 const LinearLayoutParams& params );

	const Orientation getOrientation() const;
	void setOrientation( const Orientation& orientation );

	virtual void setGravity( const unsigned short gravity );
	bool measureSelf();

	void setReverseViews( bool isReverse )
	{
		m_isReversed = isReverse;
	}

private:
	///Rules////////////////////////////////////////////////////////////////////////
	bool applyGravityRule( View* pView );
	bool applyVerticalGravity( View* pView );
	bool applyHorizontalGravity( View* pView );
	View* addView( CCNode* pNode, const RelativeLayoutParams& params );

	bool applyPositionRule( View* pView );
	bool applyVerticalPosition( View* pPreviousView, View* pView );
	bool applyHorizontalPosition( View* pPreviousView, View* pView );

	///Rules////////////////////////////////////////////////////////////////////////
};

typedef LinearLayout::Orientation Orientation;
}
#endif /* CCLINEARLAYOUT_H */

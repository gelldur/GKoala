/*
 * RelativeLayout.h
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCRELATIVELAYOUT_H_
#define CCRELATIVELAYOUT_H_

#include "view/layout/AbstractLayout.h"
#include "view/SizePolicy.h"
#include "view/layout/relative/RelativeLayoutParams.h"

namespace KoalaGui
{

class RelativeLayout: public AbstractLayout
{
public:
	static RelativeLayout* create( const int tag = kViewTagUnusedLayout );
	virtual bool init();
	virtual ~RelativeLayout();

	View* addView( CCNode* pNode, const RelativeLayoutParams& params );
	bool measureSelf();

protected:
	explicit RelativeLayout( int tag = kViewTagUnusedLayout );

	virtual bool updateView( View* pView );

	virtual CCSize getLeftSpaceForView( CCNode* pNode,
										const LayoutParams& params );

	virtual bool onLayout();
	virtual CCSize onMeasure( float CCSize::* pValue, const bool withBackground );

private:
	bool applyPositionRule( View* pView );

	void applyXPosition( View* pView, const View* pRelativeTo,
						 const short align );
	void applyYPosition( View* pView, const View* pRelativeTo,
						 const short align );

	/**
	 * Should be called after onLayout
	 * @param pView
	 * @return size for related views
	 */
	CCSize getRelatedSizeForView( View* pView );
};

} /* namespace KoalaGui */
#endif /* CCRELATIVELAYOUT_H_ */

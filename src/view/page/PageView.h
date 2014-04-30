/*
 * PageView.h
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#ifndef PAGEVIEW_H_
#define PAGEVIEW_H_

#include "view/page/PageViewAdapter.h"
#include "view/list/ListView.h"
#include "cocos-ext.h"
#include "utils/Utils.h"
USING_NS_CC_EXT;

namespace KoalaGui
{

class PageIndicatorController;

class PageView: public ListView
{
public:

	static PageView* create( const Orientation& orientation =
								 Orientation::HORIZONTAL,
							 int tag = kViewTagUnusedLayout );

	virtual bool init( const Orientation& orientation );

	virtual ~PageView();

	void setPage( unsigned index );
	virtual void setAdapter( ListAdapter* pAdapter );

	void setPageIndicator( PageIndicatorController* pPageIndicator );

	virtual void registerWithTouchDispatcher();

	virtual void update( float delta );

	virtual bool ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchMoved( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent );

	unsigned getCurrentPage();
	string getCurrentPageTitle();

	void onFinishPageMoveCallback();

protected:
	virtual CCAction* getMovingAction( const CCPoint& moveTo,
									   float duration = 0.3f );

private:

	PageView( const int tag );
	PageIndicatorController* m_pPageIndicator;

	bool m_isIgnoring;
	bool m_checkOnce;
	float m_startMovePage;
	unsigned m_currentPage;

	unsigned calculatePage();
	void movePageTo( const CCPoint& point );

	void setCurrentPage( unsigned page )
	{
		m_currentPage = page;
	}
};

} /* namespace KoalaGui */
#endif /* PAGEVIEW_H_ */

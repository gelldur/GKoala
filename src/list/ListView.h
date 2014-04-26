/*
 * ListView.h
 *
 *  Created on: Jun 18, 2013
 *      Author: Dawid Drozd
 */

#ifndef LISTVIEW_H_
#define LISTVIEW_H_

#include "view/layout/linear/LinearLayout.h"
#include "view/list/ListAdapter.h"
#include "view/ScissorHelper.h"
#include "utils/Utils.h"

namespace KoalaGui
{

class ContentView;

typedef void ( CCObject::*OnRowClick ) ( unsigned index, CCNode* pRow );

#define on_row_selector(_OnRowClick) (OnRowClick)(&_OnRowClick)

class ListView: public LinearLayout, private ScissorHelper
{
public:

	static ListView* create ( const Orientation& orientation = Orientation::VERTICAL,
							  int tag = kViewTagUnusedLayout );

	virtual ~ListView();

	virtual void setAdapter ( ListAdapter* pAdapter );
	ListAdapter* getAdapter()
	{
		return m_pAdapter;
	}

	unsigned getItemsCount() const
	{
		assert ( m_pAdapter );
		return m_pAdapter->getItemsCount();
	}

	virtual void setContentSize ( const CCSize& size );

	virtual bool init ( const Orientation& orientation );

	virtual void visit();

	void setCachedRows ( unsigned count );
	unsigned getCachedRows() const;


	virtual bool ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchCancelled ( CCTouch* pTouch, CCEvent* pEvent );

	virtual void onEnter();
	virtual void onExit();

	virtual void update ( float delta );

	void setOnItemClickCallback ( CCObject* pObject, OnRowClick onRowClick )
	{
		assert ( pObject );
		assert ( onRowClick );
		m_pOnRowClickObject = pObject;
		m_pOnRowClickCallback = onRowClick;
	}
	virtual void setColor ( const ccColor3B& color );
	virtual void setOpacity ( GLubyte opacity );
	void setOffset ( float offset );
	float getOffset();

	virtual void setGravity ( const unsigned short gravity );

	void reloadData();

protected:

	GET (  int, ActionBounceTag, 0x23 );
	GET (  int, ActionScrollTag, 0x22 );

	ListView ( const int tag );
	ContentView* getContentView()
	{
		return m_pContent;
	}

	void setTouched ( bool isTouched )
	{
		m_isTouched = isTouched;
	}

	unsigned getIndex() const;

	void setIndex ( unsigned index )
	{
		m_index = index;
	}

	const CCPoint& getMovingTo()
	{
		return m_movingTo;
	}

	void setMovingTo ( const CCPoint& point )
	{
		LOG_FUNCTION_CALL();
		m_movingTo = point;
	}

	virtual CCAction* getMovingAction ( const CCPoint& moveTo,
										float duration = 0.3f );

	const float getScaleForValues() const
	{
		return m_scale;
	}

private:
	const float m_scale;
	CCPoint m_movingTo;
	ContentView* m_pContent;
	ListAdapter* m_pAdapter;

	unsigned m_cachedRowsMax;
	unsigned m_cachedRows;

	float m_allRowsMaxSize;

	unsigned m_index;

	//For horizontal/vertical
	float CCPoint::* m_pAxis;
	float CCSize::* m_pValue;
	float m_factor;

	void ( CCObject::*m_pOnRowClickCallback ) ( unsigned index, CCNode* pRow );
	CCObject* m_pOnRowClickObject;

	CCNode* m_pTocuhedRow;
	CCPoint m_touchBeginPoint;
	float m_scrollSpeed;
	bool m_isTouched;
	unsigned m_touchSampleCount;

	bool m_isReloadOnEnter;

	CCNode* getCellForTouch ( CCTouch* pTouch, unsigned* const pPosition );
	void getMoreCelss();
	CCNode* getEmptyView();

	void handleEndOfList();
	void handleBeginOfList();
	void setSpeedAfterAction()
	{
		m_scrollSpeed = 0;
	}

	enum ID
	{
		EMPTY_VIEW = -9933
	};
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class ContentView: public LinearLayout
{
public:
	static ContentView* create ( const Orientation& orientation, int tag =
									 kViewTagUnusedLayout );

	//	virtual bool init ( const Orientation& orientation );

	CCSize getFirstRowsSize ( unsigned rowsCount )
	{
		CCSize size;
		vector<View*>& views = getViews();
		assert ( rowsCount <= views.size() );

		for ( size_t i = 0; i < rowsCount; ++i )
		{
			size = size + views[i]->getFullSize();
		}

		return size;
	}

	CCSize getLastRowsSize ( unsigned rowsCount )
	{
		CCSize size;
		vector<View*>& views = getViews();
		assert ( rowsCount < views.size() );

		for ( vector<View*>::reverse_iterator it = views.rbegin();
				it != views.rbegin() + rowsCount; ++it )
		{
			size = size + ( *it )->getFullSize();
		}

		return size;
	}

	CCPoint getFirsRowPositionLewftTop()
	{
		return Positioner::getLeftTopCorner ( getViews() [0]->getNode() ).origin;
	}

	CCPoint getLastRowPositionRightBottom()
	{
		assert ( getViewsCount() > 0 );
		return Positioner::getRightBottomCorner (
				   getViews() [getViewsCount() - 1]->getNode() ).origin;
	}

	unsigned getViewsCount() const
	{
		return getViews().size();
	}

	View* getView ( unsigned index )
	{
		return getViews() [index];
	}

	bool moveRowToTop();
	bool moveRowToBottom();

protected:
	ContentView ( const int tag );

private:
	void updateCell ( View* pView )
	{
		AbstractLayout* pLayout = dynamic_cast<AbstractLayout*> ( pView->getNode() );

		//For performance its better to update 1 view than all list
		if ( pLayout != nullptr )
		{
			pLayout->updateStructure();
			applySizeRuleForLayout ( KK_WIDTH, pLayout, *pView->getLayoutParams() );
			applySizeRuleForLayout ( KK_HEIGHT, pLayout, *pView->getLayoutParams() );
		}
	}
};

} /* namespace KoalaGui */
#endif /* LISTVIEW_H_ */

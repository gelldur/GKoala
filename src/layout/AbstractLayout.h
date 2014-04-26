#ifndef CCABSTRACTLAYOUT_H
#define CCABSTRACTLAYOUT_H

#include <vector>
#include <set>
#include "utils/Utils.h"
#include "cocos2d.h"
#include "view/View.h"
#include "view/layout/Positioner.h"

using std::vector;
using namespace cocos2d;

#define KK_WIDTH &CCSize::width
#define KK_HEIGHT &CCSize::height

namespace KoalaGui
{

/**
 * Assumption is that AbstractLayout have < 0 tags
 */
class AbstractLayout: public CCLayerColor
{

public:

	enum
	{
		kViewTagUnused = 0, kViewTagUnusedLayout = -2
	};

	virtual bool init();
	virtual ~AbstractLayout();

	bool applySizeRule ( CCNode* pNode, const LayoutParams& params,
						 float CCSize::* pValue );
	bool applySizeRuleForBackground ( float CCSize::*pValue, CCNode* pNode,
									  const LayoutParams& params );
	bool applySizeRuleForLayout ( float CCSize::*pValue, AbstractLayout* pLayout,
								  const LayoutParams& params );
	bool applyPaddingRule ( CCNode* pNode, const LayoutParams& params );
	bool applyVisibilityRule ( CCNode* pNode, LayoutParams& params );

	/**
	 * Update a structure of sub layouts this method should be called on most top parent
	 * on end of creating view. Also it can be called whenever you want to update your
	 * view maybe you add something new :)
	 * @return true if update was successful
	 */
	virtual bool updateStructure();

	virtual bool measureSelf() = 0;

	/**
	 * We block this method
	 */
	virtual void addChild ( CCNode* child, int zOrder, int tag );

	void setBackground ( View* pBackground );
	View* getBackground() const;

	virtual void removeChild ( CCNode* pChild, bool cleanup );
	virtual void removeAllChildren();
	virtual void removeAllChildrenWithCleanup ( bool cleanup );

	void measureRecursive ( float CCSize::* pValue, bool isWrapContenSet = false );

	virtual bool ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );
	virtual void setOnClickCallback ( Utils::CallbackOld* pCallback );
	virtual void setOnTouchCallback ( Utils::CallbackOld* pCallback );
	virtual void setOnTouchReleaseInsideCallback ( Utils::CallbackOld* pCallback );

	void setEnabled ( bool value )
	{
		m_isTouchEnabled = value;
	}

	bool isEnabled()
	{
		return m_isTouchEnabled;
	}

protected:

	/**
	 * Tag should be < 0
	 */
	AbstractLayout ( int tag );

	virtual bool onLayout();
	virtual CCSize onMeasure ( float CCSize::* pValue,
							   const bool withBackground ) = 0;
	virtual bool updateView ( View* pView );

	/**
	 *
	 * @param pNodeView
	 * @param pParams
	 * @return created View that was added
	 */
	View* addViewWithParams ( CCNode* pNodeView, LayoutParams* pParams );
	View* addViewWithParams ( View* pView );
	bool insertViewParams ( unsigned index, CCNode* pNodeView,
							LayoutParams* pParams );

	vector<View*>& getViews()
	{
		return m_views;
	}

	const vector<View*>& getViews() const
	{
		return m_views;
	}

	bool removeViewFromLayout ( CCNode* pNode );
	bool removeViewFromLayout ( int tag );

	vector<View*>::iterator getViewIterator ( const CCNode* pNode );
	vector<View*>::iterator getViewIterator ( const View* pView );

	/**
	 * @return NULL if no such View
	 */
	View* getViewByTag ( int tag ) const;

	bool isChildView ( const CCNode* pNode );

	void measureBackground ( CCSize& parentSize ) const;

	/**
	 *
	 * @param pNode
	 * @param params
	 * @return maximum size for match parent
	 */
	virtual CCSize getLeftSpaceForView ( CCNode* pNode,
										 const LayoutParams& params ) = 0;

	bool isLayout ( CCNode* pNode )
	{
		if ( pNode->getTag() < 0 )
		{
			CCAssert ( dynamic_cast<AbstractLayout*> ( pNode ),
					   "Fuckup with tags layout only should have tag < 0" );
			return true;
		}
		else
		{
			CCAssert ( dynamic_cast<AbstractLayout*> ( pNode ) == nullptr,
					   "Fuckup with tags layout should have tag < 0" );
			return false;
		}
	}

	bool hasVisibleParents();

private:
	vector<View*> m_views;
	View* m_pBackground;
	bool m_isTouchEnabled;

	Utils::CallbackOld* m_pClickCallback;
	Utils::CallbackOld* m_pTouchCallback;
	Utils::CallbackOld* m_pTouchReleaseInsideCallback;

	BLOCK_COPY_OBJECT ( AbstractLayout );

	bool isTagUnique ( int tag );

	/**
	 * We block and hide this method
	 */
	virtual void setTag ( int tag );

	void prepareView ( CCNode* pNodeView, LayoutParams* pParams );
};
}
#endif /* CCABSTRACTLAYOUT_H */

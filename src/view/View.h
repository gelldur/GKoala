/*
 * View.h
 *
 *  Created on: May 8, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCVIEW_H_
#define CCVIEW_H_

#include "cocos2d.h"
#include "view/layout/LayoutParams.h"
#include "utils/Utils.h"
USING_NS_CC;

namespace KoalaGui
{

class View: public CCObject
{
public:
	virtual ~View();

	CCNode* getNode() const;
	LayoutParams* getLayoutParams() const;
	void updateLayoutParams( LayoutParams* pLayoutParams );

	CCSize getFullSize() const;

	CCPoint getMarginLeftBottomCorner() const;
	CCPoint getMarginLeftTopCorner() const;
	CCPoint getMarginRightTopCorner() const;
	CCPoint getMarginRightBottomCorner() const;

	CCPoint getMarginCenterPoint() const;

	static View* create( CCNode* pNode, LayoutParams* pLayoutParams )
	{
		View* pRet = new View( pNode, pLayoutParams );

		if( pRet )
		{
			pRet->autorelease();
			return pRet;
		}

		CC_SAFE_DELETE( pRet );
		return pRet = nullptr;
	}

	Utils::CallbackOld* getClickCallback()
	{
		return m_pClickCallback;
	}
	Utils::CallbackOld* getTouchCallback()
	{
		return m_pTouchCallback;
	}
	Utils::CallbackOld* getTouchReleaseInsideCallback()
	{
		return m_pTouchReleaseInsideCallback;
	}

	void setClickCallback( Utils::CallbackOld* const pClickCallback )
	{
		CC_SAFE_RELEASE_NULL( m_pClickCallback );
		m_pClickCallback = pClickCallback;

		if( m_pClickCallback )
		{
			assert( m_pClickCallback->methodCCNode );
		}

		CC_SAFE_RETAIN( pClickCallback );
	}
	void setTouchCallback( Utils::CallbackOld* const pTouchCallback )
	{
		CC_SAFE_RELEASE_NULL( m_pTouchCallback );
		m_pTouchCallback = pTouchCallback;

		if( m_pTouchCallback )
		{
			assert( m_pTouchCallback->methodCCNode );
		}

		CC_SAFE_RETAIN( pTouchCallback );
	}
	void setTouchReleaseInsideCallback( Utils::CallbackOld* const
										pTouchReleaseInsideCallback )
	{
		CC_SAFE_RELEASE_NULL( m_pTouchReleaseInsideCallback );
		m_pTouchReleaseInsideCallback = pTouchReleaseInsideCallback;

		if( m_pTouchCallback )
		{
			assert( m_pTouchCallback->methodCCNode );
		}

		CC_SAFE_RETAIN( m_pTouchReleaseInsideCallback );
	}

	bool isTouched() const
	{
		return m_touched;
	}
	void setTouched( bool value )
	{
		m_touched = value;
	}

protected:
	View( CCNode* pNode, LayoutParams* pLayoutParams );

private:
	CCNode* m_pNode;
	LayoutParams* m_pLayoutParams;

	bool m_touched;
	Utils::CallbackOld* m_pClickCallback;
	Utils::CallbackOld* m_pTouchCallback;
	Utils::CallbackOld* m_pTouchReleaseInsideCallback;

	BLOCK_COPY_OBJECT( View );
};

} /* namespace KoalaGui */
#endif /* CCVIEW_H_ */

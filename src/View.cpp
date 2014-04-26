/*
 * View.cpp
 *
 *  Created on: May 8, 2013
 *      Author: Dawid Drozd
 */

#include "view/View.h"
#include "utils/Utils.h"
#include "view/layout/Positioner.h"

namespace KoalaGui
{

View::View ( CCNode* pNode, LayoutParams* pLayoutParams ) :
	m_pNode ( pNode ),
	m_pLayoutParams ( pLayoutParams ),
	m_touched ( false ),
	m_pClickCallback ( nullptr ),
	m_pTouchCallback ( nullptr ),
	m_pTouchReleaseInsideCallback ( nullptr )
{
	assert ( pNode );
	assert ( pLayoutParams );
	m_pNode->retain();
	assert ( m_pNode->retainCount() >= 2 );
}

View::~View()
{
	CC_SAFE_DELETE ( m_pLayoutParams );
	CC_SAFE_RELEASE_NULL ( m_pNode );
	CC_SAFE_RELEASE_NULL ( m_pClickCallback );
	CC_SAFE_RELEASE_NULL ( m_pTouchCallback );
	CC_SAFE_RELEASE_NULL ( m_pTouchReleaseInsideCallback );
}

CCNode* View::getNode() const
{
	return m_pNode;
}

LayoutParams* View::getLayoutParams() const
{
	return m_pLayoutParams;
}

void View::updateLayoutParams ( LayoutParams* pLayoutParams )
{
	CC_SAFE_DELETE ( m_pLayoutParams );
	m_pLayoutParams = pLayoutParams;
}

CCSize View::getFullSize() const
{
	CCSize size = Positioner::getScaledSize ( m_pNode );

	size.width += m_pLayoutParams->getMarginLeft().getValue();
	size.width += m_pLayoutParams->getMarginRight().getValue();
	size.height += m_pLayoutParams->getMarginTop().getValue();
	size.height += m_pLayoutParams->getMarginBottom().getValue();
	return size;
}

CCPoint View::getMarginLeftBottomCorner() const
{
	CCRect rect = Positioner::getLeftBottomCorner ( m_pNode );

	rect.origin.x -= m_pLayoutParams->getMarginLeft().getValue();
	rect.origin.y -= m_pLayoutParams->getMarginBottom().getValue();
	return rect.origin;
}

CCPoint View::getMarginLeftTopCorner() const
{
	CCRect rect = Positioner::getLeftTopCorner ( m_pNode );

	rect.origin.x -= m_pLayoutParams->getMarginLeft().getValue();
	rect.origin.y += m_pLayoutParams->getMarginTop().getValue();
	return rect.origin;
}

CCPoint View::getMarginRightTopCorner() const
{
	CCRect rect = Positioner::getRightTopCorner ( m_pNode );

	rect.origin.x += m_pLayoutParams->getMarginRight().getValue();
	rect.origin.y += m_pLayoutParams->getMarginTop().getValue();
	return rect.origin;
}

CCPoint View::getMarginRightBottomCorner() const
{
	CCRect rect = Positioner::getRightBottomCorner ( m_pNode );

	rect.origin.x += m_pLayoutParams->getMarginRight().getValue();
	rect.origin.y -= m_pLayoutParams->getMarginBottom().getValue();
	return rect.origin;
}

CCPoint View::getMarginCenterPoint() const
{
	CCPoint point = getMarginLeftBottomCorner();
	CCPoint pointRightTop = getMarginRightTopCorner();

	point.x = ( pointRightTop.x + point.x ) * 0.5F;
	point.y = ( pointRightTop.y + point.y ) * 0.5F;
	return point; //we return center
}

} /* namespace KoalaGui */


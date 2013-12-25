/*
 * Positioner.cpp
 *
 *  Created on: May 12, 2013
 *      Author: Dawid Drozd
 */

#include "Positioner.h"

namespace gui
{
//TODO Change getScaledHeight to beoundingbox
CCPoint Positioner::getLeftBottomCorner ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCPoint point = pNode->boundingBox().origin;
	//For now commented because changed getPosition to boundingBox
	//point.x -= pNode->getAnchorPointInPoints().x * pNode->getScaleX();
	//point.y -= pNode->getAnchorPointInPoints().y * pNode->getScaleY();
	return point;
}

CCPoint Positioner::getLeftTopCorner ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCPoint point = getLeftBottomCorner ( pNode );
	point.y += getScaledHeight ( pNode );

	return point;
}

CCPoint Positioner::getRightTopCorner ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCPoint point = getLeftTopCorner ( pNode );
	point.x += getScaledWidth ( pNode );
	return point;
}

CCPoint Positioner::getRightBottomCorner ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCPoint point = getRightTopCorner ( pNode );
	point.y -= getScaledHeight ( pNode );
	return point;
}

CCPoint Positioner::getCenterPoint ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCPoint point = getLeftBottomCorner ( pNode );
	point.x += ( getScaledWidth ( pNode ) * 0.5F );
	point.y += ( getScaledHeight ( pNode ) * 0.5F );
	return point;
}

CCPoint Positioner::getCenterPoint ( const CCSize& size )
{
	return CCPoint ( size.width * 0.5F, size.height * 0.5F );
}

void Positioner::setCornerToPosition ( CCNode* pNode,
									   const CCPoint& cornerPosition,
									   const CCPoint& position )
{
	pNode->setPosition ( ccpAdd ( ccpSub ( position, cornerPosition ),
								  pNode->getPosition() ) );
}

void Positioner::centerInParent ( CCNode* pChildNode )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	centerInParentHorizontally ( pChildNode );
	centerInParentVertically ( pChildNode );
}

void Positioner::leftEdge ( CCNode* pChildNode, const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	CCPoint leftChildTopCorner = getLeftTopCorner ( pChildNode );
	leftChildTopCorner.y = pChildNode->getPositionY();
	setCornerToPosition ( pChildNode, leftChildTopCorner,
						  CCPoint ( 0 + margin, leftChildTopCorner.y ) );
}

void Positioner::rightEdge ( CCNode* pChildNode, const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getRightTopCorner ( pChildNode );

	setCornerToPosition ( pChildNode, corner,
						  CCPoint ( ( pChildNode->getParent()->getContentSize().width ) - margin,
									corner.y ) );
}

void Positioner::topEdge ( CCNode* pChildNode, const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getRightTopCorner ( pChildNode );

	setCornerToPosition ( pChildNode, corner,
						  CCPoint ( corner.x,  ( pChildNode->getParent()->getContentSize().height ) -
									margin ) );
}

void Positioner::bottomEdge ( CCNode* pChildNode, const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getRightBottomCorner ( pChildNode );
	setCornerToPosition ( pChildNode, corner, CCPoint ( corner.x, 0 + margin ) );
}

void Positioner::centerInParentVertically ( CCNode* pChildNode,
		const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getCenterPoint ( pChildNode );
	setCornerToPosition ( pChildNode, corner,
						  CCPoint ( corner.x, pChildNode->getParent()->getContentSize().height * 0.5F +
									margin ) );
}

void Positioner::centerInParentHorizontally ( CCNode* pChildNode,
		const float margin )
{
	CCAssert ( pChildNode, "Can't be null" );

	CCAssert ( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getCenterPoint ( pChildNode );

	setCornerToPosition ( pChildNode, corner,
						  CCPoint ( pChildNode->getParent()->getContentSize().width * 0.5F + margin,
									corner.y ) );
}

float Positioner::getScaledWidth ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "Node can't be nullptr" );
	return pNode->getContentSize().width * pNode->getScaleX();
}

float Positioner::getScaledHeight ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "Node can't be nullptr" );
	return pNode->getContentSize().height * pNode->getScaleY();
}

CCSize Positioner::getScaledSize ( CCNode* pNode )
{
	CCAssert ( pNode != nullptr, "Node can't be nullptr" );
	return CCSizeMake ( getScaledWidth ( pNode ), getScaledHeight ( pNode ) );
}

void Positioner::toLeftOf ( CCNode* pToLeft, CCNode* pOf, const float margin )
{
	CCAssert ( pToLeft->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToLeft->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToLeft->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getRightBottomCorner ( pToLeft );
	Positioner::setCornerToPosition ( pToLeft, corner,
									  CCPoint ( Positioner::getLeftBottomCorner ( pOf ).x - margin, corner.y ) );
}

void Positioner::toRightOf ( CCNode* pToRight, CCNode* pOf, const float margin )
{
	CCAssert ( pToRight->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToRight->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToRight->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getLeftBottomCorner ( pToRight );
	Positioner::setCornerToPosition ( pToRight, corner,
									  CCPoint ( Positioner::getRightBottomCorner ( pOf ).x + margin, corner.y ) );
}

void Positioner::toAboveOf ( CCNode* pToAbove, CCNode* pOf, const float margin )
{
	CCAssert ( pToAbove->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToAbove->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToAbove->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getLeftBottomCorner ( pToAbove );
	Positioner::setCornerToPosition ( pToAbove, corner,
									  CCPoint ( corner.x, Positioner::getLeftTopCorner ( pOf ).y + margin ) );
}

void Positioner::toBelowOf ( CCNode* pToBelow, CCNode* pOf, const float margin )
{
	CCAssert ( pToBelow->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToBelow->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToBelow->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getLeftTopCorner ( pToBelow );
	Positioner::setCornerToPosition ( pToBelow, corner,
									  CCPoint ( corner.x, Positioner::getLeftBottomCorner ( pOf ).y - margin ) );
}

void Positioner::toCenterHorizontalOf ( CCNode* pToCenterHorizontal,
										CCNode* pOf,
										const float margin )
{
	CCAssert ( pToCenterHorizontal->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToCenterHorizontal->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToCenterHorizontal->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getCenterPoint ( pToCenterHorizontal );
	Positioner::setCornerToPosition ( pToCenterHorizontal, corner,
									  CCPoint ( Positioner::getCenterPoint ( pOf ).x + margin, corner.y ) );
}

void Positioner::toCenterVerticalOf ( CCNode* pToCenterVertical, CCNode* pOf,
									  const float margin )
{
	CCAssert ( pToCenterVertical->getParent() != pOf,
			   "This doesn't work in parent. Different coordinate system" );
	assert ( pToCenterVertical->getParent() != nullptr );
	assert ( pOf->getParent() != nullptr );
	assert ( pToCenterVertical->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getCenterPoint ( pToCenterVertical );
	Positioner::setCornerToPosition ( pToCenterVertical, corner,
									  CCPoint ( corner.x, Positioner::getCenterPoint ( pOf ).y + margin ) );
}

} /* namespace gui */

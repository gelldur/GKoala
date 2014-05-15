/*
 * Positioner.cpp
 *
 *  Created on: May 12, 2013
 *      Author: Dawid Drozd
 */

#include "Positioner.h"

namespace KoalaGui
{
//TODO Change getScaledHeight to beoundingbox
CCRect Positioner::getLeftBottomCorner( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	//For now commented because changed getPosition to boundingBox
	//	point.x -= pNode->getAnchorPointInPoints().x * pNode->getScaleX();
	//	point.y -= pNode->getAnchorPointInPoints().y * pNode->getScaleY();

	//We return rect because if something is rotated width != getScaledWidth :)
	return pNode->boundingBox();
}

CCRect Positioner::getLeftTopCorner( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );

	CCRect rect = getLeftBottomCorner( pNode );
	rect.origin.y += rect.size.height;

	return rect;
}

CCRect Positioner::getRightTopCorner( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );

	CCRect rect = getLeftTopCorner( pNode );
	rect.origin.x += rect.size.width;
	return rect;
}

CCRect Positioner::getRightBottomCorner( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );

	CCRect rect = getRightTopCorner( pNode );
	rect.origin.y -= rect.size.height;
	return rect;
}

CCRect Positioner::getCenterPoint( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "CCNode* pNode cannot be nullptr!" );
	CCRect rect = getLeftBottomCorner( pNode );
	rect.origin.x += ( rect.size.width * 0.5F );
	rect.origin.y += ( rect.size.height * 0.5F );
	return rect;
}

CCPoint Positioner::getCenterPoint( const CCSize& size )
{
	return CCPoint( size.width * 0.5F, size.height * 0.5F );
}

void Positioner::setCornerToPosition( CCNode* pNode,
									  const CCPoint& cornerPosition,
									  const CCPoint& position )
{
	pNode->setPosition( ccpAdd( ccpSub( position, cornerPosition ),
								pNode->getPosition() ) );
}

void Positioner::centerInParent( CCNode* pChildNode )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	centerInParentHorizontally( pChildNode );
	centerInParentVertically( pChildNode );
}

void Positioner::leftEdge( CCNode* pChildNode, const float margin )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	CCPoint leftChildBottomCorner = getLeftBottomCorner( pChildNode ).origin;
	leftChildBottomCorner.y = pChildNode->getPositionY();
	setCornerToPosition( pChildNode, leftChildBottomCorner,
						 CCPoint( 0 + margin, leftChildBottomCorner.y ) );
}

void Positioner::rightEdge( CCNode* pChildNode, const float margin )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getRightTopCorner( pChildNode ).origin;

	setCornerToPosition( pChildNode, corner,
						 CCPoint( ( pChildNode->getParent()->getContentSize().width ) - margin,
								  corner.y ) );
}

void Positioner::topEdge( CCNode* pChildNode, const float margin )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getLeftTopCorner( pChildNode ).origin;

	setCornerToPosition( pChildNode, corner,
						 CCPoint( corner.x, ( pChildNode->getParent()->getContentSize().height ) -
								  margin ) );
}

void Positioner::bottomEdge( CCNode* pChildNode, const float margin )
{
	CCAssert( pChildNode, "Can't be null" );
	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getLeftBottomCorner( pChildNode ).origin;
	setCornerToPosition( pChildNode, corner, CCPoint( corner.x, 0 + margin ) );
}

void Positioner::centerInParentVertically( CCNode* pChildNode,
		const float margin )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getCenterPoint( pChildNode ).origin;
	setCornerToPosition( pChildNode, corner,
						 CCPoint( corner.x, pChildNode->getParent()->getContentSize().height * 0.5F +
								  margin ) );
}

void Positioner::centerInParentHorizontally( CCNode* pChildNode,
		const float margin )
{
	CCAssert( pChildNode, "Can't be null" );

	CCAssert( pChildNode->getParent() != nullptr, "Parent can't be nullptr!" );

	const CCPoint corner = getCenterPoint( pChildNode ).origin;

	setCornerToPosition( pChildNode, corner,
						 CCPoint( pChildNode->getParent()->getContentSize().width * 0.5F + margin,
								  corner.y ) );
}

float Positioner::getScaledWidth( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "Node can't be nullptr" );
	return pNode->getContentSize().width * pNode->getScaleX();
}

float Positioner::getScaledHeight( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "Node can't be nullptr" );
	return pNode->getContentSize().height * pNode->getScaleY();
}

CCSize Positioner::getScaledSize( CCNode* pNode )
{
	CCAssert( pNode != nullptr, "Node can't be nullptr" );
	return CCSizeMake( getScaledWidth( pNode ), getScaledHeight( pNode ) );
}

void Positioner::toLeftOf( CCNode* pToLeft, CCNode* pOf, const float margin )
{
	CCAssert( pToLeft != pOf, "Bad argument!" );
	assert( pToLeft != nullptr );
	assert( pOf != nullptr );
	assert( pToLeft->getParent() != nullptr );

	const CCPoint corner = Positioner::getRightBottomCorner( pToLeft ).origin;

	if( pToLeft->getParent() == pOf )
	{
		Positioner::setCornerToPosition( pToLeft, corner,
										 CCPoint( -margin, corner.y ) );
		return;
	}


	//Different coordinate system
	assert( pToLeft->getParent() == pOf->getParent() );

	Positioner::setCornerToPosition( pToLeft, corner,
									 CCPoint( Positioner::getLeftBottomCorner( pOf ).origin.x - margin, corner.y ) );
}

void Positioner::toRightOf( CCNode* pToRight, CCNode* pOf, const float margin )
{
	CCAssert( pToRight != pOf, "Bad argument!" );
	assert( pToRight != nullptr );
	assert( pOf != nullptr );
	assert( pToRight->getParent() != nullptr );

	const CCPoint corner = Positioner::getLeftBottomCorner( pToRight ).origin;

	if( pToRight->getParent() == pOf )
	{
		Positioner::setCornerToPosition( pToRight, corner,
										 CCPoint( pOf->getContentSize().width + margin, corner.y ) );
		return;
	}

	//Different coordinate system
	assert( pToRight->getParent() == pOf->getParent() );

	Positioner::setCornerToPosition( pToRight, corner,
									 CCPoint( Positioner::getRightBottomCorner( pOf ).origin.x + margin, corner.y ) );
}

void Positioner::toAboveOf( CCNode* pToAbove, CCNode* pOf, const float margin )
{
	CCAssert( pToAbove != pOf, "Bad argument!" );
	CCAssert( pToAbove->getParent() != pOf,
			  "This doesn't work in parent. Different coordinate system" );
	assert( pToAbove->getParent() != nullptr );
	assert( pOf->getParent() != nullptr );
	assert( pToAbove->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getLeftBottomCorner( pToAbove ).origin;
	Positioner::setCornerToPosition( pToAbove, corner,
									 CCPoint( corner.x, Positioner::getLeftTopCorner( pOf ).origin.y + margin ) );
}

void Positioner::toBelowOf( CCNode* pToBelow, CCNode* pOf, const float margin )
{
	CCAssert( pToBelow != pOf, "Bad argument!" );
	CCAssert( pToBelow->getParent() != pOf,
			  "This doesn't work in parent. Different coordinate system" );
	assert( pToBelow->getParent() != nullptr );
	assert( pOf->getParent() != nullptr );
	assert( pToBelow->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getLeftTopCorner( pToBelow ).origin;
	Positioner::setCornerToPosition( pToBelow, corner,
									 CCPoint( corner.x, Positioner::getLeftBottomCorner( pOf ).origin.y - margin ) );
}

void Positioner::toCenterHorizontalOf( CCNode* pToCenterHorizontal,
									   CCNode* pOf,
									   const float margin )
{
	CCAssert( pToCenterHorizontal != pOf, "Bad argument!" );
	CCAssert( pToCenterHorizontal->getParent() != pOf,
			  "This doesn't work in parent. Different coordinate system" );
	assert( pToCenterHorizontal->getParent() != nullptr );
	assert( pOf->getParent() != nullptr );
	assert( pToCenterHorizontal->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getCenterPoint( pToCenterHorizontal ).origin;
	Positioner::setCornerToPosition( pToCenterHorizontal, corner,
									 CCPoint( Positioner::getCenterPoint( pOf ).origin.x + margin, corner.y ) );
}

void Positioner::toCenterVerticalOf( CCNode* pToCenterVertical, CCNode* pOf,
									 const float margin )
{
	CCAssert( pToCenterVertical != pOf, "Bad argument!" );
	CCAssert( pToCenterVertical->getParent() != pOf,
			  "This doesn't work in parent. Different coordinate system" );
	assert( pToCenterVertical->getParent() != nullptr );
	assert( pOf->getParent() != nullptr );
	assert( pToCenterVertical->getParent() == pOf->getParent() );

	CCPoint corner = Positioner::getCenterPoint( pToCenterVertical ).origin;
	Positioner::setCornerToPosition( pToCenterVertical, corner,
									 CCPoint( corner.x, Positioner::getCenterPoint( pOf ).origin.y + margin ) );
}

void Positioner::toCenterOf( CCNode* pToCenter, CCNode* pOf )
{
	assert( pToCenter );
	assert( pOf );
	Positioner::toCenterHorizontalOf( pToCenter, pOf, 0.0f );
	Positioner::toCenterVerticalOf( pToCenter, pOf, 0.0f );
}

} /* namespace KoalaGui */

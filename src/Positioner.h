/*
 * Positioner.h
 *
 *  Created on: May 12, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCPOSITIONER_H_
#define CCPOSITIONER_H_

#include "cocos2d.h"
USING_NS_CC;

namespace gui
{

namespace Positioner
{

CCPoint getLeftBottomCorner ( CCNode* pNode );
CCPoint getLeftTopCorner ( CCNode* pNode );
CCPoint getRightTopCorner ( CCNode* pNode );
CCPoint getRightBottomCorner ( CCNode* pNode );

CCPoint getCenterPoint ( CCNode* pNode );
CCPoint getCenterPoint ( const CCSize& size );

void setCornerToPosition ( CCNode* pNode, const CCPoint& cornerPosition,
						   const CCPoint& position );

float getScaledWidth ( CCNode* pNode );
float getScaledHeight ( CCNode* pNode );
CCSize getScaledSize ( CCNode* pNode );

void centerInParent ( CCNode* pChildNode );
void leftEdge ( CCNode* pChildNode, const float margin = 0 );
void rightEdge ( CCNode* pChildNode, const float margin = 0 );
void topEdge ( CCNode* pChildNode, const float margin = 0 );
void bottomEdge ( CCNode* pChildNode, const float margin = 0 );
void centerInParentVertically ( CCNode* pChildNode, const float margin = 0 );
void centerInParentHorizontally ( CCNode* pChildNode, const float margin = 0 );

void toLeftOf ( CCNode* pToLeft, CCNode* pOf, const float margin = 0 );
void toRightOf ( CCNode* pToRight, CCNode* pOf, const float margin = 0 );
void toAboveOf ( CCNode* pToAbove, CCNode* pOf, const float margin = 0 );
void toBelowOf ( CCNode* pToBelow, CCNode* pOf, const float margin = 0 );
void toCenterHorizontalOf ( CCNode* pToCenterHorizontal, CCNode* pOf,
							const float margin = 0 );
void toCenterVerticalOf ( CCNode* pToCenterVertical, CCNode* pOf,
						  const float margin = 0 );

}

} /* namespace gui */
#endif /* CCPOSITIONER_H_ */

#ifndef CCSCALE_H
#define CCSCALE_H

#include "cocos2d.h"

using namespace cocos2d;

namespace Scale
{

enum class Type
{
	FIT_X, FIT_Y, FIT_X_Y, NONE , ADJUST_X, ADJUST_Y
};

bool scale( CCNode* pNode, const Type& scaleType, CCSize orginalSize,
			const CCSize& desiredSize );

void scaleMatchHeight( CCNode* pNode, float height );
void scaleToFitParent( CCNode* pNode, Type scaleType = Type::FIT_X_Y, float multiplier = 1.0f );

}
#endif /* CCSCALE_H */

#include "view/Scale.h"
#include "utils/Utils.h"
namespace Scale
{
bool scale( CCNode* pNode, const Type& scaleType, CCSize orginalSize,
			const CCSize& desiredSize )
{

	CCAssert( pNode != nullptr, "Node cannot be NULL!" );

	if( orginalSize.width == 0 )
	{
		orginalSize.width = 0.0000001F;
		assert( false );
	}

	if( orginalSize.height == 0 )
	{
		orginalSize.height = 0.0000001F;
		assert( false );
	}

	const float xScale = desiredSize.width / orginalSize.width;
	const float yScale = desiredSize.height / orginalSize.height;

	switch( scaleType )
	{
		case Type::FIT_X:
		{
			pNode->setScaleX( xScale );
			break;
		}

		case Type::FIT_Y:
		{
			pNode->setScaleY( yScale );
			break;
		}

		case Type::FIT_X_Y:
		{
			pNode->setScaleX( xScale );
			pNode->setScaleY( yScale );
			break;
		}

		case Type::NONE:
		{
			pNode->setScale( 1 );
			break;
		}

		case Type::ADJUST_X:
		{
			pNode->setScale( xScale );
			break;
		}

		case Type::ADJUST_Y:
		{
			pNode->setScale( yScale );
			break;
		}

		default:
		{
			CCAssert( false, "No such scale!" );
			return false;
		}
	}

	return true;
}

void scaleMatchHeight( CCNode* pNode, float height )
{
	const float scaleFactor = height
							  / ( pNode->getContentSize().height * pNode->getScaleY() );
	pNode->setScale( scaleFactor );
}

void scaleToFitParent( CCNode* pNode, Type scaleType /*= FIT_X_Y*/, float multiplier /*= 1.0f*/ )
{
	assert( pNode );
	assert( pNode->getParent() );

	scale( pNode, scaleType, pNode->getContentSize(), pNode->getParent()->getContentSize() );
	pNode->setScaleX( pNode->getScaleX() * multiplier );
	pNode->setScaleY( pNode->getScaleY() * multiplier );
}

}

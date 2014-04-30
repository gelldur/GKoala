/*
 * ScissorHelper.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: Dawid Drozd
 */

#include "view/ScissorHelper.h"

namespace KoalaGui
{

ScissorHelper::ScissorHelper() :
	m_scissorRestored( false )
{
}

ScissorHelper::~ScissorHelper()
{
}

void ScissorHelper::beforeDraw( const CCRect& nodeRect )
{
	m_scissorRestored = false;

	if( CCEGLView::sharedOpenGLView()->isScissorEnabled() )
	{
		m_scissorRestored = true;
		m_parentScissorRect = CCEGLView::sharedOpenGLView()->getScissorRect();

		if( nodeRect.intersectsRect( m_parentScissorRect ) )
		{
			float xMax = MAX( nodeRect.origin.x, m_parentScissorRect.origin.x );
			float yMax = MAX( nodeRect.origin.y, m_parentScissorRect.origin.y );
			float xMin =
				MIN( nodeRect.origin.x + nodeRect.size.width,
					 m_parentScissorRect.origin.x + m_parentScissorRect.size.width );
			float yMin =
				MIN( nodeRect.origin.y + nodeRect.size.height,
					 m_parentScissorRect.origin.y + m_parentScissorRect.size.height );
			CCEGLView::sharedOpenGLView()->setScissorInPoints( xMax, yMax,
					xMin - xMax + 1, yMin - yMax + 1 );
		}
	}
	else
	{
		glEnable( GL_SCISSOR_TEST );
		CCEGLView::sharedOpenGLView()->setScissorInPoints( nodeRect.origin.x,
				nodeRect.origin.y, nodeRect.size.width,
				nodeRect.size.height + 1 );
	}
}


void ScissorHelper::beforeDraw( const CCRect& nodeRect,
								const CCRect& parentRect )
{
	glEnable( GL_SCISSOR_TEST );
	m_scissorRestored = false;
	m_parentScissorRect = parentRect;

	if( nodeRect.intersectsRect( m_parentScissorRect ) )
	{
		float xMax = MAX( nodeRect.origin.x, m_parentScissorRect.origin.x );
		float yMax = MAX( nodeRect.origin.y, m_parentScissorRect.origin.y );
		float xMin =
			MIN( nodeRect.origin.x + nodeRect.size.width,
				 m_parentScissorRect.origin.x + m_parentScissorRect.size.width );
		float yMin =
			MIN( nodeRect.origin.y + nodeRect.size.height,
				 m_parentScissorRect.origin.y + m_parentScissorRect.size.height );
		CCEGLView::sharedOpenGLView()->setScissorInPoints( xMax, yMax,
				xMin - xMax + 1, yMin - yMax + 1 );
	}
}


void ScissorHelper::afterDraw()
{
	if( m_scissorRestored )
	{
		//restore the parent's scissor rect
		CCEGLView::sharedOpenGLView()->setScissorInPoints(
			m_parentScissorRect.origin.x, m_parentScissorRect.origin.y,
			m_parentScissorRect.size.width,
			m_parentScissorRect.size.height );
	}
	else
	{
		glDisable( GL_SCISSOR_TEST );
	}
}

CCRect ScissorHelper::getViewRect( CCNode* pNodeObject )
{
	CCPoint screenPosition = pNodeObject->convertToWorldSpace( CCPointZero );

	float scaleX = 1;
	float scaleY = 1;

	for( CCNode* pNode = pNodeObject; pNode != nullptr; pNode =
				pNode->getParent() )
	{
		scaleX *= pNode->getScaleX();
		scaleY *= pNode->getScaleY();
	}

	if( scaleX < 0 )
	{
		screenPosition.x += pNodeObject->getContentSize().width * scaleX;
		scaleX = -scaleX;
	}

	if( scaleY < 0 )
	{
		screenPosition.y += pNodeObject->getContentSize().height * scaleY;
		scaleY = -scaleY;
	}

	return CCRect( screenPosition.x, screenPosition.y,
				   pNodeObject->getContentSize().width * scaleX,
				   pNodeObject->getContentSize().height * scaleY );
}

} /* namespace KoalaGui */

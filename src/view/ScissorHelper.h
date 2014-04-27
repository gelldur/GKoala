/*
 * ScissorHelper.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Dawid Drozd
 */

#ifndef SCISSORHELPER_H_
#define SCISSORHELPER_H_

#include "cocos2d.h"
USING_NS_CC;

namespace KoalaGui
{

class ScissorHelper
{
public:
	virtual ~ScissorHelper();

	void beforeDraw ( const CCRect& nodeRect );
	void beforeDraw ( const CCRect& nodeRect , const CCRect& parentRect );
	void afterDraw();

	CCRect getViewRect ( CCNode* pNodeObject );
protected:
	ScissorHelper();
private:
	bool m_scissorRestored;
	CCRect m_parentScissorRect;
};

} /* namespace KoalaGui */
#endif /* SCISSORHELPER_H_ */

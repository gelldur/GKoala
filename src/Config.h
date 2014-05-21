/*
 * Config.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#pragma once

#include "cocos2d.h"

namespace GKoala
{

using BaseClass = cocos2d::CCObject;

/**
 * 0 - don't use asserts
 * 1 - use asserts
 * 2 - use exceptions
 */
#define FAILURE 1

#if FAILURE == 2
//TODO Implement exceptions
#define GKoala_assert( condition,message )

#elif FAILURE == 1

#define GKoala_assert( condition ,message) CCAssert(condition,message)

#else
#define GKoala_assert( condition,message ) // ignore
#endif

#define GLOG(format, ...) CCLOG(format,##__VA_ARGS__)


#define GK_CREATE(Clazz) \
	static Clazz* create()\
	{\
		Clazz *pRet = new Clazz();\
		if (pRet)\
		{\
			pRet->autorelease();\
			return pRet;\
		}\
		CC_SAFE_DELETE(pRet);\
		return pRet = nullptr;\
	}

}

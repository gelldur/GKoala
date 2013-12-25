/*
 * Config.h
 *
 *  Created on: Dec 25, 2013
 *      Author: dawid
 */

#ifndef GKOALA_CONFIG_H_
#define GKOALA_CONFIG_H_

#include "cocos2d.h"

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

#endif /* GKOALA_CONFIG_H_ */

/*
 * ParserInterface.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include <functional>

namespace GKoala
{

class ParserInterface
{
public:
	typedef std::function<void(cocos2d::CCNode*)> Rule;
	ParserInterface(){}
	virtual ~ParserInterface(){}

	virtual void addRule(const std::string& objectName,const std::string& propertyName,const Rule& ruleFunction) = 0;
	virtual cocos2d::CCNode* parseLayout(const std::string& layoutFile) = 0;
};

}

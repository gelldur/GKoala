/*
 * ParserInterface.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "cocos2d.h"

#include <functional>
#include <regex>

namespace GKoala
{

struct Parameter
{
	Parameter(cocos2d::CCNode* const pNode):
		pOwnerNode(pNode)
	{
	}

	int _int = 0;
	float _float = 0;
	cocos2d::CCNode* const pOwnerNode = nullptr;
	cocos2d::CCNode* pNode = nullptr;
	void* pData = nullptr;
	std::string _string;

	std::vector<Parameter> parameters;
};

struct RegEx
{
	RegEx(const std::string& regex):
		stringRepresentation(regex)
		,regex(regex)
	{
	}

	RegEx(const char* pRegex):
		stringRepresentation(pRegex)
		,regex(stringRepresentation)
	{
	}

	const std::string stringRepresentation;
	const std::regex regex;

     bool operator <(const RegEx& other) const
     {
        return stringRepresentation < other.stringRepresentation;
     }
};

union Callback
{
	std::function<void(Parameter&)>
};

class ParserInterface
{
public:
	typedef std::function<void(Parameter&)> Rule;
	typedef std::function<cocos2d::CCNode*()> InitRule;
	ParserInterface(){}
	virtual ~ParserInterface(){}

	virtual void addInitRule(const RegEx& objectName,const InitRule& initRule) = 0;

	virtual void addRule(const RegEx& objectName,const RegEx& propertyName,const Rule& ruleFunction) = 0;
	virtual cocos2d::CCNode* parseLayout(const std::string& layoutFile) = 0;
};

}

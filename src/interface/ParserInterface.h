/*
 * ParserInterface.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "Config.h"
#include "parser/engine/Rule.h"
#include "parser/engine/TypesRecognizers.h"

#include "cocos2d.h"

#include <functional>
#include <regex>
#include <type_traits>


namespace GKoala
{

using InitRule = std::function<BaseClass*()>;

struct RegEx;

template<typename Engine>
class ParserInterface
{
public:
	ParserInterface(){}
	~ParserInterface(){}

	void addInitRule(const RegEx& objectName,const InitRule& initRule)
	{
		m_engine.addInitRule(objectName,initRule);
	}

	template<typename Base,typename Value>
	void addRule(const RegEx& objectName,const RegEx& propertyName,const std::function< void(Base,BaseClass* ,Value) >& ruleFunction)
	{
		m_engine.addRule(objectName,propertyName,new Rule<Base,Value>(ruleFunction));
	}


	inline cocos2d::CCNode* parseLayout(const std::string& layoutFile)
	{
		return m_engine.parseLayout(layoutFile);
	}

private:
	Engine m_engine;
};

/**
 * We only store string representation of regex for debug purposes.
 */
struct RegEx
{
	RegEx(const std::string & regex):
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

}

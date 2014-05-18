/*
 * ParsingEngine.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "interface/ParserInterface.h"
#include "parser/engine/json/ParsingException.h"
#include "json/json.h"

#include "cocos2d.h"

#include <map>

namespace GKoala
{

class ParsingEngine
{
public:
	ParsingEngine();
	virtual ~ParsingEngine();

	void addInitRule(const RegEx& objectName,const InitRule& initRule);

	void addRule(const RegEx& objectName,const RegEx& propertyName,const RuleBase* ruleFunction);

	cocos2d::CCNode* parseLayout(const std::string& layoutFile);

private:
	std::map<RegEx,InitRule> m_initRules;
	std::map<RegEx, std::map<RegEx,const RuleBase* > > m_rules;

	BaseClass* parseObject(const Json::Value& value,BaseClass* pContext)const;

	const InitRule& findInitRule(const std::string& objectField)const;
	const RuleBase* findFieldRule(const std::string& objectField,const std::string& fieldName)const;

	std::vector<int> parseArrayInt(const Json::Value& value)const;
	std::vector<float> parseArrayFloat(const Json::Value& value)const;
	std::vector<std::string> parseArrayString(const Json::Value& value)const;
	std::vector<BaseClass*> parseArrayObject(const Json::Value& value,BaseClass* pContext)const;
};

}

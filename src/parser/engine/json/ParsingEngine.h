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

class ParsingEngine : public ParserInterface
{
public:
	ParsingEngine();
	virtual ~ParsingEngine();

	virtual void addInitRule(const RegEx& objectName,const InitRule& initRule) override;
	virtual void addRule(const RegEx& objectName,const RegEx& propertyName,const Rule& ruleFunction) override;
	virtual cocos2d::CCNode* parseLayout(const std::string& layoutFile) override;

private:
	std::map<RegEx,InitRule> m_initRules;
	std::map<RegEx, std::map<RegEx, Rule > > m_rules;

	cocos2d::CCNode* parseObject(const Json::Value& value)const;
	std::vector<Parameter> parseArray(const Json::Value& value)const;

	const InitRule& findInitRule(const std::string& objectField)const;
	const Rule& findFieldRule(const std::string& objectField,const std::string& fieldName)const;
};

}

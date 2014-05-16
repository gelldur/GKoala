/*
 * ParsingEngine.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "interface/ParserInterface.h"

#include "cocos2d.h"

#include <map>

namespace GKoala
{

class ParsingEngine : public ParserInterface
{
public:
	ParsingEngine();
	virtual ~ParsingEngine();

	virtual void addRule(const std::string& objectName,const std::string& propertyName,const Rule& ruleFunction) override;
	virtual cocos2d::CCNode* parseLayout(const std::string& layoutFile) override;

private:
	std::map<std::string, std::map<std::string, Rule > > m_rules;
};

}

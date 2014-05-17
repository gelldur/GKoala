/*
 * ParsingEngine.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/engine/ParsingEngine.h"

namespace GKoala
{

ParsingEngine::ParsingEngine()
{

}

ParsingEngine::~ParsingEngine()
{
}

void ParsingEngine::addRule(const std::string& objectName,const std::string& propertyName, const Rule& ruleFunction)
{
	m_rules[objectName][propertyName] = ruleFunction;
}

cocos2d::CCNode* GKoala::ParsingEngine::parseLayout(
		const std::string& layoutFile)
{
	return nullptr;
}

}


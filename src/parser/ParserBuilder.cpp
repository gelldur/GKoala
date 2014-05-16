/*
 * ParserBuilder.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/ParserBuilder.h"

#include "parser/engine/ParsingEngine.h"

using namespace std;
USING_NS_CC;

namespace GKoala
{

ParserBuilder::ParserBuilder()
{
}

ParserBuilder::~ParserBuilder()
{
}

ParserInterface* ParserBuilder::buildStandardParser()
{
	ParsingEngine* pEngine = new ParsingEngine();
	addStandardRules(pEngine);

	return pEngine;
}

void ParserBuilder::addStandardRules(ParserInterface* pParser)
{
	addRulesLinearLayout(pParser);
	addRulesCCSprite(pParser);
}

void ParserBuilder::addRulesLinearLayout(ParserInterface* pParser)
{
	const std::string object = "LinearLayout";
	pParser->addRule(object,"orientation",
			[](CCNode* pNode)
			{
				//Add code here
			});
}

void ParserBuilder::addRulesCCSprite(ParserInterface* pParser)
{
}

}

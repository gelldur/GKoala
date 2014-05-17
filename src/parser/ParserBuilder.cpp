/*
 * ParserBuilder.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/ParserBuilder.h"

#include "parser/engine/json/ParsingEngine.h"

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
	addRulesGlobal(pParser);
	addRulesLinearLayout(pParser);
	addRulesCCSprite(pParser);
}


void ParserBuilder::addRulesGlobal(ParserInterface* pParser)
{
	const std::string object = "(.*)";//Any object
	pParser->addRule(object,"id",
	[](Parameter parameter)
	{
		parameter.pOwnerNode->setTag(0);
	});

	pParser->addRule(object,"width",
	[](Parameter parameter)
	{
		auto size = parameter.pOwnerNode->getContentSize();
		size.width = parameter._float;

		parameter.pOwnerNode->setContentSize(size);
	});

	pParser->addRule(object,"height",
	[](Parameter parameter)
	{
		auto size = parameter.pOwnerNode->getContentSize();
		size.height = parameter._float;

		parameter.pOwnerNode->setContentSize(size);
	});

}

void ParserBuilder::addRulesLinearLayout(ParserInterface* pParser)
{
	const std::string object = "LinearLayout";
	pParser->addInitRule(object,[]()
	{
		return CCNode::create();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////
	pParser->addRule(object,"views",
	[](Parameter parameter)
	{
		for(auto&& element : parameter.parameters)
		{
			parameter.pOwnerNode->addChild(element.pNode);
		}
	});

	pParser->addRule(object,"orientation",
	[](Parameter parameter)
	{
		(void)parameter;
	});
}

void ParserBuilder::addRulesCCSprite(ParserInterface* pParser)
{
	const std::string object = "CCSprite";
	pParser->addInitRule(object,[]()
	{
		return CCSprite::create();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////
	pParser->addRule(object,"source",
	[](Parameter parameter)
	{
		CCSprite* pSprite = dynamic_cast<CCSprite*>(parameter.pOwnerNode);
		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(parameter._string.c_str());
		pSprite->setTexture(pTexture);
	});
}

}

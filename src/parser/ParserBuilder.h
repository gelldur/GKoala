/*
 * ParserBuilder.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "interface/ParserInterface.h"

#include "parser/engine/json/ParsingEngine.h"

namespace GKoala
{

typedef ParserInterface<ParsingEngine> StandardParser;

class ParserBuilder
{
public:
	ParserBuilder();
	~ParserBuilder();

	StandardParser* buildStandardParser();

private:
	void addStandardRules(StandardParser* pParser);

	void addRulesGlobal(StandardParser* pParser);
	void addRulesLinearLayout(StandardParser* pParser);
	void addRulesLinearLayoutParameters(StandardParser* pParser);
	void addRulesSizePolicy(StandardParser* pParser);
	void addRulesCCSprite(StandardParser* pParser);

	void addRulesForTest(StandardParser* pParser);
};

}

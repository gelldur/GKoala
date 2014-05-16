/*
 * ParserBuilder.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "interface/ParserInterface.h"

namespace GKoala
{

class ParserBuilder
{
public:
	ParserBuilder();
	~ParserBuilder();

	ParserInterface* buildStandardParser();

private:
	void addStandardRules(ParserInterface* pParser);

	void addRulesLinearLayout(ParserInterface* pParser);
	void addRulesCCSprite(ParserInterface* pParser);
};

}

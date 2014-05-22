/*
 * ParserBuilder.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/ParserBuilder.h"

#include "layout/configuration/linear/LinearLayoutParameter.h"
#include "interface/LayoutInterface.h"

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

StandardParser* ParserBuilder::buildStandardParser()
{
	StandardParser* pEngine = new StandardParser();
	addStandardRules(pEngine);

	return pEngine;
}

void ParserBuilder::addStandardRules(StandardParser* pParser)
{
	addRulesGlobal(pParser);
	addRulesLinearLayout(pParser);
	addRulesCCSprite(pParser);
	addRulesLinearLayoutParameters(pParser);
}


void ParserBuilder::addRulesGlobal(StandardParser* pParser)
{
	using WorkingNode = CCNode*;

	const std::string object = "(.*)";//Any object

	pParser->addRule<WorkingNode,std::string&>(object,"id",
	[](WorkingNode pNode,BaseClass* pContext, std::string& tag)
	{
		pNode->setTag(tag[0]);
	});

	pParser->addRule<WorkingNode,float>(object,"width",
	[](WorkingNode pNode,BaseClass* pContext,float width)
	{
		auto size = pNode->getContentSize();
		size.width = width;

		pNode->setContentSize(size);
	});

	pParser->addRule<CCNode*,float>(object,"height",
	[](WorkingNode pNode,BaseClass* pContext,float height)
	{
		auto size = pNode->getContentSize();
		size.height = height;

		pNode->setContentSize(size);
	});

	pParser->addRule<CCNode*,float>(object,"parameters",
	[](WorkingNode pNode,BaseClass* pContext,float height)
	{
		auto size = pNode->getContentSize();
		size.height = height;

		pNode->setContentSize(size);
	});

	pParser->addRule<WorkingNode,float >(object,"margin_left_sp",
	[](WorkingNode pObject,BaseClass* pContext,float margin)
	{
		createLayoutParametersForContext(pObject);

		auto pUserObject = pObject->getUserObject();
		auto pLayoutParams = dynamic_cast<LayoutParameter*>(pUserObject);

		pLayoutParams->setMarginLeft(SP::sp(margin));
	});
	pParser->addRule<WorkingNode,float >(object,"margin_right_sp",
	[](WorkingNode pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginRight(SP::sp(margin));
	});
	pParser->addRule<RWorkingNode,float >(object,"margin_top_sp",
	[](WorkingNode pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginTop(SP::sp(margin));
	});
	pParser->addRule<WorkingNode,float >(object,"margin_bottom_sp",
	[](WorkingNode pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginBottom(SP::sp(margin));
	});
}

void ParserBuilder::addRulesLinearLayout(StandardParser* pParser)
{
	using ReturnObject = LayoutInterface;

	const std::string object = "LinearLayout";
	pParser->addInitRule(object,[]()
	{
		return ReturnObject::create();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////

	pParser->addRule<ReturnObject*,vector<CCNode*>& >(object,"views",
	[](ReturnObject* pNode,BaseClass* pContext,vector<CCNode*>& views)
	{
		for(auto&& pView : views)
		{
			auto pObject = pView->getUserObject();
			if(pObject == nullptr)
			{
				pNode->addChild(pView);
			}
			else
			{
				auto pParameter = dynamic_cast<LayoutParameter*>(pObject);
				GKoala_assert(pParameter != nullptr,"Error");
				pNode->addChildWith(pView,pParameter);
			}
		}
	});
}

void ParserBuilder::addRulesCCSprite(StandardParser* pParser)
{
	using ReturnObject = CCSprite;

	const std::string object = "CCSprite";
	pParser->addInitRule(object,[]()
	{
		return ReturnObject::create();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////

	pParser->addRule<ReturnObject*,string& >(object,"source",
	[](ReturnObject* pNode,BaseClass* pContext,string& source)
	{
		CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(source.c_str());
	    CCRect rect;
	    rect.size = pTexture->getContentSize();
	    pNode->setTextureRect(rect);
		pNode->setTexture(pTexture);
	});
}

void ParserBuilder::addRulesLinearLayoutParameters(StandardParser* pParser)
{

}

void ParserBuilder::addRulesSizePolicy(StandardParser* pParser)
{

}

void ParserBuilder::addRulesForTest(StandardParser* pParser)
{
	const std::string object = "TestObject";
	pParser->addInitRule(object,[]()
	{
		return CCNode::create();
	});

	pParser->addRule<CCNode*,int>(object,"field1",
	[](CCNode* pNode,BaseClass* pContext,int integer)
	{
		GLOG("field1");
		GLOG("value: %d",integer);
	});
	pParser->addRule<CCNode*,float>(object,"field2",
	[](CCNode* pNode,BaseClass* pContext,float integer)
	{
		GLOG("field2");
		GLOG("value: %f",integer);
	});

	pParser->addRule<CCNode*,std::string&>(object,"field7",
	[](CCNode* pNode,BaseClass* pContext,std::string& integer)
	{
		GLOG("field7");
		GLOG("value: %s",integer.c_str());
	});

	pParser->addRule<CCNode*,std::vector<int>& >(object,"field9",
	[](CCNode* pNode,BaseClass* pContext,std::vector<int>&  integer)
	{
		GLOG("field9");
		for(auto&& element : integer)
		{
			GLOG("valuez: %d",element);
		}
	});

	pParser->addRule<CCNode*,std::vector<float>& >(object,"field10",
	[](CCNode* pNode,BaseClass* pContext,std::vector<float>&  integer)
	{
		GLOG("field10");
		for(auto&& element : integer)
		{
			GLOG("valuez: %f",element);
		}
	});

	pParser->addRule<CCNode*,std::vector<std::string>& >(object,"field11",
	[](CCNode* pNode,BaseClass* pContext,std::vector<std::string>&  integer)
	{
		GLOG("field11");
		for(auto&& element : integer)
		{
			GLOG("valuez: %s",element.c_str());
		}
	});

	pParser->addRule<CCNode*,std::vector<int>& >(object,"field12",
	[](CCNode* pNode,BaseClass* pContext,std::vector<int>&  integer)
	{
		GLOG("field12");
		for(auto&& element : integer)
		{
			GLOG("valuez: %d",element);
		}
	});

	pParser->addRule<CCNode*,CCNode* >(object,"field13",
	[](CCNode* pNode,BaseClass* pContext,CCNode* integer)
	{
		GLOG("field13");
		GLOG("value: %f %p",integer->getContentSize().width,integer);
	});

	pParser->addRule<CCNode*,vector<CCNode*>& >(object,"field14",
	[](CCNode* pNode,BaseClass* pContext,vector<CCNode*>&  integer)
	{
		GLOG("field14");
		for(auto&& element : integer)
		{
			GLOG("value: %f %p",element->getContentSize().width,element);
		}
	});
}

void ParserBuilder::createLayoutParametersForContext(cocos2d::CCNode* pNode)
{
	if(pNode->getUserObject() != nullptr)
	{
		return;
	}

	auto pLayoutInterface = dynamic_cast<LayoutInterface*>(pNode->getUserObject());
	if(pLayoutInterface == nullptr)
	{
		pNode->setUserObject(LayoutParameter::create());
		GKoala_assert(false,"For now we don;t want this situation");
	}
	else
	{
		pLayoutInterface->is
	}
}

}

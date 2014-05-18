/*
 * ParserBuilder.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/ParserBuilder.h"

#include "layout/configuration/linear/LinearLayoutParameter.h"

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

	pParser->addRule<WorkingNode,float&>(object,"width",
	[](WorkingNode pNode,BaseClass* pContext,float& width)
	{
		auto size = pNode->getContentSize();
		size.width = width;

		pNode->setContentSize(size);
	});

	pParser->addRule<CCNode*,float&>(object,"height",
	[](WorkingNode pNode,BaseClass* pContext,float& height)
	{
		auto size = pNode->getContentSize();
		size.height = height;

		pNode->setContentSize(size);
	});

	pParser->addRule<CCNode*,float&>(object,"parameters",
	[](WorkingNode pNode,BaseClass* pContext,float& height)
	{
		auto size = pNode->getContentSize();
		size.height = height;

		pNode->setContentSize(size);
	});
}

void ParserBuilder::addRulesLinearLayout(StandardParser* pParser)
{
	using ReturnObject = CCNode;

	const std::string object = "LinearLayout";
	pParser->addInitRule(object,[]()
	{
		return ReturnObject::create();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////

	pParser->addRule<ReturnObject*,vector<CCNode*>& >(object,"views",
	[](ReturnObject* pNode,BaseClass* pContext,vector<CCNode*>& views)
	{
		for(auto&& element : views)
		{
			pNode->addChild(element);
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
	using ReturnObject = LinearLayoutParameter;

	const std::string object = "LinearLayoutParameter";
	pParser->addInitRule(object,[]()
	{
		return ReturnObject::createWrapWrap();
	});
	////////////////////////////////////////////////////////////////////////////////////////////////

	pParser->addRule<ReturnObject*,float >(object,"margin_left_sp",
	[](ReturnObject* pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginLeft(SP::sp(margin));
	});
	pParser->addRule<ReturnObject*,float >(object,"margin_right_sp",
	[](ReturnObject* pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginRight(SP::sp(margin));
	});
	pParser->addRule<ReturnObject*,float >(object,"margin_top_sp",
	[](ReturnObject* pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginTop(SP::sp(margin));
	});
	pParser->addRule<ReturnObject*,float >(object,"margin_bottom_sp",
	[](ReturnObject* pObject,BaseClass* pContext,float margin)
	{
		pObject->setMarginBottom(SP::sp(margin));
	});
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

	pParser->addRule<CCNode*,int&>(object,"field1",
	[](CCNode* pNode,BaseClass* pContext,int& integer)
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


	pParser->addRule<CCNode*,char>(object,"field3",
	[](CCNode* pNode,BaseClass* pContext,char integer)
	{
		GLOG("field3");
		GLOG("value: %c",integer);
	});

	pParser->addRule<CCNode*,int*>(object,"field4",
	[](CCNode* pNode,BaseClass* pContext,int* integer)
	{
		GLOG("field4");
		GLOG("value: %d",*integer);
	});
	pParser->addRule<CCNode*,double>(object,"field5",
	[](CCNode* pNode,BaseClass* pContext,double integer)
	{
		GLOG("field5");
		GLOG("value: %lf",integer);
	});
	pParser->addRule<CCNode*,std::string>(object,"field6",
	[](CCNode* pNode,BaseClass* pContext,std::string integer)
	{
		GLOG("field6");
		GLOG("value: %s",integer.c_str());
	});
	pParser->addRule<CCNode*,std::string&>(object,"field7",
	[](CCNode* pNode,BaseClass* pContext,std::string& integer)
	{
		GLOG("field7");
		GLOG("value: %s",integer.c_str());
	});

	pParser->addRule<CCNode*,std::string*>(object,"field8",
	[](CCNode* pNode,BaseClass* pContext,std::string* integer)
	{
		GLOG("field8");
		GLOG("value: %s",integer->c_str());
	});

	pParser->addRule<CCNode*,std::vector<int> >(object,"field9",
	[](CCNode* pNode,BaseClass* pContext,std::vector<int>  integer)
	{
		GLOG("field9");
		for(auto&& element : integer)
		{
			GLOG("valuez: %d",element);
		}
	});

	pParser->addRule<CCNode*,std::vector<float> >(object,"field10",
	[](CCNode* pNode,BaseClass* pContext,std::vector<float>  integer)
	{
		GLOG("field10");
		for(auto&& element : integer)
		{
			GLOG("valuez: %f",element);
		}
	});

	pParser->addRule<CCNode*,std::vector<std::string> >(object,"field11",
	[](CCNode* pNode,BaseClass* pContext,std::vector<std::string>  integer)
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
	[](CCNode* pNode,BaseClass* pContext,CCNode*  integer)
	{
		GLOG("field13");
		GLOG("value: %f %p",integer->getContentSize().width,integer);
	});

	pParser->addRule<CCNode*,vector<CCNode*> >(object,"field14",
	[](CCNode* pNode,BaseClass* pContext,vector<CCNode*>  integer)
	{
		GLOG("field14");
		for(auto&& element : integer)
		{
			GLOG("value: %f %p",element->getContentSize().width,element);
		}
	});
}

}

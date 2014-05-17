#include "HelloWorldScene.h"
USING_NS_CC;

#include "LayoutFactory.h"
#include "parser/ParserBuilder.h"
using namespace GKoala;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene* scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld* layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild ( layer );

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	ParserBuilder builder;
	auto pParser = builder.buildStandardParser();

	auto pLayout = pParser->parseLayout("layouts/simple_layout.layout");

	addChild(pLayout);

	delete pParser;

//	LayoutFactory factory;
//	auto pLayout = factory.createLinearLayout();
//	addChild(pLayout);
//
//	pLayout->setOptions(LinearLayoutParameter::Gravity::TOP | LinearConfiguration::Orientation::VERTICAL);
//
//	pLayout->setContentSize(getContentSize());
//
//	auto pParameter = LinearLayoutParameter::createWrapWrap();
//	pParameter->setMarginLeft(SP::sp(10));
//	pParameter->setMarginTop(SP::sp(20));
//	pParameter->setMarginBottom(SP::sp(50));
//	pParameter->setMarginRight(SP::sp(30));
//	pParameter->setGravity(LinearLayoutParameter::Gravity::TOP |  LinearLayoutParameter::Gravity::CENTER_HORIZONTAL);
//	pLayout->addChildWith(CCSprite::create("test1.png"),pParameter);
//
//	pParameter = LinearLayoutParameter::createWrapWrap();
//	pParameter->setGravity(LinearLayoutParameter::Gravity::TOP | LinearLayoutParameter::Gravity::CENTER_HORIZONTAL );
//
//	pLayout->addChildWith(CCSprite::create("test2.png"),pParameter);
//
//	pParameter = LinearLayoutParameter::createWrapWrap();
//	pParameter->setGravity(LinearLayoutParameter::Gravity::TOP| LinearLayoutParameter::Gravity::CENTER_HORIZONTAL );
//
//	pLayout->addChildWith(CCSprite::create("test3.png"),pParameter);
//
//	pParameter = LinearLayoutParameter::createWrapWrap();
//	pParameter->setMarginLeft(SP::sp(20));
//	pParameter->setMarginTop(SP::sp(30));
//	pParameter->setMarginBottom(SP::sp(10));
//	pParameter->setMarginRight(SP::sp(10));
//	pParameter->setGravity(LinearLayoutParameter::Gravity::TOP | LinearLayoutParameter::Gravity::CENTER_HORIZONTAL );
//
//	pLayout->addChildWith(CCSprite::create("test4.png"),pParameter);
//
//	pParameter = LinearLayoutParameter::createWrapWrap();
//	pParameter->setGravity(LinearLayoutParameter::Gravity::TOP | LinearLayoutParameter::Gravity::CENTER_HORIZONTAL );
//
//	pLayout->addChildWith(CCSprite::create("test5.png"),pParameter);

	return true;
}


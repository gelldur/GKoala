#include "HelloWorldScene.h"
USING_NS_CC;

#include "LayoutFactory.h"
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

	LayoutFactory factory;
	auto pLayout = factory.createLinearLayout();
	addChild(pLayout);

	pLayout->setContentSize(getContentSize());

	pLayout->addChild(CCSprite::create("test1.png"));
	//pLayout->addChild(CCSprite::create("test2.png"));



	return true;
}


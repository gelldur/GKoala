/*
 * Puzzle.cpp
 *
 *  Created on: Oct 5, 2013
 *      Author: dawid
 */

#include "view/Puzzle.h"
#include "json/json.h"

namespace KoalaLib
{

namespace Puzzle
{
CCSprite* parseFile( const string& jsonFileName, const string& plistFileName,
					 const string& textureFileName )
{
	CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage( textureFileName.c_str() );
	return parseFile( jsonFileName, plistFileName, pTexture );
}

CCSprite* parseFile( const string& jsonFileName, const string& plistFileName,
					 CCTexture2D* pTexture )
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( plistFileName.c_str(),
			pTexture );

	Json::Value value;
	Json::Reader reader;

	unsigned long fileDataSize = 0;
	char* pData = reinterpret_cast<char*>( CCFileUtils::sharedFileUtils()->getFileData(
			jsonFileName.c_str(), "r", &fileDataSize ) );

	if( pData == nullptr || fileDataSize == 0 )
	{
		CCAssert( false, "Can't read file!" );
		return nullptr;
	}

	bool parsSuccess = reader.parse( pData, pData + fileDataSize, value, false );

	if( parsSuccess == false )
	{
		assert( false );
		delete[] pData;
		return nullptr;
	}

	delete[] pData;

	CCSprite* pParentSprite = CCSprite::create();
	pParentSprite->setTexture( pTexture );

	pParentSprite->setContentSize( CCSize( value.get( "sprite_width", 0 ).asInt(),
										   value.get( "sprite_height", 0 ).asInt() ) );
	assert( pParentSprite->getContentSize().width != 0 );
	assert( pParentSprite->getContentSize().height != 0 );

	const Json::Value& frames = value.get( "frames", Json::nullValue );
	const CCSize parentSize = pParentSprite->getContentSize();

	for( unsigned i = 0; i < frames.size(); ++i )
	{
		const Json::Value& frame = frames[i];
		CCSprite* pSprite = CCSprite::createWithSpriteFrameName( frame.get( "file",
							"error" ).asCString() );

		if( pSprite == nullptr )
		{
			assert( false );
			continue;
		}

		pSprite->setAnchorPoint( CCPoint( 0, 1 ) );
		pSprite->setPosition( CCPoint( frame.get( "left", 0 ).asInt(),
									   parentSize.height - frame.get( "top", 0 ).asInt() ) );
		pParentSprite->addChild( pSprite );
	}

	return pParentSprite;
}

}

} /* namespace KoalaLib */


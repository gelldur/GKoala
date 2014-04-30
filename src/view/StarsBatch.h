/*
 * StarsBatch.h
 *
 *  Created on: October 7, 2013
 *      Author: Jakub Kuderski
 */

#ifndef STARS_BATCH_H
#define STARS_BATCH_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "view/layout/Positioner.h"
#include "utils/Utils.h"
#include <utility>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace KoalaGui;

namespace KoalaLib
{

struct StarsBatchParams
{
	string batchFilename;
	string plistFilename;
	string offStarName;
	string onStarName;
	string halfStarName;
	int maxStarsCount;

	StarsBatchParams( string batchFilename, string plistFilename,
					  string offStarName,
					  string onStarName, string halfStarName, int starsCount ) :
		batchFilename( batchFilename ),
		plistFilename( plistFilename ),
		offStarName( offStarName ),
		onStarName( onStarName ),
		halfStarName( halfStarName ),
		maxStarsCount( starsCount )
	{
		assert( batchFilename.empty() == false );
		assert( plistFilename.empty() == false );
		assert( offStarName.empty() == false );
		assert( onStarName.empty() == false );
		assert( halfStarName.empty() == false );
		assert( starsCount >= 0 );
	}

	StarsBatchParams() :
		maxStarsCount( 0 )
	{
	}
};

class StarsBatch: public CCSpriteBatchNode
{

public:

	virtual ~StarsBatch();
	virtual bool init( StarsBatchParams starsParams, float starsCount );

	static StarsBatch* create( StarsBatchParams starsParams, float starsCount )
	{
		StarsBatch* pRet = new StarsBatch();

		if( pRet && pRet->init( starsParams, starsCount ) )
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return pRet;
		}
	}

	void updateStarsCount( float starsCount );

	float getStarsCount() const
	{
		return m_starsCount;
	}

protected:
	StarsBatch();

private:

	enum ID
	{
		NO_STAR, HALF_STAR, FULL_STAR
	};

	string getStarFrameForIndex( int index );

	float m_starsCount;
	StarsBatchParams m_starsParams;
	vector<CCSprite*> m_stars;
};

} // namespace KoalaLib

#endif // STARS_BATCH_H

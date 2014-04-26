/*
 * Puzzle.h
 *
 *  Created on: Oct 5, 2013
 *      Author: dawid
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include <string>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

namespace KoalaLib
{

namespace Puzzle
{
CCSprite* parseFile ( const string& jsonFileName, const string& plistFileName,
					  const string& textureFileName );
CCSprite* parseFile ( const string& jsonFileName, const string& plistFileName,
					  CCTexture2D* pTexture );
}

} /* namespace KoalaLib */
#endif /* PUZZLE_H_ */

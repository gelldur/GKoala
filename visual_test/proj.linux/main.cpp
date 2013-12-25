#include "../Classes/AppDelegate.h"
#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;

USING_NS_CC;

int main ( int argc, char** argv )
{
	// get application path
	int length;
	char fullpath[500];
	length = readlink ( "/proc/self/exe", fullpath, sizeof ( fullpath ) );
	fullpath[length] = '\0';

	std::string resourcePath = fullpath;
	resourcePath = resourcePath.substr ( 0, resourcePath.find_last_of ( "/" ) );
	string nextTo = resourcePath;
	nextTo += "/Resources";
	resourcePath += "/../../../Resources/";

	CCFileUtils::sharedFileUtils()->setSearchPaths ( vector<string> ( 1,
			resourcePath ) );
	CCFileUtils::sharedFileUtils()->setSearchPaths ( vector<string> ( 1,
			nextTo ) );
	CCEGLView* pEglView = CCEGLView::sharedOpenGLView();

	switch ( 3 )
	{
		case 1:
			// 240 x 400 pixels Resolution of Samsung galaxy i5800
			pEglView->setFrameSize ( 240, 400 );
			break;

		case 2:
			//Nexus 4 768x1280
			pEglView->setFrameSize ( 768, 1280 );
			break;

		case 3:
			//Resolution of Samsung Galaxy S (Vertical)
			pEglView->setFrameSize ( 480, 800 );
			break;

		case 4:
			//Resolution of Samsung Galaxy S (Horizontal)
			pEglView->setFrameSize ( 800, 480 );
			break;

		case 5:
			// 240 x 320 pixels
			pEglView->setFrameSize ( 240, 320 );
			break;

		case 6:
			// 600 x 1024 pixels
			pEglView->setFrameSize ( 600, 1024 );
			break;

		case 7:
			// 960×640 pixels iPhone 4/4S and iPod Touch (4th generation)
			pEglView->setFrameSize ( 640, 960 );
			break;

		case 8:
			// 1136×640 pixels iPhone 5 and iPod Touch (5th generation)
			pEglView->setFrameSize ( 640, 1136 );
			break;

		case 9:
			// 2048×1536 pixels iPad (3rd/4th generation)
			pEglView->setFrameSize ( 1536, 2048 );
			break;

		case 10:
			// 2880×1800 pixels MacBook Pro with Retina Display 15"
			pEglView->setFrameSize ( 1800, 2880 );
			break;

			// sony xperia s
		case 11:
			pEglView->setFrameSize ( 720, 1280 );
			break;

			//Window content size on Nexus 4
		case 12:
			pEglView->setFrameSize ( 480, 740 );
			break;
	}

	AppDelegate* pDelegate = new AppDelegate();
	( void ) pDelegate;
	return CCApplication::sharedApplication()->run();
}

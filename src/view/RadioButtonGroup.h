/*
 * RadioButtonGroup.h
 *
 *  Created on: July 25, 2013
 *      Author: Jakub Kuderski
 */

#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "view/layout/linear/LinearLayout.h"
#include <string>
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

namespace KoalaGui
{

class RadioButtonGroup: public LinearLayout
{

public:

	static const char* notificationName;

	virtual ~RadioButtonGroup();
	virtual void onExit();
	virtual bool ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent );
	virtual void ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent );
	virtual bool init( const vector<string>& textRows, int selectedIndex );
	void initLabels( const string& fontName, float fontSize = 18.0f );

	static RadioButtonGroup* create( const vector<string>& textRows,
									 const string& activeFilename, const string& inactiveFilename )
	{
		RadioButtonGroup* pRet = new RadioButtonGroup( activeFilename,
				inactiveFilename );

		if( pRet &&
				pRet->init( textRows, -1 ) )
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

	int getSelectedIndex();
	void setSelectedIndex( int index, bool forceUpdate = false );
	unsigned getItemsCount();
	vector<CCLabelTTF*> getLabels();

protected:
	RadioButtonGroup( const string& activeFilename,
					  const string& inactiveFilename );

private:

	enum ID
	{
		BUTTON = 1111,
		LABEL
	};

	bool detectTouchOnLayout( LinearLayout* pLayout,
							  const CCTouch* const pTouch );
	LinearLayout* getRowForTouch( const CCTouch* const pTouch );
	void setSelectedRow( LinearLayout* pSelectedRow, bool forceUpdate = false );

	const string m_activeFilename;
	const string m_inactiveFilename;

	int m_selectedIndex;
	int m_touchedIndex;

	vector<LinearLayout*> m_rows;

};

} // namespace KoalaGui

#endif // RADIOBUTTON_H

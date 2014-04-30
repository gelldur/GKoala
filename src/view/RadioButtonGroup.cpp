/*
 * RadioButtonGroup.cpp
 *
 *  Created on: July 25, 2013
 *      Author: Jakub Kuderski
 */

#include "view/RadioButtonGroup.h"
#include "view/layout/Positioner.h"
#include "utils/Utils.h"
using namespace KoalaGui;

const char* RadioButtonGroup::notificationName =
	"NOTIFICATION_RADIOBUTTON_SELECTED";

RadioButtonGroup::RadioButtonGroup( const string& activeFilename,
									const string& inactiveFilename ) :
	m_activeFilename( activeFilename ),
	m_inactiveFilename( inactiveFilename ),
	m_selectedIndex( -1 ),
	m_touchedIndex( -1 )
{
	assert( activeFilename.empty() == false );
	assert( inactiveFilename.empty() == false );
}

RadioButtonGroup::~RadioButtonGroup()
{
}

bool RadioButtonGroup::init( const vector<string>& textRows,
							 int selectedIndex )
{
	if( LinearLayout::init( Orientation::VERTICAL ) == false )
	{
		return false;
	}

	setOpacity( 0 );

	setTouchEnabled( true );
	setTouchMode( kCCTouchesOneByOne );
	setTouchPriority( 0 );

	assert( selectedIndex < ( int ) textRows.size() );

	m_rows = vector<LinearLayout*> ( textRows.size(), nullptr );

	LinearLayoutParams rowParams( LayoutParamsWrapWrap );
	rowParams.setMarginBottom( SizePolicy( Policy::SP, 8 ) );

	for( unsigned i = 0; i < textRows.size(); ++i )
	{

		LinearLayout* pLayoutHorizontal = LinearLayout::create(
											  Orientation::HORIZONTAL );
		addView( pLayoutHorizontal, rowParams );
		pLayoutHorizontal->setOpacity( 0 );

		CCSprite* pButton = CCSprite::create( m_inactiveFilename.c_str() );
		{
			pButton->setTag( ID::BUTTON );

			LinearLayoutParams params( LayoutParamsWrapWrap );
			params.setGravity( Gravity::CENTER_VERTICAL );
			params.setMarginRight( SizePolicy( Policy::SP, 20 ) );
			pLayoutHorizontal->addView( pButton, params );
		}

		//TODO FIX
		CCLabelTTF* pLabel = nullptr;// FontManager::getInstance()->getStandardLabel (
		//	 textRows[i] );
		{
			pLabel->setTag( ID::LABEL );

			LinearLayoutParams params( LayoutParamsWrapWrap );
			params.setGravity( Gravity::CENTER_VERTICAL );
			pLayoutHorizontal->addView( pLabel, params );
		}
		pLayoutHorizontal->setUserData( ( void* )( long ) i );
		m_rows[i] = pLayoutHorizontal;
	}

	m_selectedIndex = selectedIndex;
	setSelectedIndex( m_selectedIndex, true );
	return true;
}

void RadioButtonGroup::initLabels( const string& fontName, float fontSize )
{
	vector<CCLabelTTF*> labels = getLabels();
	assert( labels.size() == m_rows.size() );

	for( CCLabelTTF* pLabel : labels )
	{
		pLabel->setHorizontalAlignment( kCCTextAlignmentLeft );
		pLabel->setFontName( fontName.c_str() );
		pLabel->setFontSize( fontSize );
	}
}

bool RadioButtonGroup::ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent )
{
	LinearLayout* pRow = getRowForTouch( pTouch );

	if( pRow != nullptr )
	{
		m_touchedIndex = ( long ) pRow->getUserData();
		return true;
	}

	return false;
}

void RadioButtonGroup::ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent )
{
	LinearLayout* pRow = getRowForTouch( pTouch );

	if( pRow == nullptr )
	{
		return;
	}

	int index = ( long ) pRow->getUserData();

	if( index != m_touchedIndex )
	{
		m_touchedIndex = -1;
		return;
	}

	setSelectedIndex( index );
	CCNotificationCenter::sharedNotificationCenter()->postNotification(
		notificationName, this );
	return;
}

LinearLayout* RadioButtonGroup::getRowForTouch( const CCTouch* const pTouch )
{
	for( LinearLayout* pRow : m_rows )
	{
		if( detectTouchOnLayout( pRow, pTouch ) )
		{
			return pRow;
		}
	}

	return nullptr;
}

bool RadioButtonGroup::detectTouchOnLayout( LinearLayout* pLayout,
		const CCTouch* const pTouch )
{
	assert( pLayout != nullptr );
	assert( pTouch != nullptr );

	CCPoint touchLocation = pTouch->getLocation();
	CCRect fieldRect;
	fieldRect.origin = pLayout->convertToWorldSpace( CCPointZero );
	fieldRect.size = Positioner::getScaledSize( pLayout );

	return fieldRect.containsPoint( touchLocation );
}

void RadioButtonGroup::onExit()
{
	LinearLayout::onExit();

	CCTexture2D* pActiveTexture = CCTextureCache::sharedTextureCache()->addImage(
									  m_activeFilename.c_str() );
	CCTexture2D* pInactiveTexture = CCTextureCache::sharedTextureCache()->addImage(
										m_inactiveFilename.c_str() );
	CCTextureCache::sharedTextureCache()->removeTexture( pActiveTexture );
	CCTextureCache::sharedTextureCache()->removeTexture( pInactiveTexture );
}

void RadioButtonGroup::setSelectedIndex( int index, bool forceUpdate )
{
	assert( index < ( int ) m_rows.size() );

	if( index == m_selectedIndex && forceUpdate == false )
	{
		return;
	}

	if( m_selectedIndex != -1 )
	{
		CCSprite* pOldButton = dynamic_cast<CCSprite*>
							   ( m_rows[m_selectedIndex]->getChildByTag( ID::BUTTON ) );
		{
			assert( pOldButton != nullptr );
			CCTexture2D* pInactiveTexture = CCTextureCache::sharedTextureCache()->addImage(
												m_inactiveFilename.c_str() );
			assert( pInactiveTexture != nullptr );
			pOldButton->setTexture( pInactiveTexture );
		}
	}

	m_selectedIndex = index;

	if( index == -1 )
	{
		return;
	}

	CCSprite* pNewButton = dynamic_cast<CCSprite*>( m_rows[index]->getChildByTag(
							   ID::BUTTON ) );
	{
		assert( pNewButton != nullptr );
		CCTexture2D* pActiveTexture = CCTextureCache::sharedTextureCache()->addImage(
										  m_activeFilename.c_str() );
		assert( pActiveTexture != nullptr );
		pNewButton->setTexture( pActiveTexture );
	}
}

int RadioButtonGroup::getSelectedIndex()
{
	return m_selectedIndex;
}

unsigned RadioButtonGroup::getItemsCount()
{
	return m_rows.size();
}

vector<CCLabelTTF*> RadioButtonGroup::getLabels()
{
	vector<CCLabelTTF*> labels( m_rows.size(), nullptr );

	for( unsigned i = 0; i < m_rows.size(); ++i )
	{
		labels[i] = dynamic_cast<CCLabelTTF*>( m_rows[i]->getChildByTag(
				ID::LABEL ) );
		assert( labels[i] != nullptr );
	}

	return labels;
}


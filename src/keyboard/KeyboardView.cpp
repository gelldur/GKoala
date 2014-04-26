/*
 * KeyboardView.cpp
 *
 *  Created on: Jul 14, 2013
 *      Author: Dawid Drozd
 */

#include "view/keyboard/KeyboardView.h"
#include "configuration/UserConfiguration.h"
#include "view/KKScaleSprite.h"
#include "utils/TouchManager.h"

namespace keyboard
{

KeyboardView::KeyboardView ( KeyboardController* pController,
							 const KeyboardSettings& settings, const KeyboardLayout& keyboardLayout,
							 const int tag ) :
	LinearLayout ( tag ),
	m_settings ( settings ),
	m_firstRow ( 10 ),
	m_secondRow ( 9 ),
	m_thirdRow ( 7 ),
	m_nextView ( -1 ),
	m_preferedButtonSize ( 43, 65 ),
	m_lastRowButtonSize ( 43, 65 ),
	m_pController ( pController ),
	m_pButtonSymbols ( nullptr ),
	m_pButtonLetters ( nullptr ),
	m_pButtonDone ( nullptr ),
	m_pButtonLettersOrder ( nullptr ),
	m_pButtonDelete ( nullptr ),
	m_keyboardLayout ( keyboardLayout ),
	m_pSpecialDialog ( nullptr ),
	m_pRegisteredTouches ( nullptr )
{
	m_pRegisteredTouches = CCSet::create();
	m_pRegisteredTouches->retain();
}

KeyboardView::~KeyboardView()
{
	m_buttons.clear();
	m_rows.clear();
	m_pRegisteredTouches->removeAllObjects();
	CC_SAFE_RELEASE_NULL ( m_pRegisteredTouches );
}

bool KeyboardView::init()
{
	if ( LinearLayout::init ( Orientation::VERTICAL ) == false )
	{
		return false;
	}

	m_firstRow = m_keyboardLayout.layoutRows.firstRow;
	m_secondRow = m_keyboardLayout.layoutRows.secondRow;
	m_thirdRow = m_keyboardLayout.layoutRows.thirdRow;

	m_specialSigns.clear();
	m_specialSigns.insert ( m_keyboardLayout.specialSigns.begin(),
							m_keyboardLayout.specialSigns.end() );

	assert ( m_keyboardLayout.alphabet.getLetters().size() > 0 );
	assert ( m_pController );
	setTouchEnabled ( true );
	setTouchMode ( kCCTouchesAllAtOnce );

	//we want to be first
	setTouchPriority ( INT_MIN );
	setColor ( ccc3 ( 0x12, 0x12, 0x12 ) );

	const CCSize sizeScreen = getContentSize();
	float screenWidth = sizeScreen.width;
	assert ( screenWidth > 0 );

	LinearLayout* pLayoutRow = nullptr;
	int lastZOrder = 100;

	LinearLayoutParams keyParams ( LayoutParamsWrapWrap );
	keyParams.setMarginLeft ( SizePolicy ( Policy::SP, 2 ) );
	keyParams.setMarginRight ( SizePolicy ( Policy::SP, 2 ) );

	LinearLayoutParams keyParamsLeft ( keyParams );
	keyParamsLeft.setMarginLeft ( SizePolicy ( Policy::SP, 4 ) );
	keyParamsLeft.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );

	LinearLayoutParams keyParamsRight ( keyParams );
	keyParamsRight.setMarginLeft ( SizePolicy ( Policy::SP, 4 ) );
	keyParamsRight.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );

	//setting width for buttons
	CCSize buttonSize ( m_preferedButtonSize );

	float margins = keyParams.getMarginLeft().getValue()
					+ keyParams.getMarginRight().getValue();
	float tempScreenWidth = screenWidth - margins;

	float buttonWidth = tempScreenWidth / ( float ) m_firstRow;
	buttonSize.width = buttonWidth - margins;

	m_preferedButtonSize = buttonSize;

	float ratio = sizeScreen.width / sizeScreen.height;
	m_lastRowButtonSize.width *= ratio;

	const vector<Letter>& letters = m_keyboardLayout.alphabet.getLetters();

	m_pButtonDelete = getDeleteButton();
	m_pButtonSymbols = getChangeLayoutSymbolsButton();

	pLayoutRow = getRow ( 0 );
	pLayoutRow->setZOrder ( --lastZOrder );

	screenWidth -= getButtonSize ( m_pButtonSymbols, keyParamsLeft ).width;
	screenWidth -= getButtonSize ( m_pButtonDelete, keyParamsRight ).width;

	vector<Letter>::const_reverse_iterator iterator;

	pLayoutRow->addView ( m_pButtonSymbols, keyParamsLeft );

	LinearLayout* pLayoutCenter = LinearLayout::create ( Orientation::HORIZONTAL );
	{
		LinearLayoutParams params ( LayoutParamsWrapWrap );
		params.setWidth ( SizePolicy ( Policy::SP, screenWidth ) );
		pLayoutCenter->setGravity ( Gravity::CENTER_HORIZONTAL );
		pLayoutCenter->setOpacity ( 0 );
		pLayoutCenter->setReverseViews ( true );
		pLayoutRow->addView ( pLayoutCenter, params );
	}

	pLayoutRow->addView ( m_pButtonDelete, keyParamsRight );

	short int index = 0;
	short int MAX_IN_ROW = m_thirdRow;

	screenWidth -= keyParamsLeft.getMarginRight().getValue();
	screenWidth -= keyParamsRight.getMarginLeft().getValue();
	buttonWidth = screenWidth / m_thirdRow;

	CCSize size = m_preferedButtonSize;

	if ( m_preferedButtonSize.width > buttonWidth )
	{
		size.width = buttonWidth - keyParams.getMarginLeft().getValue();
	}

	for ( iterator = letters.rbegin(); iterator != letters.rend() &&
			index < MAX_IN_ROW;
			++iterator, ++index )
	{
		const Letter& letter = *iterator;

		CCControlButton* pButton = getNewButton ( letter.getSign(), size );

		pLayoutCenter->addView ( pButton, keyParams );
	}

	//Because first buttons must be letters not special buttons ;)
	m_buttons.push_back ( m_pButtonSymbols );
	m_buttons.push_back ( m_pButtonDelete );

	assert ( iterator != letters.rbegin() );

	for ( ; iterator != letters.rend(); ++iterator, ++index )
	{
		const Letter& letter = *iterator;

		CCControlButton* pButton = getNewButton ( letter.getSign() );

		if ( ( index < MAX_IN_ROW ) == false )
		{
			pLayoutRow = getRow ( 0 );
			pLayoutRow->setReverseViews ( true );
			pLayoutRow->setZOrder ( --lastZOrder );

			if ( m_rows.size() % 2 == 0 )
			{
				MAX_IN_ROW = m_secondRow;
			}
			else
			{
				MAX_IN_ROW = m_firstRow;
			}

			index = 0;
		}

		pLayoutRow->addView ( pButton, keyParams );
	}

	///Last row space,del,change letters/////
	RelativeLayout* pLastLayoutRow = getLastRow();
	pLastLayoutRow->setZOrder ( --lastZOrder );

	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule ( AlignType::PARENT_RIGHT | AlignType::PARENT_CENTER_VERTICAL );
		params.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );

		m_pButtonDone = getDoneButton();
		pLastLayoutRow->addView ( m_pButtonDone, params );

		m_pButtonDone->setTag ( DONE );
		m_buttons.push_back ( m_pButtonDone );
	}

	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule ( AlignType::LEFT, DONE );
		params.addRule ( AlignType::PARENT_CENTER_VERTICAL );
		params.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );
		CCControl* pButton = getSpaceButton();

		pLastLayoutRow->addView ( pButton, params );

		pButton->setTag ( SPACEBAR );
		m_buttons.push_back ( pButton );
	}

	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule ( AlignType::PARENT_LEFT );
		params.addRule ( AlignType::PARENT_CENTER_VERTICAL );
		params.setMarginLeft ( SizePolicy ( Policy::SP, 4 ) );
		params.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );

		m_pButtonLetters = getChangeLayoutLettersButton();
		pLastLayoutRow->addView ( m_pButtonLetters, params );

		m_pButtonLetters->setTag ( LAYOUT_LETTERS_CHANGE );
		m_buttons.push_back ( m_pButtonLetters );
	}

	{
		RelativeLayoutParams params ( LayoutParamsWrapWrap );
		params.addRule ( AlignType::PARENT_LEFT );
		params.addRule ( AlignType::PARENT_CENTER_VERTICAL );
		params.setMarginLeft (
			SizePolicy ( Policy::SP, m_lastRowButtonSize.width * 2.0F + 11 * ratio ) );
		params.setMarginRight ( SizePolicy ( Policy::SP, 4 ) );

		m_pButtonLettersOrder = getChangeLettersOrderButton();
		pLastLayoutRow->addView ( m_pButtonLettersOrder, params );

		m_pButtonLettersOrder->setTag ( LETTERS_ORDER );
		m_buttons.push_back ( m_pButtonLettersOrder );
	}
	///////////////////

	return updateStructure();
}

void KeyboardView::onEnter()
{
	LinearLayout::onEnter();
}

void KeyboardView::onExit()
{
	LinearLayout::onExit();

	//When switching layouts button is clicked (scalled down). Actions stops when finger goes up
	// sometimes buttons stop at < 1 scale
	for ( CCControl* pButton : m_buttons )
	{
		if ( pButton->getScale() != 1 )
		{
			pButton->setScale ( 1 );
		}
	}

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers ( this );
	stopActionByTag ( KeyboardController::DeleteScheduleTag );
}

void KeyboardView::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	LOG ( "keyboard" );

	if ( m_pRegisteredTouches->count() > 0 )
	{
		CCTouch* pTouch = dynamic_cast<CCTouch*> ( m_pRegisteredTouches->anyObject() );
		assert ( pTouch );

		m_pController->ccTouchEnded ( pTouch );
		m_pRegisteredTouches->removeObject ( pTouch );
	}

	CCTouch* pTouch = dynamic_cast<CCTouch*> ( pTouches->anyObject() );
	assert ( pTouch );

	m_pRegisteredTouches->addObject ( pTouch );

	if ( KoalaLib::TouchManager::detectTouchOnNode ( this, pTouch ) == false )
	{
		LOG ( "Outside!!!!! keyboard" )
		return;
	}

	LOG ( "INSIDE keyboard" );
	assert ( m_pController );
	m_pController->ccTouchBegan ( pTouch );
	return;
}

void KeyboardView::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	assert ( m_pController );
	CCTouch* pTouch = dynamic_cast<CCTouch*> ( pTouches->anyObject() );
	assert ( pTouch );

	if ( m_pRegisteredTouches->containsObject ( pTouch ) == false )
	{
		return;
	}

	m_pController->ccTouchMoved ( pTouch );
}

void KeyboardView::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	assert ( m_pController );
	CCTouch* pTouch = dynamic_cast<CCTouch*> ( pTouches->anyObject() );
	assert ( pTouch );

	if ( m_pRegisteredTouches->containsObject ( pTouch ) == false )
	{
		return;
	}

	m_pController->ccTouchEnded ( pTouch );
	m_pRegisteredTouches->removeObject ( pTouch );
}

void KeyboardView::ccTouchesCancelled ( CCSet* pTouches, CCEvent* pEvent )
{
	assert ( m_pController );
	CCTouch* pTouch = dynamic_cast<CCTouch*> ( pTouches->anyObject() );
	assert ( pTouch );

	if ( m_pRegisteredTouches->containsObject ( pTouch ) == false )
	{
		return;
	}

	m_pController->ccTouchCancelled ( pTouch );
	m_pRegisteredTouches->removeObject ( pTouch );
}

CCControlButton* KeyboardView::getSpaceButton()
{
	CCControlButton* pButtonSpace = CCControlButton::create (
										CCLabelTTF::create ( "\\_____/", m_settings.fontName.c_str(), 16 ),
										CCScale9Sprite::create ( m_settings.buttonFile.c_str() ) );
	CCSize spaceSize ( m_lastRowButtonSize );
	spaceSize.width *= 10.9f;
	pButtonSpace->setPreferredSize ( spaceSize );
	pButtonSpace->setUserObject ( CCString::create ( " " ) );
	pButtonSpace->setZoomOnTouchDown ( false );
	pButtonSpace->setTouchEnabled ( false );
	return pButtonSpace;
}

CCControlButton* KeyboardView::getDeleteButton()
{

	CCScale9Sprite* pButtonBackground = CCScale9Sprite::create (
											m_settings.buttonFile.c_str() );
	pButtonBackground->setColor ( ccGRAY );
	CCSprite* pSprite = CCSprite::create ( m_settings.deleteName.c_str() );
	pButtonBackground->addChild ( pSprite );

	CCControlButton* pButtonDelete = CCControlButton::create ( pButtonBackground );

	CCSize size ( m_preferedButtonSize );
	size.width *= 1.6F;
	pButtonDelete->setPreferredSize ( size );
	pButtonDelete->setZoomOnTouchDown ( false );

	Positioner::centerInParent ( pSprite );

	pButtonDelete->setUserObject ( CCString::create ( "\b" ) );
	pButtonDelete->setTouchEnabled ( false );
	return pButtonDelete;
}

CCControlButton* KeyboardView::getDoneButton()
{
	CCControlButton* pButtonSpecial = CCControlButton::create (
										  CCLabelTTF::create ( "DONE", m_settings.fontName.c_str(), 16 ),
										  CCScale9Sprite::create ( m_settings.buttonFile.c_str() ) );
	pButtonSpecial->setColor ( ccGRAY );
	CCSize size ( m_lastRowButtonSize );
	size.width *= 3.F;
	pButtonSpecial->setPreferredSize ( size );
	pButtonSpecial->setZoomOnTouchDown ( false );
	pButtonSpecial->setUserObject ( CCString::create ( "" ) );
	pButtonSpecial->setTouchEnabled ( false );
	return pButtonSpecial;
}

CCControlButton* KeyboardView::getChangeLayoutSymbolsButton()
{
	CCControlButton* pButtonLayoutSymbols = CCControlButton::create (
			CCLabelTTF::create ( "?123", m_settings.fontName.c_str(), 16 ),
			CCScale9Sprite::create ( m_settings.buttonFile.c_str() ) );
	pButtonLayoutSymbols->setColor ( ccGRAY );
	CCSize size ( m_preferedButtonSize );
	size.width *= 1.3F;
	pButtonLayoutSymbols->setPreferredSize ( size );
	pButtonLayoutSymbols->setZoomOnTouchDown ( false );
	pButtonLayoutSymbols->setUserObject ( CCString::create ( "" ) );
	pButtonLayoutSymbols->setTouchEnabled ( false );
	return pButtonLayoutSymbols;
}

CCControlButton* KeyboardView::getChangeLayoutLettersButton()
{
	CCControlButton* pButtonLayoutLetters = CCControlButton::create (
			CCLabelTTF::create ( "@", m_settings.fontName.c_str(), 16 ),
			CCScale9Sprite::create ( m_settings.buttonFile.c_str() ) );
	pButtonLayoutLetters->setColor ( ccGRAY );
	CCSize size ( m_lastRowButtonSize );
	size.width *= 2.F;
	pButtonLayoutLetters->setPreferredSize ( size );
	pButtonLayoutLetters->setZoomOnTouchDown ( false );
	pButtonLayoutLetters->setUserObject ( CCString::create ( "" ) );
	pButtonLayoutLetters->setTouchEnabled ( false );
	return pButtonLayoutLetters;
}
CCControlButton* KeyboardView::getChangeLettersOrderButton()
{
	CCScale9Sprite* pBackground = CCScale9Sprite::create (
									  m_settings.buttonFile.c_str() );
	pBackground->setColor ( ccGRAY );
	KKScaleSprite* pSprite = KKScaleSprite::create (
								 m_settings.settingsName.c_str(),
								 Scale::Type::FIT_X_Y );
	pBackground->addChild ( pSprite );

	CCControlButton* pButtonLayoutChange = CCControlButton::create ( pBackground );

	CCSize size ( m_lastRowButtonSize );
	size.width *= 2.F;
	pButtonLayoutChange->setPreferredSize ( size );
	pButtonLayoutChange->setZoomOnTouchDown ( false );
	pButtonLayoutChange->setUserObject ( CCString::create ( "\t" ) );
	pButtonLayoutChange->setTouchEnabled ( false );

	size.width *= 0.65F;
	size.height *= 0.65F;
	pSprite->setContentSize ( size );

	Positioner::centerInParent ( pSprite );
	return pButtonLayoutChange;
}

bool KeyboardView::onLayout()
{
	applySizeRuleForLayout ( KK_WIDTH, this,
							 LinearLayoutParams ( SizePolicy ( Policy::PERCENT_X, 1 ),
									 SizePolicyWrapContent ) );
	applySizeRuleForLayout ( KK_HEIGHT, this,
							 LinearLayoutParams ( SizePolicy ( Policy::PERCENT_X, 1 ),
									 SizePolicyWrapContent ) );

	if ( LinearLayout::onLayout() == false )
	{
		return false;
	}

	return true;
}

LinearLayout* KeyboardView::getRow ( unsigned position )
{
	LinearLayoutParams rowParams ( LayoutParamsMatchWrap );
	rowParams.setMarginBottom ( SizePolicy ( Policy::SP, 5 ) );
	rowParams.setMarginTop ( SizePolicy ( Policy::SP, 8 ) );

	LinearLayout* pLayoutRow = LinearLayout::create ( Orientation::HORIZONTAL );
	pLayoutRow->setGravity ( Gravity::CENTER_HORIZONTAL |
							 Gravity::CENTER_VERTICAL );
	pLayoutRow->setOpacity ( 0 );

	insertView ( position, pLayoutRow, rowParams );
	m_rows.insert ( m_rows.begin() + position, pLayoutRow );
	return pLayoutRow;
}

RelativeLayout* KeyboardView::getLastRow()
{
	LinearLayoutParams rowParams ( LayoutParamsMatchWrap );
	rowParams.setMarginBottom ( SizePolicy ( Policy::SP, 5 ) );
	rowParams.setMarginTop ( SizePolicy ( Policy::SP, 8 ) );

	RelativeLayout* pLayoutRow = RelativeLayout::create();
	pLayoutRow->setOpacity ( 0 );

	insertView ( m_rows.size(), pLayoutRow, rowParams );
	m_rows.insert ( m_rows.begin() + m_rows.size(), pLayoutRow );
	return pLayoutRow;
}

CCControlButton* KeyboardView::getNewButton ( const string& sign, CCSize size )
{
	if ( size.equals ( CCSize ( 0, 0 ) ) )
	{
		size = m_preferedButtonSize;
	}

	CCControlButton* pButton = CCControlButton::create (
								   CCLabelTTF::create ( sign.c_str(), m_settings.fontName.c_str(), 24 ),
								   CCScale9Sprite::create ( m_settings.buttonFile.c_str() ) );
	pButton->setUserObject ( CCString::create ( sign ) );
	pButton->setPreferredSize ( size );
	pButton->setZoomOnTouchDown ( false );
	pButton->setTouchEnabled ( false );
	m_buttons.push_back ( pButton );
	return pButton;
}

CCSize KeyboardView::getButtonSize ( CCNode* pNode,
									 const LinearLayoutParams& params )
{
	CCSize buttonSize = pNode->getContentSize();
	buttonSize.width += ( params.getMarginLeft().getValue()
						  + params.getMarginRight().getValue() );
	return buttonSize;
}

void KeyboardView::setHintForButton ( CCControl* pButton,
									  const string& character )
{
	assert ( pButton );

	if ( pButton->getChildByTag ( ID::HINT ) != nullptr )
	{
		return;
	}

	CCScale9Sprite* pHint = CCScale9Sprite::create ( m_settings.hintName.c_str() );
	pHint->setColor ( MAKE_CCC3 ( 0x169ce1 ) );
	pHint->setContentSize ( pButton->getContentSize() * 1.2f );
	CCLabelTTF* pChar = CCLabelTTF::create ( character.c_str(),
						m_settings.fontName.c_str(), 30 );
	pChar->setColor ( ccBLACK );
	pHint->addChild ( pChar );
	Positioner::centerInParent ( pChar );

	pHint->setTag ( ID::HINT );
	pButton->addChild ( pHint );
	Positioner::setCornerToPosition ( pHint,
									  Positioner::getLeftBottomCorner ( pHint ).origin,
									  Positioner::getLeftTopCorner ( pButton ).origin );
	Positioner::centerInParentHorizontally ( pHint );
	CCPoint position = pHint->getPosition();
	position.y += 16;
	pHint->setPosition ( position );
}

void KeyboardView::concatenateSpecialSigns (
	const map<const string, const string>& specialSigns )
{
	//TODO extract to controller
	map<string, string>::iterator iterator;
	map<string, string> temporaryMap;

	for ( const pair<const string, const string>& pair : specialSigns )
	{
		iterator = m_specialSigns.find ( pair.first );

		if ( iterator == m_specialSigns.end() )
		{
			temporaryMap.insert ( pair );
			continue;
		}

		ostringstream stream;
		stream << pair.second << iterator->second;

		Alphabet alphabet ( stream.str() );
		ostringstream specialSignsStream;

		for ( const Letter& letter : alphabet.getLetters() )
		{
			specialSignsStream << letter.getSign();
		}

		temporaryMap.insert ( make_pair ( pair.first, specialSignsStream.str() ) );
	}

	m_specialSigns.clear();
	m_specialSigns.insert ( temporaryMap.begin(), temporaryMap.end() );

	m_keyboardLayout.specialSigns.clear();
	m_keyboardLayout.specialSigns.insert ( m_specialSigns.begin(),
										   m_specialSigns.end() );
}

} /* namespace keyboard */


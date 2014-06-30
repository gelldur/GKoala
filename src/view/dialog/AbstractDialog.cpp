/*
 * AbstractDialog.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: kolo
 */

#include "AbstractDialog.h"

namespace KoalaGui
{

const SizePolicy AbstractDialog::HEADER_HEIGHT = SizePolicy( Policy::SP, 40 );

AbstractDialog::AbstractDialog( int tag ) :
	LinearLayout( tag ),
	m_pDialogBackground( nullptr ),
	m_dismissedOnTouch( true ),
	m_dismissedOnBack( true ),
	m_wasSceneKeypadEnabled( false ),
	m_isShown( false ),
	m_pDismissCallback( nullptr )
{
}

AbstractDialog::~AbstractDialog()
{
	m_pDialogBackground->removeFromParentAndCleanup( true );
	assert( m_pDialogBackground->retainCount() > 0 );
	CC_SAFE_RELEASE_NULL( m_pDialogBackground );

	m_isShown = false;
	CC_SAFE_RELEASE_NULL( m_pDismissCallback );
}

bool AbstractDialog::init()
{
	if( LinearLayout::init( Orientation::VERTICAL ) == false )
	{
		return false;
	}

	m_pDialogBackground = CCLayerColor::create( ccc4( 0x44, 0x44, 0x44, 0 ) );
	m_pDialogBackground->retain();

	setOpacity( 0 );

	setContentSize(
		CCSize( CCDirector::sharedDirector()->getWinSize().width * 0.9F,
				HEADER_HEIGHT.getValue() ) );

	//We want to be first
	setTouchEnabled( true );
	setTouchPriority( INT_MIN );
	setTouchMode( kCCTouchesOneByOne );

	return true;
}

void AbstractDialog::show( KKScene* pScene /*= getCurrentScene()*/ )
{
	if( m_isShown )
	{
		assert( getParent() != nullptr );
		return;
	}

	assert( getParent() == nullptr );

	setScale( 1 );

	m_isShown = true;
	assert( pScene );

	pScene->addChild( m_pDialogBackground, INT_MAX );
	pScene->addChild( this, m_pDialogBackground->getZOrder() );

	if( pScene->isKeypadEnabled() )
	{
		m_wasSceneKeypadEnabled = true;
		pScene->setKeypadEnabled( false );
	}

	updateStructure();

	showAnimation( pScene );
}

void AbstractDialog::dismiss( bool showAnimation )
{
	if( m_isShown == false )
	{
		return;
	}

	m_isShown = false;

	retain();

	if( m_pDismissCallback != nullptr )
	{
		assert( m_pDismissCallback->methodVoid );
		m_pDismissCallback->callMethodVoid();
	}

	if( getParent() == nullptr )
	{
		release();
		return;
	}

	release();

	CCNode* pNode = getParent();
	assert( pNode );

	KKScene* pScene = dynamic_cast<KKScene*>( pNode );
	assert( pScene );

	if( pScene == nullptr )
	{
		assert( false );
		return;
	}

	if( m_wasSceneKeypadEnabled )
	{
		m_wasSceneKeypadEnabled = false;
		pScene->setKeypadEnabled( true );
	}


	if( showAnimation )
	{
		dismissAnimation( pScene );
	}
	else
	{
		stopAllActions();
		m_pDialogBackground->stopAllActions();
		pScene->removeChild( m_pDialogBackground );
		pScene->removeChild( this );
	}
}

void AbstractDialog::dismissAnimation( KKScene* pShowingScene )
{
#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WP8 )
	runAction( CCEaseSineOut::create( CCSequence::create( CCScaleTo::create(
										  0.2F, 0 ),
									  CCRemoveSelf::create( true ), static_cast<void*>( nullptr ) ) ) );

	getDialogBackground()->runAction( CCSequence::create( CCFadeTo::create(
										  0.19F,
										  0 ),
									  CCRemoveSelf::create( true ), static_cast<void*>( nullptr ) ) );
#else
	getDialogBackground()->removeFromParent();
	this->removeFromParent();
#endif
}

bool AbstractDialog::ccTouchBegan( CCTouch* pTouch, CCEvent* pEvent )
{
	if( isShown() )
	{
		return true;
	}

	return false;
}

void AbstractDialog::ccTouchEnded( CCTouch* pTouch, CCEvent* pEvent )
{
	if( m_dismissedOnTouch )
	{
		dismiss();
	}
}

void AbstractDialog::setDismissOnTouch( const bool& isDismissable )
{
	m_dismissedOnTouch = isDismissable;
}

bool AbstractDialog::getDismissOnTouch() const
{
	return m_dismissedOnTouch;
}

bool AbstractDialog::isShown()
{
	return m_isShown;
}

bool AbstractDialog::getDismissOnBack() const
{
	return m_dismissedOnBack;
}

void AbstractDialog::setDismissOnBack( const bool& isDismissable )
{
	m_dismissedOnBack = isDismissable;
}

bool AbstractDialog::onLayout()
{
	applySizeRuleForLayout( KK_WIDTH, this,
							LayoutParams( SizePolicyWrapContent, SizePolicyWrapContent ) );
	applySizeRuleForLayout( KK_HEIGHT, this,
							LayoutParams( SizePolicyWrapContent, SizePolicyWrapContent ) );

	if( LinearLayout::onLayout() == false )
	{
		return false;
	}

	return true;
}

void AbstractDialog::onEnter()
{
	LinearLayout::onEnter();
	KKScene* pScene = dynamic_cast<KKScene*>( getParent() );

	if( pScene == nullptr )
	{
		CCAssert( false, "Dialog should have KKScene as a parent" );
		return;
	}

	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate( this );
}

void AbstractDialog::onExit()
{
	LinearLayout::onExit();
	CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate( this );
}

void AbstractDialog::keyBackClicked()
{
	if( m_isShown == false || m_dismissedOnBack == false )
	{
		return;
	}

	CCNode* pNode = getParent();

	if( pNode == nullptr )
	{
		return;
	}

	CCArray* pChildren = pNode->getChildren();
	CCObject* pObject = nullptr;

	for( int i = pChildren->count() - 1; i >= 0; --i )
	{
		pObject = pChildren->objectAtIndex( i );
		AbstractDialog* pDialog = dynamic_cast<AbstractDialog*>( pObject );

		if( pDialog != nullptr )
		{
			if( pDialog == this )
			{
				runAction( CCCallFunc::create( this,
											   callfunc_selector( AbstractDialog::defaultDismiss ) ) );
			}

			return;
		}
	}
}

void AbstractDialog::setOnDismissCallback( Utils::CallbackOld*
		pDismissCallback )
{
	CC_SAFE_RELEASE_NULL( m_pDismissCallback );
	m_pDismissCallback = pDismissCallback;
	CC_SAFE_RETAIN( pDismissCallback );
}

void AbstractDialog::onDismissClickCallback( CCObject* pObject,
		CCControlEvent event )
{
	dismiss();
}

void AbstractDialog::showAnimation( KKScene* pShowingScene )
{
	Positioner::setCornerToPosition( this, Positioner::getCenterPoint( this ).origin,
									 Positioner::getCenterPoint( pShowingScene ).origin );

	setScale( 0 );
	runAction( CCEaseSineIn::create( CCScaleTo::create( 0.3F, 1 ) ) );

	getDialogBackground()->runAction( CCFadeTo::create( 0.3F, 160 ) );
}

void AbstractDialog::onDismiss( CCNode* pNode )
{
	dismiss();
}

}

/* namespace KoalaGui */


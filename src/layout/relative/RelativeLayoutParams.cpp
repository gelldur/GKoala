/*
 * RelativeLayoutParams.cpp
 *
 *  Created on: May 7, 2013
 *      Author: Dawid Drozd
 */

#include "view/layout/relative/RelativeLayoutParams.h"

namespace KoalaGui
{

RelativeLayoutParams::RelativeLayoutParams ( const SizePolicy& width,
		const SizePolicy& height ) :
	LayoutParams ( width, height )
{
	m_params.push_back (
		RelativeLayoutParams::Align (
			AlignType::PARENT_TOP | AlignType::PARENT_LEFT, kCCNodeTagInvalid ) );
}

KoalaGui::RelativeLayoutParams::RelativeLayoutParams (
	const LayoutParams& params ) :
	LayoutParams ( params )
{
}

RelativeLayoutParams::~RelativeLayoutParams()
{
	m_params.clear();
}

void RelativeLayoutParams::addRule ( const unsigned short alignType,
									 const int tag /*= kCCNodeTagInvalid*/ )
{
	CCAssert ( alignType & 16383, "Align must be in range 1 - 16383!" );
	vector<RelativeLayoutParams::Align>::iterator it = getIteratorByTag ( tag );

	if ( it != m_params.end() )
	{
		it->addAlignType ( alignType );
	}
	else
	{
		m_params.push_back ( RelativeLayoutParams::Align ( alignType, tag ) );
	}
}

const vector<RelativeLayoutParams::Align>& RelativeLayoutParams::getRules()
const
{
	return m_params;
}

vector<RelativeLayoutParams::Align>::iterator
RelativeLayoutParams::getIteratorByTag (
	const int tag )
{
	return find_if ( m_params.begin(), m_params.end(),
					 [&] ( RelativeLayoutParams::Align param )->bool
	{
		return param.getTag() == tag;
	} );
}

RelativeLayoutParams::Align::Align ( const unsigned short align,
									 const int tag )
{
	m_align = align;
	m_tag = tag;
}

const unsigned short RelativeLayoutParams::Align::getAlignType() const
{
	return m_align;
}

const int RelativeLayoutParams::Align::getTag() const
{
	return m_tag;
}

void RelativeLayoutParams::Align::addAlignType ( const unsigned short align )
{
	assert ( LEFT == 64 );
	assert ( CENTER_HORIZONTAL == 2048 );

	//We need lower bits set to 1 something like 11110000
	const unsigned short MASK_X_POSITION = ( 4095 >> 6 ) << 6;
	//Bit mask for Y position something like 00001111
	const unsigned short MASK_Y_POSITION = ( LEFT - 1 );

	unsigned short xPosition = MASK_X_POSITION & align;
	unsigned short yPosition = MASK_Y_POSITION & align;

	assert ( yPosition <= MASK_Y_POSITION );
	assert ( xPosition <= MASK_X_POSITION );

	if ( xPosition != 0 && ( m_align & MASK_X_POSITION ) != 0 )
	{
		//We remove all aligns for X position
		m_align = m_align & ( ~MASK_X_POSITION );
	}

	if ( yPosition != 0 && ( m_align & MASK_Y_POSITION ) != 0 )
	{
		//We remove all aligns for Y position
		m_align = m_align & ( ~MASK_Y_POSITION );
	}

	m_align |= align;
}

} /* namespace KoalaGui */


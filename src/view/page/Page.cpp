/*
 * Page.cpp
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#include "view/page/Page.h"

namespace KoalaGui
{

Page::Page( const std::string& title ) :
	m_pContent( nullptr )
{
	m_title = title;
}

Page::~Page()
{
	m_pContent = nullptr;
}

} /* namespace KoalaGui */


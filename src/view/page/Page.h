/*
 * Page.h
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#ifndef PAGE_H_
#define PAGE_H_

#include "view/layout/linear/LinearLayout.h"
#include <string>

namespace KoalaGui
{

class Page: public CCObject
{
private:
	std::string m_title;
	CCNode* m_pContent;

protected:
	Page( const std::string& title );

public:

	virtual ~Page();

	virtual void inflateContent() = 0;

	CCNode* getContent() const
	{
		return m_pContent;
	}

	virtual void setContentSize( const CCSize& size ) = 0;

	void setContent( CCNode* pContent )
	{
		m_pContent = pContent;
	}

	const std::string getTitle() const
	{
		return m_title;
	}

	void setTitle( const std::string title )
	{
		m_title = title;
	}

	bool isInflated() const
	{
		return m_pContent != nullptr;
	}

	virtual void updatePage() = 0;
};

} /* namespace KoalaGui */
#endif /* PAGE_H_ */

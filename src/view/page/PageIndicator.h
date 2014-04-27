/*
 * PageIndicator.h
 *
 *  Created on: May 28, 2013
 *      Author: Dawid Drozd
 */

#ifndef CCPAGEINDICATOR_H_
#define CCPAGEINDICATOR_H_

#include "view/page/PageViewAdapter.h"
#include "view/page/PageView.h"

namespace KoalaGui
{

class PageIndicatorController
{
public:
	PageIndicatorController();

	virtual ~PageIndicatorController();

	void setPageView ( PageView* pPageView );

	bool nextPage();
	bool previousPage();

	virtual void setCurrentPage ( unsigned index );

protected:
	PageView* getPageView() const
	{
		assert ( m_pPageView );
		return m_pPageView;
	}

private:
	PageView* m_pPageView;
};

} /* namespace KoalaGui */
#endif /* CCPAGEINDICATOR_H_ */

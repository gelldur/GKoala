/*
 * ListPage.h
 *
 *  Created on: Jun 1, 2013
 *      Author: Dawid Drozd
 */

#ifndef LISTPAGE_H_
#define LISTPAGE_H_

#include "view/page/Page.h"
#include "view/list/ListView.h"
using namespace KoalaGui;

class ListPage: public Page
{
public:
	virtual ~ListPage();
	virtual void inflateContent();
	virtual void setContentSize( const CCSize& size );

protected:
	ListPage( const string& title );

	/**
	 * @copydoc CCListView::setAdapter(CCListViewAdapter*)
	 * @param pAdapter
	 */
	void setAdapter( ListAdapter* pAdapter );
	ListView* getListView() const;

	void reloadAllData();

private:
	ListView* m_pListView;
};

#endif /* LISTPAGE_H_ */

/*
 * KKScene.h
 *
 *  Created on: May 10, 2013
 *      Author: Dawid Drozd
 */

#ifndef KKSCENE_H_
#define KKSCENE_H_

#include "cocos2d.h"
#include "view/layout/linear/LinearLayout.h"
USING_NS_CC;

namespace KoalaGui
{

class KKScene: public CCScene, public CCKeypadDelegate
{
public:

	bool isKeypadEnabled()
	{
		return m_isKeypadEnabled;
	}

	void setKeypadEnabled ( bool isEnabled );

	virtual ~KKScene();

	virtual bool init();

	/**
	 * Called when user want to exit from this KKScene
	 * @param count how many times to call
	 */
	virtual void onBack();

	virtual void keyBackClicked()
	{
		onBack();
	}

	virtual void onEnter();
	virtual void onExit();

protected:
	KKScene();

private:
	bool m_isKeypadEnabled;
};

} /* namespace KoalaGui */
#endif /* KKSCENE_H_ */

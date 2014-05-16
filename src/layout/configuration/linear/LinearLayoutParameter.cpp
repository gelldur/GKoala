/*
 * LinearLayoutParameter.cpp
 *
 *  Created on: Dec 26, 2013
 *      Author: dawid
 */

#include "layout/configuration/linear/LinearLayoutParameter.h"

namespace GKoala
{

LinearLayoutParameter::LinearLayoutParameter()
{
}

LinearLayoutParameter::~LinearLayoutParameter()
{
}


LinearLayoutParameter* LinearLayoutParameter::createWrapWrap()
{
	LinearLayoutParameter* pParameter = new LinearLayoutParameter();
	pParameter->setWidth ( SP::wrapContent() );
	pParameter->setHeight ( SP::wrapContent() );
	return pParameter;
}

LinearLayoutParameter* LinearLayoutParameter::createWrapMatch()
{
	LinearLayoutParameter* pParameter = new LinearLayoutParameter();
	pParameter->setWidth ( SP::wrapContent() );
	pParameter->setHeight ( SP::matchParent() );
	return pParameter;
}

LinearLayoutParameter* LinearLayoutParameter::createMatchMatch()
{
	LinearLayoutParameter* pParameter = new LinearLayoutParameter();
	pParameter->setWidth ( SP::matchParent() );
	pParameter->setHeight ( SP::matchParent() );
	return pParameter;
}

LinearLayoutParameter* LinearLayoutParameter::createMatchWrap()
{
	LinearLayoutParameter* pParameter = new LinearLayoutParameter();
	pParameter->setWidth ( SP::matchParent() );
	pParameter->setHeight ( SP::wrapContent() );
	return pParameter;
}

LinearLayoutParameter* LinearLayoutParameter::create ( const SizePolicy& width,
		const SizePolicy& height )
{
	LinearLayoutParameter* pParameter = new LinearLayoutParameter();
	pParameter->setWidth ( width );
	pParameter->setHeight ( height );
	return pParameter;
}

void LinearLayoutParameter::setGravity ( int gravity )
{
	m_gravity = gravity;
}


} /* namespace GKoala */

/*
 * ParserInterface.h
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#pragma once

#include "cocos2d.h"

#include <functional>
#include <regex>
#include <type_traits>

#include "Config.h"

namespace GKoala
{

using BaseClass = cocos2d::CCObject;
using InitRule = std::function<BaseClass*()>;

struct RegEx
{
	RegEx(const std::string& regex):
		stringRepresentation(regex)
		,regex(regex)
	{
	}

	RegEx(const char* pRegex):
		stringRepresentation(pRegex)
		,regex(stringRepresentation)
	{
	}

	const std::string stringRepresentation;
	const std::regex regex;

     bool operator <(const RegEx& other) const
     {
        return stringRepresentation < other.stringRepresentation;
     }
};

class RuleBase
{
public:
	virtual ~RuleBase(){}
	enum Type
	{
		UNKNOWN = 0,INT = 1,FLOAT = 2,STRING = 4,VECTOR = 8,OBJECT = 16
	};

	int type = UNKNOWN;

	virtual void call(BaseClass* pBase,BaseClass* pContext,int& value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,float& value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::string& value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,BaseClass* pValue )const = 0;

	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<int>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<float>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<std::string>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<BaseClass*>& pValues )const = 0;
};

template<typename>
	struct is_vector;

	template<typename T>
	struct is_vector<std::vector<T>>
	{
		static const bool value = true;
		using type = T;
	};

	template<typename T>
	struct is_vector
	{
		static const bool value = false;
		using type = T;
	};

	template<typename T>
	using MyType = typename is_vector<T>::type;

template<typename Base,typename Value>
class Rule : public RuleBase
{
public:
	Rule(const std::function< void(Base,BaseClass* ,Value) >& ruleFunction):
		ruleFunction(ruleFunction)
	{
	}

	virtual ~Rule(){}

	const std::function< void(Base,BaseClass* ,Value) > ruleFunction;

private:

public:

	template<typename Base2,typename Value2>
	struct Normal
	{
		static inline void execute(Base2* pBase,BaseClass* pContext,Value2 value,const std::function< void(Base2,BaseClass* ,Value2) >& ruleFunction)
		{
			ruleFunction(pBase,pContext,value);
		}
	};

	template<typename Base2,typename Value2>
	struct Dummy
	{
		static inline void execute(Base2* pBase,BaseClass* pContext,Value2 value,const std::function< void(Base2,BaseClass* ,Value2) >& ruleFunction){}
	};

	virtual void call(BaseClass* pBase,BaseClass* pContext,int& value )const override
	{
		typedef typename std::conditional< std::is_same<Value,int&>::value, Normal<Base,Value> ,Dummy<Base,Value>  >::type Executor;

		Base* pCastedBase = dynamic_cast<Base*>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,value,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,float& value )const override
	{
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::string& value )const override
	{
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,BaseClass* pValue )const override
	{

	}

	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<int>& values )const override
	{

	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<float>& values )const override
	{

	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<std::string>& values )const override
	{

	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<BaseClass*>& pValues )const override
	{

	}
};

template<typename Engine>
class ParserInterface
{
public:
	ParserInterface(){}
	~ParserInterface(){}

	inline void addInitRule(const RegEx& objectName,const InitRule& initRule)
	{
		m_engine.addInitRule(objectName,initRule);
	}

	template<typename Base,typename Value>
	inline void addRule(const RegEx& objectName,const RegEx& propertyName,const std::function< void(Base,BaseClass* ,Value) >& ruleFunction)
	{
		using BaseNoPointer = typename std::remove_pointer<Base>::type;
		static_assert(std::is_base_of<BaseClass,BaseNoPointer>::value,"Base must inherit from BaseClass");

		static_assert
		(
				std::is_same<Value,int&>::value
				|| std::is_same<Value,float&>::value
				|| std::is_same<Value,std::string&>::value
				|| std::is_base_of<Value,BaseClass*>::value
				|| std::is_same<Value,std::vector<int>& >::value
				|| std::is_same<Value,std::vector<float>& >::value
				|| std::is_same<Value,std::vector<std::string>& >::value
				|| (std::is_reference<Value>::value
						&& is_vector<Value>::value
						&& std::is_base_of< MyType<Value>,BaseClass>::value)
		,"Value can be only one of mentioned above. Look at call method in Rule class");

		m_engine.addRule(objectName,propertyName,new Rule<Base,Value>(ruleFunction));
	}


	inline cocos2d::CCNode* parseLayout(const std::string& layoutFile)
	{
		return m_engine.parseLayout(layoutFile);
	}

private:
	Engine m_engine;
};

}

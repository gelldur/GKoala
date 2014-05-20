#pragma once

#include "Config.h"
#include "parser/engine/TypesRecognizers.h"

namespace GKoala
{

/**
 * RuleBase - abstract class for common interface
 */
class RuleBase
{
public:
	virtual ~RuleBase(){}

	int type = Type::UNKNOWN;

	virtual void call(BaseClass* pBase,BaseClass* pContext,int value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,float value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::string& value )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,BaseClass* pValue )const = 0;

	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<int>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<float>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<std::string>& values )const = 0;
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<BaseClass*>& values )const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Implementation of abstract RuleBase
 */
template<typename Base,typename Value>
class Rule : public RuleBase
{
public:
	Rule(const std::function< void(Base,BaseClass* ,Value) >& ruleFunction):
		ruleFunction(ruleFunction)
	{
		using BaseNoPointer = typename std::remove_pointer<Base>::type;
		using ValueNoPointer = typename std::remove_pointer<Value>::type;
		using ValueNoReference = typename std::remove_reference<ValueNoPointer>::type;
		static_assert(!std::is_same<Value,const char*>::value
					|| !std::is_same<Value,char*>::value
					,"Sorry we don't support CStrings use std::string");
		static_assert(std::is_base_of<BaseClass,BaseNoPointer>::value,"Base must inherit from BaseClass");
		static_assert(
				!std::is_pointer<Value>::value
				|| std::is_base_of<ValueNoPointer,BaseNoPointer>::value
				,"Value must inherit from CCObject");
		static_assert(
				!std::is_reference<Value>::value
				|| is_vector< ValueNoReference >::value
				|| std::is_same<Value,std::string&>::value
				,"You can only use references for vector or std::string");

		static_assert
		(
				std::is_same<Value,int>::value
				|| std::is_same<Value,float>::value
				|| std::is_same<Value,std::string&>::value
				|| std::is_base_of<ValueNoPointer,BaseNoPointer>::value
				|| std::is_same<Value,std::vector<int>& >::value
				|| std::is_same<Value,std::vector<float>& >::value
				|| std::is_same<Value,std::vector<std::string>& >::value
				|| (is_vector< ValueNoReference >::value
						&& std::is_base_of< typename std::remove_pointer< VectorType< ValueNoReference > >::type,BaseNoPointer>::value
						&& std::is_reference<Value>::value)
		,"Value can be only one of mentioned above. Look at call method in Rule class."
				" Example it must be int,float,std::string&,CCObject*,"
				"vector<int>&,vector<float>&,vector<std::string>&,vector<CCObject*>&");

		setType<ValueNoReference>(type);
		GLOG("I create with type %d",type);
		GKoala_assert(type != UNKNOWN,"I don't know how to create it!");
	}

	virtual ~Rule(){}

	const std::function< void(Base,BaseClass* ,Value) > ruleFunction;

private:

	template<typename BaseForNormal,typename ValueForNormal>
	struct Normal
	{
		static inline void execute(BaseForNormal pBase,BaseClass* pContext,ValueForNormal value,
				const std::function< void(BaseForNormal,BaseClass* ,Value) >& ruleFunction)
		{
			ruleFunction(pBase,pContext,value);
		}
	};
	template<typename BaseForNormal>
	struct Normal<BaseForNormal,BaseClass*>
	{
		static inline void execute(BaseForNormal pBase,BaseClass* pContext,BaseClass* pValue,
				const std::function< void(BaseForNormal,BaseClass* ,Value) >& ruleFunction)
		{

			Value pCastedValue = dynamic_cast<Value>(pValue);
			GKoala_assert(pCastedValue != nullptr,"Something isn't working");

			ruleFunction(pBase,pContext,pCastedValue);
		}
	};
	template<typename BaseForNormal>
	struct Normal<BaseForNormal,std::vector<BaseClass*>&>
	{
		static inline void execute(BaseForNormal pBase,BaseClass* pContext,std::vector<BaseClass*>& values,
				const std::function< void(BaseForNormal,BaseClass* ,Value) >& ruleFunction)
		{
			using ValueNoReference = typename std::remove_pointer<Value>::type;
			using VectorElementType = VectorType<ValueNoReference>;

			std::vector< VectorElementType > castedValues;
			castedValues.reserve(values.size());

			for(auto&& pElement : values)
			{
				VectorElementType pCastedElement = dynamic_cast<VectorElementType>(pElement);
				GKoala_assert(pCastedElement != nullptr,"Something isn't working");

				castedValues.emplace_back(pCastedElement);
			}

			ruleFunction(pBase,pContext,castedValues);
		}
	};

	template<typename BaseForDummy,typename ValueForDummy>
	struct Dummy
	{
		static inline void execute(BaseForDummy pBase,BaseClass* pContext,ValueForDummy value,
				const std::function< void(BaseForDummy,BaseClass* ,Value) >& ruleFunction)
		{
			//Empty do notihing
			GKoala_assert(false,"This method shouldn't be called");
		}
	};

public:

	virtual void call(BaseClass* pBase,BaseClass* pContext,int value )const override
	{
		using Executor = typename std::conditional< std::is_same<Value,int>::value,
				Normal<Base,Value> ,
				Dummy<Base,int>  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,value,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,float value )const override
	{
		using Executor = typename std::conditional< std::is_same<Value,float>::value,
				Normal<Base,Value> ,
				Dummy<Base,float>  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,value,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::string& value )const override
	{
		using Executor = typename std::conditional< std::is_same<Value,std::string&>::value,
				Normal<Base,Value> ,
				Dummy<Base,std::string&>  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,value,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,BaseClass* pValue )const override
	{
		using ValueNoPointer = typename std::remove_pointer<Value>::type;
		using Executor = typename std::conditional< std::is_base_of<ValueNoPointer,BaseClass>::value,
				Normal<Base,BaseClass*> ,
				Dummy<Base,BaseClass*>  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,pValue,ruleFunction);
	}

	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<int>& values )const override
	{
		using Executor = typename std::conditional< std::is_same<Value,std::vector<int>& >::value,
				Normal<Base,Value> ,
				Dummy<Base,std::vector<int>& >  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,values,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<float>& values )const override
	{
		using Executor = typename std::conditional< std::is_same<Value,std::vector<float>& >::value,
				Normal<Base,Value> ,
				Dummy<Base,std::vector<float>& >  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,values,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<std::string>& values )const override
	{
		using Executor = typename std::conditional<
				std::is_same<Value,std::vector<std::string>& >::value,
				Normal<Base,Value> ,
				Dummy<Base,std::vector<std::string>& >  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,values,ruleFunction);
	}
	virtual void call(BaseClass* pBase,BaseClass* pContext,std::vector<BaseClass*>& values )const override
	{
		using ValueNoReference = typename std::remove_pointer<Value>::type;
		using Executor = typename std::conditional<
				std::is_base_of< VectorType<ValueNoReference> ,BaseClass>::value,
				Normal<Base,std::vector<BaseClass*>& > ,
				Dummy<Base,std::vector<BaseClass*>& >  >::type;

		Base pCastedBase = dynamic_cast<Base>(pBase);
		GKoala_assert(pCastedBase != nullptr,"Something isn't working");

		Executor::execute(pCastedBase,pContext,values,ruleFunction);
	}
};

}




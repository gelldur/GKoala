/*
 * ParsingEngine.cpp
 *
 *  Created on: May 16, 2014
 *      Author: dordoz
 */

#include "parser/engine/json/ParsingEngine.h"
#include "Config.h"

#include <memory>
#include <algorithm>

USING_NS_CC;
using namespace std;

static const std::string FIELD_OBJECT = "object";

namespace GKoala
{

ParsingEngine::ParsingEngine()
{

}

ParsingEngine::~ParsingEngine()
{
	//TODO cleanup!
}

void ParsingEngine::addInitRule(const RegEx& objectName,const InitRule& initRule)
{
	GKoala_assert(m_initRules.find(objectName) == m_initRules.end(),"You already add init rule for this object");
	m_initRules[objectName] = initRule;
}


void ParsingEngine::addRule(const RegEx& objectName,const RegEx& propertyName, const RuleBase* ruleFunction)
{
	GKoala_assert(m_rules.find(objectName) == m_rules.end() ||
			m_rules.find(objectName)->second.find(propertyName) == m_rules.find(objectName)->second.end(),
			"You already add this property rule for this object");

	m_rules[objectName][propertyName] = ruleFunction;
}

cocos2d::CCNode* GKoala::ParsingEngine::parseLayout(
		const std::string& layoutFile)
{
	if(layoutFile.empty())
	{
		throw ParsingException("layoutFile name can't be empty");
	}

	CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();

	const string fileNameWithPath = CCFileUtils::sharedFileUtils()->fullPathForFilename( layoutFile.c_str() );

	unsigned long fileDataSize = 0;
	std::unique_ptr<char[]> pLayoutData( reinterpret_cast<char*>( pFileUtils->getFileData(fileNameWithPath.c_str(), "r", &fileDataSize ) ) );

	if( pLayoutData.get() == nullptr || fileDataSize == 0 )
	{
		throw ParsingException(std::string("Can't open layout file:") + layoutFile );
	}

	Json::Value root;
	Json::Reader reader;

	bool parsSuccess = reader.parse( pLayoutData.get(), pLayoutData.get() + fileDataSize, root,false );

	if( parsSuccess == false )
	{
		throw ParsingException(std::string("Invalid JSON\n") + reader.getFormatedErrorMessages());
	}

	return static_cast<CCNode*>(parseObject(root,nullptr));
}


const InitRule& ParsingEngine::findInitRule(const std::string& objectField)const
{
	std::smatch matcher;

	for(auto&& initRule : m_initRules)
	{
		std::regex_match(objectField,matcher,initRule.first.regex);
		if(matcher.empty() == false)
		{
			//TODO multiple match
			return initRule.second;
		}
	}

	throw ParsingException("I don't know how create object:" + objectField +
			"\nI can't find matching rule.\nYou should add init rule see ParserInterface::addObjectInitRule");
}


const RuleBase* ParsingEngine::findFieldRule(const std::string& objectField,const std::string& fieldName)const
{
	std::smatch matcher;

	const RuleBase* pRule = nullptr;
	std::string matched;

	for(auto&& elementObject : m_rules)
	{
		std::regex_match(objectField,matcher,elementObject.first.regex);
		if(matcher.empty())
		{
			continue;
		}

		auto&& mapOfFields = elementObject.second;
		for(auto&& elementRule : mapOfFields)
		{
			std::regex_match(fieldName,matcher,elementRule.first.regex);
			if(matcher.empty())
			{
				continue;
			}
			if(pRule != nullptr)
			{
				throw ParsingException("You have multiple matching rules for this field:" + fieldName + "\nPrevious matched:" + matched + "\nCurrent:" + elementRule.first.stringRepresentation);
			}

			pRule = elementRule.second;
			matched = elementRule.first.stringRepresentation;
		}
	}

	if(pRule == nullptr)
	{
		throw ParsingException("I don't have any matching parsing rules for object:" + objectField + "\nI don't know how to parse:" + fieldName);
	}

	return pRule;
}

BaseClass* ParsingEngine::parseObject(const Json::Value& value,BaseClass* pContext) const
{
	if(value.isNull() || value.isObject() == false)
	{
		throw ParsingException("This isn't JSON object\n"+ value.toStyledString());
	}

	std::string objectField = value.get(FIELD_OBJECT,"").asString();
	if(objectField.empty())
	{
		throw ParsingException("You don't set 'object' field in\n" + value.toStyledString());
	}

	auto& buildRule = findInitRule(objectField);

	//First we need to construct object so we look up for init rule
	BaseClass* pCreated = buildRule();

	if(pCreated == nullptr)
	{
		throw ParsingException("init rule should return new object but you return nullptr");
	}

	//Parameter parameterObject(pNodeOut);

	auto members = value.getMemberNames();
	members.erase(std::find(members.begin(),members.end(),FIELD_OBJECT));

	//If we don't have fields we can skip
	if(members.empty())
	{
		return pCreated;
	}

	for(auto&& element : members)
	{
		const auto& elementValue = value[element];
		GLOG("%s = %s",element.c_str(),elementValue.toStyledString().c_str());

		auto fieldRule = findFieldRule(objectField,element);

		if(fieldRule->type == RuleBase::Type::INT)
		{
			if(elementValue.isConvertibleTo(Json::ValueType::intValue) == false)
			{
				throw ParsingException("Can't convert " + element + " to int in " + value.toStyledString());
			}

			int value = elementValue.asInt();
			fieldRule->call(pCreated,pContext,value);
		}
		else if(fieldRule->type == RuleBase::Type::FLOAT)
		{
			if(elementValue.isConvertibleTo(Json::ValueType::realValue) == false)
			{
				throw ParsingException("Can't convert " + element + " to float in " + value.toStyledString());
			}

			float value = elementValue.asDouble();
			fieldRule->call(pCreated,pContext,value);
		}
		else if(fieldRule->type == RuleBase::Type::STRING)
		{
			if(elementValue.isConvertibleTo(Json::ValueType::stringValue) == false)
			{
				throw ParsingException("Can't convert " + element + " to std::string in " + value.toStyledString());
			}

			std::string value = elementValue.asString();
			fieldRule->call(pCreated,pContext,value);
		}
		else if(fieldRule->type == RuleBase::Type::OBJECT)
		{
			if(elementValue.isObject() == false)
			{
				throw ParsingException("This field:" + element + " is'n a JSON object." + value.toStyledString());
			}

			BaseClass* pValue = parseObject(elementValue,pCreated);
			fieldRule->call(pCreated,pContext,pValue);
		}
		else if((fieldRule->type & RuleBase::Type::VECTOR) == RuleBase::Type::VECTOR)
		{
			if(elementValue.isArray() == false)
			{
				throw ParsingException("This field:" + element + " is'n a JSON array." + value.toStyledString());
			}

			if((fieldRule->type & RuleBase::Type::INT) == RuleBase::Type::INT)
			{
				vector<int> values = parseArrayInt(elementValue);
				fieldRule->call(pCreated,pContext,values);
			}
			else if((fieldRule->type & RuleBase::Type::FLOAT) == RuleBase::Type::FLOAT)
			{
				vector<float> values = parseArrayFloat(elementValue);
				fieldRule->call(pCreated,pContext,values);
			}
			else if((fieldRule->type & RuleBase::Type::STRING) == RuleBase::Type::STRING)
			{
				vector<std::string> values = parseArrayString(elementValue);
				fieldRule->call(pCreated,pContext,values);
			}
			else if((fieldRule->type & RuleBase::Type::OBJECT) == RuleBase::Type::OBJECT)
			{
				vector<BaseClass*> values = parseArrayObject(elementValue,pCreated);
				fieldRule->call(pCreated,pContext,values);
			}
			else
			{
				throw ParsingException("Something isn't ok please check this! I shloud have vector of?");
			}
		}
		else
		{
			throw ParsingException("Something isn't ok please check this!");
		}
	}

	return pCreated;
}

std::vector<int> ParsingEngine::parseArrayInt(const Json::Value& value) const
{
	if(value.isNull() || value.isArray() == false)
	{
		throw ParsingException("This isn't JSON array\n"+ value.toStyledString());
	}

	std::vector<int> parameters;
	for(auto&& element : value )
	{
		if(element.isConvertibleTo(Json::ValueType::intValue) == false)
		{
			throw ParsingException("Can't convert to int in " + value.toStyledString());
		}

		parameters.emplace_back(element.asInt());
	}

	return parameters;
}

std::vector<float> ParsingEngine::parseArrayFloat(const Json::Value& value) const
{
	if(value.isNull() || value.isArray() == false)
	{
		throw ParsingException("This isn't JSON array\n"+ value.toStyledString());
	}

	std::vector<float> parameters;
	for(auto&& element : value )
	{
		if(element.isConvertibleTo(Json::ValueType::realValue) == false)
		{
			throw ParsingException("Can't convert to float in " + value.toStyledString());
		}

		parameters.emplace_back(element.asDouble());
	}

	return parameters;
}

std::vector<std::string> ParsingEngine::parseArrayString(const Json::Value& value) const
{
	if(value.isNull() || value.isArray() == false)
	{
		throw ParsingException("This isn't JSON array\n"+ value.toStyledString());
	}

	std::vector<std::string> parameters;
	for(auto&& element : value )
	{
		if(element.isConvertibleTo(Json::ValueType::stringValue) == false)
		{
			throw ParsingException("Can't convert to std::string in " + value.toStyledString());
		}

		parameters.emplace_back(element.asString());
	}

	return parameters;
}

std::vector<BaseClass*> ParsingEngine::parseArrayObject(const Json::Value& value,BaseClass* pContext) const
{
	if(value.isNull() || value.isArray() == false)
	{
		throw ParsingException("This isn't JSON array\n"+ value.toStyledString());
	}

	std::vector<BaseClass*> parameters;
	for(auto&& element : value )
	{
		if(element.isObject() == false)
		{
			throw ParsingException("This field isn't JSON Object. " + value.toStyledString());
		}

		BaseClass* pValue = parseObject(element,pContext);
		parameters.emplace_back(pValue);
	}

	return parameters;
}

}


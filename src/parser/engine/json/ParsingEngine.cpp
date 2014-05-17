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
}

void ParsingEngine::addInitRule(const RegEx& objectName,const InitRule& initRule)
{
	GKoala_assert(m_initRules.find(objectName) == m_initRules.end(),"You already add init rule for this object");
	m_initRules[objectName] = initRule;
}


void ParsingEngine::addRule(const RegEx& objectName,const RegEx& propertyName, const Rule& ruleFunction)
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

	return parseObject(root);
}


const ParserInterface::InitRule& ParsingEngine::findInitRule(const std::string& objectField)const
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


const ParserInterface::Rule& ParsingEngine::findFieldRule(const std::string& objectField,const std::string& fieldName)const
{
	std::smatch matcher;

	const Rule* pRule = nullptr;
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

			pRule = &elementRule.second;
			matched = elementRule.first.stringRepresentation;
		}
	}

	if(pRule == nullptr)
	{
		throw ParsingException("I don't have any matching parsing rules for object:" + objectField + "\nI don't know how to parse:" + fieldName);
	}

	return *pRule;
}

cocos2d::CCNode* ParsingEngine::parseObject(const Json::Value& value) const
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
	CCNode* pNodeOut = buildRule();

	if(pNodeOut == nullptr)
	{
		throw ParsingException("init rule should return new object but you return nullptr");
	}

	Parameter parameterObject(pNodeOut);

	auto members = value.getMemberNames();
	members.erase(std::find(members.begin(),members.end(),FIELD_OBJECT));

	//If we don't have fields we can skip
	if(members.empty())
	{
		return pNodeOut;
	}

	for(auto&& element : members)
	{
		const auto& elementValue = value[element];
		GLOG("%s = %s",element.c_str(),elementValue.toStyledString().c_str());

		auto fieldRule = findFieldRule(objectField,element);

		if(elementValue.isIntegral())
		{
			parameterObject._int = elementValue.asInt();
		}
		else if(elementValue.isDouble())
		{
			parameterObject._float = elementValue.asDouble();
		}
		else if(elementValue.isString())
		{
			parameterObject._string = elementValue.asString();
		}
		else if(elementValue.isArray())
		{
			parameterObject.parameters = parseArray(elementValue);
		}
		else if(elementValue.isObject())
		{
			parameterObject.pNode = parseObject(elementValue);
		}

		fieldRule(parameterObject);
	}


	return pNodeOut;
}

std::vector<Parameter> ParsingEngine::parseArray(const Json::Value& value) const
{
	if(value.isNull() || value.isArray() == false)
	{
		throw ParsingException("This isn't JSON array\n"+ value.toStyledString());
	}

	std::vector<Parameter> parameters;
	for(auto&& element : value )
	{
		if(element.isIntegral())
		{
			Parameter parameter(nullptr);
			parameter._int = element.asInt();
			parameters.emplace_back(parameter);
		}
		else if(element.isDouble())
		{
			Parameter parameter(nullptr);
			parameter._float = element.asDouble();
			parameters.emplace_back(parameter);
		}
		else if(element.isString())
		{
			Parameter parameter(nullptr);
			parameter._string = element.asString();
			parameters.emplace_back(parameter);
		}
		else if(element.isArray())
		{
			Parameter parameter(nullptr);
			parameter.parameters = parseArray(element);
			parameters.emplace_back(parameter);
		}
		else if(element.isObject())
		{
			Parameter parameter(nullptr);
			parameter.pNode = parseObject(element);
			parameters.emplace_back(parameter);
		}
	}

	return parameters;
}

}


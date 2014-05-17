#pragma once

#include <exception>
#include <string>

namespace GKoala
{

class ParsingException : public std::exception
{
public:
	ParsingException( const std::string& message );
	virtual ~ParsingException();

	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;
private:
	std::string m_message;
};

} /* namespace GKoala */

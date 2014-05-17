#include "parser/engine/json/ParsingException.h"

namespace GKoala
{

ParsingException::ParsingException( const std::string& message ) :
		m_message(message )
{
}

ParsingException::~ParsingException()
{
}

const char* ParsingException::what() const _GLIBCXX_USE_NOEXCEPT
{
	return m_message.c_str();
}

} /* namespace GKoala */

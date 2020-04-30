#pragma once

#include "IParserState.hpp"
#include "IParser.hpp"
#include <sstream>


namespace Zara
{
	class CommandState : virtual public IParserState
	{
	private:
		IParser* parser;
		std::ostringstream result;
	public:
		CommandState(IParser* parser) : parser(parser) {}
		CommandState(IParser* parser, char first);
		void NextChar(char ch) override;
	};
}
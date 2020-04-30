#pragma once

#include "IParserState.hpp"
#include "IParser.hpp"


namespace Zara
{
	class DefaultState : virtual public IParserState
	{
	private:
		IParser* parser;
	public:
		DefaultState(IParser* parser) : parser(parser) {}
		void NextChar(char ch) override;
	};
}

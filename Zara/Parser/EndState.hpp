#pragma once

#include "IParserState.hpp"
#include "IParser.hpp"


namespace Zara
{
	class EndState : virtual public IParserState
	{
	public:
		EndState(IParser* parser)
		{
			parser->StopParse();
		}
		void NextChar(char ch) override {}
	};
}

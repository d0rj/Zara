#pragma once

#include "IParserState.hpp"
#include "IParser.hpp"
#include <sstream>


namespace Zara
{
	class ArgumentState : virtual public IParserState
	{
	private:
		IParser* parser;
		std::ostringstream result;
		int indicator = 0;
		bool nextStepOut = false;
	public:
		ArgumentState(IParser* parser) : parser(parser) {}
		void NextChar(char ch) override;
	};
}
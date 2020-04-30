#include "CommandState.hpp"
#include "ArgumentState.hpp"
#include "EndState.hpp"


Zara::CommandState::CommandState(IParser* parser, char first) : CommandState(parser)
{
	result << first;
}


void Zara::CommandState::NextChar(char ch)
{
	if (ch == ' ')
	{
		parser->SetCommand(result.str());
		parser->SetState(new ArgumentState(parser));
	}
	else if (ch == '\n')
	{
		parser->SetCommand(result.str());
		parser->SetArgument("");
		parser->SetState(new EndState(parser));
	}
	else
	{
		result << ch;
	}
}

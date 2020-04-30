#include "DefaultState.hpp"
#include "CommandState.hpp"
#include "EndState.hpp"


void Zara::DefaultState::NextChar(char ch)
{
	if (ch == '\n')
	{
		parser->SetState(new EndState(parser));
	}
	else if (ch == '-')
	{
		parser->SetState(new CommandState(parser));
	}
	else
	{
		parser->SetState(new CommandState(parser, ch));
	}
}

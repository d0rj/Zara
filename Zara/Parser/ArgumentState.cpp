#include "ArgumentState.hpp"
#include "CommandState.hpp"


void Zara::ArgumentState::NextChar(char ch)
{
	if (nextStepOut)
	{		
		parser->SetState(new CommandState(parser));
		return;
	}

	if (ch == '{')
	{
		++indicator;

		if (indicator != 1)
			result << ch;
	}
	else if (ch == '}')
	{
		--indicator;

		if (indicator <= 0)
		{
			nextStepOut = true;
			parser->SetArgument(result.str());
		}
		else
			result << ch;
	}
	else
	{
		result << ch;
	}
}

#include "EagleParser.hpp"


void Zara::EagleParser::SetState(IParserState* newState)
{
	state = newState;
}


void Zara::EagleParser::SetCommand(std::string command)
{
	tempCommand = command;
}


void Zara::EagleParser::SetArgument(std::string arg)
{
	if (tempCommand.empty())
	{
		// TODO: handle errors
		return;
	}

	parsed[tempCommand] = arg;

	tempCommand = std::string();
}


void Zara::EagleParser::StopParse()
{
	parsing = false;
}


std::map<std::string, std::string> Zara::EagleParser::Parse(std::string str)
{
	parsing = true;

	for (char ch : str)
	{
		if (!parsing)
			break;

		state->NextChar(ch);
	}

	return parsed;
}


nlohmann::json Zara::EagleParser::ParseJson(std::string str)
{
	return nlohmann::json::parse(str);
}

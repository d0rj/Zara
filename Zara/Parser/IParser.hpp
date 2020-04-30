#pragma once 

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include "IParserState.hpp"


namespace Zara
{
	class IParser
	{
	public:
		virtual void SetState(IParserState* newState) = 0;
		virtual void SetCommand(std::string command) = 0;
		virtual void SetArgument(std::string arg) = 0;
		virtual void StopParse() = 0;
		virtual std::map<std::string, std::string> Parse(std::string str) = 0;
		virtual nlohmann::json ParseJson(std::string str) = 0;
	};
}

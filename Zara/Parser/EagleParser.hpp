#pragma once

#include "IParser.hpp"
#include "DefaultState.hpp"


namespace Zara
{
	class EagleParser : virtual public IParser
	{
	private:
		IParserState* state;
		bool parsing = false;
		std::unordered_map<std::string, std::string> parsed;
		std::string tempCommand;
	public:
		EagleParser() : state(new DefaultState(this)) {}

		void SetState(IParserState* newState) override;
		void SetCommand(std::string command) override;
		void SetArgument(std::string arg) override;
		void StopParse() override;
		std::unordered_map<std::string, std::string> Parse(std::string str) override;
		nlohmann::json ParseJson(std::string str) override;
	};
}

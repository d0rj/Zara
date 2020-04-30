#pragma once 

#include <string>
#include <vector>


namespace Zara
{
	class IParserState
	{
	public:
		virtual void NextChar(char ch) = 0;
	};
}

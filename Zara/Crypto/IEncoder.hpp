#pragma once

#include <string>


namespace Zara
{
	class IEncoder
	{
	public:
		virtual std::string Encode(std::string data, std::string key) = 0;
		virtual std::string Decode(std::string data, std::string key) = 0;
	};
}
